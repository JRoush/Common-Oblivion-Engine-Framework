/*
    ExtraData is Bethesda's tool for attaching arbitrary information to other classes.
    BaseExtraList seems to be the generic extra data manager, and is used for DialogExtraData
    ExtraDataList seems to be targeted specifically to forms (?)
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"

// argument classes
class   BSExtraData;    // ExtraData/BSExtraData.h

class IMPORTCLASS BaseExtraList
{// size 14/14
public:

    // members
    //     /*00/00*/ void**         vtbl          
    MEMBER /*04/04*/ BSExtraData*   extraList; // LL of extra data nodes
    MEMBER /*08/08*/ UInt8          extraTypes[0x0C];    // if a bit is set, then the extralist should contain that extradata
                                    // bits are numbered starting from the lsb

    // virtual methods
    IMPORT /*00/00*/ virtual        ~BaseExtraList(); // destroys all data in list.  The vtbl entry is actually the
                                    // compiler-generated 'vector deleting destructor', which calls this method

    // methods
    IMPORT void             AddExtra(BSExtraData* data); // inserts in list and updates type bitfield
    IMPORT void             RemoveExtra(BSExtraData* data, bool destroy); // removes from list & updates type bitfield, optionally destroys node
    IMPORT void             RemoveExtra(UInt32 extraType); // removes & destroys all extra data of the specified type
    IMPORT void             RemoveAllExtra(bool destroy); // clears extraList* and bitfield, optionally destroys all the extra nodes
    IMPORT UInt32           ExtraCount();
    IMPORT BSExtraData*     GetExtra(UInt32 extraType);
    IMPORT BSExtraData*     GetPrevExtraData(UInt32 extraType); // returns extra node *before* the node of the specified type

    // constructor
    INLINE BaseExtraList() : extraList(0) {memset(extraTypes,0,sizeof(extraTypes));} // importable in CS but completely inlined by game

    // use FormHeap for class new & delete
    USEFORMHEAP
};

class IMPORTCLASS ExtraDataList : public BaseExtraList
{// size 14/14
/*
    Todo:
    -   Decaode methods, determine difference between this and BaseExtraList
*/
public:
    
    // no additional members

    // no additional virtual methods, no vmethod overrides

    // methods
        // ...

    // constructor
    IMPORT ExtraDataList();
};