/* 
    TESForm is the base class for all object 'types' (or 'Base forms') in the game.
    It provides a common, very sophisticated interface for Serialization, Revision Control,
    Inter-form references, and editing (in the CS).

    NOTE on the CS reference-tracking:
    A Form may point to other forms through it's member fields; these are it's "Form", or "Component" references.
    These list of other forms that point to this one are it's "Cross" references.
    The exception is if this form is a base for a placed TESObjectREFR - refs that point to this form are it's "Object" references.
    The notation is admittedly akward, a result of choosing names before a clear picture of the system emerged.
    Form and Cross references are tracked individually through a global table.  Object refs are not tracked individually, but
    every instance of TESBoundObject does maintain a cell-by-cell count.

    NOTE on dependencies:
    TESForm has several container objects (formID map, editorID map, etc) that implement NetImmerse and Bethesda container classes.
    By default, the headers for these classes are included here so that the member objects can be used noramlly.
    The TESFORM_MINIMAL_DEPENDENCIES macro can be defined to prevent this (eliminating TESForm.h dependency on BSTCaseInsensitiveStringMap.h, 
    NiTMap.h, and NiTArray.h).  This will also prevent the container objects from being defined, so use this macro with due care.    
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h"  // BaseFormComponent
#include "API/BSTypes/BSSimpleList.h"
#include "API/BSTypes/BSStringT.h"
#ifndef TESFORM_MINIMAL_DEPENDENCIES
#include "API/BSTypes/BSTCaseInsensitiveStringMap.h"
#include "API/NiTypes/NiTMap.h"
#include "API/NiTypes/NiTArray.h"
#endif

// argument classes
class   TESFile;        // TESFiles/TESFile.h
class   RecordInfo;     // TESFiles/TESFile.h
class   TESObjectREFR;  // TESForms/TESObjectREFR.h
template <class TKEY, class TVAL> class NiTMap; // NiTypes/NiTMap.h

struct TrackingData 
{// size 04/04
/*
    Used by the built-in revision-control in the CS, which seems to be disabled in the public version
    This struct is public (not a subclass of TESForm) because it is also used by TESFile
*/
    UInt16  date;               // 00 low byte is day of month, high byte is number of months with 1 = Jan. 2003 (Decembers are a little weird)
    UInt8   lastUser;           // 02 userID that last had this form checked out
    UInt8   currentUser;        // 03 userID that has this form checked out
};

class IMPORTCLASS TESForm : public BaseFormComponent
{// size 018/024
/*
    Partial:
    -   virtual methods
    Notes:
    -   NOUSE virtual methodsa are defined as stubs here so that the compiler will not object to derived classes
*/
public:

    typedef BSSimpleList<TESForm*>  FormReferenceList;  // for reference tracking in the CS

    enum FormType
    {
        kFormType_None          = 0x00,
        kFormType_TES4,
        kFormType_Group,
        kFormType_GMST,
        kFormType_Global,
        kFormType_Class,
        kFormType_Faction,
        kFormType_Hair,
        kFormType_Eyes          = 0x08,
        kFormType_Race,
        kFormType_Sound,
        kFormType_Skill,
        kFormType_EffectSetting,
        kFormType_Script,
        kFormType_LandTexture,
        kFormType_Enchantment,
        kFormType_Spell         = 0x10,
        kFormType_BirthSign,
        kFormType_Activator     = 0x12,
        kFormType_Apparatus     = 0x13,
        kFormType_Armor         = 0x14,
        kFormType_Book          = 0x15,
        kFormType_Clothing      = 0x16,
        kFormType_Container     = 0x17,
        kFormType_Door          = 0x18,
        kFormType_Ingredient    = 0x19,
        kFormType_Light         = 0x1A,
        kFormType_Misc          = 0x1B,
        kFormType_Static        = 0x1C,
        kFormType_Grass         = 0x1D,
        kFormType_Tree          = 0x1E,
        kFormType_Flora         = 0x1F,
        kFormType_Furniture     = 0x20,
        kFormType_Weapon        = 0x21,
        kFormType_Ammo          = 0x22,
        kFormType_NPC           = 0x23,
        kFormType_Creature      = 0x24,
        kFormType_LeveledCreature,
        kFormType_SoulGem       = 0x26,
        kFormType_Key           = 0x27,
        kFormType_AlchemyItem   = 0x28,
        kFormType_SubSpace,
        kFormType_SigilStone,
        kFormType_LeveledItem,
        kFormType_SNDG,
        kFormType_Weather,
        kFormType_Climate,
        kFormType_Region,
        kFormType_Cell          = 0x30,
        kFormType_REFR,
        kFormType_ACHR,
        kFormType_ACRE,
        kFormType_PathGrid,
        kFormType_WorldSpace,
        kFormType_Land,
        kFormType_TLOD,
        kFormType_Road          = 0x38,
        kFormType_Topic,
        kFormType_TopicInfo,
        kFormType_Quest,
        kFormType_Idle,
        kFormType_Package,
        kFormType_CombatStyle,
        kFormType_LoadScreen,
        kFormType_LeveledSpell  = 0x40,
        kFormType_AnimObject,
        kFormType_WaterForm,
        kFormType_EffectShader,
        kFormType_TOFT          = 0x44,
        kFormType__MAX          = 0x45
    };

