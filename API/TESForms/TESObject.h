/* 
    TESObject is the parent for all inventory items & placed object refs.  Most(all?) TESObjects are connected 
    in a doubly-linked list, maintained by the global data handler.

    TESBoundObject is a subclass, used for (I think) all objects that can be independently rendered.
*/
#pragma once

// base classes
#include "API/TESForms/TESForm.h"
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   TESObject;
class   TESObjectCELL;  // GameWorld/TESObjectCELL.h

class IMPORTCLASS TESObjectListHead 
{// size 10/10 - control structure for object list.  Seems to be BoundObjects in game, but includes static objects in CS.
/*
    Partial:
    -   members
*/
public:
    // members
    MEMBER /*00*/ UInt32        objectCount;
    MEMBER /*04*/ TESObject*    first;
    MEMBER /*08*/ TESObject*    last;
    MEMBER /*0C*/ UInt32        unkC;
};

class IMPORTCLASS TESObject : public TESForm
{// size 024/034
/*
    Partial:
    -   members
    -   virtual methods
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    // members
    MEMBER /*18/24*/    TESObjectListHead*  head;
    #ifndef OBLIVION
    MEMBER /*--/28*/    void*               unkObj28;   // for ref counting of loaded models (?)
    #endif
    MEMBER /*1C/2C*/    TESObject*          prev;
    MEMBER /*20/30*/    TESObject*          next;   

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual              ~TESObject();
    _NOUSE /*0A8/0AC*/ virtual bool         UnkForm0A8() {return false;} //

    // additional virtual methods
    _NOUSE /*0DC/124*/ virtual UInt32       UnkObj0DC() {return 0;}
    _NOUSE /*0E0/128*/ virtual bool         UnkObj0E0() {return false;}
    INLINE /*0E4/12C*/ virtual bool         IsObjectAutoCalc() {return false;} // magicka, cost, attribute autocalcs
    INLINE /*0E8/130*/ virtual void         SetObjectAutoCalc(bool autoCalc) {} // magicka, cost, attribute autocalcs
    _NOUSE /*0EC/134*/ virtual UInt32       UnkObj0EC(UInt32 arg0, UInt32 arg1) {return 0;}
    _NOUSE /*0F0/138*/ virtual void         UnkObj0F0(UInt32 arg0) {}
    _NOUSE /*0F4/13C*/ virtual bool         UnkObj0F4() {return false;}
    _NOUSE /*0F8/140*/ virtual void         UnkObj0F8() {}
    _NOUSE /*0FC/144*/ virtual bool         UnkObj0FC() {return false;}
    _NOUSE /*100/148*/ virtual bool         UnkObj100(UInt32 arg0) {return false;}
    _NOUSE /*104/14C*/ virtual void         UnkObj104(UInt32 arg0) {}
    _NOUSE /*108/150*/ virtual UInt32       UnkObj108() {return 0;}
    _NOUSE /*10C/154*/ virtual UInt32       UnkObj10C() {return 0;}
    _NOUSE /*110/158*/ virtual void         UnkObj110(UInt32 arg0) {}

    // constructor
    _NOUSE TESObject() {}   // inlined by game, differs between game & CS
};

#ifdef OBLIVION
#define TESBoundObjectParents       public TESObject
#else
class IMPORTCLASS TESCellUseList
{// size --/08 ? - size may be as large as 20, but probably is no larger than 08
public:

    struct CellUseInfo
    {
        TESObjectCELL*  cell;
        UInt32          count;
    };
    
    // members
    MEMBER /*00/00*/ BSSimpleList<CellUseInfo*>     cellUses;

    // methods
    IMPORT void         IncreaseUseCount(TESObjectCELL* cell, UInt32 count);
    IMPORT void         DecreaseUseCount(TESObjectCELL* cell, UInt32 count);
    IMPORT void         Clear();
    IMPORT void         CopyFrom(const TESCellUseList& copyFrom); // WARNING: does NOT duplicate CellUseInfo, just copies pointers

    // construction, destruction
    IMPORT TESCellUseList();
    IMPORT ~TESCellUseList();

    // use FormHeap for class new & delete
    USEFORMHEAP 
};
#define TESBoundObjectParents       public TESObject, public TESCellUseList
#endif

class IMPORTCLASS TESBoundObject : TESBoundObjectParents // parents are TESObject, + TESCellUseList in CS
{// size 24/58
/*  
    Partial:
    -   members
    -   virtual methods
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public: 

    // members
    //     /*00/00*/ TESObject
    #ifndef OBLIVION
    //     /*--/34*/ TESCellUseList
    MEMBER /*--/3C*/ UInt32             unkBoundObj3C;
    MEMBER /*--/40*/ UInt32             unkBoundObj40;
    MEMBER /*--/44*/ UInt32             unkBoundObj44;
    MEMBER /*--/48*/ UInt32             unkBoundObj48;
    MEMBER /*--/4C*/ UInt32             unkBoundObj4C;
    MEMBER /*--/50*/ UInt32             unkBoundObj50;
    MEMBER /*--/54*/ UInt32             unkBoundObj54; // object (container) use count ?
    #endif

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual              ~TESBoundObject();
    _NOUSE /*0A4/0A8*/ virtual bool         UnkForm0A4() {return true;} // 
    #ifdef OBLIVION
    _NOUSE /*0CC/---*/ virtual bool         UnkForm0CC(TESObjectREFR* arg0, TESObjectREFR* arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4) {return false;}
    #else
    _NOUSE /*0B0/0B4*/ virtual UInt32       UnkForm0B0() {return 0;} // 
    #endif

    // TESObject virtual methods
    _NOUSE /*0EC/134*/ virtual UInt32       UnkObj0EC(UInt32 arg0, UInt32 arg1) {return 0;}
    _NOUSE /*0FC/144*/ virtual bool         UnkObj0FC() {return false;}
    #ifndef OBLIVION
    _NOUSE /*108/150*/ virtual UInt32       UnkObj108() {return 0;}
    _NOUSE /*10C/154*/ virtual UInt32       UnkObj10C() {return 0;}
    #endif

    // additional virtual methods
    _NOUSE /*114/15C*/ virtual UInt32       UnkBoundObj114(UInt32 arg0, UInt32 arg1) {return 0;}
    _NOUSE /*118/160*/ virtual bool         UnkBoundObj118(UInt32 arg0) {return false;}
    _NOUSE /*11C/164*/ virtual void         UnkBoundObj11C(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3,
                                                            UInt32 arg4, UInt32 arg5, UInt32 arg6, UInt32 arg7) {}

    // constructor
    IMPORT TESBoundObject();
    
    // use FormHeap for class new & delete
    USEFORMHEAP 
};

class TESBoundAnimObject : public TESBoundObject
{// size 24/58
/*  
    Partial:
    -   virtual methods
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:
    
    // no additional members or virtual methods

    // TESObject virtual methods
    _NOUSE /*0E0/128*/ virtual bool         UnkObj0E0() {return true;}

    // TESBoundObject virtual methods
    _NOUSE /*118/160*/ virtual bool         UnkBoundObj118(UInt32 arg0) {return false;}
    
    // uses TESBoundObject constructor + destructor
};
