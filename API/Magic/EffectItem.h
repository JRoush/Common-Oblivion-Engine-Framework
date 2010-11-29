/*
    EffectItem - data for individual effects on magic items.  Stores effect setting & override values + additional data
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/Magic/Magic.h" // constant enumerations
#include "API/Magic/EffectSetting.h" // necessary for EffectItemList in CS
#include "API/BSTypes/BSStringT.h"
#include "API/BSTypes/BSSimpleList.h"

#include <Commctrl.h>

// argument classes
class   TESForm;        // TESForms/TESForm.h
class   TESFile;
class   Actor;
class   Script;
class   MagicItem;      // Magic/MagicItem.h

class IMPORTCLASS EffectItem
{// size 24/34
public:

    enum DialogControlIDs
    {
        IDC_Effect                      = 0x819, // COMBOBOX
        IDC_ActorValue                  = 0x816, // COMBOBOX
        IDC_Range                       = 0x818, // COMBOBOX
        IDC_Area                        = 0x81A, // EDIT
        IDC_Duration                    = 0x5F4, // EDIT
        IDC_Magnitude                   = 0x81B, // EDIT
        IDC_EffectCost                  = 0x81D, // STATIC
        IDC_ActorValueLabel             = 0x815, // STATIC
        IDC_ScriptEffectInfoFrame       = 0x822, // BUTTON
        IDC_ScriptLabel                 = 0x81F, // STATIC
        IDC_Script                      = 0x4CA, // COMBOBOX
        IDC_EditScript                  = 0x426, // BUTTON
        IDC_NameLabel                   = 0x820, // STATIC
        IDC_Name                        = 0x817, // EDIT
        IDC_SchoolLabel                 = 0x821, // STATIC
        IDC_School                      = 0x441, // COMBOBOX
        IDC_MagicItemCost               = 0x81E, // STATIC
        IDC_EffectBaseCost              = 0x81C, // STATIC
        IDC_MagicItemMasteryLabel       = 0x826, // STATIC
        IDC_MagicItemMastery            = 0x823, // STATIC
        IDC_VFX                         = 0x824, // COMBOBOX
        IDC_Hostility                   = 0x3EE, // BUTTON
    };
    static const UInt32     IDD_EffectItem = 0x10B; // Dialog Tempate ID

    class ScriptEffectInfo
    {// size 18/18
    public:
        // members
        MEMBER /*00/00*/ UInt32         scriptFormID;
        MEMBER /*04/04*/ UInt32         school;
        MEMBER /*08/08*/ BSStringT      name;
        MEMBER /*10/10*/ UInt32         fxMgefCode; // effect setting from which FX are taken
        MEMBER /*14/14*/ bool           hostile;
        MEMBER /*15/15*/ UInt8          pad15[3];
        // use FormHeap for class new & delete
        USEFORMHEAP
    };

    // members
    // NOTE: setting these directly can break invariants assumed by game code (that 'effect' is a valid pointer, etc.)
    // wherever possible, use the Get/Set methods instead
    MEMBER /*00/00*/ UInt32                mgefCode;
    MEMBER /*04/04*/ SInt32                magnitude;
    MEMBER /*08/08*/ SInt32                area;
    MEMBER /*0C/0C*/ SInt32                duration;
    MEMBER /*10/10*/ UInt32                range;
    MEMBER /*14/14*/ UInt32                actorValue;  // this can be set directly, as no methods exist to do so
    MEMBER /*18/18*/ ScriptEffectInfo*  scriptInfo; // set to a new ScriptEffectInfo if SEFF, null otherwise
    MEMBER /*1C/1C*/ EffectSetting*     effect; // quite a bit of game code will CTD if this is null
    MEMBER /*20/20*/ float                cost;
    #ifndef OBLIVION
    // These fields are initialized before or just after an EffectItem is edited in the Efit dialog
    MEMBER /*24/24*/ EffectSetting*     filterMgef; // filter effect from source magic item
    MEMBER /*28/28*/ SInt32             origBaseMagicka; // pre-editing base magicka cost
    MEMBER /*2C/2C*/ SInt32             origItemMagicka; // pre-editing base magicka cost of magic item
    MEMBER /*30/30*/ UInt32             origItemMastery; // pre-editing mastery of magic item (not used by vanilla dialog)
    #endif

    // methods - other effect items
    IMPORT bool                 CompareTo(const EffectItem& compareTo) const ; // return false if equivalent
                                // compares param even if not used
    IMPORT void                 CopyFrom(const EffectItem& copyFrom); 
    #ifdef OBLIVION
    IMPORT bool                 Match(const EffectItem& compareTo); // returns true if they match
                                // compares effect code and (if used) the actorVal.  
                                // used for alchemy; effect items that match contribute their effect to the final potion
    #endif

    // methods - serialization
    IMPORT bool                 Load(TESFile& file, const char* parentEditorID); // deserialize from current file record.  
                                // editorid of parent form is used only when printing error messages
    IMPORT void                 Save(); // serialize to current file record, including scripteffectinfo if present

    // methods - magnitude, duration, area, range, effect setting
    IMPORT SInt32               GetMagnitude() const ; // always returns 0 if effect cannot have magnitude
    IMPORT bool                 SetMagnitude(SInt32 magnitude); // returns false on failure
    IMPORT SInt32               GetArea() const ; // always returns 0 if effect cannot have area
    IMPORT bool                 SetArea(SInt32 area); // returns false on failure
    IMPORT SInt32               GetDuration() const ; // always returns 0 if effect cannot have duration
    IMPORT bool                 SetDuration(SInt32 duration); // returns false on failure
    INLINE UInt32               GetRange() const {return range;} // completely inlined in game/CS code
    IMPORT bool                 SetRange(UInt32 range);  // returns false on failure
    INLINE EffectSetting*       GetEffectSetting() const {return effect;} // completely inlined in game/CS code
    IMPORT void                 SetEffectSetting(const EffectSetting& mgef); // sets effect & creates/cleans up script info.  

    // methods - magicka cost
    IMPORT float                MagickaCost(); // base magicka cost
    #ifdef OBLIVION
    IMPORT float                MagickaCostForCaster(Actor* caster = 0); // magicka cost modified by caster skill
    #endif

    // methods - properties of underlying effect, use instead of accessing the effect directly
    IMPORT UInt32               GetSchool() const ;
    IMPORT Script*              GetScript() const ; // returns null if not a script effect
    IMPORT bool                 IsHostile() const ;
    IMPORT UInt32               GetSummonedForm() const ; // returns summoned formID for summoning effects, 0 otherwise
    IMPORT BSStringT            GetEffectName() const ; // copy of effect name, or script effect name if present. 
                                // actually 'BSStringT& GetEffectName(BSStringT& temp)', temp arg handled by compiler
    #ifdef OBLIVION    
    IMPORT BSStringT            GetDisplayText(UInt32 magicType, float effectiveness, bool noDuration, bool noRange, bool onStrikeRange) const ; //
                                // Complete item text as shown in game menus.  OnStrikeRange only applies if noRange is false
                                // actually 'BSStringT& GetDisplayText(BSStringT& temp, ...)', temp arg handled by compiler
    IMPORT BSStringT            GetDisplayText(const MagicItem& parentItem, float effectiveness = 1.0) const ; // overload for convenience
    IMPORT void                 GetAVQualifiedName(char* buffer) const ; // effective name 
                                // for ValueModifier effects w/ the UseSkill/UseAttribute flags
    #endif

    // methods - dialog interface for CS
    #ifndef OBLIVION    
    IMPORT void                 BuildActorValueList(HWND comboList); // build AV list appropriate to item
    IMPORT void                 BuildEffectList(HWND comboList); // build effect list using filter mgef 
    IMPORT void                 BuildRangeList(HWND comboList); // build range list using flags from effect
    IMPORT void                 BuildSchoolList(HWND comboList); // build school list
    IMPORT void                 BuildScriptList(HWND comboList); // build list of magic effect scripts
    IMPORT void                 InitializeDialog(HWND dialog); // initialize dialog & temp effect item   
    IMPORT void                 SetInDialog(HWND dialog); // set dialog controls from effect members
    IMPORT void                 GetFromDialog(HWND dialog); // update effect item from dialog controls      
    IMPORT bool                 HandleDialogEvent(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); //
                                // non-static method for handling events that require access to members
    IMPORT static bool          DialogWndProc(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam); // WndProc for effect item dialog
    #endif

    // construction, destruction
    IMPORT void                 Initialize(const EffectSetting& effect);    // common code between constructors
    IMPORT                      EffectItem(const EffectSetting& effect);
    IMPORT                      EffectItem(const EffectItem& source);
    IMPORT                      ~EffectItem();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