    enum FormFlags
    {
        kFormFlags__SavedInRecord              = 0x000A8EE0,    // flag bits copied to record during serialization
        kFormFlags_FromMaster           = /*00*/ 0x00000001,   // form is from an esm file
        kFormFlags_FromActiveFile       = /*01*/ 0x00000002,   // form is overriden by active mod or save file
        //kFormFlags_Loaded             = /*02*/ 0x00000004,   // ?? (from OBSE ModEntry defininition)
        kFormFlags_Linked               = /*03*/ 0x00000008,   // set after formids have been resolved into TESForm*
        kFormFlags_Deleted              = /*05*/ 0x00000020,   // set on deletion, not saved in CS or savegame
        kFormFlags_QuestItem            = /*0A*/ 0x00000400,   // aka Persistent Reference for TESObjectREFR
        kFormFlags_Ignored              = /*0C*/ 0x00001000,   // (records)
        kFormFlags_Temporary            = /*0E*/ 0x00004000,   // not saved in CS, probably game as well
        kFormFlags_Compressed           = /*12*/ 0x00040000,   // (records)

        // TODO - move these to the appropriate form class:
        //kFormFlags_Dangerous          = /*11*/ 0x00020000,   // TESObjectACTV
        //kFormFlags_BorderRegion       = /*06*/ 0x00000040,   // TESRegion
    };

    enum ModifiedFlags
    {
        kModified_FormFlags = 0x00000001,
    };    

    struct FormTypeInfo
    {// size 0C/0C
        UInt32          formType;   // 00 form type code, also offset in array
        const char*     shortName;  // 04 offset to 4-letter type descriptor: 'GRUP', 'ARMO', etc.
        UInt32          chunkType;  // 08 appears to be name string in byte-reversed order, used to mark form records
    };

    // members
    //     /*00/00*/ void**                 vtbl;
    MEMBER /*04/04*/ UInt8                  formType; 
    MEMBER /*05/05*/ UInt8                  formPad05[3]; 
    MEMBER /*08/08*/ UInt32                 formFlags; 
    MEMBER /*0C/0C*/ UInt32                 formID; 
    #ifndef OBLIVION
    MEMBER /*--/10*/ BSStringT              editorID; 
    MEMBER /*--/18*/ TrackingData           trackingData; 
    #endif
    MEMBER /*10/1C*/ BSSimpleList<TESFile*> fileList; // list of TESFiles that override this form

