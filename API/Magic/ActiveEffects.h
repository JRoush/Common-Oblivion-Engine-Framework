/*
    ActiveEffects (the that are derived directly from ActiveEffect, rather than ValueModifierEffect or AssociatedItemEffect)

    -   ActiveEffect
        -   ScriptEffect
        -   SunDamageEffect
        -   DemoralizeEffect
        -   CommandEffect
            -   CommandCreatureEffect
            -   CommandHumanoidEffect
        -   ReanimateEffect
        -   TurnUndeadEffect
        -   VampirismEffect        
        -   LightEffect
        -   DispelEffect
        -   CureEffect
        -   DisintegrateArmorEffect
        -   DisintegrateWeaponEffect
        -   LockEffect
        -   OpenEffect
        -   SoulTrapEffect
*/
#pragma once

// base classes
#include "API/Magic/ActiveEffect.h"

// argument classes
class   Script;
class   ScriptEventList;
class   Vector3;        // NiVector3?  currently defined in Utilities/ITypes.h
class   NiRefObject;

class IMPORTCLASS ScriptEffect : public ActiveEffect
{// size 40
public:
	
    // members
    //     /*00*/ ActiveEffect
	MEMBER /*38*/ Script*           script;
	MEMBER /*3C*/ ScriptEventList*	eventList;

    // ActiveEffect virtual methods    
	IMPORT /*000*/ virtual                  ~ScriptEffect();
	IMPORT /*004*/ virtual ActiveEffect*	Clone() const;
    IMPORT /*008*/ virtual void			    UpdateEffect(float timeElapsed);
	IMPORT /*00C*/ virtual UInt16			EffectSaveSize(TESObjectREFR* parent);
	IMPORT /*010*/ virtual void			    SaveEffect(TESObjectREFR* parent);
	IMPORT /*014*/ virtual void			    LoadEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void			    PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void			    CopyTo(const ActiveEffect& copyTo) ;    
    IMPORT /*038*/ virtual void			    ApplyEffect() ;
    IMPORT /*03C*/ virtual void			    RemoveEffect() ;

    // no additional virtual methods

