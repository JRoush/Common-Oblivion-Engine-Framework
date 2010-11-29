/* 
    'Magic' is a generic container class used to group the various global data and functions related 
    to the magic system.  As such, it has only static members and methods.  It may actually exist as 
    a class in Bethesdah's source, or a namespace, etc., but it left no trace in the assembly.

    Some of this data may actually belong as static data in a more specific class.  However, it (mostly)
    appears in contiguous memory in the assembly image, which implies it was probably very close together
    in the original source.
*/
#pragma once

// argument classes
class   GameSetting;        // Settings/Settings.h
class   SpellItem;          // Magic/MagicItemForm.h
class   TESEffectShader;
class   TESSound;

class Magic
{
public:

    // Schools
    enum Schools
    {
        kSchool_Alteration      = 0x0,
        kSchool_Conjuration     = 0x1,
        kSchool_Destruction     = 0x2,
        kSchool_Illusion        = 0x3,
        kSchool_Mysticism       = 0x4,
        kSchool_Restoration     = 0x5,
        kSchool__MAX            = 0x6,  // used as an 'Invalid' or 'None' value
    };   
    IMPORT static GameSetting*      schoolNames[kSchool__MAX]; // 'sMagicSchool***'
                                    // there are also 3 built-in sounds - CastFailure, EnchantHit (?), 
                                    // and EnchantDraw (?), for each of the 6 schools
    #ifdef OBLIVION   
    IMPORT static UInt32            GetSkillAVForSchool(UInt32 school);  // In game, school is linked to a skill, and uses the 
    IMPORT static UInt32            GetSchoolForSkillAV(UInt32 skillAV); // skill's name instead of the gmst value
    IMPORT static TESSound*         GetSchoolCastFailureSound(UInt32 school);
    #else    
    IMPORT static const char*       GetSchoolName(UInt32 school); // In CS, school name is fetched from gmst.      
                                    // Schools are not explicitly linked with skills.  Heavily inlined
    #endif

    // magic types - these correspond to (perhaps are precisely) the type codes of magic items
    enum MagicTypes
    {
        kMagicType_Spell           = 0x0,
        kMagicType_Disease         = 0x1,
        kMagicType_Power           = 0x2,
        kMagicType_LesserPower     = 0x3,
        kMagicType_Ability         = 0x4,
        kMagicType_Poison          = 0x5,   // Apparently dropped for potions w/ all hostile effects, but still included in a lot of code
        kMagicType_Enchantment     = 0x6,
        kMagicType_AlchemyItem     = 0x7,
        kMagicType_Ingredient      = 0x8,
        kMagicType__MAX            = 0x9
    };
    IMPORT static GameSetting*      magicTypeNames[kMagicType__MAX]; // 'sMagicType***'
    IMPORT static const char*       GetMagicTypeName(UInt32 magicType); // fetched from gmsts
                                    // heavily inlined

    // casting types - these correspond to (perhaps are precisely) the four types of enchantments
    enum CastTypes
    {
        kCast_Once              = 0x0, // Scroll
        kCast_WhenUsed          = 0x1, // Staff
        kCast_WhenStrikes       = 0x2, // Weapon
        kCast_Constant          = 0x3, // Apparel
        kCast__MAX              = 0x4
    };
    IMPORT static GameSetting*      castTypeNames[kCast__MAX]; // 'sMagicCast***'
    #ifndef OBLIVION
    IMPORT static const char*       GetCastTypeName(UInt32 castType); // fetched from gmsts.  
                                    // Apparently never used in game code.  Heavily inlined
    #endif
    
    // effect ranges
    enum RangeTypes
    {
        kRange_Self             = 0x0,
        kRange_Touch            = 0x1,
        kRange_Target           = 0x2,
        kRange__MAX             = 0x3  // used in some places as an 'any' or 'all' value
    };
    IMPORT static GameSetting*      rangeNames[kRange__MAX]; // 'sMagicRange***'
    IMPORT static const char*       GetRangeName(UInt32 range); // fetched from gmsts
                                    // heavily inlined

    // projectile types
    enum ProjectileTypes
    {
        kProjType_Ball          = 0x0,
        kProjType_Bolt          = 0x1,
        kProjType_Spray         = 0x2,
        kProjType_Fog           = 0x3,
        kProjType__MAX          = 0x4   // this and all other values seem to default to 'ball' in many cases
    };
    IMPORT static GameSetting*      projectileTypeNames[kProjType__MAX]; // 'sMagicProjectileType***'
                                    // access function was apparently completely inlined away - only used once (in Magic Effect Dlg) anyway
    #ifdef OBLIVION
    IMPORT static void              GetProjectileDistance(UInt32 projType, float& optimalDist, float& maxDist); // 
                                    // fetches max/optimal distance from gmsts : 'fMagic***MaximumDistance'/'fMagic***OptimalDistance'
                                    // distances are not referenced in the CS
    #endif

    // built-in spells - once created, they are referenced only through these pointers (e.g. not looked up by formid)
    IMPORT static SpellItem*        defaultPlayerSpell; // guarantees that player always has a valid magic item equipped
    IMPORT static SpellItem*        marksmanParalyzeSpell; // for master perk (paralysis)

    // built-in shaders - once created, they are referenced only through these pointers (e.g. not looked up by formid)
    // NOTE - due to an apparent error by Bethesda, the reflect shader is called 'effectAbsorb' in the CS, and vice-versa
    IMPORT static TESEffectShader*  absorbShader; // applied when a spell is absorbed.
    IMPORT static TESEffectShader*  reflectShader; // applied when a spell is reflected
    IMPORT static TESEffectShader*  lifeDetectedShader; // applied to detected actors for the Detect Life effect
 
};