/*	   
    NiTList class hierarchy: templated doubly-linked list, used mainly for pointers & 32-bit data types

    [ NOTICE: This is a templated class ]
    Because it uses templates, this class requires a seperate set of imports for every specialization.  
    In practice, most of the specializations map back to a few basic methods, but we'd still need to
    import the appropriate symbols for each.  This makes it difficult to hook and patch - but it also means
    that noone is likely to patch it, so the hooking is not strictly necessary.  We instead attempt to
    reproduce the original templated source code.
    CAUTIONS:
    -   Great care must be taken for some functions (e.g. hash compuation) to make sure they match the game code
    -   Because of simplication to template & inheritance structure, dynamic_cast may fail for this class

    Alterations from original code:
    -   Removed template parameter for allocator, replaced with FormHeap
    -   Removed all inheritance from NiMemObject
    -   Switched NIASSERT to cassert

    [Original] Notes on Useage:

    Example of iteration from head to tail:

       NiTPointerList<T> kList;
       NiTListIterator kPos = kList.GetHeadPos();
       while (kPos)
       {
           T element = kList.GetNext(kPos);
           <process element here>;
       }

    NiTPointerListBase defines the interface for NiTList and NiTPointerList.
     The interface works best for pointer types T, however, other types
     will work when they satisfy the conditions below.  The template
     class assumes that type T has the following:
       1.  A "zero" element (i.e., T var; var = 0; is supported)
           which is considered to be a null array element.
       2.  The default constructor for T must exist and create the
           "zero" element.  The constructor must also handle all necessary
           actions for constructing elements.  That is, the template array
           class cannot make any post-construction member calls that are
           specific to class T.
       3.  Copy constructor T::T(const T&) must work properly.  The class T is
           responsible for implementing this if need be.
       4.  The destructor must handle all necessary actions for destroying
           elements.  That is, the template array class cannot make any
           pre-destruction member calls that are specific to class T.
       5.  bool operator== (const T&);
       6.  bool operator!= (const T&);
       7.  T& operator= (const T&);

     NiTPointerList<T> requires sizeof(T) == sizeof(unsigned int), so that
     elements may be block allocated from a shared memory pool.

     NiTObjectList is designed to hold compound objects efficiently. It
     is responsible for allocating and freeing the elements in the list. The
     interface does not support copy construction assignment of T directly.
     Instead, element access is provided via a pointer to the element.
     T is block allocated from a shared memory pool. The template class 
     requires that a default constructor for T exists, even if it does nothing.

     The pool uses some non-local static variables that must be declared using
     the NiAllocatorDeclareStatics macro. For example:
       NiAllocatorDeclareStatics(T, size);
     where T is the template type T and size is the block size for the memory
     pool. To free this memory pool, use:
       NiAllocatorShutdown(T);
     NiAllocatorShutdown assumes that any NiTObjectList<T> instances have been
     destroyed. Accessing a pre-existing NiTObjectList<T> after calling
     NiAllocatorShutdown(T) will cause a memory access violation. After calling
     NiAllocatorShutdown(T), new NiTObjectList<T> instances may be created, but
     of course they should be cleaned up with another call to
     NiAllocatorShutdown(T).
     
     Remove and FindPos search for an element based on pointer equality. 
*/
#pragma once
#include "API/TES/MemoryHeap.h"

typedef void* NiTListIterator;

template <class T> class NiTListItem
{
public:
    NiTListItem*    m_pkNext;
    NiTListItem*    m_pkPrev;
    T               m_element;
    // use FormHeap for class new & delete
    USEFORMHEAP 
};

