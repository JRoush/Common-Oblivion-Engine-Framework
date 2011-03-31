/* 
    TESAIForm is a base component controlling animation functionality.
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h"
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   TESLevItem;
class   TESFaction;
class   TESActorBase;

class IMPORTCLASS TESActorBaseData : public BaseFormComponent
{// size 020/020
public:

    enum ModifiedFlags
	{
		kModified_ActorBaseInfo =	0x00000010, // non-faction data
		kModified_BaseFactions =	0x00000040,
	};

    enum NPCFlags
    {
        kNPCFlag_Female                 = 0x00000001,
        kNPCFlag_Essential              = 0x00000002,
        kNPCFlag_Respawn                = 0x00000008,
        kNPCFlag_AutoCalcStats          = 0x00000010,
        kNPCFlag_PCLevelOffset          = 0x00000080,
        kNPCFlag_NoLowProc              = 0x00000200,
        kNPCFlag_NoRumors               = 0x00002000,
        kNPCFlag_Summonable             = 0x00004000,
        kNPCFlag_NoPersuasion           = 0x00008000,
        kNPCFlag_CanCorpseCheck         = 0x00100000,
    };

    enum CreatureFlags
    {
        kCreatureFlag_Biped             = 0x00000001,
        kCreatureFlag_Essential         = 0x00000002,
        kCreatureFlag_WeaponAndShield   = 0x00000004,
        kCreatureFlag_Respawn           = 0x00000008,
        kCreatureFlag_Swims             = 0x00000010,
        kCreatureFlag_Flies             = 0x00000020,
        kCreatureFlag_Walks             = 0x00000040,
        kCreatureFlag_PCLevelOffset     = 0x00000080,
        kCreatureFlag_HasSounds         = 0x00000100,   // has a CreatureSoundArray (instead of a TESCreature* for inherited sounds)
        kCreatureFlag_NoLowProc         = 0x00000200,
        kCreatureFlag_NoBloodParticle   = 0x00000800,
        kCreatureFlag_NoBloodTexture    = 0x00001000,
        kCreatureFlag_NoRumors          = 0x00002000,
        kCreatureFlag_Summonable        = 0x00004000,
        kCreatureFlag_NoHead            = 0x00008000,
        kCreatureFlag_NoRightArm        = 0x00010000,
        kCreatureFlag_NoLeftArm         = 0x00020000,
        kCreatureFlag_NoCombatInWater   = 0x00040000,
        kCreatureFlag_NoShadow          = 0x00080000,
        kCreatureFlag_NoCorpseCheck     = 0x00100000,   // inverse of corresponding flag for NPCs
    };

    struct FactionInfo
    {// size 08
        TESFaction*     faction;    // 00
        SInt8           rank;       // 04
        UInt8           pad[3];     // 05
        // use FormHeap for class new & delete
        USEFORMHEAP 
    };

    typedef BSSimpleList<FactionInfo*> FactionListT;

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ UInt32         actorFlags; 
    MEMBER /*08/08*/ UInt16         magicka; // init to 50
    MEMBER /*0A/0A*/ UInt16         fatigue; // init to 50
    MEMBER /*0C/0C*/ UInt16         barterGold;
    MEMBER /*0E/0E*/ UInt16         level; // init to 1
    MEMBER /*10/10*/ UInt16         minLevel;    // if PCLevelOffset
    MEMBER /*12/12*/ UInt16         maxLevel;    // if PCLevelOffset
    MEMBER /*14/14*/ TESLevItem*    deathItem;
    MEMBER /*18/18*/ FactionListT   factionList; 

    // virtual method overrides:
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*004/004*/ virtual void         ClearComponentReferences(); // doesn't clear deathItem ref
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION
    IMPORT /*---/010*/ virtual void         BuildComponentFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/014*/ virtual void         RemoveComponentFormRef(TESForm& referencedForm); // doesn't check deathItem ref
    IMPORT /*---/018*/ virtual bool         ComponentFormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList);
    IMPORT /*---/01C*/ virtual void         GetRevisionUnmatchedComponentFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output);
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);                    
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    IMPORT /*---/030*/ virtual void         ComponentDlgCleanup(HWND dialog);
    #endif

    // additional virtual methods
    // for Creature-only (NPC-only) flags & fields, these functions return the default value or do nothing to NPCs (Creatures)
    INLINE /*010/034*/ virtual bool         GetNoPersuasion() {return true;}
    INLINE /*014/038*/ virtual bool         GetCanCorpseCheck() {return true;}
    INLINE /*018/03C*/ virtual bool         GetNoLeftArm() {return false;}
    INLINE /*01C/040*/ virtual bool         GetNoRightArm() {return false;}
    INLINE /*020/044*/ virtual bool         GetNoHead() {return false;}
    INLINE /*024/048*/ virtual bool         GetNoShadow() {return false;}
    INLINE /*028/04C*/ virtual bool         GetNoBloodParticle() {return false;}
    INLINE /*02C/050*/ virtual void         SetNoBloodParticle(bool newVal) {}
    INLINE /*030/054*/ virtual bool         GetNoBloodTexture() {return false;}
    INLINE /*034/058*/ virtual void         SetNoBloodTexture(bool newVal) {}
    IMPORT /*038/05C*/ virtual const char*  GetBloodTexturePath();
    INLINE /*03C/060*/ virtual void         SetBloodTexturePath(const char* ddsPath) {}
    IMPORT /*040/064*/ virtual const char*  GetBloodParticlePath();
    INLINE /*044/068*/ virtual void         SetBloodParticlePath(const char* nifPath) {}
    IMPORT /*048/06C*/ virtual UInt16       GetMagicka();
    IMPORT /*04C/070*/ virtual UInt16       GetFatigue();
    #ifndef OBLIVION
    IMPORT /*---/074*/ virtual void         InitializeDialogLevelControls(HWND dialog); // configure controls based on state of PCLevelOffset flag
    #endif
    IMPORT /*050/078*/ virtual void         MarkComponentModified(UInt32 modifiedMask); // does nothing in CS, calls parent form method in game
    #ifdef OBLIVION   
    IMPORT /*054/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask);
    IMPORT /*058/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask);
    IMPORT /*05C/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags);
    #endif

    // methods - non-faction stats
    // methods that set variables will also call MarkComponentModified()
    IMPORT UInt16           GetLevel(); // computes relative level is PCLevelOffset flag is set
    IMPORT void             SetMagicka(UInt16 value);
    IMPORT void             SetFatigue(UInt16 value);
    IMPORT void             SetBarterGold(UInt16 value);
    IMPORT void             SetLevel(UInt16 value);
    #ifndef OBLIVION    
    IMPORT void             SetMinLevel(UInt16 value); // these functions apparently not used in game, so culled by compiler
    IMPORT void             SetMaxLevel(UInt16 value);
    IMPORT void             SetActorFlag(UInt32 flagMask, bool value);
    #endif

    // methods - faction list
    IMPORT void             ClearFactions(); // clears faction list
    IMPORT SInt8            GetFactionRank(TESFaction* faction, bool noFlagUnk03); // returns -1 if faction not found in list
                            // if second arg is true, factions with flag 0x08 set will be skipped
    IMPORT void             SetFactionRank(TESFaction* faction, SInt8 rank); // pass -1 to remove faction from list
    #ifdef OBLIVION
    IMPORT void             ModFactionRank(TESFaction* faction, SInt8 rankChange, bool noFlagUnk03); // change rank, will clamp
                            // new rank to be >= 0.  if second arg is true, factions with flag 0x08 set will be skipped
    IMPORT SInt32           GetFactionReaction(TESActorBase* target, SInt32& rank); // get reaction value + rank of shared faction with
                            // greatest reaction value (if no shared factions, return strongest reaction from any pair)
    #endif

    // methods - serialization
    // NOTE: there is no LoadComponent() for this class; it is completely integrated into the LoadForm method of it's assorted descendents 
    IMPORT void             SaveComponent(); // game version does not save factions
    IMPORT void             LinkComponent(TESForm& parentForm);

    // constructor, destructor
    IMPORT TESActorBaseData();
    IMPORT ~TESActorBaseData();
};