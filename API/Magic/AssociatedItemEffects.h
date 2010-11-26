/*
    ActiveEffects derived from AssociatedItemEffect, which handles summoning

    -   ActiveEffect
        -   AssociatedItemEffect
            -   SummonCreatureEffect
            -   BoundItemEffect
*/
#pragma once

// base classes
#include "API/Magic/ActiveEffect.h"

// argument classes
class   TESForm;
class   Actor;
class   Vector3;    // NiVector3?  currently defined in Utilities/ITypes.h

class AssociatedItemEffect : public ActiveEffect
{// size 3C
public:
 
    // members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ TESForm*      associatedItem; // base form, a ref will be created or an instance added to inventory
   
    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);

    // no additional virtual methods

    // constructor - no creator function
    IMPORT AssociatedItemEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class SummonCreatureEffect : public AssociatedItemEffect
{// size 64
public:

    // members
    //     /*00*/ AssociatedItemEffect
    MEMBER /*3C*/ Actor*                summonedActor;
	MEMBER /*40*/ UInt8                 unkSumn40;
	MEMBER /*41*/ UInt8	                padSumn41[3];
	MEMBER /*44*/ float                 unkSumn44;
	MEMBER /*48*/ Vector3               position;
	MEMBER /*54*/ Vector3               rotation;
	MEMBER /*60*/ bool                  unkSumn60; // checked other active effects to enforce summoning limit
    MEMBER /*61*/ UInt8                 unkSumn61;
	MEMBER /*62*/ UInt8                 padSumn62[2];   

    // AssociatedItemEffect virtual methods   
    IMPORT /*000*/ virtual                  ~SummonCreatureEffect();
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent);
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor - no creator function
    IMPORT SummonCreatureEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class BoundItemEffect : public AssociatedItemEffect
{// size 8C
public:

    // members
    //     /*00*/ AssociatedItemEffect
    MEMBER /*3C*/ UInt32                unkBdit3C;
	MEMBER /*40*/ UInt32                unkBdit40;
	MEMBER /*44*/ UInt32                unkBdit44;
	MEMBER /*48*/ UInt32                unkBdit48;
	MEMBER /*4C*/ UInt32                unkBdit4C;
	MEMBER /*50*/ UInt32                unkBdit50;
	MEMBER /*54*/ UInt32                unkBdit54;
	MEMBER /*58*/ UInt32                unkBdit58;
	MEMBER /*5C*/ UInt32                unkBdit5C;
	MEMBER /*60*/ UInt32                unkBdit60;
	MEMBER /*64*/ UInt32                unkBdit64;
	MEMBER /*68*/ UInt32                unkBdit68;
	MEMBER /*6C*/ UInt32                unkBdit6C;
	MEMBER /*70*/ UInt32                unkBdit70;
	MEMBER /*74*/ UInt32                unkBdit74;
	MEMBER /*78*/ UInt32                unkBdit78;
	MEMBER /*7C*/ UInt32                unkBdit7C;
	MEMBER /*80*/ float                 unkBdit80;
	MEMBER /*84*/ UInt8                 unkBdit84;
	MEMBER /*85*/ UInt8                 unkBdit85;
	MEMBER /*86*/ UInt8                 unkBdit86;
	MEMBER /*87*/ UInt8                 unkBdit87;
	MEMBER /*88*/ UInt8                 unkBdit88;
	MEMBER /*89*/ UInt8                 padBdit89[3];
     
    // AssociatedItemEffect virtual methods   
    IMPORT /*000*/ virtual                  ~BoundItemEffect();
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor - no creator function
    IMPORT BoundItemEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};
