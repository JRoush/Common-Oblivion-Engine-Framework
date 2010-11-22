#include <cassert>

// NiTListBase
//---------------------------------------------------------------------------
template <class T> 
inline NiTListBase<T>::NiTListBase()
{
    m_uiCount = 0;
    m_pkHead = 0;
    m_pkTail = 0;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListBase<T>::~NiTListBase()
{
    // RemoveAll() must be called from derived classes as it relies on
    // the pure virtual DeleteItem(). If RemoveAll is called here without
    // being called in the derived class, R6025 runtime error will be
    // generated (pure virtual function called).
}
//---------------------------------------------------------------------------
template <class T> 
inline UInt32 NiTListBase<T>::GetSize() const
{
    return m_uiCount;
}
//---------------------------------------------------------------------------
template <class T> 
inline bool NiTListBase<T>::IsEmpty() const
{
    return m_uiCount == 0;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTListBase<T>::RemoveAll()
{
    NiTListItem<T>* pkCurr = m_pkHead;
    while (pkCurr)
    {
        NiTListItem<T>* pkDel = pkCurr;
        pkCurr = pkCurr->m_pkNext;
        DeleteItem(pkDel);
    }

    m_uiCount = 0;
    m_pkHead = 0;
    m_pkTail = 0;
} 
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTListBase<T>::GetHeadPos() const
{
    return m_pkHead;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTListBase<T>::GetTailPos() const
{
    return m_pkTail;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTListBase<T>::
    GetNextPos(NiTListIterator kPos) const
{
    return kPos ? ((NiTListItem<T>*) kPos)->m_pkNext : 0;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTListBase<T>::
    GetPrevPos(NiTListIterator kPos) const
{
    return kPos ? ((NiTListItem<T>*) kPos)->m_pkPrev : 0;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTListBase<T>::AddNodeHead(NiTListItem<T>* pkNode)
{
    pkNode->m_pkPrev = 0;
    pkNode->m_pkNext = m_pkHead;
    
    if (m_pkHead)
        m_pkHead->m_pkPrev = pkNode;
    else
        m_pkTail = pkNode;
    
    m_pkHead = pkNode;
    m_uiCount++;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTListBase<T>::AddNodeTail(NiTListItem<T>* pkNode)
{
    pkNode->m_pkNext = 0;
    pkNode->m_pkPrev = m_pkTail;
    
    if (m_pkTail)
        m_pkTail->m_pkNext = pkNode;
    else
        m_pkHead = pkNode;
    
    m_pkTail = pkNode;
    m_uiCount++;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTListBase<T>::
    AddNodeBefore(NiTListIterator kPos, NiTListItem<T>* pkNode)
{
    NiTListItem<T>* pkNext = (NiTListItem<T>*) kPos;

    pkNode->m_pkNext = pkNext;
    pkNode->m_pkPrev = pkNext->m_pkPrev;
    
    if (pkNext->m_pkPrev)
        pkNext->m_pkPrev->m_pkNext = pkNode;
    else
        m_pkHead = pkNode;

    pkNext->m_pkPrev = pkNode;

    m_uiCount++;
    return pkNode;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTListBase<T>::
    AddNodeAfter(NiTListIterator kPos, NiTListItem<T>* pkNode)
{
    NiTListItem<T>* pkPrev = (NiTListItem<T>*) kPos;

    pkNode->m_pkPrev = pkPrev;
    pkNode->m_pkNext = pkPrev->m_pkNext;
    
    if (pkPrev->m_pkNext)
        pkPrev->m_pkNext->m_pkPrev = pkNode;
    else
        m_pkTail = pkNode;

    pkPrev->m_pkNext = pkNode;

    m_uiCount++;
    return pkNode;
}
//---------------------------------------------------------------------------

// NiTPointerListBase
//---------------------------------------------------------------------------
template <class T> 
inline NiTPointerListBase<T>::~NiTPointerListBase() 
{
    // RemoveAll is called from here because it depends on virtual functions
    // implemented in the derived class.
    // It will also be called in the parent destructor, 
    // but the map will already be empty.
    NiTPointerListBase<T>::RemoveAll();
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListItem<T>* NiTPointerListBase<T>::NewItem()
{
    return new NiTListItem<T>;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTPointerListBase<T>::DeleteItem(NiTListItem<T>* pkItem)
{
    pkItem->m_element = 0;
    delete pkItem;
}
//---------------------------------------------------------------------------
template <class T> 
inline const T& NiTPointerListBase<T>::GetHead() const
{
    return NiTListBase<T>::m_pkHead->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline const T& NiTPointerListBase<T>::GetTail() const
{
    return NiTListBase<T>::m_pkTail->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline const T& NiTPointerListBase<T>::GetNext(NiTListIterator& kPos)
    const
{
    assert(kPos != 0);

    const T& element= ((NiTListItem<T>*) kPos)->m_element;

    kPos = ((NiTListItem<T>*) kPos)->m_pkNext;
    return element;
}
//---------------------------------------------------------------------------
template <class T> 
inline const T& NiTPointerListBase<T>::GetPrev(NiTListIterator& kPos)
    const
{
    assert(kPos != 0);

    const T& element= ((NiTListItem<T>*) kPos)->m_element;
    kPos = ((NiTListItem<T>*) kPos)->m_pkPrev;

    return element;
}
//---------------------------------------------------------------------------
template <class T> 
inline const T& NiTPointerListBase<T>::Get(NiTListIterator kPos) const
{
    assert(kPos != 0);
    return ((NiTListItem<T>*) kPos)->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T NiTPointerListBase<T>::RemoveHead()
{
    assert((NiTListBase<T>::m_pkHead));

    NiTListItem<T>* pkNode = NiTListBase<T>::m_pkHead;

    NiTListBase<T>::m_pkHead = NiTListBase<
        T>::m_pkHead->m_pkNext;
    
    if (NiTListBase<T>::m_pkHead)
        NiTListBase<T>::m_pkHead->m_pkPrev = 0;
    else
        NiTListBase<T>::m_pkTail = 0;
    
    T element = pkNode->m_element;
    
    DeleteItem(pkNode);

    NiTListBase<T>::m_uiCount--;

    return element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T NiTPointerListBase<T>::RemoveTail()
{
    assert((NiTListBase<T>::m_pkTail));
    
    NiTListItem<T>* pkNode = NiTListBase<T>::m_pkTail;

    NiTListBase<T>::m_pkTail = NiTListBase<
        T>::m_pkTail->m_pkPrev;
    
    if (NiTListBase<T>::m_pkTail)
        NiTListBase<T>::m_pkTail->m_pkNext = 0;
    else
        NiTListBase<T>::m_pkHead = 0;
    
    T element = pkNode->m_element;
    
    DeleteItem(pkNode);

    NiTListBase<T>::m_uiCount--;
    
    return element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T NiTPointerListBase<T>::RemovePos(NiTListIterator& kPos)
{
    NiTListItem<T>* pkNode = (NiTListItem<T>*) kPos;

    assert(pkNode);

    if (pkNode == NiTListBase<T>::m_pkHead)
    {
        kPos = pkNode->m_pkNext; // kPos points to new head
        return RemoveHead();
    }
    if (pkNode == NiTListBase<T>::m_pkTail)
    {
        kPos = 0; // kPos has walked off end of list
        return RemoveTail();
    }

    NiTListItem<T>* pkPrev = pkNode->m_pkPrev;
    NiTListItem<T>* pkNext = pkNode->m_pkNext;

    kPos = pkNext;
    
    if (pkPrev)
        pkPrev->m_pkNext = pkNext;
    if (pkNext)
        pkNext->m_pkPrev = pkPrev;
    
    T element = pkNode->m_element;
    
    DeleteItem(pkNode);
    
    NiTListBase<T>::m_uiCount--;
    
    return element;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTPointerListBase<T>::InsertAfter(NiTListIterator kPos, const T& element)
{
    assert(kPos);

    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    return NiTListBase<T>::AddNodeAfter(kPos,pkNode);
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTPointerListBase<T>::
    InsertBefore(NiTListIterator kPos, const T& element)
{
    assert(kPos);

    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    return NiTListBase<T>::AddNodeBefore(kPos,pkNode);
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTPointerListBase<T>::AddHead(const T& element)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;
    
    NiTListBase<T>::AddNodeHead(pkNode);
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTPointerListBase<T>::AddTail(const T& element)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    pkNode->m_element = element;

    NiTListBase<T>::AddNodeTail(pkNode);
}
//---------------------------------------------------------------------------
template <class T> 
inline T NiTPointerListBase<T>::Remove(const T& element)
{
    NiTListIterator kPos = NiTPointerListBase<T>::FindPos(element);

    return kPos ? RemovePos(kPos) : element;
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTPointerListBase<T>::FindPos(const T& element, NiTListIterator kStart) const
{
    if (kStart == 0)
        kStart = NiTPointerListBase<T>::GetHeadPos();
    
    while (kStart)
    {
        NiTListIterator kPos = kStart;
        if (element == GetNext(kStart))
            return kPos;
    }
    return 0;
}
//---------------------------------------------------------------------------

// NiTObjectList
//---------------------------------------------------------------------------
template <class T> 
inline NiTObjectList<T>::~NiTObjectList()
{
    // RemoveAll is called from here because it depends on virtual functions
    // implemented in NiTAllocatorMap.  It will also be called in the 
    // parent destructor, but the map will already be empty.
    NiTObjectList<T>::RemoveAll();
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::AddNewHead()
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();
    
    NiTListBase<T>::AddNodeHead(pkNode);

    return &pkNode->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::AddNewTail()
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    NiTListBase<T>::AddNodeTail(pkNode);

    return &pkNode->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::InsertNewBefore(NiTListIterator kPos, 
    NiTListIterator& kNewIterator)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    kNewIterator = 
        NiTListBase<T>::AddNodeBefore(kPos,pkNode);

    return &pkNode->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::InsertNewAfter(NiTListIterator kPos, 
    NiTListIterator& kNewIterator)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    kNewIterator = 
        NiTListBase<T>::AddNodeAfter(kPos,pkNode);

    return &pkNode->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::InsertNewBefore(NiTListIterator kPos)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();

    NiTListBase<T>::AddNodeBefore(kPos,pkNode);

    return &pkNode->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::InsertNewAfter(NiTListIterator kPos)
{
    NiTListItem<T>* pkNode;
    pkNode = (NiTListItem<T>*) NewItem();
    
    NiTListBase<T>::AddNodeAfter(kPos,pkNode);

    return &pkNode->m_element;
}//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::GetHead() const
{
    return NiTListBase<T>::m_pkHead ? 
        &NiTListBase<T>::m_pkHead->m_element : NULL;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::GetTail() const
{
    return NiTListBase<T>::m_pkTail ?
        &NiTListBase<T>::m_pkTail->m_element : NULL;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::GetNext(NiTListIterator& kPos) const 
{
    if (kPos == 0)
        return NULL;

    T* pkElement = &((NiTListItem<T>*) kPos)->m_element;
    kPos = ((NiTListItem<T>*) kPos)->m_pkNext;
    return pkElement;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::GetPrev(NiTListIterator& kPos) const
{
    if (kPos == 0)
        return NULL;

    T* pkElement = &((NiTListItem<T>*) kPos)->m_element;
    kPos = ((NiTListItem<T>*) kPos)->m_pkPrev;
    return pkElement;
}
//---------------------------------------------------------------------------
template <class T> 
inline T* NiTObjectList<T>::Get(NiTListIterator kPos) const
{
    if (!kPos)
        return NULL;
    
    return &((NiTListItem<T>*) kPos)->m_element;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTObjectList<T>::RemoveHead()
{
    assert((NiTListBase<T>::m_pkHead));

    NiTListItem<T>* pkNode = NiTListBase<T>::m_pkHead;

    NiTListBase<T>::m_pkHead = 
        NiTListBase<T>::m_pkHead->m_pkNext;
    
    if (NiTListBase<T>::m_pkHead)
        NiTListBase<T>::m_pkHead->m_pkPrev = 0;
    else
        NiTListBase<T>::m_pkTail = 0;
    
    DeleteItem(pkNode);

    NiTListBase<T>::m_uiCount--;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTObjectList<T>::RemoveTail()
{
    assert((NiTListBase<T>::m_pkTail));
    
    NiTListItem<T>* pkNode = NiTListBase<T>::m_pkTail;

    NiTListBase<T>::m_pkTail =
        NiTListBase<T>::m_pkTail->m_pkPrev;
    
    if (NiTListBase<T>::m_pkTail)
        NiTListBase<T>::m_pkTail->m_pkNext = 0;
    else
        NiTListBase<T>::m_pkHead = 0;
    
    DeleteItem(pkNode);

    NiTListBase<T>::m_uiCount--;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTObjectList<T>::RemovePos(NiTListIterator& kPos)
{
    NiTListItem<T>* pkNode = (NiTListItem<T>*) kPos;

    assert(pkNode);

    if (pkNode == NiTListBase<T>::m_pkHead)
    {
        kPos = pkNode->m_pkNext; // kPos points to new head
        RemoveHead();
        
    }
    else if (pkNode == NiTListBase<T>::m_pkTail)
    {
        kPos = 0; // kPos has walked off end of list
        RemoveTail();
    }
    else
    {
        NiTListItem<T>* pkPrev = pkNode->m_pkPrev;
        NiTListItem<T>* pkNext = pkNode->m_pkNext;

        kPos = pkNext;
    
        if (pkPrev)
            pkPrev->m_pkNext = pkNext;
        if (pkNext)
            pkNext->m_pkPrev = pkPrev;
        
        DeleteItem(pkNode);
    
        NiTListBase<T>::m_uiCount--;
    }
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTObjectList<T>::Remove(const T* element)
{
    NiTListIterator kPos = FindPos(element);
    if (kPos)
        RemovePos(kPos);
}
//---------------------------------------------------------------------------
template <class T> 
inline NiTListIterator NiTObjectList<T>::FindPos(const T* element, 
    NiTListIterator kStart) const
{
    if (kStart == 0)
        kStart = NiTObjectList<T>::GetHeadPos();
    
    while (kStart)
    {
        NiTListIterator kPos = kStart;
        if (element == GetNext(kStart))
            return kPos;
    }
    return 0;
}       
//---------------------------------------------------------------------------
template <class T> 
inline NiTListItem<T>* NiTObjectList<T>::NewItem()
{
    return new NiTListItem<T>;
}
//---------------------------------------------------------------------------
template <class T> 
inline void NiTObjectList<T>::DeleteItem(NiTListItem<T>* pkItem)
{
    delete pkItem;
}
//---------------------------------------------------------------------------