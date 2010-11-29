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

class IMPORTCLASS TESObjectREFR : public TESForm, public TESMemContextForm, public TESChildCell
{// size 58/60
/*
    Partial:
    -   virtual methods
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    enum DialogControlIDs // default control IDs
    {
        // Main Dialog
        IDC_EditorIDLabel               = 0x62E, // STATIC
        IDC_FormID                      = 0x631, // STATIC
        IDC_BaseFormLabel               = 0x62F, // STATIC
        IDC_BaseForm                    = 0x630, // STATIC
        IDC_EditBase                    = 0x5D1, // BUTTON
        IDC_Tabs                        = 0x6F1, // SysTabControl32
        IDC_PersistentReference         = 0x3EE, // BUTTON
        IDC_TurnOffFire                 = 0x3EF, // BUTTON
        IDC_VisibleWhenDistant          = 0x3F0, // BUTTON
        IDC_InitiallyDisabled           = 0xFB3, // BUTTON
        IDC_OpenByDefault               = 0xFB7, // BUTTON
        IDC_CastsShadows                = 0x697, // BUTTON
        IDC_SpaceLabel                  = 0x632, // STATIC
        IDC_Space                       = 0x633, // STATIC
        // Locked Tab
        IDC_Locked                      = 0x3F1, // BUTTON
        IDC_LockKey                     = 0x428, // COMBOBOX
        IDC_LockLeveled                 = 0x3F2, // BUTTON
        IDC_LockLevel                   = 0x401, // COMBOBOX
        // Teleport Tab
        IDC_Teleport                    = 0x5D0, // BUTTON
        IDC_SelectTeleportRef           = 0x7BE, // BUTTON
        IDC_TeleportCell                = 0x42A, // COMBOBOX
        IDC_TeleportRef                 = 0x7BF, // COMBOBOX
        IDC_ViewTeleportDoor            = 0x7C1, // BUTTON
        IDC_ViewTeleportMarker          = 0x7C0, // BUTTON
        // Ownership Tab
        IDC_OwnerNPC                    = 0x3FD, // COMBOBOX
        IDC_OwnerGlobalVariable         = 0x3FE, // COMBOBOX
        IDC_OwnerFaction                = 0x3FF, // COMBOBOX
        IDC_OwnerFactionRank            = 0x491, // COMBOBOX
        // Extra Data Tab
        IDC_ExtraCountLabel             = 0x62E, // STATIC
        IDC_ExtraCount                  = 0x402, // EDIT
        IDC_ExtraCountDefault           = 0x3FA, // BUTTON
        IDC_ExtraHealthLabel            = 0x62F, // STATIC
        IDC_ExtraHealth                 = 0x400, // EDIT
        IDC_ExtraHealthDefault          = 0x3FB, // BUTTON
        IDC_ExtraChargeLabel            = 0x62D, // STATIC
        IDC_ExtraCharge                 = 0x43D, // EDIT
        IDC_ExtraChargeDefault          = 0x3F9, // BUTTON
        IDC_ExtraTimeLeftLabel          = 0x630, // STATIC
        IDC_ExtraTimeLeft               = 0x435, // EDIT
        IDC_ExtraTimeLeftDefault        = 0x3FD, // BUTTON
        IDC_ExtraSoul                   = 0x4AD, // COMBOBOX
        IDC_ExtraSoulDefault            = 0x3FC, // BUTTON
        // Marker Tab
        IDC_MarkerData                  = 0x869, // BUTTON
        IDC_MarkerName                  = 0x424, // EDIT
        IDC_MarkerCamp                  = 0x40A, // BUTTON
        IDC_MarkerCave                  = 0x40B, // BUTTON
        IDC_MarkerMine                  = 0x40F, // BUTTON
        IDC_MarkerElvenRuin             = 0x40D, // BUTTON
        IDC_MarkerFortRuin              = 0x40E, // BUTTON
        IDC_MarkerLandmark              = 0x410, // BUTTON
        IDC_MarkerTavern                = 0x411, // BUTTON
        IDC_MarkerCity                  = 0x40C, // BUTTON
        IDC_MarkerSettlement            = 0x412, // BUTTON
        IDC_MarkerShrine                = 0x413, // BUTTON
        IDC_MarkerVisible               = 0x86A, // BUTTON
        IDC_MarkerFastTravel            = 0x402, // BUTTON
        IDC_MarkerOblivionGate          = 0x414, // BUTTON
        // Enable Parent Tab
        IDC_SelectEnableParentRef       = 0x8D9, // BUTTON
        IDC_EnableParentCell            = 0x8DA, // COMBOBOX
        IDC_EnableParentRef             = 0x8DB, // COMBOBOX
        IDC_ViewEnableParentRef         = 0x8D8, // BUTTON
        IDC_EnableParentLabel           = 0x8DC, // STATIC
        IDC_EnableParentOppositeState   = 0xFB4, // BUTTON
        // Leveled Creature Tab
        IDC_LeveledCreatureLevelMod     = 0x95A, // EDIT
    };
    static const UInt32     IDD_TESObjectREFR                       = 0x08D; // Dialog Tempate ID
    static const UInt32     IDD_TESObjectREFR_LockTab               = 0x101;
    static const UInt32     IDD_TESObjectREFR_TeleportTab           = 0x102;
    static const UInt32     IDD_TESObjectREFR_OwnershipTab          = 0x103;
    static const UInt32     IDD_TESObjectREFR_ExtraDataTab          = 0x104;
    static const UInt32     IDD_TESObjectREFR_MarkerTab             = 0xC97;
    static const UInt32     IDD_TESObjectREFR_EnableParentTab       = 0xCB0;
    static const UInt32     IDD_TESObjectREFR_LeveledCreatureTab    = 0xCBB;
    
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
    _NOUSE /*---/0F0*/ virtual void             UnkForm0F0(void* arg0);
    IMPORT /*---/0F4*/ virtual void             RemoveFormReference(TESForm& form);
    _NOUSE /*---/0F8*/ virtual bool             UnkForm0F8(UInt32 arg0);
    _NOUSE /*---/0FC*/ virtual void             UnkForm0FC(UInt32 arg0, BSStringT& output);
    _NOUSE /*---/100*/ virtual bool             UnkForm100(UInt32 arg0);
    IMPORT /*---/104*/ virtual bool             UpdateUsageInfo();
    IMPORT /*---/10C*/ virtual void             DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void             SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void             GetFromDialog(HWND dialog);
    IMPORT /*---/11C*/ virtual void             CleanupDialog(HWND dialog);
    #endif
   
