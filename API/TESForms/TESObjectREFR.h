/* 
    TESObjectREFR is the parent for all 'instances' of base forms in the game world
    While each ref is a distinct form, it also points back to the 'base' form that it instantiates
    Refs store local data like position, size, flags, etc.
*/
#pragma once

// base classes
#include "API/TESForms/TESForm.h"
#include "API/ExtraData/ExtraDataList.h"
#include "API/GameWorld/TESObjectCELL.h" // TESChildCell

// argument classes
class   Vector3;        // NiVector3?  currently defined in Utilities/ITypes.h
class   NiNode;
class   ActorAnimData;  // TODO: need to decode ActorAnimData
class   MagicCaster;    // Magic/MagicCaster.h
class   MagicTarget;    // Magic/MagicTarget.h
class   TESTopic;

class IMPORTCLASS TESObjectREFR : public TESForm, public TESMemContextForm, public TESChildCell
{// size 58/60
/*
    Partial:
    -   virtual methods
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    enum FormFlags
    {        
        kFormFlags_TurnOffFire          = /*07*/ 0x00000080,   // light sources only
        kFormFlags_CastShadows          = /*09*/ 0x00000200,   // light sources only
        kFormFlags_Disabled             = /*0B*/ 0x00000800,   
        kFormFlags_VisibleWhenDistant   = /*0F*/ 0x00008000,
    };
    
    enum ModifiedFlags
    {// some of these may be specific to derived classes, and should be moved there
        kModified_ParentCell            = /*02*/ 0x00000004, 
        kModified_HavokMove             = /*03*/ 0x00000008, // CHANGE_REFR_HAVOK_MOVE
        kModified_Scale                 = /*04*/ 0x00000010, // CHANGE_REFR_SCALE        
        kModified_ExtraOwner            = /*07*/ 0x00000080, // CHANGE_OBJECT_EXTRA_OWNER
        kModified_MapMarkerFlags        = /*0A*/ 0x00000400, // CHANGE_MAPMARKER_EXTRA_FLAGS
        kModified_HadHavokMoveFlag      = /*0B*/ 0x00000800, // CHANGEFLAG_REFR_HAD_HAVOK_MOVE_FLAG - ?
        kModified_IsEmpty               = /*10*/ 0x00010000, // CHANGE_OBJECT_EMPTY - no data? (only if not Actor?)        
        kModified_DoorOpenDefaultState  = /*12*/ 0x00040000, // CHANGE_DOOR_OPEN_DEFAULT_STATE
        kModified_DoorOpenState         = /*13*/ 0x00080000, // CHANGE_DOOR_OPEN_STATE        
        kModified_DoorExtraTeleport     = /*14*/ 0x00100000, // CHANGE_DOOR_EXTRA_TELEPORT
        kModified_Animation             = /*19*/ 0x02000000, // CHANGE_REFR_ANIMATION - (only if not Actor?)
        kModified_Inventory             = /*1B*/ 0x08000000, // CHANGE_REFR_INVENTORY - see 0048BA40
    };

    // members
    //     /*00/00*/ TESForm          
    //     /*18/24*/ TESChildCell
    //     /*1C/28*/ TESMemContextForm - empty, no members
    MEMBER /*1C/28*/ TESForm*           baseForm;
    MEMBER /*20/2C*/ Vector3            rotation;
    MEMBER /*2C/38*/ Vector3            position;
    MEMBER /*38/44*/ float              scale;
    #ifdef OBLIVION
    MEMBER /*3C/--*/ NiNode*            niNode;
    #endif
    MEMBER /*40/48*/ TESObjectCELL*     parentCell;
    MEMBER /*44/4C*/ ExtraDataList      extraData;

    // TESChildCell virtual methods
    IMPORT /*000/000*/ virtual TESObjectCELL*    GetChildCell();

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual                  ~TESObjectREFR();
    IMPORT /*014/038*/ virtual void             InitializeAllComponents();
    IMPORT /*018/03C*/ virtual void             ClearAllComponentReferences();
    IMPORT /*01C/040*/ virtual bool             LoadForm(TESFile& file);
    IMPORT /*024/048*/ virtual void             SaveFormChunks();
    IMPORT /*030/054*/ virtual bool             LessThanGroup(const RecordInfo& groupRecord);
    IMPORT /*034/058*/ virtual bool             LessThan(TESForm& form);
    IMPORT /*038/05C*/ virtual TESForm*         Clone(bool autoEditorID, NiTMap<TESForm*,TESForm*>* cloneMap);
    _NOUSE /*03C/060*/ virtual void             UnkForm03C(UInt32 arg0);
    #ifdef OBLIVION
    IMPORT /*04C/---*/ virtual UInt16           ModifiedFormSize(UInt32 modifiedMask);
    IMPORT /*050/---*/ virtual void             SaveModifiedForm(UInt32 modifiedMask);
    IMPORT /*054/---*/ virtual void             LoadModifiedForm(UInt32 modifiedMask, UInt32 unkFlags);
    IMPORT /*058/---*/ virtual void             LinkModifiedForm(UInt32 modifiedMask, UInt32 arg1);
    IMPORT /*05C/---*/ virtual void             PostLinkModifiedForm(UInt32 modifiedMask, UInt32 arg1);
    IMPORT /*060/---*/ virtual void             PreLoadModifiedForm(UInt32 modifiedMask);
    #endif
    IMPORT /*06C/070*/ virtual void             LinkForm();
    IMPORT /*070/074*/ virtual UInt8            GetFormType();
    IMPORT /*074/078*/ virtual void             GetDebugDescription(BSStringT& output);
    IMPORT /*078/07C*/ virtual bool             IsQuestItem();
    _NOUSE /*088/08C*/ virtual bool             UnkForm088();
    IMPORT /*08C/090*/ virtual void             SetDeleted(bool deleted);
    IMPORT /*090/094*/ virtual void             SetFromActiveFile(bool fromActiveFile);
    INLINE /*094/098*/ virtual void             SetQuestItem(bool questItem) {return;} // does nothing for TESObjectREFR
    IMPORT /*0B4/0B8*/ virtual void             CopyFrom(TESForm& form);
    IMPORT /*0B8/0BC*/ virtual bool             CompareTo(TESForm& compareTo);
    IMPORT /*0BC/0C0*/ virtual bool             MatchGroupRecord(const RecordInfo& groupRecord, bool matchAllLevels, bool arg2);
    IMPORT /*0C0/0C4*/ virtual void             CreateGroupRecord(RecordInfo& newGroupRecord, RecordInfo& currentGroupRecord);
    IMPORT /*0D4/0C8*/ virtual const char*      GetEditorID();
    #ifndef OBLIVION
    _NOUSE /*---/0D4*/ virtual bool             UnkForm0D4();
    IMPORT /*---/0F0*/ virtual void             BuildFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/0F4*/ virtual void             RemoveFormReference(TESForm& referencedForm);
    IMPORT /*---/0F8*/ virtual bool             FormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList);
    IMPORT /*---/0FC*/ virtual void             GetRevisionUnmatchedFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output);
    _NOUSE /*---/100*/ virtual bool             UnkForm100(UInt32 arg0);
    IMPORT /*---/104*/ virtual bool             UpdateUsageInfo();
    IMPORT /*---/10C*/ virtual bool             DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void             SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void             GetFromDialog(HWND dialog);
    IMPORT /*---/11C*/ virtual void             CleanupDialog(HWND dialog);
    #endif
   
    // TESObjectREFR virtual methods
    #ifdef OBLIVION
    _NOUSE /*0DC/---*/ virtual float            UnkRefr0DC(TESTopic& topic, TESObjectREFR* speaker, bool arg2, bool arg3, UInt32 arg4); // Say?
                                                // Invoked by Say command when target is not an actor.  Returns number of seconds required to speak lines.
    #endif
    IMPORT /*0E0/124*/ virtual bool             GetCastsShadows();
    IMPORT /*0E4/128*/ virtual void             SetCastsShadows(bool castsShadows);
    #ifdef OBLIVION
    INLINE /*0E8/---*/ virtual bool             IsProjectile() { return false; } // returns true for arrows and magic projectiles
    #endif
    IMPORT /*0EC/12C*/ virtual float            GetScale();
    IMPORT /*0F0/130*/ virtual Vector3          GetInitialRotation(); // 
                                                // actually 'Vector3& GetInitialRotation(Vector3& temp)', but the compiler handles the temp
    IMPORT /*0F4/134*/ virtual Vector3          GetInitialPosition(); // 
                                                // actually 'Vector3& GetInitialPosition(Vector3& temp)', but the compiler handles the temp 
    IMPORT /*0F8/138*/ virtual void             MoveInitialPosition(Vector3 position); // moves starting position to specified coords
                                                // or current position if coords are (0,0,0)
    IMPORT /*0FC/13C*/ virtual bool             UpdateLights(); // updates flicker effects for refs that are light sources.  not sure what return value means
    IMPORT /*100/140*/ virtual bool             RemoveItem(TESForm* toRemove, ExtraDataList* entryExtra, SInt32 count, // which form and subgroup                                                   
                                                    bool useContainerOwnership = false, // copies this container owner to transfered items w/o an owner
                                                    bool drop = false, // items dropped in game world, rather than transfered or destroyed
                                                    TESObjectREFR* destRef = 0, // target container for transfer
                                                    Vector3* destPosition = 0, Vector3* destRotation = 0, // coordinates for drop (default is near this ref)
                                                    bool arg8 = true, // unk?
                                                    bool useExistingEntryExtra = false); // use of existing entry ExtraDataList if entryExtra == 0
    IMPORT /*104/144*/ virtual void             RemoveItemByType(UInt32 formType, bool useContainerOwnership, SInt32 count); // remove forms of specified type
    IMPORT /*108/148*/ virtual bool             EquipItem(TESForm* toEquip, SInt32 count, ExtraDataList* entryExtra, bool noUnequip); // 
                                                // noUnequip has same effect as argument to script command
    IMPORT /*10C/14C*/ virtual bool             UnequipItem(TESForm* toEquip, SInt32 count, ExtraDataList* entryExtra);
    _NOUSE /*110/150*/ virtual void             UnkRefr110(UInt32 arg0, UInt32 arg1); 
    #ifdef OBLIVION
    IMPORT /*114/---*/ virtual void             AddItem(TESForm* item, ExtraDataList* entryExtra, UInt32 count); // add item to container ref
    _NOUSE /*118/---*/ virtual void             UnkRefr118(); 
    _NOUSE /*11C/---*/ virtual void             UnkRefr11C(UInt32 arg0); 
    IMPORT /*120/---*/ virtual MagicCaster*     GetMagicCaster();
    IMPORT /*124/---*/ virtual MagicTarget*     GetMagicTarget();
    #endif
    INLINE /*128/154*/ virtual TESForm*         GetTemplateForm() { return 0; } // template forms for refs spawned by LevCreature lists
    INLINE /*12C/158*/ virtual void             SetTemplateForm(TESForm* templateForm) {} 
    _NOUSE /*130/15C*/ virtual UInt32           UnkRefr130(UInt32 arg0); 
    _NOUSE /*134/160*/ virtual UInt32           UnkRefr134(UInt32 arg0); 
    _NOUSE /*138/164*/ virtual UInt32           UnkRefr138(UInt32 arg0); 
    _NOUSE /*13C/168*/ virtual UInt32           UnkRefr13C(UInt32 arg0); 
    _NOUSE /*140/16C*/ virtual bool             UnkRefr140();
    _NOUSE /*144/170*/ virtual bool             UnkRefr144(); 
    _NOUSE /*148/174*/ virtual UInt8            UnkRefr148(); // inits animation-related data, and more
    IMPORT /*14C/178*/ virtual NiNode*          GenerateNiNode(); // ?
    IMPORT /*150/17C*/ virtual void             Set3D(NiNode* niNode); // ?
    IMPORT /*154/180*/ virtual NiNode*          GetNiNode();
    _NOUSE /*158/184*/ virtual Vector3          UnkRefr158(); 
    _NOUSE /*15C/188*/ virtual Vector3          UnkRefr15C(UInt32 arg0); 
    IMPORT /*160/18C*/ virtual void             UpdateNiNode(); // updates lights, animations.
    _NOUSE /*164/190*/ virtual ActorAnimData*   GetAnimationData();
    _NOUSE /*168/194*/ virtual void*            UnkRefr168(); // returns some kind of ExtraData
    _NOUSE /*16C/198*/ virtual void             UnkRefr16C(UInt32 arg0); 
    IMPORT /*170/19C*/ virtual TESForm*         GetBaseForm();
    IMPORT /*174/1A0*/ virtual Vector3&         GetPosition();
    _NOUSE /*178/1A4*/ virtual void             UnkRefr178(bool arg0); 
    #ifdef OBLIVION
    _NOUSE /*17C/---*/ virtual void             UnkRefr17C(UInt32 arg0); 
    _NOUSE /*180/---*/ virtual void             UnkRefr180(UInt32 arg0);
    _NOUSE /*184/---*/ virtual void             UnkRefr184(bool arg0); 
    #endif
    INLINE /*188/1A8*/ virtual bool             IsMobileObject() {return false;} // hard to confirm, since all children of TESObjectREFR are MobileObjects
    #ifdef OBLIVION
    _NOUSE /*18C/---*/ virtual UInt32           UnkRefr18C(); // returns 0 for REFR, mobile obj, returns process.Unk36C() for actor, 
                                                // which returns 0 for low-midlow proc and sign-extended process.Unk11D for midhigh-high proc
                                                // some kind of enum value
    INLINE /*190/---*/ virtual bool             IsActor() {return false;} 
    #endif
    IMPORT /*194/1AC*/ virtual void             ChangeCell(TESObjectCELL* newCell); 
    IMPORT /*198/1B0*/ virtual bool             IsDead(bool arg0); // arg0 = count deathstate 6 as dead, for actors.  ignored for base class
    INLINE /*19C/1B4*/ virtual bool             HasFatigue() {return false;} // always false for base class
    INLINE /*1A0/1B4*/ virtual bool             IsParalyzed() {return false;} // always false for base class

    // constructor
    IMPORT TESObjectREFR();
};