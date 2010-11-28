/*
    ActiveEffects derived from ValueModifierEffect, which handles changes to actor values

    -   ActiveEffect
        -   ValueModifierEffect
            -   AbsorbEffect
            -   ParalysisEffect
            -   ShieldEffect
            -   CalmEffect
            -   FrenzyEffect
            -   ChameleonEffect
            -   InvisibilityEffect
            -   DetectLifeEffect
            -   NightEyeEffect
            -   DarknessEffect
            -   TelekinesisEffect
*/
#pragma once

// TEMPLATE:

 //   // no additional members

 //   // members
 //   //     /*00*/ ValueModifierEffect
 //   MEMBER /*3C*/ 

 //   // ValueModifierEffect virtual methods   
 //   IMPORT /*000*/ virtual                  ~ValueModifierEffect() DNAME()
 //   IMPORT /*004*/ virtual ActiveEffect*    Clone() const DNAME(0)
 //   IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed) DNAME() 
 //   IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent) DNAME(0)
 //   IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent) DNAME()
 //   IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent) DNAME()
 //   IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent) DNAME()
 //   IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent) DNAME()
 //   IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent) DNAME()
 //   IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster) DNAME(0)
 //   IMPORT /*028*/ virtual bool             DoesHealthDamage() const  DNAME(0)
 //   IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo) DNAME()
 //   IMPORT /*030*/ virtual bool             UnkAE30(UInt32 arg) DNAME(0)
 //   IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget) DNAME(0)
 //   IMPORT /*038*/ virtual void             ApplyEffect() DNAME()
 //   IMPORT /*03C*/ virtual void             RemoveEffect() DNAME()

 //   // no additional virtual methods

 //   // constructor - no creator function
 //   IMPORT ValueModifierEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) : ValueModifierEffect(caster,magicItem,effectItem) DNAME()

 //   // constructor & creator function
 //   INLINE ValueModifierEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
 //          : ValueModifierEffect(caster,magicItem,effectItem) {}
 //   IMPORT static ValueModifierEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) DNAME(0)

// base classes
#include "API/Magic/ActiveEffect.h"

// argument classes
class   Actor;
class   NiRefObject;
class   TESObjectREFR;

class IMPORTCLASS ValueModifierEffect : public ActiveEffect
{// size 3C
public:

    // members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ UInt32	    avCode; // av to modify

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*028*/ virtual bool             DoesHealthDamage() const ;
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // additional virtual methods
    IMPORT /*040*/ virtual void             SetAVCode(UInt32 newAVCode);
    IMPORT /*044*/ virtual UInt32           GetAVCode();

    // methods
    IMPORT void             ModifyValue(Actor* targetActor, float magnitude); // determines modifier & applies change to AV
    IMPORT void             DisplayStatPenaltyWarning(); // displays an in-game warning message if the effect is damaging/draining the players stats
    IMPORT float            GetEffectiveMagnitude(float rawMagnitude); // adjust magnitude such that attributes & skills won't be reduced < 0

    // constructor - no creator function
    IMPORT ValueModifierEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS AbsorbEffect : public ValueModifierEffect
{// size 4C
public:

    // members
    //     /*00*/ ValueModifierEffect
    MEMBER /*3C*/ NiRefObject*	unkAbsb3C;  // NiNode*?
	MEMBER /*40*/ NiRefObject*	unkAbsb40;  // NiNode*?
	MEMBER /*44*/ NiRefObject*	unkAbsb44;  // NiNode*?
	MEMBER /*48*/ NiRefObject*	unkAbsb48;  // NiNode*?

    // ValueModifierEffect virtual methods   
    IMPORT /*000*/ virtual                  ~AbsorbEffect();
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor - no creator function
    IMPORT AbsorbEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS ParalysisEffect : public ValueModifierEffect
{// size 3C
public:

    // no additional members

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE ParalysisEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
            : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static ParalysisEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);

};

class IMPORTCLASS ShieldEffect : public ValueModifierEffect
{// size 40
public:
	
    // members
    //     /*00*/ ValueModifierEffect   // avCode: for SHLD = DefendBonus, for FRSH = ResistFrost, for FISH = ResistFire, for LISH = ResistShock
    MEMBER /*3C*/ UInt32	            secondAVCode; // for SHLD = kActorVal__MAX, for FRSH, FISH & LISH = DefendBonus

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor - no creator function
    IMPORT ShieldEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS CalmEffect : public ValueModifierEffect
{// size 3C
public:

    // no additional members

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE CalmEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static CalmEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS FrenzyEffect : public ValueModifierEffect
{// size 40
public:
	
    // members
    //     /*00*/ ValueModifierEffect
    MEMBER /*3C*/ UInt8	                unkFrnz3C;
	MEMBER /*3D*/ UInt8	                padFrnz3D[3];

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE FrenzyEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem), unkFrnz3C(0)  {}
    IMPORT static FrenzyEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS ChameleonEffect : public ValueModifierEffect
{// size 3C
public:

    // no additional members

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE ChameleonEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static ChameleonEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS InvisibilityEffect : public ValueModifierEffect
{// size 3C
public:

    // no additional members

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE InvisibilityEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static InvisibilityEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);

};

class IMPORTCLASS DetectLifeEffect : public ValueModifierEffect
{// size 3C
public:

    // no additional members

    // ValueModifierEffect virtual methods   
    INLINE /*000*/ virtual                  ~DetectLifeEffect() {} // compeltely inlined by game
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE DetectLifeEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static DetectLifeEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS NightEyeEffect : public ValueModifierEffect
{// size 3C
public:

    // no additional members

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE NightEyeEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static NightEyeEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);

};

class IMPORTCLASS DarknessEffect : public ValueModifierEffect
{// size 3C
public:
	
    // no additional members

    // ValueModifierEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE DarknessEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
           : ValueModifierEffect(caster,magicItem,effectItem) {}
    IMPORT static DarknessEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS TelekinesisEffect : public ValueModifierEffect
{// size 50
public:

    // members
    //     /*00*/ ValueModifierEffect
    MEMBER /*3C*/ NiRefObject*          unkTele3C; // NiNode* ?
	MEMBER /*40*/ float                 unkTele40;
	MEMBER /*44*/ float                 unkTele44;
	MEMBER /*48*/ TESObjectREFR*        targetObject;
	MEMBER /*4C*/ UInt8                 unkTele4C;
	MEMBER /*4D*/ UInt8                 unkTele4D;
	MEMBER /*4E*/ UInt8                 padTele4E[2];

    // ValueModifierEffect virtual methods   
    IMPORT /*000*/ virtual                  ~TelekinesisEffect();
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    IMPORT TelekinesisEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
    IMPORT static TelekinesisEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);

};
