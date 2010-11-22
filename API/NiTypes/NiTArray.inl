#include <cassert>

//  NiTArray
//---------------------------------------------------------------------------
template <class TVAL>
inline NiTArray<TVAL>::NiTArray(UInt16 uiMaxSize, UInt16 uiGrowBy)
    : m_pBase(0), 
    m_usMaxSize(0), 
    m_usSize(0), 
    m_usESize(0), 
    m_usGrowBy(static_cast<UInt16>(uiGrowBy))
{
    assert(uiGrowBy <= USHRT_MAX);

    SetSize(uiMaxSize);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline NiTArray<TVAL>::~NiTArray()
{
    if (m_pBase) MemoryHeap::FormHeapFree(m_pBase);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL* NiTArray<TVAL>::GetBase()
{
    return m_pBase;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::GetSize() const
{
    return m_usSize;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::GetEffectiveSize() const
{
    return m_usESize;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::GetAllocatedSize() const
{
    return m_usMaxSize;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::GetGrowBy() const
{
    return m_usGrowBy;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::SetSize(UInt16 uiMaxSize)
{
    assert(uiMaxSize <= USHRT_MAX);
    UInt16 usMaxSize = (UInt16)(uiMaxSize & 0xFFFF);

    if (usMaxSize == m_usMaxSize)
    {
        return;
    }

    // If the number of slots gets smaller, the elements in the unwanted
    // slots must be zeroed in case class TVAL has side effects that must occur.
    // For example, if TVAL is a smart pointer class, then decrementing the ref
    // count must occur.
    UInt16 i;
    if (usMaxSize < m_usSize)
    {
        for (i = usMaxSize; i < m_usSize; i++)
        {
            if (m_pBase[i] != TVAL(0))
            {
                m_pBase[i] = TVAL(0);
                m_usESize--;
            }
        }

        m_usSize = usMaxSize;
    }

    TVAL* pSaveBase = m_pBase;
    m_usMaxSize = usMaxSize;
    if (usMaxSize > 0)
    {
        // allocate a new array
        m_pBase = (TVAL*)MemoryHeap::FormHeapAlloc(m_usMaxSize * sizeof(TVAL));
        assert(m_pBase);

        // copy old array to new array
        for (i = 0; i < m_usSize; i++)
        {
            m_pBase[i] = pSaveBase[i];
        }
        // initialize new memory
        for (i = m_usSize; i < m_usMaxSize; i++)
        {
            m_pBase[i] = TVAL(0);  
        }

        
    }
    else
    {
        m_pBase = 0;
    }

    // delete old array
    if (pSaveBase) MemoryHeap::FormHeapFree(pSaveBase);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::SetGrowBy(UInt16 uiGrowBy)
{
    assert(uiGrowBy <= USHRT_MAX);
    UInt16 usGrowBy = (UInt16)(uiGrowBy & 0xFFFF);
    m_usGrowBy = usGrowBy;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline const TVAL& NiTArray<TVAL>::GetAt(UInt16 uiIndex) const
{
    assert(uiIndex < m_usMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL& NiTArray<TVAL>::GetAt(UInt16 uiIndex)
{
    assert(uiIndex < m_usMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL& NiTArray<TVAL>::operator [](UInt16 uiIndex) const
{
    assert(uiIndex < m_usMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::SetAt(UInt16 uiIndex, const TVAL& element)
{
    assert(uiIndex < m_usMaxSize);
    UInt16 usIndex = (UInt16)(uiIndex & 0xFFFF);

    if (usIndex >= m_usSize)
    {
        m_usSize = usIndex+1;
        if (element != TVAL(0))
        {
            m_usESize++;
        }
    }
    else 
    {
        if (element != TVAL(0))
        {
            if (m_pBase[usIndex] == TVAL(0))
            {
                m_usESize++;
            }
        }
        else
        {
            if (m_pBase[usIndex] != TVAL(0))
            {
                m_usESize--;
            }
        }
    }

    m_pBase[usIndex] = element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::SetAtGrow(UInt16 uiIndex, const TVAL& element)
{
    assert(uiIndex <= USHRT_MAX);
    UInt16 usIndex = (UInt16)(uiIndex & 0xFFFF);

    if (usIndex >= m_usMaxSize)
    {
        SetSize(usIndex + m_usGrowBy);
    }

    SetAt(usIndex, element);
    return usIndex;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::Add(const TVAL& element)
{
    return SetAtGrow(m_usSize, element);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::AddFirstEmpty(const TVAL& element)
{
    if (element == TVAL(0))
    {
        return 0xffffffff;
    }

    for (UInt16 i = 0; i < m_usSize; i++)
    {
        if (m_pBase[i] == TVAL(0))
        {
            // empty slot - add here
            m_pBase[i] = element;
            m_usESize++;
            return i;
        }
    }

    // no empty slots - add at end
    return SetAtGrow(m_usSize, element);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::Insert(UInt16 uiBeforeIndex, const TVAL& element)
{
    assert(uiBeforeIndex <= USHRT_MAX);

    // make sure it is a good size
    UInt16 usNewSize = 
        (UInt16)uiBeforeIndex > (m_usSize + 1) ? 
        (UInt16)uiBeforeIndex : (m_usSize + 1);
    if (usNewSize >= m_usMaxSize)
    {  
        SetSize(usNewSize + m_usGrowBy);
    }

    // Copy all the array
    for (UInt16 i = m_usSize; i > (UInt16)uiBeforeIndex; i--)
    {
        m_pBase[i] = m_pBase[i - 1];
    }

    m_pBase[uiBeforeIndex] = element;
    m_usSize++;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTArray<TVAL>::RemoveAt(UInt16 uiIndex)
{
    if (uiIndex >= m_usSize)
    {
        return TVAL(0);
    }
    
    TVAL element = m_pBase[uiIndex];
    m_pBase[uiIndex] = TVAL(0);

    if (element != TVAL(0))
    {
        m_usESize--;
    }

    if (uiIndex == (UInt16)(m_usSize - 1))
    {
        m_usSize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTArray<TVAL>::RemoveAtAndFill(UInt16 uiIndex)
{
    if (uiIndex >= m_usSize)
    {
        return TVAL(0);
    }
    
    m_usSize--;
    TVAL element = m_pBase[uiIndex];

    m_pBase[uiIndex] = m_pBase[m_usSize];
    m_pBase[m_usSize] = TVAL(0);

    if (element != TVAL(0))
    {
        m_usESize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTArray<TVAL>::RemoveEnd()
{
    if (m_usSize == 0)
        return TVAL(0);

    m_usSize--;
    TVAL element = m_pBase[m_usSize];
    m_pBase[m_usSize] = TVAL(0);

    if (element != TVAL(0))
    {
        m_usESize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::RemoveAll()
{
    // The elements in the to-be-removed slots must be zeroed in case class
    // TVAL has side effects that must occur.  For example, if TVAL is a smart
    // pointer class, then decrementing the ref count must occur.
    for (UInt16 i = 0; i < m_usSize; i++)
    {
        m_pBase[i] = TVAL(0);
    }

    m_usSize = 0;
    m_usESize = 0;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::Remove(const TVAL& element)
{
    if (element != TVAL(0))
    {
        for (UInt16 i = 0; i < m_usSize; i++)
        {
            if (m_pBase[i] == element)
            {
                m_pBase[i] = TVAL(0);

                m_usESize--;
                if (i == m_usSize - 1)
                    m_usSize--;

                return i;
            }
        }
    }

    return (UInt16)~0;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTArray<TVAL>::Delete(UInt16 uiIndex)
{
    if (uiIndex >= m_usSize)
    {
        return TVAL(0);
    }

    TVAL element = m_pBase[uiIndex];

    for (UInt16 i = (UInt16)uiIndex; i < m_usSize - 1; i++)
    {
        m_pBase[i] = m_pBase[i + 1];
    }

    // Decrement the size
    m_usSize--;

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt16 NiTArray<TVAL>::Find(const TVAL& element, UInt16 uiStartIdx)
{
    if (element != TVAL(0))
    {
        for (UInt16 i = (UInt16)uiStartIdx; i < m_usSize; i++)
        {
            if (m_pBase[i] == element)
            {
                return i;
            }
        }
    }

    return (UInt16)~0;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::Compact()
{
    if (m_usESize == m_usMaxSize)
    {
        return;
    }

    // move elements to contiguous memory at beginning of array
    if (m_usESize)
    {
        for (UInt16 i = 0, j = 0; i < m_usSize; i++)
        {
            if (m_pBase[i] != TVAL(0))
            {
                if (m_pBase[j] != m_pBase[i])
                {
                    m_pBase[j] = m_pBase[i];
                }
                j++;
            }
        }
    }

    // downsize storage
    TVAL* pSaveBase = m_pBase;
    m_usSize = m_usESize;
    m_usMaxSize = m_usSize;
    if (m_usMaxSize > 0)
    {
        m_pBase = (TVAL*)MemoryHeap::FormHeapAlloc(m_usMaxSize * sizeof(TVAL));
        assert(m_pBase);

        // copy old array to new array
        for (UInt16 i = 0; i < m_usSize; i++)
        {
            m_pBase[i] = pSaveBase[i];
        }
    }
    else
    {
        m_pBase = 0;
    }

    // delete old array
    if (pSaveBase) MemoryHeap::FormHeapFree(pSaveBase);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTArray<TVAL>::UpdateSize()
{
    while (m_usSize > 0)
    {
        if (m_pBase[m_usSize - 1] != TVAL(0))
        {
            break;
        }

        m_usSize--;
    }
}
//---------------------------------------------------------------------------

//  NiTLargeArray
//---------------------------------------------------------------------------
template <class TVAL>
inline NiTLargeArray<TVAL>::NiTLargeArray(UInt32 uiMaxSize, UInt32 uiGrowBy)
: m_pBase(0), 
    m_uiMaxSize(0), 
    m_uiSize(0), 
    m_uiESize(0), 
    m_uiGrowBy(uiGrowBy)
{
    SetSize(uiMaxSize);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline NiTLargeArray<TVAL>::~NiTLargeArray()
{
    if (m_pBase) MemoryHeap::FormHeapFree(m_pBase);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL* NiTLargeArray<TVAL>::GetBase()
{
    return m_pBase;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::GetSize() const
{
    return m_uiSize;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::GetEffectiveSize() const
{
    return m_uiESize;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::GetAllocatedSize() const
{
    return m_uiMaxSize;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::GetGrowBy() const
{
    return m_uiGrowBy;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTLargeArray<TVAL>::SetSize(UInt32 uiMaxSize)
{
    

    if (uiMaxSize == m_uiMaxSize)
    {
        return;
    }

    // If the number of slots gets smaller, the elements in the unwanted
    // slots must be zeroed in case class TVAL has side effects that must occur.
    // For example, if TVAL is a smart pointer class, then decrementing the ref
    // count must occur.
    UInt32 i;
    if (uiMaxSize < m_uiSize)
    {
        for (i = (UInt32)uiMaxSize; i < m_uiSize; i++)
        {
            if (m_pBase[i] != TVAL(0))
            {
                m_pBase[i] = TVAL(0);
                m_uiESize--;
            }
        }

        m_uiSize = uiMaxSize;
    }

    TVAL* pSaveBase = m_pBase;
    m_uiMaxSize = uiMaxSize;
    if (uiMaxSize > 0)
    {
        // allocate a new array
        m_pBase = (TVAL*)MemoryHeap::FormHeapAlloc(m_uiMaxSize * sizeof(TVAL));
        assert(m_pBase);

        // copy old array to new array
        for (i = 0; i < m_uiSize; i++)
        {
            m_pBase[i] = pSaveBase[i];
        }
        // initialize new memory
        for (i = m_uiSize; i < m_uiMaxSize; i++)
        {
            m_pBase[i] = TVAL(0);  
        }

        
    }
    else
    {
        m_pBase = 0;
    }

    // delete old array
    if (pSaveBase) MemoryHeap::FormHeapFree(pSaveBase);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTLargeArray<TVAL>::SetGrowBy(UInt32 uiGrowBy)
{
    m_uiGrowBy = uiGrowBy;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline const TVAL& NiTLargeArray<TVAL>::GetAt(UInt32 uiIndex) const
{
    assert(uiIndex < m_uiMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL& NiTLargeArray<TVAL>::GetAt(UInt32 uiIndex)
{
    assert(uiIndex < m_uiMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL& NiTLargeArray<TVAL>::operator [](UInt32 uiIndex) const
{
    assert(uiIndex < m_uiMaxSize);
    return m_pBase[uiIndex];
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTLargeArray<TVAL>::SetAt(UInt32 uiIndex, const TVAL& element)
{
    assert(uiIndex < m_uiMaxSize);

    if (uiIndex >= m_uiSize)
    {
        m_uiSize = uiIndex+1;
        if (element != TVAL(0))
        {
            m_uiESize++;
        }
    }
    else 
    {
        if (element != TVAL(0))
        {
            if (m_pBase[uiIndex] == TVAL(0))
            {
                m_uiESize++;
            }
        }
        else
        {
            if (m_pBase[uiIndex] != TVAL(0))
            {
                m_uiESize--;
            }
        }
    }

    m_pBase[uiIndex] = element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::SetAtGrow(UInt32 uiIndex, const TVAL& element)
{
    if (uiIndex >= m_uiMaxSize)
    {
        SetSize(uiIndex + m_uiGrowBy);
    }

    SetAt(uiIndex, element);
    return uiIndex;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::Add(const TVAL& element)
{
    return SetAtGrow(m_uiSize, element);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::AddFirstEmpty(const TVAL& element)
{
    if (element == TVAL(0))
    {
        return 0xffffffff;
    }

    for (UInt32 i = 0; i < m_uiSize; i++)
    {
        if (m_pBase[i] == TVAL(0))
        {
            // empty slot - add here
            m_pBase[i] = element;
            m_uiESize++;
            return i;
        }
    }

    // no empty slots - add at end
    return SetAtGrow(m_uiSize, element);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTLargeArray<TVAL>::RemoveAt(UInt32 uiIndex)
{
    if (uiIndex >= m_uiSize)
    {
        return TVAL(0);
    }
    
    TVAL element = m_pBase[uiIndex];
    m_pBase[uiIndex] = TVAL(0);

    if (element != TVAL(0))
    {
        m_uiESize--;
    }

    if (uiIndex == m_uiSize - 1)
    {
        m_uiSize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTLargeArray<TVAL>::RemoveAtAndFill(UInt32 uiIndex)
{
    if (uiIndex >= m_uiSize)
    {
        return TVAL(0);
    }
    
    m_uiSize--;
    TVAL element = m_pBase[uiIndex];

    m_pBase[uiIndex] = m_pBase[m_uiSize];
    m_pBase[m_uiSize] = TVAL(0);

    if (element != TVAL(0))
    {
        m_uiESize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline TVAL NiTLargeArray<TVAL>::RemoveEnd()
{
    if (m_uiSize == 0)
        return TVAL(0);

    m_uiSize--;
    TVAL element = m_pBase[m_uiSize];
    m_pBase[m_uiSize] = TVAL(0);

    if (element != TVAL(0))
    {
        m_uiESize--;
    }

    return element;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTLargeArray<TVAL>::RemoveAll()
{
    // The elements in the to-be-removed slots must be zeroed in case class
    // TVAL has side effects that must occur.  For example, if TVAL is a smart
    // pointer class, then decrementing the ref count must occur.
    for (UInt32 i = 0; i < m_uiSize; i++)
    {
        m_pBase[i] = TVAL(0);
    }

    m_uiSize = 0;
    m_uiESize = 0;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline UInt32 NiTLargeArray<TVAL>::Remove(const TVAL& element)
{
    if (element != TVAL(0))
    {
        for (UInt32 i = 0; i < m_uiSize; i++)
        {
            if (m_pBase[i] == element)
            {
                m_pBase[i] = TVAL(0);

                m_uiESize--;
                if (i == m_uiSize - 1)
                    m_uiSize--;

                return i;
            }
        }
    }

    return (UInt32)~0;
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTLargeArray<TVAL>::Compact()
{
    if (m_uiESize == m_uiMaxSize)
    {
        return;
    }

    // move elements to contiguous memory at beginning of array
    if (m_uiESize)
    {
        for (UInt32 i = 0, j = 0; i < m_uiSize; i++)
        {
            if (m_pBase[i] != TVAL(0))
            {
                if (m_pBase[j] != m_pBase[i])
                {
                    m_pBase[j] = m_pBase[i];
                }
                j++;
            }
        }
    }

    // downsize storage
    TVAL* pSaveBase = m_pBase;
    m_uiSize = m_uiESize;
    m_uiMaxSize = m_uiSize;
    if (m_uiMaxSize > 0)
    {
        m_pBase = (TVAL*)MemoryHeap::FormHeapAlloc(m_uiMaxSize * sizeof(TVAL));
        assert(m_pBase);

        // copy old array to new array
        for (UInt32 i = 0; i < m_uiSize; i++)
        {
            m_pBase[i] = pSaveBase[i];
        }
    }
    else
    {
        m_pBase = 0;
    }

    // delete old array
    if (pSaveBase) MemoryHeap::FormHeapFree(pSaveBase);
}
//---------------------------------------------------------------------------
template <class TVAL>
inline void NiTLargeArray<TVAL>::UpdateSize()
{
    while (m_uiSize > 0)
    {
        if (m_pBase[m_uiSize - 1] != TVAL(0))
        {
            break;
        }

        m_uiSize--;
    }
}
//---------------------------------------------------------------------------
