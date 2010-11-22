#include <cassert>

// NiTMapBase
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline NiTMapBase<TKEY, TVAL>::NiTMapBase(UInt32 uiHashSize)
{
    assert(uiHashSize > 0);

    m_uiHashSize = uiHashSize;
    m_uiCount = 0;

    UInt32 uiSize = sizeof(NiTMapItem<TKEY,TVAL>*) * m_uiHashSize;
    m_ppkHashTable = (NiTMapItem<TKEY,TVAL>**)MemoryHeap::FormHeapAlloc(uiSize);
    assert(m_ppkHashTable);
    memset(m_ppkHashTable, 0, uiSize);
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline NiTMapBase<TKEY, TVAL>::~NiTMapBase()
{
    RemoveAll();
    MemoryHeap::FormHeapFree(m_ppkHashTable);
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline UInt32 NiTMapBase<TKEY, TVAL>::GetCount() const
{ 
    return m_uiCount;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline bool NiTMapBase<TKEY,TVAL>::IsEmpty() const
{ 
    return m_uiCount == 0;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline void NiTMapBase<TKEY,TVAL>::SetAt(TKEY key, TVAL val)
{
    if (!m_ppkHashTable) return;  // no hash table allocated

    // look up hash table location for key
    UInt32 uiIndex = KeyToHashIndex(key);
    NiTMapItem<TKEY, TVAL>* pkItem = m_ppkHashTable[uiIndex];
    
    UInt32 uiDepth = 0;

    // search list at hash table location for key
    while (pkItem)
    {
        uiDepth++;
        if (IsKeysEqual(key, pkItem->m_key))
        {
            // item already in hash table, set its new value
            pkItem->m_val = val;
            return;
        }
        pkItem = pkItem->m_pkNext;
    }

    // add object to beginning of list for this hash table index
    pkItem = (NiTMapItem<TKEY, TVAL>*)NewItem();
    
    assert(pkItem);
    SetValue(pkItem, key, val);
    pkItem->m_pkNext = m_ppkHashTable[uiIndex];
    m_ppkHashTable[uiIndex] = pkItem;
    m_uiCount++;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline bool NiTMapBase<TKEY,TVAL>::RemoveAt(TKEY key)
{
    if (!m_ppkHashTable) return false;  // no hash table allocated

    // look up hash table location for key
    UInt32 uiIndex = KeyToHashIndex(key);
    NiTMapItem<TKEY, TVAL>* pkItem = m_ppkHashTable[uiIndex];

    // search list at hash table location for key
    if (pkItem)
    {
        if (IsKeysEqual(key, pkItem->m_key))
        {
            // item at front of list, remove it
            m_ppkHashTable[uiIndex] = pkItem->m_pkNext;
            ClearValue(pkItem);
            
            DeleteItem(pkItem);

            m_uiCount--;
            return true;
        }
        else
        {
            // search rest of list for item
            NiTMapItem<TKEY, TVAL>* pkPrev = pkItem;
            NiTMapItem<TKEY, TVAL>* pkCurr = pkPrev->m_pkNext;
            while (pkCurr && !IsKeysEqual(key, pkCurr->m_key))
            {
                pkPrev = pkCurr;
                pkCurr = pkCurr->m_pkNext;
            }
            if (pkCurr)
            {
                // found the item, remove it
                pkPrev->m_pkNext = pkCurr->m_pkNext;
                ClearValue(pkCurr);
                
                DeleteItem(pkCurr);

                m_uiCount--;
                return true;
            }
        }
    }

    return false;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline void NiTMapBase<TKEY, TVAL>::RemoveAll()
{
    if (!m_ppkHashTable) return;  // no hash table allocated

    for (UInt32 ui = 0; ui < m_uiHashSize; ui++) 
    {
        while (m_ppkHashTable[ui])
        {
            NiTMapItem<TKEY, TVAL>* pkSave = m_ppkHashTable[ui];
            m_ppkHashTable[ui] = m_ppkHashTable[ui]->m_pkNext;
            ClearValue(pkSave);
            
            DeleteItem(pkSave);
        }
    }

    m_uiCount = 0;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline bool NiTMapBase<TKEY,TVAL>::GetAt(TKEY key, TVAL& val) const
{
    if (!m_ppkHashTable) return false;  // no hash table allocated
    // look up hash table location for key
    UInt32 uiIndex = KeyToHashIndex(key);
    NiTMapItem<TKEY, TVAL>* pkItem = m_ppkHashTable[uiIndex];

    // search list at hash table location for key
    while (pkItem)
    {
        if (IsKeysEqual(key, pkItem->m_key))
        {
            // item found
            val = pkItem->m_val;
            return true;
        }
        pkItem = pkItem->m_pkNext;
    }

    return false;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline UInt32 NiTMapBase<TKEY,TVAL>::KeyToHashIndex(TKEY key) const
{
    return (UInt32) (((size_t) key) % m_uiHashSize);
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline bool NiTMapBase<TKEY,TVAL>::IsKeysEqual(TKEY key1, TKEY key2) const
{
    return key1 == key2;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline void NiTMapBase<TKEY,TVAL>::SetValue(NiTMapItem<TKEY, TVAL>* pkItem, TKEY key, TVAL val)
{
    pkItem->m_key = key;
    pkItem->m_val = val;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline void NiTMapBase<TKEY, TVAL>::ClearValue(NiTMapItem<TKEY, TVAL>* /* pkItem */)
{
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline NiTMapIterator NiTMapBase<TKEY,TVAL>::GetFirstPos() const
{
    for (UInt32 ui = 0; ui < m_uiHashSize; ui++) 
    {
        if (m_ppkHashTable[ui])
            return m_ppkHashTable[ui];
    }
    return 0;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline void NiTMapBase<TKEY,TVAL>::GetNext(NiTMapIterator& pos, TKEY& key, TVAL& val) const
{
    NiTMapItem<TKEY, TVAL>* pkItem = (NiTMapItem<TKEY, TVAL>*) pos;
    
    key = pkItem->m_key;
    val = pkItem->m_val;

    if (pkItem->m_pkNext)
    {
        pos = pkItem->m_pkNext;
        return;
    }

    UInt32 ui = KeyToHashIndex(pkItem->m_key);
    for (++ui; ui < m_uiHashSize; ui++) 
    {
        pkItem = m_ppkHashTable[ui];
        if (pkItem) 
        {
            pos = pkItem;
            return;
        }
    }

    pos = 0;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline void NiTMapBase<TKEY,TVAL>::Resize(UInt32 uiNewHashSize)
{
    assert(uiNewHashSize > 0);

    // store old members
    NiTMapItem<TKEY,TVAL>** oldHashTable = m_ppkHashTable;
    UInt32 oldHashSize = m_uiHashSize;
    UInt32 oldCount = m_uiCount;    
    
    // re-initialize 
    m_uiHashSize = uiNewHashSize;
    m_uiCount = 0;
    UInt32 uiSize = sizeof(NiTMapItem<TKEY,TVAL>*) * m_uiHashSize;
    m_ppkHashTable = (NiTMapItem<TKEY,TVAL>**)MemoryHeap::FormHeapAlloc(uiSize);
    assert(m_ppkHashTable);
    memset(m_ppkHashTable, 0, uiSize);
    
    // Go through all entries in the old hash array and transfer to the new hash array.
    for (UInt32 ui = 0; ui < oldHashSize; ui++) 
    {
        while (oldHashTable[ui])
        {
            // Remove the entry from the old array
            NiTMapItem<TKEY, TVAL>* pkItem = oldHashTable[ui];
            oldHashTable[ui] = pkItem->m_pkNext;
            oldCount--;
            
            // Clear values in the entry
            pkItem->m_pkNext = 0;
            
            // Insert the entry in the new map
            UInt32 uiIndex = KeyToHashIndex(pkItem->m_key);
            pkItem->m_pkNext = m_ppkHashTable[uiIndex];
            m_ppkHashTable[uiIndex] = pkItem;
            m_uiCount++;
        }
    }

    // At the end, the existing hash array should be empty, and we can just delete it.
    assert(oldCount == 0);
    MemoryHeap::FormHeapFree(oldHashTable);
}

// NiTMap
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
const UInt32 NiTMap<TKEY, TVAL>::NUM_PRIMES = 18;
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
const UInt32 NiTMap<TKEY, TVAL>::PRIMES[18] = {
    7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749,
    65521, 131071, 262139, 524287, 999983};
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline NiTMap<TKEY, TVAL>::~NiTMap()
{
    // RemoveAll is called from here because it depends on virtual functions
    // implemented in NiTAllocatorMap.  It will also be called in the 
    // parent destructor, but the map will already be empty.
    NiTMap<TKEY, TVAL>::RemoveAll();
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline NiTMapItem<TKEY, TVAL>* NiTMap<TKEY, TVAL>::NewItem()
{
    return new NiTMapItem<TKEY, TVAL>;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline void NiTMap<TKEY, TVAL>::DeleteItem(NiTMapItem<TKEY, TVAL>* pkItem)
{
    // set key and val to zero so that if they are smart pointers
    // their references will be decremented.
    pkItem->m_val = 0;
    delete pkItem;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL>
inline UInt32 NiTMap<TKEY, TVAL>::NextPrime(const UInt32 uiTarget)
{
    UInt32 ui;
    for (ui = 0; ui < NUM_PRIMES - 1 && PRIMES[ui] < uiTarget; ui++) { }
    return PRIMES[ui];
}

// NiTPointerMap
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline NiTPointerMap<TKEY,TVAL>::~NiTPointerMap()
{
    // RemoveAll is called from here because it depends on virtual functions
    // implemented in NiTAllocatorMap.  It will also be called in the 
    // parent destructor, but the map will already be empty.
    NiTPointerMap<TKEY,TVAL>::RemoveAll();
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline NiTMapItem<TKEY, TVAL>* NiTPointerMap<TKEY, TVAL>::NewItem()
{
    return new NiTMapItem<TKEY, TVAL>;
}
//---------------------------------------------------------------------------
template <class TKEY, class TVAL> 
inline void NiTPointerMap<TKEY, TVAL>::DeleteItem(NiTMapItem<TKEY, TVAL>* pkItem)
{
    // set key and val to zero so that if they are smart pointers
    // their references will be decremented.
    pkItem->m_val = 0;
    delete pkItem;
}

// NiTStringTemplateMap
//---------------------------------------------------------------------------
template <class TPARENT, class TVAL>  
inline NiTStringTemplateMap<TPARENT,TVAL>::~NiTStringTemplateMap()
{
    if (m_bCopy)
    {
        for (unsigned int i = 0; i < TPARENT::m_uiHashSize; i++) 
        {
            NiTMapItem<const char*, TVAL>* pkItem = TPARENT::m_ppkHashTable[i];
            while (pkItem) 
            {
                NiTMapItem<const char*, TVAL>* pkSave = pkItem;
                pkItem = pkItem->m_pkNext;
                MemoryHeap::FormHeapFree((void*)pkSave->m_key);
            }
        }
    }
}
//---------------------------------------------------------------------------
template <class TPARENT, class TVAL> 
inline void NiTStringTemplateMap<TPARENT,TVAL>::SetValue(NiTMapItem<const char*, TVAL>* pkItem, const char* pcKey, TVAL val)
{
    if (m_bCopy)
    {
        size_t stLen = strlen(pcKey) + 1;
        pkItem->m_key = (char*)MemoryHeap::FormHeapAlloc(stLen);
        assert(pkItem->m_key);
        strcpy_s((char*)pkItem->m_key, stLen, pcKey);
    }
    else
    {
        pkItem->m_key = pcKey;
    }
    pkItem->m_val = val;
}
//---------------------------------------------------------------------------
template <class TPARENT, class TVAL>  
inline void NiTStringTemplateMap<TPARENT,TVAL>::ClearValue(NiTMapItem<const char*, TVAL>* pkItem)
{
    if (m_bCopy)
    {
        MemoryHeap::FormHeapFree((void*)pkItem->m_key);
    }
}
//---------------------------------------------------------------------------
template <class TPARENT, class TVAL> 
inline UInt32 NiTStringTemplateMap<TPARENT,TVAL>::KeyToHashIndex(const char* pKey) const
{
    UInt32 uiHash = 0;

    while (*pKey)
        uiHash = (uiHash << 5) + uiHash + *pKey++;

    return uiHash % m_uiHashSize;
}
//---------------------------------------------------------------------------
template <class TPARENT, class TVAL> 
inline bool NiTStringTemplateMap<TPARENT,TVAL>::IsKeysEqual(const char* pcKey1, const char* pcKey2) const
{
    return strcmp(pcKey1, pcKey2) == 0;
}
//---------------------------------------------------------------------------

