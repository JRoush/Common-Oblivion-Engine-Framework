/*       
    NiTArray & NiTLargeArray: dynamically sizeable sparse arrays
    Often (always?) used for pointers, see notes on usage below

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
    -   unsigned int -> UInt16,UInt32 for Array,LargeArray respectively

    [Original] Notes on Usage:

    While the template arrays work best for pointer types TVAL, other types
    certainly will work when they satisfy the conditions below.  The template
    class assumes that type TVAL has the following:
     1.  A "zero" element (i.e., TVAL var; var = 0; is supported)
         which is considered to be a null array element.
     2.  The default constructor for TVAL must exist and create the
         "zero" element.  The constructor must also handle all necessary
         actions for constructing elements.  That is, the template array
         class cannot make any post-construction member calls that are
         specific to class TVAL.
     3.  Copy constructor TVAL::TVAL(const TVAL&) must work properly.  The class TVAL is
         responsible for implementing this if need be.
     4.  The destructor must handle all necessary actions for destroying
         elements.  That is, the template array class cannot make any
         pre-destruction member calls that are specific to class TVAL.
     5.  bool operator== (const TVAL&);
     6.  bool operator!= (const TVAL&);
     7.  TVAL& operator= (const TVAL&);
  
    An example to illustrate what the members of NiTArray mean.  Shown is
    an array of elements (0 = null element, x = an element)
       index:    0 1 2 3 4 5 6 7 8 9
       element:  x 0 x x 0 0 x 0 0 0
  
       m_uiMaxSize = 10 (number of slots in array)
       m_uiSize    =  7 (next available slot, useful for traversing minimum
                         block of elements while searching for non-null items)
       m_uiESize   =  4 (number of used slots)
  
    Note that when m_uiSize = m_uiMaxSize, an attempt to add a new element
    requires growing the array.  SetAtGrow does this.  SetAt does not.

    Methods for NiTArray (NiTLargeArray) that return an index return the maximum 
    UInt16 (UInt32) as a special 'not found' or 'invalid' value.
*/
#pragma once
#include "API/TES/MemoryHeap.h"

template <class TVAL> class NiTArray
{// size 10/10

            // members
            //void**    vtbl;           // 00
protected:  TVAL*       m_pBase;        // 04 pointer to the array storage
            UInt16      m_usMaxSize;    // 08 number of slots in array
            UInt16      m_usSize;       // 0A first available empty slot in array
            UInt16      m_usESize;      // 0C number of filled slots
            UInt16      m_usGrowBy;     // 0E number of slots to grow array when full

            // virtual methods
public:     virtual     ~NiTArray();    // 00 vtbl entry is actually 'scalar deleting destructor', which calls this method    

            // array size
public:     inline UInt16       GetSize() const;                    // Number of slots used in array, up to 1 past last used slot
            inline UInt16       GetEffectiveSize() const;           // Number of non-zero items in array, less or equal to GetSize()
            inline UInt16       GetAllocatedSize() const;           // Total number of allocated slots
            inline UInt16       GetGrowBy() const;
            inline void         SetSize(UInt16 uiSize);
            inline void         SetGrowBy(UInt16 uiGrowBy);
            // set and get elements
            inline TVAL*        GetBase();
            inline const TVAL&  GetAt(UInt16 uiIndex) const;
            inline TVAL&        GetAt(UInt16 uiIndex);
            inline void         SetAt(UInt16 uiIndex, const TVAL& element);
            inline UInt16       SetAtGrow(UInt16 uiIndex, const TVAL& element);    
            // add and remove elements
            inline UInt16       Add(const TVAL& element);
            inline UInt16       AddFirstEmpty(const TVAL& element);
            inline TVAL         RemoveAt(UInt16 uiIndex);    
            inline TVAL         RemoveAtAndFill(UInt16 uiIndex);    // Removes last element in array and places it at uiIndex 
            inline TVAL         RemoveEnd();
            inline void         RemoveAll();
            inline UInt16       Remove(const TVAL& element);        // Remove the first occurrence
            inline UInt16       Find(const TVAL& element, UInt16 uiStartIdx = 0); // Find the first occurrence after start
            inline void         UpdateSize();                       // After deletions before m_uiSize slot, m_uiSize may no longer be accurate
            // Compact all elements to contiguous space. 
            inline void         Compact();                          // Reallocation is performed to eliminate unused slots.
            // Operator to simulate access like a conventional array
            inline TVAL&        operator [] (UInt16 uiIndex) const;
            // Slow insert and delete functions to maintain the order
            inline void         Insert(UInt16 uiIndex, const TVAL& element);
            inline TVAL         Delete(UInt16 uiIndex);
            // construction
            NiTArray(UInt16 uiMaxSize = 0, UInt16 uiGrowBy = 1);
            // use FormHeap for class new & delete
            USEFORMHEAP 

            // prevent compiler from generating the default copy constructor or default assignment operator 
private:    NiTArray(const NiTArray&);
            NiTArray& operator=(const NiTArray&);
};

template <class TVAL> class NiTLargeArray
{// size 18/18

            // members
            //void**    vtbl;           // 00
protected:  TVAL*       m_pBase;        // 04 pointer to the array storage
            UInt32      m_uiMaxSize;    // 08 number of slots in array
            UInt32      m_uiSize;       // 0C first available empty slot in array
            UInt32      m_uiESize;      // 10 number of filled slots
            UInt32      m_uiGrowBy;     // 14 number of slots to grow array when full

            // virtual methods
public:     virtual     ~NiTLargeArray();

            // array size
public:     inline UInt32       GetSize() const;                    // Number of slots used in array, up to 1 past last used slot
            inline UInt32       GetEffectiveSize() const;           // Number of non-zero items in array, less or equal to GetSize()
            inline UInt32       GetAllocatedSize() const;           // Total number of allocated slots
            inline UInt32       GetGrowBy() const;
            inline void         SetSize(UInt32 uiSize);
            inline void         SetGrowBy(UInt32 uiGrowBy);
            // set and get elements
            inline TVAL*        GetBase();
            inline const TVAL&  GetAt(UInt32 uiIndex) const;
            inline TVAL&        GetAt(UInt32 uiIndex);
            inline void         SetAt(UInt32 uiIndex, const TVAL& element);
            inline UInt32       SetAtGrow(UInt32 uiIndex, const TVAL& element);
            // add and remove elements
            inline UInt32       Add(const TVAL& element);
            inline UInt32       AddFirstEmpty(const TVAL& element);
            inline TVAL         RemoveAt(UInt32 uiIndex);            
            inline TVAL         RemoveAtAndFill(UInt32 uiIndex);    // Removes last element in array and places it at uiIndex 
            inline TVAL         RemoveEnd();
            inline void         RemoveAll();
            inline UInt32       Remove(const TVAL& element);        // Remove the first occurrence            
            inline void         UpdateSize();                       // After deletions before m_uiSize slot, m_uiSize may no longer be accurate
            // Compact all elements to contiguous space. 
            inline void         Compact();  // Reallocation is performed to eliminate unused slots.
            // Operator to simulate access like a conventional array
            inline TVAL&        operator [] (UInt32 uiIndex) const;
            // construction
            NiTLargeArray(UInt32 uiMaxSize = 0, UInt32 uiGrowBy = 1);
            // use FormHeap for class new & delete
            USEFORMHEAP 

            // prevent compiler from generating the default copy constructor or default assignment operator 
private:    NiTLargeArray(const NiTLargeArray&);
            NiTLargeArray& operator=(const NiTLargeArray&);
};

#include "API/NiTypes/NiTArray.inl"
