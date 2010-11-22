/* 
    MagicTarget is a common base class for any TESObjectREFR that can cast magic.
    The magic item currently active and the magic target (when applicable) are accessed via virtual methods rather than
    passed around as arguments.

    Call chains - each is triggered by the 'start' event:
    -   Spell/Power/Scroll: 
        start:  Activated by AI or by player, or from a call to Cast script command (must call SetActiveMagicItem)
        ->  CastMagicItem
        ->  PlayCastingAnimation
        ->  [... animation ...] 
        ->  UseActiveMagicItem
            ->  IsMagicItemUseable
            ->  ApplyMagicItemCost
        ->  ApplyActiveMagicItem
            ->  CreateActiveEffect

    -   Ability:
        start:  Added to actor at load time or by call to AddSpell script command
        -> AddAbility 
        -> ApplyActiveMagicItem  
            ->  CreateActiveEffect

    -   Disease:
        start:  Found in spell list during loading, a call to AddSpell script command, 
                or melee strike on the player by a carrier
        -> Actor::AddSpell 
        -> AddAbility 
        -> ApplyActiveMagicItem 
            ->  CreateActiveEffect

    -   Melee Weapon Enchantment/Poison:
        start:  weapon strikes target (must call SetActiveMagicItem)
        ->  UseActiveMagicItem
            ->  IsMagicItemUseable
            ->  ApplyMagicItemCost
        ->  ApplyActiveMagicItem
            ->  CreateActiveEffect

    -   Arrow/Bow Weapon Enchantment/Poison:
        start:  arrow strikes target *or* world geometry (must call SetActiveMagicItem)
        ->  [... bhkListener routines if the arrow has more than one assoc magic item ...]
        ->  UseActiveMagicItem
            ->  IsMagicItemUseable
            ->  ApplyMagicItemCost
        ->  ApplyActiveMagicItem
            ->  CreateActiveEffect

    -   Potion/Ingredient:
        start:  Activated by AI or by player, or a call to EquipItem script command (must call SetActiveMagicItem)
        ->  UseActiveMagicItem
            ->  IsMagicItemUseable
            ->  ApplyMagicItemCost
        ->  ApplyActiveMagicItem
            ->  CreateActiveEffect
*/
#pragma once
#ifdef OBLIVION // game only

// base classes
#include "API/TES/MemoryHeap.h"

// argument classes
class   TESObjectREFR;      // TESForms/TESObjectREFR.h
class   NiNode;
class   TESObjectLIGH;
class   NiPointLight;
class   TESBoundObject;     // TESForms/TESObject.h
class   MagicTarget;        // Magic/MagicTarget.h
class   MagicCaster;        // Magic/MagicCaster.h
class   MagicItem;          // Magic/MagicItem.h
class   MagicItemForm;      // Magic/MagicItemForm.h
class   EffectItem;         // Magic/EffectItem.h
class   ActiveEffect;       // Magic/ActiveEffect.h

class MagicCaster
{// size 0C
public:

    enum CastingStates
    {
        kCastingState_NoSpell               = 0x0,
        kCastingState_Aim                   = 0x1,
        kCastingState_Cast                  = 0x2,
        //kCastingState_??                  = 0x3, 
        kCastingState_FindTargets           = 0x4,
        kCastingState_ErrSpellDisabled      = 0x5,
        kCastingState_ErrAlreadyCasting     = 0x6,
        kCastingState_ErrCannotCast         = 0x7,
    };

    enum CastingFailureCodes
    {
        kCastingFail_NONE                   = 0x0,
        kCastingFail_InsufficientMagicka    = 0x1,
        kCastingFail_Silence                = 0x2,
        kCastingFail_InsufficientSkill      = 0x3,
        kCastingFail_PowerUsed              = 0x4,
        kCastingFail_MultipleBoundEffects   = 0x5,
        kCastingFail_RangedUnderwater       = 0x6,
    };

    class CastingVFX // VFX to accompany casting animation
    {// size 1C
    /*
        Partial:
        -   members
        Todo:
        -   methods; determine what this class is used for
    */
    public:
        // members
        MEMBER /*00*/ NiNode*           magicNode; // from GetMagicNode().  actually a smart pointer - don't set directly.
        MEMBER /*04*/ TESObjectLIGH*    effectLight; // lighting around caster, caster's hand(s)
        MEMBER /*08*/ NiPointLight*     pointLight; // actually a smart pointer - don't set directly.
        MEMBER /*0C*/ float             freq; // ?? seems reasonable if 0x14 is 'end'.  init to 1.0
        MEMBER /*10*/ float             current; // current FX play time? init to 0.0
        MEMBER /*14*/ float             end; // init to 0.0, seen set to NiControllerSequence.end (NiControllerSequence + 0x30)
        MEMBER /*18*/ bool              unk18; // set to 1 for casting anim, zero for areaeffect anim and cleanup.  plays FX in reverse?
        MEMBER /*19*/ UInt8             pad19[3];
        // constructor, destructor
        IMPORT CastingVFX(UInt32 lightFormID, NiNode* magicNode); // in game code, lightFormID is from TESObjectLIGH of the dominant effect
        IMPORT ~CastingVFX();
        // use FormHeap for class new & delete
        USEFORMHEAP
    };

