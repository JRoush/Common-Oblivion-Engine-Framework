/* 
    EffectSetting, aka 'Magic Effect'.  This class stores relatvent global data for each of the 'named'
    magic effects (e.g. 'Fire Damage') in Oblivion, including 3D effects, icon, base cost, etc.

    EffectSettings are not tracked through the DataHandler like other forms - they have their own map,
    indexed by 'magic effect code'.  This code is static, and is used to uniquely identify an effect
    through the engine *instead* of formids.  EffectSettings are hard coded into the engine without
    preset formids.
*/
#pragma once

// base classes
#include "API/Magic/Magic.h" // constant enumerations
#include "API/TESForms/TESForm.h"
#include "API/TESForms/BaseFormComponent.h" // TESModel, TESDescription, TESFullName, TESIcon
#include "API/NiTypes/NiTMap.h"

// argument classes
class   TESObjectLIGH;
class   TESEffectShader;
class   TESSound;
class   EffectSetting;  

class IMPORTCLASS EffectSettingCollection : public NiTMap< UInt32, EffectSetting* >
{// size 10/10
public:

    typedef BSSimpleList<EffectSetting*> FilteredEffectList;

    // virtual method overrides             
    IMPORT /*000/000*/ virtual             ~EffectSettingCollection(); 

    // methods
    IMPORT void                         RemoveAll(); // destroy & remove all stored effect settings

    // static methods - applied to global instance
    IMPORT static EffectSetting*        LookupByCode(UInt32 mgefCode); // returns null if not found
    IMPORT static EffectSetting*        LookupByCodeString(const char* mgefCodeString); // returns null if not found.
                                        // interprets first 4 chars as byte-reversed integer code
                                        // argument *must* point to a valid  c-style string (null-terminated).  Returns null is string size < 4    
    IMPORT static int                   Comparator(EffectSetting& effectA, EffectSetting& effectB); // returns -1 (A<B), 0 (A==B), +1 (A>B)
                                        // sorting comparator for effects, compares full names
    IMPORT static FilteredEffectList*   BuildFilteredList(EffectSetting* templateEffect, bool lazyFlagMatch, UInt32 clearedFlags, bool sort); //
                                        // builds a filtered list from map, using EffectSetting::Filter() with the supplied args
                                        // If sort is true, list is sorted from least to greatest using Comparator()
    IMPORT static EffectSetting*        Add(UInt32 mgefCode, const char* name, UInt32 school, float baseCost, UInt32 param, 
                                                UInt32 mgefFlags, UInt32 resistAV, UInt16 numCounters, .../*counter effect codes*/); //
                                        // Allocate & initialize a new effect setting (copy arguments), and add to map
    IMPORT static void                  AddAll(); // adds all built-in magic effects to map, with default parameters
    IMPORT static void                  Reset(); // clears map & re-add all built-in effects
    
    // constructor
    IMPORT EffectSettingCollection();

    // unique global instance of collection
    IMPORT static EffectSettingCollection   collection;
};

