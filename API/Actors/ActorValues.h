/* 
    'ActorValues' is a generic container class used to group the various global data and functions related 
    to actor values.  As such, it has only static members and methods.  These may actually be part of
    the Actor class or some other class, but they are grouped independently here so that we don't have to
    include Actor.h every time we need a simple constant definition.

    AVCollection is used to collectively process actor values from several actor form components
    Appears in Process & Actor objects.  Bethesda may have used another name for this object.
    This class is used in the game code only, as the CS does not instantiate Actor Refs or Processes.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSSimpleList.h" 

// arument classes
class   GameSetting;    // Settings/Settings.h

class ActorValues
{
public:    

    // ActorValues - enumeration
    enum ActorValueCodes
    {
        kActorVal_Strength              = 0x00,
        kActorVal_Intelligence,
        kActorVal_Willpower,
        kActorVal_Agility,
        kActorVal_Speed,
        kActorVal_Endurance             = 0x05,
        kActorVal_Personality,
        kActorVal_Luck                  = 0x07,
        kActorVal__MAX_Attribute,
        //
        kActorVal_Health                = 0x08,
        kActorVal_Magicka,
        kActorVal_Fatigue               = 0x0A,
        kActorVal_Encumbrance           = 0x0B,
        kActorVal__MAX_DerivedAttribute,
        //
        kActorVal_Armorer               = 0x0C,
        kActorVal_Athletics,
        kActorVal_Blade,
        kActorVal_Block                 = 0x0F,
        kActorVal_Blunt                 = 0x10,
        kActorVal_HandToHand,
        kActorVal_HeavyArmor,
        kActorVal_Alchemy,            
        kActorVal_Alteration,
        kActorVal_Conjuration           = 0x15,
        kActorVal_Destruction,
        kActorVal_Illusion,
        kActorVal_Mysticism             = 0x18,
        kActorVal_Restoration,
        kActorVal_Acrobatics            = 0x1A,
        kActorVal_LightArmor,
        kActorVal_Marksman,
        kActorVal_Mercantile,
        kActorVal_Security,
        kActorVal_Sneak                 = 0x1F,
        kActorVal_Speechcraft           = 0x20,
        kActorVal__MAX_Skill,
        //
        kActorVal_Aggression            = 0x21,
        kActorVal_Confidence,
        kActorVal_Energy,
        kActorVal_Responsibility        = 0x24,
        kActorVal__MAX_Trait,
        //
        kActorVal_Bounty                = 0x25,
        kActorVal_Fame,
        kActorVal_Infamy                = 0x27,
        kActorVal__MAX_Virtue,
        //
        kActorVal_MagickaMultiplier     = 0x28,
        kActorVal_NightEyeBonus         = 0x29,
        kActorVal_AttackBonus           = 0x2A,
        kActorVal_DefendBonus           = 0x2B,
        kActorVal_CastingPenalty        = 0x2C,
        kActorVal_Blindness,
        kActorVal_Chameleon,
        kActorVal_Invisibility          = 0x2F,
        kActorVal_Paralysis             = 0x30,
        kActorVal_Silence,
        kActorVal_Confusion,
        kActorVal_DetectItemRange,
        kActorVal_SpellAbsorbChance,
        kActorVal_SpellReflectChance    = 0x35,
        kActorVal_SwimSpeedMultiplier   = 0x36,
        kActorVal_WaterBreathing,
        kActorVal_WaterWalking          = 0x38,
        kActorVal_StuntedMagicka,
        kActorVal_DetectLifeRange       = 0x3A,
        kActorVal_ReflectDamage,
        kActorVal_Telekinesis,
        kActorVal_ResistFire,
        kActorVal_ResistFrost,
        kActorVal_ResistDisease         = 0x3F,
        kActorVal_ResistMagic           = 0x40,
        kActorVal_ResistNormalWeapons,
        kActorVal_ResistParalysis,
        kActorVal_ResistPoison,
        kActorVal_ResistShock,
        kActorVal_Vampirism             = 0x45,
        kActorVal_Darkness,
        kActorVal_ResistWaterDamage     = 0x47,
        //
        kActorVal__MAX                  = 0x48, // 0x48, 0xFF, and 0xFFFFFFFF are all used to indicate
        kActorVal__NONE                 = 0xFF, // 'invalid' or 'no av'.  This can cause confusion (and bugs)
    };

    // ActorValues - static data arrays
    IMPORT static GameSetting*  actorValueNames[kActorVal__MAX_Virtue]; // 'sAttributeName***', 'sSkillName***', etc.
    IMPORT static const char*   actorValueExtraNames[kActorVal__MAX - kActorVal__MAX_Virtue]; // name strings for AVs >= kActorVal__MAX_Virtue
    IMPORT static GameSetting*  actorValueDescriptions[kActorVal__MAX_Skill]; // 'sAttributeDesc***', 'sSkillDesc***', etc.
    IMPORT static GameSetting*  actorValueIconPaths[kActorVal__MAX_Skill]; // 'sAttributeIcon***', 'sSkillIcon***', etc.
    
    // ActorValues - static methods, accept *all* avCodes
    IMPORT static const char*   ActorValueName(UInt32 avCode);
    #ifdef OBLIVION
    IMPORT static const char*   ActorValueDescription(UInt32 avCode); // apparently never used in CS
    IMPORT static const char*   ActorValueIconPath(UInt32 avCode); // apparently never used in CS
    #endif

    // ActorValues - groups
    enum ActorValueGroups
    { // inferred from CS code
        kAVGroup_Attribute          = 0x0,  // 0x00 - 0x07
        kAVGroup_DerivedAttribute   = 0x1,  // 0x08 - 0x0B
        kAVGroup_Skill              = 0x2,  // 0x0C - 0x20
        kAVGroup_Trait              = 0x3,  // 0x21 - 0x24 (for AI forms)
        kAVGroup_Virtue             = 0x4,  // 0x25 - 0x27 (for player only?)
        kAVGroup_Misc               = 0x5,  // 0x28 - 0x29  not sure of name or purpose
        kAVGroup_Combat             = 0x6,  // 0x2A - ????  not sure of name or purpose
        kAVGroup__MAX               = 0x7,  
    };

    // ActorValues - modifiers
    enum ActorValueModifiers
    {
        kAVModifier_Max         = 0x0, // stored in MiddleLow Process
        kAVModifier_Offset      = 0x1, // stored in Actor Ref
        kAVModifier_Damage      = 0x2, // stored in Low Process
    };

    // Skill mastery levels
    enum SkillMasteryLevels 
    {   
        kMastery_Novice         = 0x0,
        kMastery_Apprentice     = 0x1,
        kMastery_Journeyman     = 0x2,
        kMastery_Expert         = 0x3,
        kMastery_Master         = 0x4,
        kMastery__MAX           = 0x5,
    };  
    IMPORT static GameSetting*  skillMasteryNames[kMastery__MAX]; // 'sSkillLevel***'
    IMPORT static const char*   SkillMasteryName(UInt32 masteryLevel);

};

#ifdef OBLIVION

class IMPORTCLASS AVCollection
{// size 14/14
public:

    class AVNode
    {
    public:
        // members
        UInt32      avCode;
        float       value;
        // use FormHeap for class new & delete
        USEFORMHEAP
    };

    static const UInt32 kAVArraySize = 0x12;

    // members
    MEMBER /*00/00*/ BSSimpleList<AVNode*>  avList;     // stores any AV modifier that doesn't fit into other members
    MEMBER /*08/08*/ AVNode*                magicka;    // magicka stored directly - very frequent access
    MEMBER /*0C/0C*/ AVNode*                fatigue;    // fatigue stored directly - very frequent access
    MEMBER /*10/10*/ AVNode**               avArray;    // array of frequently accessed AV modifiers, size 0x12

    // methods
    IMPORT float        GetAV(UInt32 actorVal);
    IMPORT void         ModAV(UInt32 actorVal, float value, bool allowPositive = true);
    IMPORT void         Clear();
    IMPORT void         CopyFrom(const AVCollection& copyFrom);
    IMPORT UInt16       ModifierSaveSize();
    IMPORT void         SaveModifiers(); // serialize to current savegame
    IMPORT void         LoadModifiers(); // load from current savegame

    // construction, destruction
    IMPORT AVCollection();
    IMPORT ~AVCollection();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

#endif