/*
    ActiveEffect encapsulates a magic effect that has been applied to a target.  The virtual functions of the 
    various active effectclasses are what actually implement the 'magic' - in particular ApplyEffect, 
    RemoveEffect, and UpdateEffect.

    ActiveEffect states (Applied,Terminated,Removed):
    -   (0,0,0) Added to AE list on a magic target, but not yet applied
    -   (1,0,0) Active on a target
    -   (1,1,0) Active on a target, but scheduled for removal in next cycle
    -   (1,1,1) Still in AE list on a magic target, but has been fully deactivated (awaiting deletion)
    
    Member functions dealing with serialization take a TESObjectREFR* argument "parent", which seems to be 
    the 'grandparent', i.e. the parent ref of the parent magic target.
*/
#pragma once
#ifdef OBLIVION // game only

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/Magic/Magic.h" // constants
#include "API/BSTypes/BSSimpleList.h"
#include "API/NiTypes/NiTMap.h"

// argument classes
class   TESObjectREFR;      // TESForms/TESObjectREFR.h
class   MagicHitEffect; 
class   TESBoundObject;     // TESForms/TESObject.h
class   MagicTarget;        // Magic/MagicTarget.h
class   MagicCaster;        // Magic/MagicCaster.h
class   MagicItem;          // Magic/MagicItem.h
class   EffectItem;         // Magic/EffectItem.h
class   ActiveEffect;

class IMPORTCLASS ActiveEffect
{// size 38
/*
    Partial:
    -   members
    -   flag bits
    Notes:
    -   virtual methods are fully decoded, UnkAE30() is undecodable unless someone can find an instance of it beign called
*/
public:

    typedef BSSimpleList<ActiveEffect*> ActiveEffectList;
    typedef BSSimpleList<MagicHitEffect*> HitEffectList;    
    typedef ActiveEffect* (__cdecl* AECreatorFunc)(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);

    enum ActiveEffectFlags
    {
        kAEFlag_NoHitVFX                = /*00*/ 0x00000001, // same function as 'NoHitVFX' flag on effect - neither Model nor Shader Hit visuals played
        kAEFlag_NoShaderHitVFX          = /*01*/ 0x00000002, // Hit shader for the effect is not played
        kAEFlag_NoModelHitVFX           = /*02*/ 0x00000004, // Hit VFX from the effect's nif file are not played
        kAEFlag_Unk03                   = /*03*/ 0x00000008, // Disables some kind of FX, set for all AEs that are not the dominant effect
        kAEFlag_Unk04                   = /*04*/ 0x00000010, // Also disables HitShader - perhaps other shaders as well?  Set w/ 1<<2 if casting w/o hit FX
        kAEFlag_HitAFXQueued            = /*05*/ 0x00000020, // Hit sound queued (until menu is closed)
        kAEFlag_HitVFXQueued            = /*06*/ 0x00000040, // Hit visual effects queued (until menu is closed)
    };    

    // members
    //     /*00*/ void**            vtbl;
    MEMBER /*04*/ float             timeElapsed;
    MEMBER /*08*/ MagicItem*        magicItem;
    MEMBER /*0C*/ EffectItem*       effectItem;
    MEMBER /*10*/ bool              applied; // set after application
    MEMBER /*11*/ bool              terminated; // set when effect is to be removed
    MEMBER /*12*/ bool              removed; // set after removal
    MEMBER /*13*/ UInt8             pad13; // copied & saved, but not initialized
    MEMBER /*14*/ UInt32            aeFlags; // frequently set to 0xE (no hit VFX) for effects that aren't dominant on their magic item
    MEMBER /*18*/ float             magnitude;
    MEMBER /*1C*/ float             duration; // 0.0 for persistent effects (abilities, diseases, apparel enchantments)
    MEMBER /*20*/ MagicTarget*      target; // parent magic target
    MEMBER /*24*/ MagicCaster*      caster; // source caster
    MEMBER /*28*/ UInt32            magicType; //  from Magic::MagicTypes
    MEMBER /*2C*/ void*             unk2C; // not saved or copied, but cleaned up after removal.
                                    // pointer to something with member functions but no vtbl.  something to do with AFX - queued sounds, perhaps?
    MEMBER /*30*/ TESBoundObject*   boundObject; // parent object, for enchantments
    MEMBER /*34*/ HitEffectList*    hitEffects; // saved but not copied.  non-null for light, detectlife, shield, and possibly others