template <class T> class NiTListBase
{// size 10/10

            // members
            //void**            vtbl            // 00
protected:  NiTListItem<T>*     m_pkHead;       // 04 points to head of list
            NiTListItem<T>*     m_pkTail;       // 08 points to tail of list
            UInt32              m_uiCount;      // 0C number of elements in list

            // virtual methods
protected:  virtual                 ~NiTListBase() = 0;
            virtual NiTListItem<T>* NewItem() = 0;
            virtual void            DeleteItem(NiTListItem<T>* pkItem) = 0;

            // counting elements in list
public:     inline UInt32           GetSize() const;
            inline bool             IsEmpty() const;           
            // clear list
            inline void             RemoveAll();
            // iteration support
            inline NiTListIterator  GetHeadPos() const;
            inline NiTListIterator  GetTailPos() const;             
            inline NiTListIterator  GetNextPos(NiTListIterator kPos) const;
            inline NiTListIterator  GetPrevPos(NiTListIterator kPos) const;
            // add nodes
protected:  inline void AddNodeHead(NiTListItem<T>* pkNode);
            inline void AddNodeTail(NiTListItem<T>* pkNode);
            inline NiTListIterator AddNodeAfter(NiTListIterator pos, NiTListItem<T>* pkNode);
            inline NiTListIterator AddNodeBefore(NiTListIterator pos,NiTListItem<T>* pkNode);
            // constructor
            NiTListBase();
            // use FormHeap for class new & delete
public:     USEFORMHEAP 

            // prevent compiler from generating the default copy constructor or default assignment operator 
private:    NiTListBase(const NiTListBase&);
            NiTListBase& operator=(const NiTListBase&);
};

template <class T> class NiTPointerListBase : public NiTListBase<T>
{// size 10/10

            // virtual methods
public:     virtual                 ~NiTPointerListBase();
protected:  virtual NiTListItem<T>* NewItem();
            virtual void            DeleteItem(NiTListItem<T>* pkItem);

            // Add/Insert nodes 
public:     inline void             AddHead(const T& element);
            inline void             AddTail(const T& element);
            inline NiTListIterator  InsertBefore(NiTListIterator kPos, const T& element);
            inline NiTListIterator  InsertAfter(NiTListIterator kPos, const T& element);
            // Element access
            inline const T&         GetHead() const;
            inline const T&         GetTail() const;
            inline const T&         Get(NiTListIterator kPos) const;
            // Element access with iteration support
            inline const T&         GetNext(NiTListIterator& kPos) const;
            inline const T&         GetPrev(NiTListIterator& kPos) const;
            // Remove nodes
            inline T                RemoveHead();
            inline T                RemoveTail();
            inline T                Remove(const T& element);
            inline T                RemovePos(NiTListIterator& kPos);
            // Element search
            inline NiTListIterator  FindPos(const T& element, NiTListIterator kStart = 0) const;
};

template <class T> class NiTPointerList : public NiTPointerListBase<T> {};

template <class T> class NiTList : public NiTPointerListBase<T> {};

template <class T> class NiTObjectList :  public NiTListBase<T>
{// size 10/10

            // virtual methods
public:     virtual                 ~NiTObjectList();
protected:  virtual NiTListItem<T>* NewItem();
            virtual void            DeleteItem(NiTListItem<T>* pkItem);

            // Add/Insert functions return a pointer to a new object.
            // No constructor is called; data must be explicitly initialized.
public:     inline T*               AddNewHead();
            inline T*               AddNewTail();
            inline T*               InsertNewBefore(NiTListIterator kPos, NiTListIterator& kNewIterator);
            inline T*               InsertNewAfter(NiTListIterator kPos, NiTListIterator& kNewIterator);
            inline T*               InsertNewBefore(NiTListIterator kPos);
            inline T*               InsertNewAfter(NiTListIterator kPos);
            // Element access
            inline T*               GetHead() const;
            inline T*               GetTail() const;
            inline T*               Get(NiTListIterator kPos) const;
            // Element access with iteration support
            inline T*               GetNext(NiTListIterator& kPos) const;
            inline T*               GetPrev(NiTListIterator& kPos) const;
            // Remove functions free the object but do not call the destructor.
            // If cleanup is required it must be done explicitly.
            inline void             RemoveHead();
            inline void             RemoveTail();
            inline void             Remove(const T* element);
            inline void             RemovePos(NiTListIterator& kPos);
            // Element search
            inline NiTListIterator  FindPos(const T* element, NiTListIterator kStart = 0) const;
};

// include inline method definitions
#include "API/NiTypes/NiTList.inl"