/* 
    MagicTarget is a common base class for any TESObjectREFR that can be affected by magic (i.e. has an Active Effects list)

    Life cycle of AE on the magic target:
    1.  AttemptAddEffect - called by MagicCaster routines
    2.  AddEffect
    3.  ProcessEffect - update effect at intervals until it's marked as terminated, then remove
*/
#pragma once
#ifdef OBLIVION // game only

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   TESObjectREFR;      // TESForms/TESObjectREFR.h
class   TESBoundObject;     // TESForms/TESObject.h
class   MagicCaster;        // Magic/MagicCaster.h
class   MagicItem;          // Magic/MagicItem.h
class   ActiveEffect;       // Magic/ActiveEffect.h
class   ActiveEffectList;   // Magic/ActiveEffect.h

class IMPORTCLASS MagicTarget
{// size 08
/*
    Partial:
    -   members
*/
public:

    typedef BSSimpleList<ActiveEffect*> ActiveEffectList;

    // members
    //     /*00*/ void**        vtbl;
    MEMBER /*04*/ UInt8         unkMagtgt04;
    MEMBER /*05*/ UInt8            padMagtgt05[3];

    // virtual methods
    IMPORT /*00*/ virtual bool              AttemptAddEffect(MagicCaster* caster, MagicItem* magicItem, ActiveEffect* activeEffect, bool arg3); //
                                            // run failure (absorb, etc.) checks, call AddEffect(), handle hostility AI.  returns true if added.  
                                            // arg3 controls whether arrow enchantments trigger a MagicHit event on actors                                       
    INLINE /*04*/ virtual TESObjectREFR*    GetParentREFR() {return 0;} // get parent reference object
    INLINE /*08*/ virtual ActiveEffectList*    GetActiveEffectList() {return 0;} 
    INLINE /*0C*/ virtual bool              IsActor() {return false;}
    INLINE /*10*/ virtual void              PostAddEffect(ActiveEffect* activeEffect) {} // called after addition to AE list, but before application
                                            // play extra shaders (elemental shield, pain FX), update cached elemental shield mask for target
    INLINE /*14*/ virtual void              PostRemoveEffect(ActiveEffect* activeEffect) {} // called after removal from AE list
                                            // update cached elemental shield mask for target
    INLINE /*18*/ virtual void              PlayReflectShader(MagicCaster* caster, ActiveEffect* activeEffect) {} // plays preset SpellReflect shader
    INLINE /*1C*/ virtual void              PlayAbsorbShader(MagicCaster* caster, ActiveEffect* activeEffect) {} // plays preset SpellAbsorb shader
    IMPORT /*20*/ virtual float             ResistanceFactor(MagicCaster* caster, MagicItem* magicItem, ActiveEffect* activeEffect); //
                                            // calculates resistance factor that multiplies magnitude/duration
    INLINE /*24*/ virtual bool              AttemptAbsorb(MagicCaster* caster, MagicItem* magicItem, ActiveEffect* activeEffect, bool reflected) {return false;} //
                                            // runs a spell absorbtion check, return true if absorbed
    INLINE /*28*/ virtual bool              AttemptReflect(MagicCaster* caster, MagicItem* magicItem, ActiveEffect* activeEffect) {return false;} //
                                            // runs a spell reflection check, return true if reflected

    // methods
    IMPORT bool             AddEffect(MagicCaster* caster, MagicItem* magicItem, ActiveEffect* activeEffect); // apply resistance,
                            // remove duplicate effects, add to AE list, call PostAddEffect().  returns true if added.
    IMPORT void             ProcessEffects(float timeElapsed); // call ActiveEffect::ProcessEffect() on all effects.  
                            // If effect terminated, remove from AE list & call PostRemoveEffect().
    IMPORT bool             HasEffect(UInt32 mgefCode); // returns true if target has an active effect w/ specified code
                            // NOTE - counts effects that haven't been applied or are flagged for removal
    IMPORT bool             HasMagicItemEffect(MagicItem* magicItem); // returns true if target has an active effect from specified item
                            // NOTE - counts effects that haven't been applied or are flagged for removal
    IMPORT void             RemoveEffects(MagicItem* sourceItem, MagicCaster* sourceCaster = 0, ActiveEffect* templateEffect = 0); //
                            // if templateEffect is null: removes all effects from sourceItem, optionally restricted to sourceCaster
                            // if templateEffect is valid: removes single effect from sourceItem, optionally cast by sourceCaster, that
                            // matches the boundObject and effectItem from the template.
    IMPORT void             RemoveBoundObjectEffects(TESBoundObject* sourceObject, bool removeImmediately); // remove all effects from
                            // the specified bound object
    IMPORT void             RemoveAllNonpersistentEffects(bool removeImmediately); // remove all effects from non-peristent magic items
    IMPORT static int       ActiveEffectCompareFunc(const ActiveEffect& effectA, const ActiveEffect& effectB); // helper function for sorting
                            // AE list.  returns -1 (A<B), 0 (A==B), +1 (A>B)

    // constructor, destructor
    IMPORT MagicTarget();
    IMPORT ~MagicTarget();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

/* This is a sketch.  TODO: finish definition
class NonActorMagicTarget : public BSExtraData
{// size 20
public:
    NonActorMagicTarget();
    ~NonActorMagicTarget();

    // base
    MagicTarget        magicTarget;    // 00C
    TESObjectREFR*  parentREFR;        // 014 passed to c'tor, is the "nonactor" refr
    BSSimpleList<ActiveEffect*> effects;
};
*/

#endif  // end of game-only section