    // BaseFormComponent virtual methods
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*004/004*/ virtual void         ClearComponentReferences();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);       
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);     
    #endif

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual              ~TESForm(); //
                                            // The vtbl entry is actually the compiler-generated 'scalar deleting destructor', which calls this method
    IMPORT /*014/038*/ virtual void         InitializeAllComponents(); // use to init forms before loading their record from file
    IMPORT /*018/03C*/ virtual void         ClearAllComponentReferences(); // used to clear forms before calling InitAllComponents and loading record from file
    IMPORT /*01C/040*/ virtual bool         LoadForm(TESFile& file);
    IMPORT /*020/044*/ virtual bool         SaveFormRecord(TESFile& file); // calls aveFormChunks, then commits memory buffer to file
    IMPORT /*024/048*/ virtual void         SaveFormChunks(); // saves form chunks to memory buffer
    _NOUSE /*028/04C*/ virtual bool         UnkForm028(TESFile& file) {return false;} // call LoadForm
    IMPORT /*02C/050*/ virtual bool         SaveForm(TESFile& file); // writes an empty record for deleted forms, otherwise calls SaveFormRecord
    IMPORT /*030/054*/ virtual bool         LessThanGroup(const RecordInfo& groupRecord); // similar to LessThan
                                            // returns true if form type is less than target form type of group record
    IMPORT /*034/058*/ virtual bool         LessThan(TESForm& form); // provides sorting support for forms?
                                            // returns true if form type is less than other form type
                                            // special treatement for Cells, Worldspaces, Topics, and IdleAnims
    IMPORT /*038/05C*/ virtual TESForm*     Clone(bool autoEditorID, NiTMap<TESForm*,TESForm*>* cloneMap); //
                                            // if autoEditorID, assigns a new unique editor id by appending 'COPY####'.
                                            // is cloneMap is provided, an entry is added for (thisForm*->newForm*)
    _NOUSE /*03C/060*/ virtual void         UnkForm03C(UInt32 arg0) {}
    IMPORT /*040/064*/ virtual void         MarkAsModified(UInt32 modifiedMask);
    IMPORT /*044/068*/ virtual void         ClearModified(UInt32 modifiedMask);
    _NOUSE /*048/06C*/ virtual void         MarkAsModified2(UInt32 modifiedMask) {} // ?? needs further investigation
    #ifdef OBLIVION
    IMPORT /*04C/---*/ virtual UInt16       ModifiedFormSize(UInt32 modifiedMask); // size of form modifier record
    IMPORT /*050/---*/ virtual void         SaveModifiedForm(UInt32 modifiedMask); // saves form modifier record to savegame
    IMPORT /*054/---*/ virtual void         LoadModifiedForm(UInt32 modifiedMask, UInt32 unkFlags); // loads modifier record from savegame
    INLINE /*058/---*/ virtual void         LinkModifiedForm(UInt32 modifiedMask, UInt32 arg1) {} // convert formids in to pointers, etc
                                            // both arguments appear to be modifierMasks, not sure of purpose
    INLINE /*05C/---*/ virtual void         PostLinkModifiedForm(UInt32 modifiedMask, UInt32 arg1) {} // rebuild temporary data that wasn't saved
                                            // e.g. visual effects & shaders
    INLINE /*060/---*/ virtual void         PreLoadModifiedForm(UInt32 modifiedMask) {} // reset fields specified by mask to defaults
                                            // in preparation for loading the modified values
    _NOUSE /*064/---*/ virtual void         UnkForm064(UInt32 arg0) {}
    _NOUSE /*068/---*/ virtual bool         UnkForm068(UInt32 arg0) {return false;}
    #endif
    IMPORT /*06C/070*/ virtual void         LinkForm(); // resolve formids into pointers
    IMPORT /*070/074*/ virtual UInt8        GetFormType();
    IMPORT /*074/078*/ virtual void         GetDebugDescription(BSStringT& output); // descriptive string (for debugging ?) 
    IMPORT /*078/07C*/ virtual bool         IsQuestItem(); // Quest item status only (not persistent ref); refs will return flag of their base form
    _NOUSE /*07C/080*/ virtual bool         UnkForm07C() {return false;} // gets flag (1<<6), "BorderRegion"
    _NOUSE /*080/084*/ virtual bool         UnkForm080() {return false;} // gets flag (1<<16)
    IMPORT /*084/088*/ virtual bool         IsOffLimits();
    IMPORT /*088/08C*/ virtual bool         IsDangerous();
    IMPORT /*08C/090*/ virtual void         SetDeleted(bool deleted); // changes deleted flag, plus additional bookeeping
    IMPORT /*090/094*/ virtual void         SetFromActiveFile(bool fromActiveFile); // add/remove form entry from active mod/save file
    IMPORT /*094/098*/ virtual void         SetQuestItem(bool questItem);
    _NOUSE /*098/09C*/ virtual void         UnkForm098(bool newValue) {} // sets flag (1<<6), "BorderRegion"
    _NOUSE /*09C/0A0*/ virtual void         UnkForm09C(bool newValue) {} // sets flag (1<<16)
    _NOUSE /*0A0/0A4*/ virtual void         UnkForm0A0(bool newValue) {} // sets flag (1<<17), "OffLimits/Dangerous", but doesn't call MarkAsModified
    _NOUSE /*0A4/0A8*/ virtual bool         UnkForm0A4() {return false;}
    _NOUSE /*0A8/0AC*/ virtual bool         UnkForm0A8() {return false;}
    INLINE /*0AC/0B0*/ virtual bool         IsMagicItem() {return false;} // apparently used to avoid unnecessary dynamic casts
    INLINE /*0B0/0B4*/ virtual UInt32       GetObjectRefCount() {return 0;} // get object ref count (placed references with this as base form)
    IMPORT /*0B4/0B8*/ virtual void         CopyFrom(TESForm& form); // copies all fields, including base components
    IMPORT /*0B8/0BC*/ virtual bool         CompareTo(TESForm& compareTo); // returns 0 if equivalent.  compares all fields, including base components.
    IMPORT /*0BC/0C0*/ virtual bool         MatchGroupRecord(const RecordInfo& groupRecord, bool matchAllLevels, bool arg2); //
                                            // returns true if the group record matches this form
                                            // if matchAllLevels is false, only the group that should be direct parent of this form is matched
                                            // have only seen arg2 as true
    IMPORT /*0C0/0C4*/ virtual void         CreateGroupRecord(RecordInfo& newGroupRecord, RecordInfo& currentGroupRecord); //
                                            // creates a group record for the form type, given the lowest currently open group record
    #ifdef OBLIVION
    _NOUSE /*0C4/---*/ virtual bool         UnkForm0C4() {return false;}
    _NOUSE /*0C8/---*/ virtual bool         UnkForm0C8(UInt32 arg0) {return false;}
    _NOUSE /*0CC/---*/ virtual bool         UnkForm0CC(TESObjectREFR* arg0, TESObjectREFR* arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4) {return false;} //
                                            // related to activate, refr1 is activating refr, refr1 is a reference to this TESForm being activated. 
                                            // seen arg3 == 0
    _NOUSE /*0D0/---*/ virtual const char*  UnkForm0D0() {return 0;}
    #endif
    IMPORT /*0D4/0C8*/ virtual const char*  GetEditorID();
    #ifdef OBLIVION
    IMPORT /*0D8/---*/ virtual bool         SetEditorID(const char* newEditorID); // this is a regular method in the CS
    #else
    _NOUSE /*---/0CC*/ virtual bool         UnkForm0CC() {return false;}
    _NOUSE /*---/0D0*/ virtual bool         UnkForm0D0(UInt32 arg0) {return false;}
    _NOUSE /*---/0D4*/ virtual bool         UnkForm0D4() {return false;} // checks for valid current or last userid
    _NOUSE /*---/0D8*/ virtual bool         UnkForm0D8() {return false;}
    IMPORT /*---/0DC*/ virtual void         GetObjectWindowTreePath(BSStringT& path); // used to subdivide entries in the object window
    _NOUSE /*---/0E0*/ virtual bool         UnkForm0E0(UInt32 arg0) {return false;}
    _NOUSE /*---/0E4*/ virtual bool         UnkForm0E4() {return false;}
    _NOUSE /*---/0E8*/ virtual bool         UnkForm0E8(UInt32 arg0) {return false;}
    _NOUSE /*---/0EC*/ virtual bool         UnkForm0EC(UInt32 arg0, UInt32 arg1) {return false;}
    IMPORT /*---/0F0*/ virtual void         BuildFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/0F4*/ virtual void         RemoveFormReference(TESForm& referencedForm);
    IMPORT /*---/0F8*/ virtual bool         FormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList);
    IMPORT /*---/0FC*/ virtual void         GetRevisionUnmatchedFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output);
    _NOUSE /*---/100*/ virtual bool         UnkForm100(UInt32 arg0) {return false;} // checkout form for current user? (might not be used by public CS)
    IMPORT /*---/104*/ virtual bool         UpdateUsageInfo(); // update trackingData and (?) fileList
    _NOUSE /*---/108*/ virtual bool         UnkForm108(UInt32 arg0) {return false;} // undo checkout?
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); //
                                            // processes messages not handled by main DialogProc.  Returns true if message was handled
    IMPORT /*---/110*/ virtual bool         IsDialogValid(HWND dialog); // Checks for necessary controls by id, returns true if found
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog); // Set dialog control values to reflect members
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog); // Get member values from dialog controls
    IMPORT /*---/11C*/ virtual void         CleanupDialog(HWND dialog); // called when dialog closes (?)
    IMPORT /*---/120*/ virtual const char*  GetSubtypeName(); // text for 'Type' field in Object Window
    #endif

    // methods - serialization to/from TESFiles
    IMPORT TESFile*             GetOverrideFile(int index); // returns last override if index > #overrides or -1
    IMPORT void                 InitializeFormRecord(); // initializes a RecordInfo structure for the form and allocates a global buffer
                                // In the CS, also saves EDID chunk if editorID is a non-empty string
    IMPORT static void          PutFormRecordChunkData(UInt32 chunkType, void* buffer, UInt32 size); // appends chunk to form record buffer                      
    IMPORT void                 FinalizeFormRecord(); // finalizes the form record buffer
    IMPORT static bool          ResolveFormID(UInt32& formID, TESFile& file); // resolves a formid from a TESFile

    // methods - serialization to/from TESSaveLoadGame
    #ifdef OBLIVION
    IMPORT void                 SaveDataToCurrentSaveGame(void* buffer, UInt32 size); // These four methods map directly to the corresponding
    IMPORT void                 LoadDataFromCurrentSaveGame(void* buffer, UInt32 size); // methods on TESSaveLoadGame
    IMPORT void                 SaveFormIDToCurrentSaveGame(UInt32& formIDs, UInt32 size = 4);
    IMPORT void                 LoadFormIDFromCurrentSaveGame(UInt32& formIDs, UInt32 size = 4);
    #endif

    // methods - formID, editorID
    IMPORT void                 SetFormID(UInt32 newFormID, bool arg2 = true); // arg2=true to reserve formid from datahandler ?
    IMPORT static TESForm*      LookupByFormID(UInt32 formID);
    IMPORT static bool          IsFormIDBuiltin(UInt32 formID); // returns true if formID is a 'builtin' form (e.g. >= 0x800)
    #ifndef OBLIVION
    IMPORT bool                 SetEditorID(const char* newEditorID); // this is a virtual method in the game
    IMPORT static TESForm*      LookupByEditorID(const char* editorID);
    IMPORT static bool          SanitizeEditorIDString(char* editorID); // strips out non-alphanumeric characters.  returns true if string was changed.
    #endif

    // methods - CS reference tracking
    #ifndef OBLIVION
    IMPORT FormReferenceList*   GetCrossReferenceList(bool create); // if 'create', then an empty list will be created if none is found
    IMPORT UInt32               GetCrossReferenceCount();
    IMPORT void                 AddCrossReference(TESForm* referencingForm);
    IMPORT void                 RemoveCrossReference(TESForm* referencingForm);
    IMPORT void                 ClearCrossReferenceList();
    #endif

    // methods - batch handling of select, very simple base form components
    IMPORT void                 SaveGenericComponents(void* extraDataBuffer, UInt16 extraDataSize); //
                                // Saves components and the specified extra data                                              
    IMPORT void                 LoadGenericComponents(TESFile& file, void* extraDataBuffer, UInt16 extraDataSize); //
                                // Saves components and the any stored extra data
    IMPORT void                 CopyAllComponentsFrom(TESForm& form); // copies components
    IMPORT bool                 CompareAllComponentsTo(TESForm& compareTo); // compares components

    // methods - misc
    IMPORT static const char*   GetFormTypeName(UInt8 formType);
    IMPORT void                 MakeTemporary(); // removes from formID map, editorID map, active file list, etc. and sets temporary flag

    // constructor  
    IMPORT TESForm();

    // global static objects    
    IMPORT static FormTypeInfo                      formTypeList[kFormType__MAX]; // list of form types, with short names & chunk types
    #ifndef TESFORM_MINIMAL_DEPENDENCIES
    IMPORT static NiTPointerMap<UInt32,TESForm*>    formIDMap; // maps formIDs to form pointers
            // actually NiTMapBase< NiTPointerAllocator<unsigned int>, unsigned int, TESForm* >
    IMPORT static NiTLargeArray<TESForm*>           activeFileFormList; // list of forms modified by active file
    #ifndef OBLIVION
    IMPORT static BSTCaseInsensitiveStringMap<TESForm*>         editorIDMap;  // maps editorIDs to form pointers
    IMPORT static NiTPointerMap<TESForm*,FormReferenceList*>    formReferenceMap; // maps form pointers to list of referencing forms
            // actually NiTMapBase< NiTPointerAllocator<unsigned int>, TESForm*, BSSimpleList< TESForm* >* >
    #endif
    #endif
};

