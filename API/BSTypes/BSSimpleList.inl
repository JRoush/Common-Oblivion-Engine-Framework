
//******************** BSSimpleList **************************
template <class TVAL>
inline bool BSSimpleList<TVAL>::Empty()
{
    return (firstNode.data == 0 && firstNode.next == 0);
}

template <class TVAL>
inline void BSSimpleList<TVAL>::Clear()
{
    // destroy nodes
    while (firstNode.next)
    {
        Node* n = firstNode.next;
        firstNode.next = n->next;
        delete n;
    }
    // clear data for first node to invalid
    firstNode.data = 0;
}

template <class TVAL>
inline void BSSimpleList<TVAL>::PushFront(TVAL data)
{
    if (data == 0) return;
    if (firstNode.data != 0)
    {
        Node* n = new Node(firstNode.data);
        n->next = firstNode.next;
        firstNode.next = n;
    }
    firstNode.data = data;
}

template <class TVAL>
inline void BSSimpleList<TVAL>::PushBack(TVAL data)
{
    if (data == 0) return;
    if (firstNode.data == 0)
    {
        firstNode.data = data;
        return;       
    }
    Node* n = &firstNode;
    while (n->next) n = n->next;
    n->next = new Node(data);
}

template <class TVAL>
inline void BSSimpleList<TVAL>::Remove(TVAL data)
{
    if (data == 0 || Empty()) return;

    Node* cur = firstNode.next;
    if (data == firstNode.data)
    {
        if (cur == 0) 
        {
            firstNode.data = 0; 
            return;
        }
        else 
        {
            firstNode.data = cur->data;
            firstNode.next = cur->next;
            delete cur;
            return;
        }
    }

    for (Node* prev = &firstNode; cur; cur = cur->next)
    {
        if (data == cur->data)
        {
            prev->next = cur->next;
            delete cur;
            return;
        }
        prev = cur;        
    }

}
template <class TVAL>
inline bool BSSimpleList<TVAL>::Find(TVAL data)
{
    if (data == 0 || Empty()) return false;
    for (Node* n = &firstNode; n; n = n->next)
    {
        if (n->data == data) return true;
    }
    return false;
}
template <class TVAL>
inline UInt32 BSSimpleList<TVAL>::Count()
{
    if (Empty()) return 0;
    UInt32 size = 0;
    for (Node* n = &firstNode; n; n = n->next)
    {
        if (n->data != 0) size++;
    }
    return size;
}