class IMPORTCLASS EffectSetting : public TESFormIDListView, public TESModel, public TESDescription, public TESFullName, public TESIcon
{// size A8/CC
/*
    Partial:
    -   members
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    #ifndef OBLIVION
    IMPORT static HWND      dialogHandle; // used to track single instance of editing dialog
    #endif

    enum EffectSettingFlags
    {   //                                      # Can be overriden in a vanilla mod file (other overrides are discarded by EffectSetting.Load())
        //                                        + Displayed in CS with a checkbox                                       
        kMgefFlag_Hostile               = /* 00   + */ 0x00000001,
        kMgefFlag_Recovers              = /* 01   + */ 0x00000002, // used for some effect types - changes are reversed when removed
        kMgefFlag_Detrimental           = /* 02   + */ 0x00000004, // used for ValueModifier effects - AV is decreased rather than increased
        kMgefFlag_MagnitudeIsPercent    = /* 03   + */ 0x00000008, // for display purposes only
        kMgefFlag_OnSelf                = /* 04   + */ 0x00000010,
        kMgefFlag_OnTouch               = /* 05   + */ 0x00000020,
        kMgefFlag_OnTarget              = /* 06   + */ 0x00000040,
        kMgefFlag_NoDuration            = /* 07   + */ 0x00000080,
        kMgefFlag_NoMagnitude           = /* 08   + */ 0x00000100,
        kMgefFlag_NoArea                = /* 09   + */ 0x00000200,
        kMgefFlag_FXPersists            = /* 0A # + */ 0x00000400, // Effect (Hit) shader persists until effect expires (vs. playing only once)
        kMgefFlag_Spells                = /* 0B # + */ 0x00000800, // ignored in CS, filters for spellmaking menu in game
        kMgefFlag_Enchantments          = /* 0C # + */ 0x00001000, // ignored in CS, filters for enchanting menu in game
        kMgefFlag_NoAlchemy             = /* 0D # + */ 0x00002000, // ignored in CS, no effect in game
        kMgefFlag_UnknownF              = /* 0E     */ 0x00004000, // no effects have this flag set, checks the missing checkbox (nDlgItem = 0x666)
        kMgefFlag_NoRecast              = /* 0F   + */ 0x00008000, // no effects have this flag set, not sure quite what it does
        kMgefFlag_UseWeapon             = /* 10   + */ 0x00010000, // MgefParam is formID of summoned weapon
        kMgefFlag_UseArmor              = /* 11   + */ 0x00020000, // MgefParam is formID of summoned armor
        kMgefFlag_UseActor              = /* 12   + */ 0x00040000, // MgefParam is formID of summoned actor
        kMgefFlag_UseSkill              = /* 13   + */ 0x00080000, // uses skill avCode stored on effect items
        kMgefFlag_UseAttribute          = /* 14   + */ 0x00100000, // uses attribute avCode stored on effect items
        kMgefFlag_PCHasEffect           = /* 15     */ 0x00200000, // Whether no not the player 'knows' the effect, e.g. useable in spellmaking
                                                                    // Can technically be overriden, but is immediately forced to zero during the
                                                                    // EffectSetting.Load() routine. Set for any spell, or only one pc can cast?
                                                                    // List of effects player knows is apparently stored as mgefCodes in savegame
        kMgefFlag_Disabled              = /* 16 #   */ 0x00400000, // No default effects have this.  many (all?) of the methods in EffectItemList 
                                                                    // that loop over the effect items ignore effects with this flag.
                                                                    // Spells with an effect with this flag are apparently uncastable.
        kMgefFlag_UnknownO              = /* 17 #   */ 0x00800000, // POSN,DISE - these effects have *only* this bit set, perhaps a flag for 'meta' effects?
                                                                    // exempts effect from valid range check during creation
                                                                    // makes effect unavailable in spellmaking alter
        kMgefFlag_UseActorValue         = /* 18 #   */ 0x01000000, // MgefParam field is avCode for target value
                                                                    // Once set (by default or by a previously loaded mod file), it *cannot* be unset in
                                                                    // EffectSetting.Load(), and the 'Data' field cannot be overriden either.
        kMgefFlag_ProjectileTypeLow     = /* 19 #   */ 0x02000000, // 2-bit field for projectile type: {00 = Ball, 01 = Spray, 10 = Bolt, 11 = Fog}
        kMgefFlag_ProjectileTypeHigh    = /* 1A #   */ 0x04000000, //  These are set with a pull-down list rather than a checkbox
        kMgefFlag_NoHitVisualFX         = /* 1B # + */ 0x08000000, // No Hit shader or Hit model VFX are played on target.  Doesn't affect sounds.
        kMgefFlag_PersistOnDeath        = /* 1C     */ 0x10000000, // Effect is not automatically removed when its target dies
        kMgefFlag_ExplodesWithForce     = /* 1D     */ 0x20000000, // causes explosion that can move loose objects (e.g. ragdolls)
        kMgefFlag_MagnitudeIsLevel      = /* 1E     */ 0x40000000, // for display purposes only
        kMgefFlag_MagnitudeIsFeet       = /* 1F     */ 0x80000000, // for display purposes only
    };

    // global list of flags with corresponding checkboxes in CS dialog
    IMPORT static UInt32                DisplayedFlagCount;
    struct DisplayedFlagData
    {// size 08
        MEMBER /*00*/ UInt32    mgefFlag;       // flag value
        MEMBER /*04*/ bool      overridable;    // can be overriden by a file during loading (checkbox enabled)
        MEMBER /*05*/ UInt8     pad[3];
    };    
    IMPORT static DisplayedFlagData     DisplayedFlags[];
    #ifndef OBLIVION
    IMPORT static UInt32                DisplayedFlagDlgIDCs[]; // dialog control IDs for flag checkboxes (see enumeration above)
    #endif
    
    // global list of hidden but overridable flags
    IMPORT static UInt32                OverridableFlagCount;
    IMPORT static UInt32                OverridableFlags[];

    // typedef for filtering callback.  function returns true if effect passes, given the filter param
    typedef bool (*FilterFunc)(const EffectSetting& effect, void* filterParam);

    // members
    //     /*00/00*/ TESForm           
    //     /*18/24*/ TESModel
    //     /*30/48*/ TESDescription
    //     /*38/58*/ TESFullName 
    //     /*44/64*/ TESIcon    
    MEMBER /*50/7C*/ FilterFunc         filterFunc; // not saved. called if present on a template effect during Filter(), 
                                        // using target effect & filterParam as args.
    MEMBER /*54/80*/ void*              filterParam; // not saved. passed to filterFunc if it's called during Filter()
    MEMBER /*58/84*/ UInt32             mgefFlags;
    MEMBER /*5C/88*/ float              baseCost;
    MEMBER /*60/8C*/ UInt32             mgefParam;
    MEMBER /*64/90*/ UInt32             school;
    MEMBER /*68/94*/ UInt32             resistAV; // an actor value i.e. kActorVal_ResistFire
    MEMBER /*6C/98*/ UInt16             numCounters; // count of counter effects, is size of counterArray
    MEMBER /*6E/9A*/ UInt16             padMgef06E;
    MEMBER /*70/9C*/ TESObjectLIGH*     light;
    MEMBER /*74/A0*/ float              projSpeed;
    MEMBER /*78/A4*/ TESEffectShader*   effectShader;
    MEMBER /*7C/A8*/ TESEffectShader*   enchantShader;
    MEMBER /*80/AC*/ TESSound*          castingSound;
    MEMBER /*84/B0*/ TESSound*          boltSound;
    MEMBER /*88/B4*/ TESSound*          hitSound;
    MEMBER /*8C/B8*/ TESSound*          areaSound;
    MEMBER /*90/BC*/ float              enchantFactor;
    MEMBER /*94/C0*/ float              barterFactor;
    MEMBER /*98/C4*/ UInt32             mgefCode; // in CS, editor ID is initialized to this re-interpreted as a 4-character string
    MEMBER /*9C/C8*/ UInt32*            counterArray; // counters stored as dynamic array[numCounters] of effect codes
    #ifdef OBLIVION
    MEMBER /*A0/--*/ SInt32             unkMgefA0; // not saved.  something to do with queued loading for the effect's model.
    MEMBER /*A4/--*/ SInt32             unkMgefA4; // not saved.  
    #endif

    // virtual methods - TESFormIDListView
    IMPORT /*010/034*/ virtual              ~EffectSetting();
    IMPORT /*01C/040*/ virtual bool         LoadForm(TESFile& file); 
    IMPORT /*024/048*/ virtual void         SaveFormChunks();
    IMPORT /*06C/070*/ virtual void         LinkForm();
    IMPORT /*0B4/0B8*/ virtual void         CopyFrom(TESForm& form); 
    IMPORT /*0B8/0BC*/ virtual bool         CompareTo(TESForm& compareTo);
    #ifndef OBLIVION
    IMPORT /*---/0F4*/ virtual void         RemoveFormReference(TESForm& form); //
    _NOUSE /*---/0F8*/ virtual bool         UnkForm0F8(UInt32 arg0) {return false;}
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); 
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    IMPORT /*---/11C*/ virtual void         CleanupDialog(HWND dialog);
    #endif

    // no additional virtual methods

    // methods
    IMPORT bool         Filter(EffectSetting* templateEffect, bool lazyMatchSetFlags, UInt32 clearedFlag) const ; // returns true if effect matches filter
                        // always returns false for effects with flag (1<<23), the 'meta' flag
                        // template filter callback is called, with this effect and template filter param as arguments
                        // flags set by template must be set (all flags, or at least one if lazyMatchSetFlags)
                        // clearedFlag must not be set (if it contains multiple flags, only having them all set will fail)
                        // school must match template school    
    IMPORT void         SetCounterEffects(UInt16 effectCount, UInt32* counterEffectList);
    IMPORT UInt32       GetProjectileType() const ;
    #ifndef OBLIVION
    IMPORT void         SetProjectileType(UInt32 newType);  // apparently never used in the game
    #endif

    // convenience methods
    INLINE static EffectSetting*        LookupByCode(UInt32 mgefCode) {return EffectSettingCollection::LookupByCode(mgefCode);}
    INLINE static EffectSetting*        LookupByCodeString(const char* mgefCodeString) {return EffectSettingCollection::LookupByCodeString(mgefCodeString);} 
    
    // constructors
    IMPORT EffectSetting();
};

