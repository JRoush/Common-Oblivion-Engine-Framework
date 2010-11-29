/* 
    Very simple templated LL 
    Often (always?) used for pointers, see notes on usage below
    Makes heavy use of inlining in game code

    [ NOTICE: This is a templated class ]
    Because it uses templates, this class requires a seperate set of imports for every specialization.  
    In practice, most of the specializations map back to a few basic methods, but we'd still need to
    import the appropriate symbols for each.  This makes it difficult to hook and patch - but it also means
    that no-one is likely to patch it, so the hooking is not strictly necessary.  We instead attempt to
    reproduce the original templated source code.
    CAUTIONS:
    -   Great care must be taken for some functions (e.g. hash compuation) to make sure they match the game code
    -   Because of simplication to template & inheritance structure, dynamic_cast may fail for this class

    Notes on Usage:
    TVAL MUST BE THE SAME SIZE AS A POINTER, AND ZERO (0x00000000) CANNOT BE A VALID VALUE
    The template class assumes that type TVAL has the following:
       1.  Default constructor that assigns zero, TVAL::TVAL() == 0x00000000
       2.  Copy constructor, TVAL::TVAL(const TVAL&);
       3.  Assignment, TVAL& operator=(const TVAL&);
       4.  Comparison, bool TKEY::operator==(const TKEY&)
       5.  Assignment and comparison to zero as an invalid value

   Because the first node is a member, it is allowed to be empty - denoted by value 0.  All add operations will fill
   this preferentially.  Any further nodes are assumed to contain valid (!= 0) values.  Loops over the node list must
   be aware of this, and check for valid node data.

   TODO: delete inline code & switch to imported
*/
#pragma once
#include "API/TES/MemoryHeap.h"

template <class TVAL> 
class IMPORTCLASS BSSimpleList
{// size 08/08
public:

    class Node
    {
    public:
        // Node members
        MEMBER /*00*/ TVAL      data;
        MEMBER /*04*/ Node*     next;
        // constructor
        INLINE Node(TVAL newData) : data(newData), next(0) {}
        // use FormHeap for class new & delete
        USEFORMHEAP
    };    

    // members
    Node            firstNode;      // 00

    // sorting function typedef
    typedef int (*CompareFunc)(TVAL dataA, TVAL dataB); // returns -1 (A<B), 0 (A==B), +1 (A>B)

    // methods
    INLINE bool     Empty();
    INLINE void     PushFront(TVAL data);
    INLINE void     PushBack(TVAL data);
    _NOUSE void     InsertedSorted(TVAL data, CompareFunc comparator);  // inserts such that all previous nodes are < new node
    INLINE void     Remove(TVAL data);
    INLINE void     Clear();
    INLINE bool     Find(TVAL data);
    INLINE UInt32   Count();

    // constructors (heavily inlined), destructor
    INLINE          BSSimpleList(TVAL firstData = 0) : firstNode(firstData) {}
    INLINE          ~BSSimpleList() {Clear();}
    // use FormHeap for class new & delete
    USEFORMHEAP 
};

#include "API\BSTypes\BSSimpleList.inl"