    // constructor - no creator function
    IMPORT ScriptEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS SunDamageEffect : public ActiveEffect
{// size 38
public:

	// members
    //     /*00*/ ActiveEffect
	MEMBER /*38*/ float	        unkSunDmg38;
	MEMBER /*3C*/ UInt8	        unkSunDmg3C;
	MEMBER /*3D*/ UInt8	        unkSunDmg3D;
	MEMBER /*3E*/ UInt8	        padSunDmg3E[2];

    // ActiveEffect virtual methods 
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void			    UpdateEffect(float timeElapsed); 
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void			    SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void			    LoadEffect(TESObjectREFR* parent);
    IMPORT /*01C*/ virtual void			    PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*028*/ virtual bool			    DoesHealthDamage() const ;
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool			    IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*03C*/ virtual void			    RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE SunDamageEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
            : ActiveEffect(caster,magicItem,effectItem), unkSunDmg38(0), unkSunDmg3C(0), unkSunDmg3D(0) {}
    IMPORT static SunDamageEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS DemoralizeEffect : public ActiveEffect
{// size 3C
public:

	// members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ UInt8	unkDemo38;
	MEMBER /*39*/ UInt8	padDemo39[3];

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void			    LoadEffect(TESObjectREFR* parent);
    IMPORT /*024*/ virtual bool			    RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*034*/ virtual bool			    IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void			    ApplyEffect();
    IMPORT /*03C*/ virtual void			    RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE DemoralizeEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem), unkDemo38(0) {}
    IMPORT static DemoralizeEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS CommandEffect : public ActiveEffect
{// size 38
public:
	
    // no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*024*/ virtual bool			    RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*038*/ virtual void			    ApplyEffect();
    IMPORT /*03C*/ virtual void			    RemoveEffect();

    // no additional virtual methods

    // constructor - no creator function
    IMPORT CommandEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS CommandCreatureEffect : public CommandEffect
{// size 38
public:
	
    // no additional members 

    // CommandEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);

    // no additional virtual methods

    // constructor & creator function
    INLINE CommandCreatureEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
            : CommandEffect(caster,magicItem,effectItem) {}
    IMPORT static CommandCreatureEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS CommandHumanoidEffect : public CommandEffect
{// size 38
public:
		
    // no additional members 

    // CommandEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);

    // no additional virtual methods

    // constructor & creator function
    INLINE CommandHumanoidEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
            : CommandEffect(caster,magicItem,effectItem) {}
    IMPORT static CommandHumanoidEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS ReanimateEffect : public ActiveEffect
{// size 60
public:

	struct UnkReanimStruct50
	{// size 10
		UInt32	unk0;	// 00
		UInt32	unk4;	// 04
		UInt32	unk8;	// 08
		UInt32	unkC;	// 0C
	};

	// members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ UInt32	        unkRean38;
	MEMBER /*3C*/ UInt32	        unkRean3C;
	MEMBER /*40*/ float	            unkRean40;
	MEMBER /*44*/ Vector3	        unkRean44;
	MEMBER /*50*/ UnkReanimStruct50	unkRean50;

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    IMPORT ReanimateEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
    IMPORT static ReanimateEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS TurnUndeadEffect : public ActiveEffect
{// size 3C
public:

	// members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ UInt8	        unkTurn38;
	MEMBER /*39*/ UInt8	        padTurn39[3];

    // ActiveEffect virtual methods   
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
    INLINE TurnUndeadEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem), unkTurn38(0) {}
    IMPORT static TurnUndeadEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS VampirismEffect : public ActiveEffect
{// size 38
public:

    // no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent);
    IMPORT /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE VampirismEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem) {}
    IMPORT static VampirismEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS LightEffect : public ActiveEffect
{// size 3C
public:

    // members
    //     /*00*/ ValueModifierEffect
    MEMBER /*3C*/ NiRefObject*          unkLight3C; // NiNode*?

    // ActiveEffect virtual methods   
    IMPORT /*000*/ virtual                  ~LightEffect();
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
    IMPORT LightEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
    IMPORT static LightEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS DispelEffect : public ActiveEffect
{// size 38
public:	

    // no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE DispelEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem) {}
    IMPORT static DispelEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS CureEffect : public ActiveEffect
{// size 40
public:

    // members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ UInt32        magicTypeToCure; // see Magic::MagicTypes
	MEMBER /*3C*/ UInt32        effectCodeToCure; //for CUPA = 'PARA', for CUDA & CUPO = 0xFFFFFFFF

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();

    // no additional virtual methods

    // constructors - no creator functions
    IMPORT CureEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem, Magic::MagicTypes magicType); // for CUPO,CUDI
    IMPORT CureEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem, UInt32 mgefCode); // for CUPA
};

class IMPORTCLASS DisintegrateArmorEffect : public ActiveEffect
{// size 3C
public:

	// members
    //     /*00*/ ActiveEffect
    MEMBER /*38*/ UInt32        unkDisArm38;

    // ActiveEffect virtual methods   
    IMPORT /*000*/ virtual                  ~DisintegrateArmorEffect();
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent);
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent);
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE DisintegrateArmorEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem), unkDisArm38(0) {}
    IMPORT static DisintegrateArmorEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS DisintegrateWeaponEffect : public ActiveEffect
{// size 38
public:
    
	// no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE DisintegrateWeaponEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem)   // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem) {}
    IMPORT static DisintegrateWeaponEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS LockEffect : public ActiveEffect
{// size 38
public:

    // no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE LockEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem) {}
    IMPORT static LockEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS OpenEffect : public ActiveEffect
{// size 38
public:
	
    // no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*038*/ virtual void             ApplyEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE OpenEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem) {}
    IMPORT static OpenEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};

class IMPORTCLASS SoulTrapEffect : public ActiveEffect
{// size 38
public:
	
    // no additional members

    // ActiveEffect virtual methods   
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const;
    IMPORT /*008*/ virtual void             UpdateEffect(float timeElapsed); 
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster);
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo);
    IMPORT /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget);
    IMPORT /*03C*/ virtual void             RemoveEffect();

    // no additional virtual methods

    // constructor & creator function
    INLINE SoulTrapEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem) // completely inlined by game
        : ActiveEffect(caster,magicItem,effectItem) {}
    IMPORT static SoulTrapEffect* Make(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);
};