    // virtual methods    
    IMPORT /*000*/ virtual                  ~ActiveEffect();
                                            // The vtbl entry is acutally the compiler-generated 'vector deleting destructor', which calls this method
    IMPORT /*004*/ virtual ActiveEffect*    Clone() const; // allocate new AE of the same type & copy member values to it
    INLINE /*008*/ virtual void             UpdateEffect(float timeElapsed) {} // update effect, called every frame
    IMPORT /*00C*/ virtual UInt16           EffectSaveSize(TESObjectREFR* parent); // size of type-specific members when serialized
    IMPORT /*010*/ virtual void             SaveEffect(TESObjectREFR* parent); // helper function, saves type-specific members
    IMPORT /*014*/ virtual void             LoadEffect(TESObjectREFR* parent); // helper function, loads type-specific members
    IMPORT /*018*/ virtual void             LinkEffect(TESObjectREFR* parent); // resolve formids into pointers & call Link on child objects
    IMPORT /*01C*/ virtual void             PostLinkEffect(TESObjectREFR* parent); // recreate any derived data that wasn't saved
                                            // (e.g. shaders & VFX)
    INLINE /*020*/ virtual void             PreLoadEffect(TESObjectREFR* parent) {} // resets data in preparation for loading
                                            // nontrivial for Invis, Chameleon, Light, Dark, NightEye, Paralyze, Reanim, SEFF
    IMPORT /*024*/ virtual bool             RemoveCaster(MagicCaster* removedCaster); // clears caster member it matches argument.                                             
                                            // for Calm, Demoralize, Rally, Frenzy, SummonCr, TurnUndead, Command, Reanimate, Telek, 
                                            // SoulTrap, and Absorb this dispels the effect entirely.  Probably called if caster dies.
                                            // returns true if caster removed.
    INLINE /*028*/ virtual bool             DoesHealthDamage() const {return false;} // returns true if effect damages (not drains) health
                                            // actually checks ambient light level (?) for sun damage.  called before fast-traveling
    IMPORT /*02C*/ virtual void             CopyTo(const ActiveEffect& copyTo) const; // copy members
    INLINE /*030*/ virtual bool             UnkAE30(UInt32 arg) {return true;} // return true by every single derived AE class
                                            // perhaps a CompareTo() function?  If so, then apparently no two active effects are ever equivalent
                                            // after some testing, I (JRoush) am not sure this method is actually used by the game
    INLINE /*034*/ virtual bool             IsMagicTargetValid(MagicTarget* newTarget) {return true;} // effect is not applied if false
    INLINE /*038*/ virtual void             ApplyEffect() {} // apply/initialize effect on target
    INLINE /*03C*/ virtual void             RemoveEffect() {} // remove/cleanup effect from target

    // methods   
    IMPORT void                     ApplyScalingFactor(float factor); // scales magnitude/duration by factor.  used to apply resistances, effectiveness, etc.                   
    IMPORT void                     Remove(bool immediate); // schedules effect for removal. pass true to avoid waiting for next processing cycle 
                                    // does not remove from magic targets AE list, that is done by the target itself at some later point
    IMPORT void                     ProcessEffect(float timeElapsed); // checks state flags & performs application, removal, update, & other tasks as needed
    IMPORT static ActiveEffect*     Create(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem, TESBoundObject* boundObject); //
                                    // Active Effect 'Factory' - constructs a new active effect of the appropriate type.                    
    IMPORT static void              Save(ActiveEffect& activeEffect, TESObjectREFR* parent); // serialize effect to savegame
                                    // why isn't this a member function?
    IMPORT static ActiveEffect*     Load(TESObjectREFR* parent); // deserialize from savegame, decodes type & uses DynamicCreate to allocate

    // methods - ActiveEffectList
    IMPORT static UInt16            ActiveEffectListSaveSize(ActiveEffectList* aeList, TESObjectREFR* parent); // size of serialized AE list
    IMPORT static void              SaveActiveEffectList(ActiveEffectList* aeList, TESObjectREFR* parent); // serialize AE list to savegame
    IMPORT static void              LoadActiveEffectList(ActiveEffectList* aeList, TESObjectREFR* parent); // deserialize AE list from savegame
    IMPORT static void              LinkActiveEffectList(ActiveEffectList* aeList, TESObjectREFR* parent);
    IMPORT static void              PostLinkActiveEffectList(ActiveEffectList* aeList, TESObjectREFR* parent);
    IMPORT static void              PreLoadActiveEffectList(ActiveEffectList* aeList, TESObjectREFR* parent);

    // constructor
    IMPORT ActiveEffect(MagicCaster* caster, MagicItem* magicItem, EffectItem* effectItem);

    // global static maps
    IMPORT static NiTMap<UInt32,AECreatorFunc> creatorFuncMap; // map of mgefCodes -> ActiveEffect creation funcs
        // only used for effect settings that have their own dedicated AE type (~ 30%), the rest are created using hard-coded routines
        // actually NiTMapBase< NiTPointerAllocator<unsigned int>, MagicSystem::EffectID, ActiveEffect* (__cdecl*)(MagicCaster*,MagicItem*,EffectItem*) >

    // use FormHeap for class new & delete
    USEFORMHEAP
};

#endif  // end of Game-only section