class IMPORTCLASS EffectItemList : public BSSimpleList<EffectItem*>
{// size 10/DC
public:

    enum DialogControlIDs // default control IDs
    {
        IDC_EffectList      = 0x815, // SysListView32
    };

    // members
    //     /*00/00*/ void**             vtbl;    
    //     /*04/04*/ Node<EffectItem*>  firstEffect;
    MEMBER /*0C/0C*/ UInt32             hostileCount; // includes 'ignored' effects, so technically broken, but no vanilla effects are ignored
    #ifndef OBLIVION
    MEMBER /*--/10*/ EffectSetting      filterEffect; // used as a filter for allowable magic effects
    #endif

    // virtural methods:
    #ifdef OBLIVION
    IMPORT /*00/00*/ virtual float      GetMagickaCost(const Actor* caster); // total magicka cost
    #else
    IMPORT /*00/00*/ virtual SInt32     GetMagickaCost(const Actor* caster); // total magicka cost, rounds down to nearest int
                                        // caster ref is vestigial (ignored)
    #endif
    IMPORT /*04/04*/ virtual UInt32     GetMaxEffectCount(); // 0 indicates no maximum
    IMPORT /*08/08*/ virtual UInt32     GetMasteryLevel(); 

    // methods - add, remove, iterate effect items
    IMPORT void                 Remove(const EffectItem* item); // remove item & update hostile count. Does NOT delete item.
    IMPORT void                 Add(EffectItem* item); // append item to end & update hostile count.
    IMPORT void                 Clear(); // delete all items & nodes.  WARNING: Does NOT zero hostile count (probably a bug) 
    IMPORT EffectItem*          GetItemByIndex(SInt32 index); // get item by position 
    #ifdef OBLIVION
    IMPORT UInt8                GetIndexOfItem(EffectItem* item);
    #endif
    
