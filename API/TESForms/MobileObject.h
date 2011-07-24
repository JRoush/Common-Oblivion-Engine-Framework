/* 
    MobileObject is the base class for all object references which need regular processing, i.e. Actors and Projectiles.
    It's defined only in the game, as all references are treated identically in the CS.

    Each instance of a MobileObject has an attached Process.
*/
#pragma once
#ifdef OBLIVION

// base classes
#include "API/TESForms/TESObjectREFR.h"

// argument classes
class   BaseProcess;    // GameProcess/BaseProcess.h

class IMPORTCLASS MobileObject : public TESObjectREFR
{// size 5C/--
/*
    Partial:
    -   virtual methods
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
    -   NOUSE virtual methods are defined as stubs here so that the compiler will not object to derived classes
*/
public:

    // members
    //     /*00*/ TESForm          
    //     /*18*/ TESChildCell
    //     /*1C*/ TESMemContextForm
    //     /*1C*/ TESForm*          baseForm;
    //     /*20*/ Vector3           rotation;
    //     /*2C*/ Vector3           position;
    //     /*38*/ float             scale;
    //     /*3C*/ NiNode*           niNode;
    //     /*40*/ TESObjectCELL*    parentCell;
    //     /*44*/ ExtraDataList     extraData;
    MEMBER /*58*/ BaseProcess*      process;

    // TESForm virtual method overrides
    IMPORT /*010/034*/ virtual                  ~MobileObject();
    IMPORT /*04C/---*/ virtual UInt16           ModifiedFormSize(UInt32 modifiedMask);
    IMPORT /*050/---*/ virtual void             SaveModifiedForm(UInt32 modifiedMask);
    IMPORT /*054/---*/ virtual void             LoadModifiedForm(UInt32 modifiedMask, UInt32 unkFlags);
    IMPORT /*058/---*/ virtual void             LinkModifiedForm(UInt32 modifiedMask, UInt32 arg1);
    IMPORT /*05C/---*/ virtual void             PostLinkModifiedForm(UInt32 modifiedMask, UInt32 arg1);
    IMPORT /*060/---*/ virtual void             PreLoadModifiedForm(UInt32 modifiedMask);
    IMPORT /*08C/090*/ virtual void             SetDeleted(bool deleted);

    // TESObjectREFR virtual method overrides
    _NOUSE /*144/170*/ virtual bool             UnkRefr144() {return 0;} 
    _NOUSE /*148/174*/ virtual UInt8            UnkRefr148() {return 0;}
    IMPORT /*14C/178*/ virtual NiNode*          GenerateNiNode();
    _NOUSE /*158/184*/ virtual Vector3          UnkRefr158() {return Vector3();} 
    _NOUSE /*15C/188*/ virtual Vector3          UnkRefr15C(UInt32 arg0) {return Vector3();} 
    _NOUSE /*178/1A4*/ virtual void             UnkRefr178(bool arg0) {return;} 
    INLINE /*188/1A8*/ virtual bool             IsMobileObject() {return true;}
    IMPORT /*194/1AC*/ virtual void             ChangeCell(TESObjectCELL* newCell); 

    // no TESChildCell overrides

    // additional virtual methods
    _NOUSE /*1A4*/ virtual bool             UnkMobileObject1A4() {return 0;} // called when object is moved to high process?
    _NOUSE /*1A8*/ virtual bool             UnkMobileObject1A8() {return 0;} 
    _NOUSE /*1AC*/ virtual bool             UnkMobileObject1AC() {return 0;}  
    _NOUSE /*1B0*/ virtual bool             UnkMobileObject1B0() {return 0;}
    _NOUSE /*1B4*/ virtual UInt32           Move(float arg0, Vector3& arg1, UInt32 arg2) {return 0;}
    _NOUSE /*1B8*/ virtual UInt32           Jump() {return 0;} 
    _NOUSE /*1BC*/ virtual void             UnkMobileObject1BC(UInt32 arg0) {return;}  
    _NOUSE /*1C0*/ virtual void             UnkMobileObject1C0(UInt32 arg0) {return;}
    _NOUSE /*1C4*/ virtual bool             UnkMobileObject1C4() {return 0;} 
    _NOUSE /*1C8*/ virtual bool             UnkMobileObject1C8() {return 0;} 
    _NOUSE /*1CC*/ virtual void             UnkMobileObject1CC(UInt32 arg0) {return;}  
    _NOUSE /*1D0*/ virtual void             UnkMobileObject1D0(Vector3& arg0) {return;}
    _NOUSE /*1D4*/ virtual void             UnkMobileObject1D4(UInt32 arg0) {return;} 
    _NOUSE /*1D8*/ virtual float            UnkMobileObject1D8() {return 0;} 
    _NOUSE /*1DC*/ virtual void             UnkMobileObject1DC() {return;}  
    IMPORT /*1E0*/ virtual float            GetZRotation(); // heading in XY plane
    _NOUSE /*1E4*/ virtual void             UnkMobileObject1E4(TESObjectREFR& arg0) {return;} 
    _NOUSE /*1E8*/ virtual void             UnkMobileObject1E8(float arg0) {return;}
    _NOUSE /*1EC*/ virtual void             UnkMobileObject1EC(float arg0) {return;}  
    _NOUSE /*1F0*/ virtual bool             UnkMobileObject1F0() {return 0;}
    IMPORT /*1F4*/ virtual float            GetJumpScale(); 
    _NOUSE /*1F8*/ virtual bool             UnkMobileObject1F8() {return 0;} 
    _NOUSE /*1FC*/ virtual bool             UnkMobileObject1FC() {return 0;}  
    _NOUSE /*200*/ virtual void             UnkMobileObject200(bool arg0) {return;} 

    // constructor
    IMPORT MobileObject();
};

#endif  // end ifdef OBLIVION