    // TESObjectREFR virtual methods
    #ifdef OBLIVION
    _NOUSE /*0DC/---*/ virtual float            UnkRefr0DC(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
    #endif
    _NOUSE /*0E0/124*/ virtual bool             UnkRefr0E0();
    _NOUSE /*0E4/128*/ virtual void             UnkRefr0E4(bool newValue);
    #ifdef OBLIVION
    _NOUSE /*0E8/---*/ virtual bool             UnkRefr0E8(); 
    #endif
    IMPORT /*0EC/12C*/ virtual float            GetScale();
    IMPORT /*0F0/130*/ virtual Vector3          GetInitialRotation(); // 
                                                // actually 'Vector3& GetInitialRotation(Vector3& temp)', but the compiler handles the temp
    IMPORT /*0F4/134*/ virtual Vector3          GetInitialPosition(); // 
                                                // actually 'Vector3& GetInitialPosition(Vector3& temp)', but the compiler handles the temp 
    _NOUSE /*0F8/138*/ virtual Vector3          UnkRefr0F8(); // 
                                                // actually 'Vector3& GetInitialPosition(Vector3& temp)', but the compiler handles the temp 
    _NOUSE /*0FC/13C*/ virtual bool             UnkRefr0FC(); 
    _NOUSE /*100/140*/ virtual void             RemoveItem(TESForm* toRemove, void* extraList, UInt32 arg2, UInt32 arg3, UInt32 arg4, 
                                                    TESObjectREFR* destRef, UInt32 arg6, UInt32 arg7, UInt32 arg8, UInt8 arg9); // 
                                                // unk2 quantity?  need to decode remaining arguments
    _NOUSE /*104/144*/ virtual void             UnkRefr104(UInt32 arg0, UInt32 arg1, UInt32 arg2); 
    _NOUSE /*108/148*/ virtual bool             UnkRefr108(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3); 
    _NOUSE /*10C/14C*/ virtual UInt8            UnkRefr10C(UInt32 arg0, UInt32 arg1, UInt32 arg2); 
    _NOUSE /*110/150*/ virtual void             UnkRefr110(UInt32 arg0, UInt32 arg1); 
    #ifdef OBLIVION
    _NOUSE /*114/---*/ virtual void             UnkRefr114(UInt32 arg0, UInt32 arg1, UInt32 arg2); 
    _NOUSE /*118/---*/ virtual void             UnkRefr118(); 
    _NOUSE /*11C/---*/ virtual void             UnkRefr11C(UInt32 arg0); 
    IMPORT /*120/---*/ virtual MagicCaster*     GetMagicCaster();
    IMPORT /*124/---*/ virtual MagicTarget*     GetMagicTarget();
    #endif
    _NOUSE /*128/154*/ virtual UInt32           UnkRefr128(); 
    _NOUSE /*12C/158*/ virtual void             UnkRefr12C(UInt32 arg0); 
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
    _NOUSE /*15C/188*/ virtual Vector3          UnkRefr15C(); 
    _NOUSE /*160/18C*/ virtual void             UnkRefr160(); // returns AnimData* ?
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
    INLINE /*188/---*/ virtual bool             IsMobileObject() {return false;} // ? 
                                                // hard to confirm, since all children of TESObjectREFR are also children of MobileObect
    _NOUSE /*18C/---*/ virtual void             UnkRefr18C(void); 
    INLINE /*190/---*/ virtual bool             IsActor() {return false;} 
    #else
    _NOUSE /*---/1A8*/ virtual bool             UnkRefr1A8(); // could be IsMobileObject, or IsActor, or neither.  called during destruction
    #endif
    IMPORT /*194/1AC*/ virtual void             ChangeCell(TESObjectCELL* newCell); 
    IMPORT /*198/1B0*/ virtual bool             IsDead(); 
    _NOUSE /*19C/1B4*/ virtual bool             UnkRefr19C();
    _NOUSE /*1A0/1B4*/ virtual bool             UnkRefr1A0();

    // constructor
    IMPORT TESObjectREFR();
};