    // methods - aggregate properties
    IMPORT bool                 HasEffect(UInt32 mgefCode, UInt32 avCode); // list has specified effect code.   
                                // avCode is checked only for effects that use the ActorValue field on effect items
                                // pass an avCode of 72 (ActorVal__MAX) to skip the avCoder check
    IMPORT bool                 HasEffectWithFlag(UInt32 mgefFlag);                    
    IMPORT bool                 HasHostileEffect(); // used to determine hostile/nonhostile for spells & enchantments
    IMPORT bool                 HasAllHostileEffects(); // used to determine potion/poison for alchemy items
    IMPORT EffectItem*          GetStrongestItem(UInt32 range = Magic::kRange__MAX, bool areaRequired = false); //
                                // range = 3 (kRange__MAX) for any range.  areaRequired = true for only AOE effects
    #ifdef OBLIVION        
    IMPORT bool                 HasAreaEffect(); // checks for effects with area > 1
    IMPORT bool                 HasEffectCode(UInt32 count, ...); // checks for any effect with the specified codes
                                // '...' is a list of effect codes, count is the number of codes given
    IMPORT bool                 HasIgnored(); // checks for an effect with the 'ignored' flag (1<<22)
    IMPORT bool                 HasScriptEffect(Script* targetScript); // checks for a scripted effect with the specified script
    IMPORT bool                 HasSelfEffect();
    IMPORT bool                 HasTouchEffect();
    IMPORT bool                 HasTargetEffect();
    IMPORT UInt32               GetSchoolAV(); // avCode (not school code) for overall school
    IMPORT BSStringT            SkillRequiredText(); // e.g. 'Requires Illusion skill of 50'
                                // actually 'BSStringT& SkillRequiredText(BSStringT& temp, ...)', temp arg handled by compiler
    IMPORT BSStringT            MagicSchoolText(); // e.g. 'Magic School : Restoration'
                                // actually 'BSStringT& MagicSchoolText(BSStringT& temp, ...)', temp arg handled by compiler
    #endif

    // methods - other effect item lists
    IMPORT bool                 CompareEffectsTo(const EffectItemList& compareTo); // return false if equivalent                       
    IMPORT void                 CopyEffectsFrom(const EffectItemList& copyFrom); // uses Clear(), so hostile count may not be copied properly
                                // does not copy filter effect, in CS
     
    // methods - serialization
    IMPORT void                 SaveEffects(); // serialize entire list to current record
    IMPORT void                 LoadEffects(TESFile& file, const char* parentEditorID); // deserialize single item from current file record.  
                                // editorid of parent form is used only when printing error messages
    // methods - CD dialog
    #ifndef OBLIVION
    IMPORT bool                 EffectListDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); //
                                // non-static method for handling events that require access to members
    IMPORT void                 PopulateDialogEffectList(HWND listView); // clears & populates listview control with effect items
    IMPORT static void          InitializeDialogEffectList(HWND listView); // size list & setup columns
    IMPORT bool                 ValidateEffectForDialog(EffectItem& item); // checks item against filter effect, return true if valid
    IMPORT void                 ValidateEffectListForDialog(HWND dialog); // checks each item with ValidateItem() & notifies user of problems
    IMPORT static void          GetEffectDisplayInfo(void* displayInfo); // used callback to build display strings
                                // for effect lists in magic item & sigil stone dialogs
                                // argument is a NMLVDISPINFO*, defined in <Commctrl.h>
    IMPORT static int           EffectItemComparator(const EffectItem& itemA, const EffectItem& itemB, int compareBy); //
                                // callback for sorting effect item lists.  returns -1 (A<B), 0 (A==B), +1 (A>B). 
                                // compareBy is the column index in the listview, and is negative for reversed ordering
    #endif

    // constructor, destructor
    IMPORT EffectItemList();
    INLINE ~EffectItemList() {Clear();} // inlined by game/CS code, clears list

    // use FormHeap for class new & delete
    USEFORMHEAP
};