#ifdef OBLIVION
typedef TESForm TESFormIDListView;
#else
class IMPORTCLASS TESFormIDListView : public TESForm
{// size 18/24
/*
    Partial:
    -   virtual methods
    Notes:
    **CS ONLY** 
    This class is apprently used to centralize code for certain types of dialog windows in the CS.
    It does not appear in the RTTI structure of the game code. 
*/  
public:

    // no additional members

    // TESForm virtual methods
    INLINE /*010/034*/ virtual              ~TESFormIDListView() {} // stub, calls TESForm::~TESForm()

    // additional virtual methods
    IMPORT /*---/124*/ virtual void         SetupFormListColumns(HWND listView); // sets up columns (name, size, caption, etc)
    IMPORT /*---/128*/ virtual void         PopulateFormList(HWND listView); // adds forms to list
    IMPORT /*---/12C*/ virtual void         RefreshFormList(HWND listView); // repopulate and resort list, then scroll down to previous position
                                            // best used when no items are selected
    IMPORT /*---/130*/ virtual void         GetFormListDispInfo(void* displayInfo); // get text to display in list view
                                            // argument is a NMLVDISPINFO*, defined in <Commctrl.h>
    IMPORT /*---/134*/ virtual int          CompareInFormList(const TESForm& compareTo, int compareBy); //
                                            // callback for sorting form list.  returns -1 (A<B), 0 (A==B), +1 (A>B). 
                                            // column index in the listview is |compareBy| - 1
                                            // the current compareBy value is stored in the window 'UserData' for the ListView control

    // methods
    IMPORT static INT_PTR CALLBACK      DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); //
                                        // There is a bug(?) that causes this method to prematurely discard certain WM_COMMAND messages recieved
                                        // while processing a popup menu selection.  This can cause problems, as processing the menu selection
                                        // might require sending command messages to dialog controls.
    IMPORT static int CALLBACK          FormListComparator(const TESFormIDListView& formA, const TESFormIDListView& formB, int compareBy); //
                                        // calls formA.CompareInFormList(formB), and reverses result if compareBy is negative
                                        // column index in the listview is |compareBy| - 1, compareBy is negative for reversed ordering

    // constructor
    IMPORT TESFormIDListView();
};
#endif

class IMPORTCLASS TESMemContextForm 
{// size 00/00
/*
    Todo:
    -   Possible static members or static data
    -   possible inlined destructor
    Notes:
    TESMemContextForm is a parent class for TESObjectCELL and TESObjectREFR.  It has no members and is not polymorphic,
    so it probably has only static methods & data.  It may have something to do with the 'Memory Useage' debugging code
    used by Bethesda, and it is possible that it has no use at all in the released game. 
*/

    // no members

    // no virtual methods

    // no constructor
};