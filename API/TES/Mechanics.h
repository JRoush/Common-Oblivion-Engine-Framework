/* 
    'Mechanics' is a generic container class used to group the various core mechanic functions.
    As such, it has only static members and methods.  It may actually exist as a class in Bethesdah's source, 
    or a namespace, etc., but it left no trace in the assembly.

    Some of this data may actually belong as static data in a more specific class.  However, it (mostly)
    appears in contiguous memory in the assembly image, which implies it was probably very close together
    in the original source.

    There are lierally dozens of functions to be decoded here; only a few are actually laid out so far.
*/
#pragma once

class Mechanics
{
public:

    // skill modifiers    
    IMPORT static float     LuckModifiedSkill(SInt32 skill, SInt32 luck) ; // applies luck modifier & caps between 0 & 100
    IMPORT static float     FatigueFactor(float fatigueLevel); // fatigueLevel is (current fatigue / base fatigue)

    // global calculator for magicka costs
    IMPORT static float     BaseMagickaCost(float mgefBaseCost, UInt32 area, UInt32 duration, UInt32 magnitude, bool onTarget);    
    IMPORT static UInt32    MagickaMasteryLevel(float baseMagickaCost);
    #ifdef OBLIVION
    IMPORT static float     SkillModifiedMagickaCost(float baseMagickaCost, UInt32 skill, UInt32 luck);
    #endif

    // Detection - Blindness, LightLevelm & Invis are % capped between 0-100
    #ifdef OBLIVION
    IMPORT static SInt32    DetectionLevel(SInt32 detectorSneakSkill, SInt32 targetSneakSkill, bool lineOfSight, float distance, 
                                        SInt32 detectorBlindness, SInt32 targetLightLevel, SInt32 targetInvisLevel, SInt32 targetBootWeight, 
                                        bool targetIsMoving, bool targetIsSneaking, bool targetAttackedDetector, bool targetIsInCombat, 
                                        bool targetIsRunning, bool targetIsUnderwater, bool detectorIsSleeping, bool interiorSpace);
    #endif
};