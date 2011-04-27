/*       
    NiTMap class hierarchy: hashtable-based associative containers   

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
    -   Removed all template parameters for the allocator, hash & comparison functors
        The hash & comparison functors actually manifest as virtual methods
    -   Removed all inheritance from NiMemObject
    -   Switched NIASSERT to cassert
    -   Switched NiMalloc & NiFree to FormHeap

    [Original] Notes on Usage:

     The map class implements a hash table of TKEY to store values of TVAL.
     It uses modular arithmetic for building the hash keys with a default
     table size of 37.  If you want a larger table size, the best bet is to
     us a large prime number.  Consult a standard text on hashing for the
     basic theory.

     TKEY MUST BE THE SAME SIZE AS A POINTER!

     The template class assumes that type TKEY has the following:
       1.  Default constructor, TKEY::TKEY();
       2.  Copy constructor, TKEY::TKEY(const TKEY&);
       3.  Assignment, TKEY& operator=(const TKEY&);
       4.  Comparison, bool TKEY::operator==(const TKEY&), or supply a
           specialized equality testing class in your template.
       5.  Implicit conversion, TKEY::operator long(), for building hash key,
           or you must pass in your own hash function class in your template.

     The template class assumes that type TVAL has the following:
       1.  Default constructor, TVAL::TVAL();
       2.  Copy constructor, TVAL::TVAL(const TVAL&);
       3.  Assignment, TVAL& operator=(const TVAL&);

     In both cases, the compiler-generated default constructor, copy
     constructor, and assignment operator are acceptable.

     Example of iteration over map

         NiTMap<TKEY,TVAL> kMap;
         NiTMapIterator pos = kMap.GetFirstPos();
         while (pos)
         {
             TKEY key;
             TVAL val;
             kMap.GetNext(pos,key,val);
             <process key and val here>;
         }

*/
#pragma once
#include "API/TES/MemoryHeap.h"

typedef void* NiTMapIterator;

template <class TKEY, class TVAL> class NiTMapItem
{// size 0C/0C
public:
    NiTMapItem* m_pkNext;
    TKEY        m_key;
    TVAL        m_val;
    // use FormHeap for class new & delete
    USEFORMHEAP 
};

template <class TKEY, class TVAL> class NiTMapBase
{// size 10/10 

            // members
            //void**                vtbl;               // 00
protected:  UInt32                  m_uiHashSize;       // 04 maximum slots in hash table
            NiTMapItem<TKEY,TVAL>** m_ppkHashTable;     // 08 hash table storage
            UInt32                  m_uiCount;          // 0C number of elements in list

            // virtual methods              
public:     virtual                         ~NiTMapBase();                                                  // 000 
                                            // vtbl entry is actually 'scalar deleting destructor', which calls this method       
protected:  virtual UInt32                  KeyToHashIndex(TKEY key) const;                                 // 004 actually a templated functor
            virtual bool                    IsKeysEqual(TKEY key1, TKEY key2) const;                        // 008 actually a templated functor
            virtual void                    SetValue(NiTMapItem<TKEY,TVAL>* pkItem, TKEY key, TVAL val);    // 00C
            virtual void                    ClearValue(NiTMapItem<TKEY,TVAL>* pkItem);                      // 010
            virtual NiTMapItem<TKEY, TVAL>* NewItem() = 0;                                                  // 014
            virtual void                    DeleteItem(NiTMapItem<TKEY, TVAL>* pkItem) = 0;                 // 018
    
            // methods - counting elements in map
public:     inline UInt32           GetCount() const;
            inline bool             IsEmpty() const;
            // methods - add or remove elements
            inline void             SetAt(TKEY key, TVAL val);
            inline bool             RemoveAt(TKEY key);
            inline void             RemoveAll();
            // methods - element access
            inline bool             GetAt(TKEY key, TVAL& val) const;
            // methods - map traversal
            inline NiTMapIterator   GetFirstPos() const;
            inline void             GetNext(NiTMapIterator& pos, TKEY& key, TVAL& val) const;  
            // methods - Resize the map (e.g. the underlying hash table)
            inline void             Resize(UInt32 uiNewHashSize);         
            // methods - construction
public:     NiTMapBase(UInt32 uiHashSize = 37); 
            // use FormHeap for class new & delete
            USEFORMHEAP 
            // prevent compiler from generating the default copy constructor or default assignment operator 
private:    NiTMapBase(const NiTMapBase&);              
            NiTMapBase& operator=(const NiTMapBase&);           
};