    // members
    //     /*00*/ void**            vtbl;
	MEMBER /*04*/ CastingVFX*       castingVFX; // constructed & used during casting animation, destroyed afterwards
	MEMBER /*08*/ UInt32            castingState;

    // virtual methods  
	IMPORT /*00*/ virtual void	            AddAbility(MagicItemForm* ability, bool noHitVFX = false); // used to apply abilities.  
	IMPORT /*04*/ virtual void	            AddDisease(MagicItemForm* disease, MagicTarget* target, bool noHitVFX = false);
                                            // used to apply diseases.  target must be valid but is otherwise ignored; disease is applied to caster
                                            // noHitVFX disables the Hit shdaer and Hit components of the nif model
	IMPORT /*08*/ virtual void	            AddObjectEnchantment(MagicItem* magicItem, TESBoundObject* sourceObj, bool noHitVFX = false); // 
                                            // used to apply Apparel-type enchantments
                                            // noHitVFX disables the Hit shdaer and Hit components of the nif model
	IMPORT /*0C*/ virtual MagicTarget*      FindTouchTarget(); // used to select a nearby target for touch effects during casting,
                                            // only used if no ActiveTarget has been set
	IMPORT /*10*/ virtual void	            PlayTargetedCastingAnimation(); // starts the casting animation.  Called only for OnTarget spells.
                                            // Stub, simply calls PlayCastingAnimation().
	IMPORT /*14*/ virtual void	            PlayCastingAnimation(); // starts the spellcasting animation.  Use SetActiveMagicItem() before calling.
    INLINE /*18*/ virtual void	            ApplyMagicItemCost(MagicItem* magicItem, bool applyStatChanges = true) {} //
                                            // applies power cooldowns, combat controller misc
                                            // if applyStatChanges is true, apply magicka + fatigue + experience changes                                           
	IMPORT /*1C*/ virtual bool	            IsMagicItemUseable(MagicItem* magicItem, float* wortcraftSkill = 0, UInt32* failureCode = 0, bool useBaseMagicka = false); // 
                                            // returns true if item is currently useable.  effective wortcraft skill is calculated for ingredients
                                            // failure code provides reason if unuseable.  base magicka may be used instead of current magicka.
    INLINE /*20*/ virtual TESObjectREFR*	GetParentREFR() {return 0;}
	INLINE /*24*/ virtual NiNode*	        GetMagicNode() {return 0;} // looks up "magicNode" node in caster's NiNode
	IMPORT /*28*/ virtual bool	            AddEffectToSelf(ActiveEffect* actvEff); // used internally for *all* OnSelf effects, once they are created.
                                            // caster must also be a magic target
	IMPORT /*2C*/ virtual float	            GetCastingEffectiveness(bool ignoreFatigue = false, float currentFatigue = 0.0); // 
                                            // if currentFatigue <= 0, current fatigue of caster is used.  
                                            // fatigue can be ignored (only worn armor contributes)
	INLINE /*30*/ virtual MagicItem*        GetActiveMagicItem() {return 0;} // magic item for current casting action
    INLINE /*34*/ virtual void	            SetActiveMagicItem(MagicItem* item) {}
	INLINE /*38*/ virtual MagicTarget*      GetCastingTarget() {return 0;} // target for current casting action - often null for self & target spells
    INLINE /*3C*/ virtual void	            SetCastingTarget(MagicTarget* magicTarget) {}
    IMPORT /*40*/ virtual ActiveEffect*     CreateActiveEffect(MagicItem* magicItem, EffectItem* effectItem, TESBoundObject* sourceObj);  

    // methods
    IMPORT bool                 CastMagicItem(MagicItem* magicItem, MagicTarget* target, bool noHitVFX = false); // used to cast spells, returns false on failure
                                // if magicItem is a disease/ability calls AddDisease/AddAbility.  Otherwise, starts the casting animation
                                // noHitVFX disables the Hit shdaer and Hit components of the nif model  
    IMPORT void                 UseActiveMagicItem(TESBoundObject* sourceObj = 0); // helper function. 
                                // checks that item is useable & applies costs, then applies the item
    IMPORT void                 ApplyActiveMagicItem(float wortcraftSkill = 0, TESBoundObject* sourceObj = 0, bool noHitVFX = false); //
                                // helper function.  creates active effects & hands them off to the magic target
                                // wortcraftSkill is used for inedible ingredients; generally use the result from IsMagicItemUseable
                                // noHitVFX disables the Hit shdaer and Hit components of the nif model                                     

    // constructor, destructor
    IMPORT 	MagicCaster();
	IMPORT ~MagicCaster();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

/* This is a sketch.  TODO: finish definition
class NonActorMagicCaster : public BSExtraData
{// size 24
public:
	NonActorMagicCaster();
	~NonActorMagicCaster();

	//base class
	MagicCaster magicCaster;	//00C

	MagicItem*		activeMagicItem;    //018
	MagicTarget*    castingTarget;      //01C
	TESObjectREFR*  caster;		        //020
};
*/

#endif  // end of Game-only section