template <class TKEY, class TVAL> class NiTMap : public NiTMapBase<TKEY, TVAL>
{// size 10/10 
            // members
public:     static const UInt32 NUM_PRIMES;
            static const UInt32 PRIMES[];

            // virtual methods
            virtual ~NiTMap();                                          // 000
protected:  virtual NiTMapItem<TKEY, TVAL>* NewItem();                  // 014
            virtual void DeleteItem(NiTMapItem<TKEY, TVAL>* pkItem);    // 018

            // methods
public:     inline NiTMap(UInt32 uiHashSize = 37) :  NiTMapBase<TKEY, TVAL>(uiHashSize) {};
            static UInt32 NextPrime(const UInt32 uiTarget);
};

template <class TKEY, class TVAL> class NiTPointerMap : public NiTMapBase<TKEY, TVAL>
{// size 10/10 
            // virtual methods
public:     virtual ~NiTPointerMap();                                   // 000
protected:  virtual NiTMapItem<TKEY, TVAL>* NewItem();                  // 014
            virtual void DeleteItem(NiTMapItem<TKEY, TVAL>* pkItem);    // 018

             // methods
public:     NiTPointerMap(UInt32 uiHashSize = 37) :  NiTMapBase<TKEY, TVAL>(uiHashSize) {};
            // use FormHeap for class new & delete
            USEFORMHEAP 
};

template <class TPARENT, class TVAL> class NiTStringTemplateMap : public TPARENT
{// size 14/14 
            // members
protected:  bool m_bCopy;       // 10

            // virtual methods
           
public:     virtual                     ~NiTStringTemplateMap();                                                        // 000     
protected:  virtual UInt32              KeyToHashIndex(const char* key) const;                                          // 004
            virtual bool                IsKeysEqual(const char* key1, const char* key2) const;                          // 008
                                        // NOTE: Bethesda used a case-insesnitive string comparison for this field, even though the default
                                        // hash function is case sensitive.  This violates the core invariant of hash tables - that key equality
                                        // implies hash equality.  As a result, some parts of the key space will theoretically overlap.
                                        // In practice with a hash table of reasonable size, however, this doesn't seem to be a problem.  
                                        // For correctness, this definition uses the intended case sensitive comparator.
            virtual void                SetValue(NiTMapItem<const char*, TVAL>* pkItem, const char* pcKey, TVAL val);   // 00C
            virtual void                ClearValue(NiTMapItem<const char*, TVAL>* pkItem);                              // 010

            // methods            
public:     NiTStringTemplateMap(UInt32 uiHashSize = 37, bool bCopy = true) : TPARENT(uiHashSize), m_bCopy(bCopy) {}
            // use FormHeap for class new & delete
            USEFORMHEAP 
};

template <class TVAL> class NiTStringMap : public NiTStringTemplateMap<NiTMap<const char*, TVAL>,TVAL>
{// size 14/14 
public:     
    // methods
    NiTStringMap(UInt32 uiHashSize = 37, bool bCopy = true) : NiTStringTemplateMap<NiTMap<const char*, TVAL>,TVAL>(uiHashSize, bCopy) {}
    // use FormHeap for class new & delete
    USEFORMHEAP 
};

template <class TVAL> class NiTStringPointerMap : public NiTStringTemplateMap<NiTPointerMap<const char*, TVAL>, TVAL>
{// size 14/14 
public:     
    // methods
    NiTStringPointerMap(UInt32 uiHashSize = 37, bool bCopy = true) : NiTStringTemplateMap<NiTPointerMap<const char*, TVAL>, TVAL>(uiHashSize, bCopy) {}
    // use FormHeap for class new & delete
    USEFORMHEAP 
};

#include "API/NiTypes/NiTMap.inl"
