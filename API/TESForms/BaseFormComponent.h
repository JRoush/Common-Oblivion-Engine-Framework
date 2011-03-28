/* 
    BaseFormComponents define the various 'components' that true TESForm classes can inherit,
    e.g. Name, Texture, Model, Weight, etc.  The root of the hierarchy is the BaseFormComponent
    class, which exposes a common interface for Comparison, Copying, Serialization, etc.

    NOTE: "Form" or "Component" References refer to members that point to another form, which are
    tracked by the CS.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/NiTypes/NiTList.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/BSTypes/BSStringT.h"

// argument classes
class   TESFile;
class   TESForm;    // TESForms/TESForm.h
class   NiNode;
struct  FULL_HASH;  // Useage unknown
class   Script;
class   SpellItem;
class   TESLevSpell;

class IMPORTCLASS BaseFormComponent
{// size 04/04
/*
    Partial:
    -   virtual methods
*/
public:

    // members
    //void**        vtbl;        // 00

    // virtual methods:
    IMPORT /*000/000*/ virtual void         InitializeComponent() = 0 {} // 
                                            // Initializes member values of the component, called by constructor (always a stub for TESForms)
    INLINE /*004/004*/ virtual void         ClearComponentReferences() {} // AKA clear FormReferences
                                            // e.g. Useage stats for TESForm, name hash list (?) for TESModel, etc.
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source) = 0;
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const = 0; // return false if equivalent
    #ifndef OBLIVION
    INLINE /*---/010*/ virtual void         BuildComponentFormRefList(BSSimpleList<TESForm*>* formRefs) {} // append referenced forms to list
                                            // ensures forms appear at most once in list.
    INLINE /*---/014*/ virtual void         RemoveComponentFormRef(TESForm& referencedForm) {}
    INLINE /*---/018*/ virtual bool         ComponentFormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList) {return true;} // checks each referenced form
                                            // using TESForm::DoesRevisionMatch(), returns false if a referenced form requires checkin and isn't in list
    INLINE /*---/01C*/ virtual void         GetRevisionUnmatchedComponentFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output) {} // 
                                            // checks each referenced form using TESForm::DoesRevisionMatch()
                                            // builds a string of referenced forms that require checkin but aren't in list
    INLINE /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {return false;} // 
                                            // Mostly for click events on relevant controls.  Returns true if message handled.                                   
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog) = 0; // Checks for necessary controls by id, returns true if found.
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog) = 0;   // Set dialog control values to reflect members
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog) = 0; // Get member values from dialog controls
    INLINE /*---/030*/ virtual void         ComponentDlgCleanup(HWND dialog) {}   // Called when parent dialog is closed
    #endif

    // constructor, destructor - none, since this is an abstract class with no explicit members

    // use FormHeap for class new & delete - this is passed down to the *entire* TESForm hierarchy
    USEFORMHEAP 
};

class IMPORTCLASS TESFullName : public BaseFormComponent
{// size 0C/0C
public: 

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ BSStringT      name;

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);       
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);     
    #endif
    // no additional virtual methods

    // methods
    IMPORT BSStringT            Name(); // creates copy. actually 'BSStringT& Name(BSStringT& temp)', temp arg handled by compiler
    IMPORT static const char*   GetNameForForm(const TESForm* form); // cast form into fullname & return name if sucessful, or "" if not
    IMPORT void                 SaveComponent(); // serialize to active file buffer
    IMPORT static void          LoadComponent(TESFullName& name, TESFile& file); // deserialize from file buffer

    // constuctor, destructor
    INLINE TESFullName() : name() {}    // inlined by game/CS, calls BSStringT() 
    INLINE ~TESFullName() {}            // inlined by game/CS, calls ~BSStringT()
};

class IMPORTCLASS TESDescription : public BaseFormComponent
{// size 08/10
public:

    // static cache for loaded description (game only)
    #ifdef OBLIVION
    IMPORT static TESDescription*   cachedDescriptionOwner; 
    IMPORT static BSStringT         cachedDescription; 
    #endif

    // members
    //     /*00/00*/ void**         vtbl;
    #ifdef OBLIVION
    MEMBER /*04/--*/ UInt32         descDiskOffset; // 
    #else
    MEMBER /*--/04*/ BSStringT      description; // 
    MEMBER /*--/0C*/ UInt32         descDialogItem; // Dialog Control ID for description control
    #endif    

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);       
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);     
    #endif

    // additional virtual methods
    IMPORT /*010/034*/ virtual const char*  GetDescription(TESForm* parentForm = 0, UInt32 chunkType = 0);  // 
                                            // Game - Loads description into cache from disk. CS - returns string ptr from desc member (ignores args)

    // methods
    IMPORT void                 SaveComponent(); // serialize to active file buffer
    IMPORT static void          LoadComponent(TESDescription& desc, TESFile& file);  // deserialize from file buffer

    // constructor
    IMPORT TESDescription();
    INLINE ~TESDescription() {}     // inlined by game/CS, calls ~BSStringT() in CS
};

class IMPORTCLASS TESTexture : public BaseFormComponent
{// size 0C/18
/*
    Partial:
    -   members
*/
public: 

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ BSStringT      texturePath; // 
    #ifndef OBLIVION
    MEMBER /*--/0C*/ UInt32         unkTexture0C; // cached image?
    MEMBER /*--/10*/ UInt32         texturePathDlgItem; // Dialog Control ID for texture path control
    MEMBER /*--/14*/ UInt32         textureImageDlgItem; // Dialog Control ID for texture image control
    #endif

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*004/004*/ virtual void         ClearComponentReferences();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);       
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);   
    IMPORT /*---/030*/ virtual void         ComponentDlgCleanup(HWND dialog);
    #endif

    // additional virtual methods
    #ifndef OBLIVION
    IMPORT /*---/034*/ virtual bool         GetTexturePathFromDlg(HWND dialog, UInt32 pathDialogItem); // 
                                            // Sets the texture path from the text of the specified dialog control 
    #endif
    IMPORT /*010/038*/ virtual void         GetNormalMapPath(BSStringT& normalPath); // 
                                            // Copy of filename with '_n' inserted between the filename and '.dds' extension. 
    IMPORT /*014/03C*/ virtual const char*  BaseTexturePath();  // returns base path from 'Oblivion\Data' in game, from 'Oblivion' in CS
    #ifndef OBLIVION   
    IMPORT /*---/040*/ virtual const char*  NoTextureText();   // text to display when texture is empty
    IMPORT /*---/044*/ virtual void         SetTextureImageInDlg(char* FullPath); //
                                            // sets texture display control to image. path relative to 'Oblivion\Data'
    #endif

    // methods
    IMPORT void                SaveComponent(UInt32 ChunkType);                      // serialize to active file buffer
    IMPORT static void         LoadComponent(TESTexture& texture, TESFile& file);    // deserialize from file buffer

    // constructor, destructor
    IMPORT TESTexture();
    IMPORT ~TESTexture();
};

class IMPORTCLASS TESIcon : public TESTexture
{// size 0C/18
public:

    // no additional members

    // virtual method overrides
    IMPORT /*014/03C*/ virtual const char*  BaseTexturePath();
    #ifndef OBLIVION   
    IMPORT /*---/040*/ virtual const char*  NoTextureText();
    IMPORT /*---/044*/ virtual void         SetTextureImageInDlg(char* FullPath);
    #endif

    // no additional virtual methods

    // constuctor, destructor
    INLINE TESIcon() : TESTexture() {}  // inlined by game/CS, calls TESTexture() 
    INLINE ~TESIcon() {}                // inlined by game/CS, calls ~TESTexture()
};

class IMPORTCLASS TESModel : public BaseFormComponent
{// size 18/24
/*
    Partial:
    -   members
    -   virtual methods
*/
public:

    // members
    //     /*00/00*/ void**                 vtbl;
    MEMBER /*04/04*/ BSStringT              modelPath; // 
    MEMBER /*0C/0C*/ float                  modelBound; // 
    #ifdef OBLIVION
    MEMBER /*10/--*/ UInt8                  unkModel10; // 
    MEMBER /*11/--*/ UInt8                  unkModel11[3]; // 
    MEMBER /*14/--*/ void*                  unkModel14; // Object released by destructor, ClearComponentReferences()
    #else
    MEMBER /*--/10*/ NiTList<FULL_HASH*>    unkModel10; // 
    MEMBER /*--/20*/ UInt32                 modelPathDlgItem; // Dialog Control ID for model path
    #endif

    // virtual method overrides
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

    // additional virtual methods 
    IMPORT /*010/034*/ virtual              ~TESModel();    // 
                                            // The vtbl entry is acutally the compiler-generated 'vector deleting destructor', which calls this method
    IMPORT /*014/038*/ virtual const char*  ModelPath();
    #ifndef OBLIVION
    IMPORT /*---/03C*/ virtual bool         GetModelPathFromDlg(HWND dialog, UInt32 pathDialogItem);   //
                                            // Sets the Model path to the text of the specified control    
    #endif
    IMPORT /*018/040*/ virtual void         SetModelPath(const char* path);
    #ifndef OBLIVION
    IMPORT /*---/044*/ virtual void         UpdateModelBound(NiNode* node, TESForm* parentForm = 0, float newBound = 0.0);
    _NOUSE /*---/048*/ virtual void         UnkTESModel048(UInt32 arg0, TESForm* parentForm = 0) {} // update hash table member??
    #endif

    // methods
    IMPORT void                SaveComponent(UInt32 PathChunkType, UInt32 BoundChunkType, UInt32 ExtraChunkType);   // serialize to active file buffer  
    IMPORT static void         LoadComponent(TESModel& model, TESFile& file);   // deserialize from file buffer

    // constructor
    IMPORT TESModel();
};

class IMPORTCLASS TESScriptableForm : public BaseFormComponent
{// size 0C/0C
/*
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ Script*    script; // script formid stored here during loading
    MEMBER /*08/08*/ bool       scriptLinked; // set once formid has been resolved into a Script*
    MEMBER /*09/09*/ UInt8      scriptPad09[3]; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/010*/ virtual void         BuildComponentFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/014*/ virtual void         RemoveComponentFormRef(TESForm& referencedForm);
    IMPORT /*---/018*/ virtual bool         ComponentFormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList);
    IMPORT /*---/01C*/ virtual void         GetRevisionUnmatchedComponentFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output);
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);       
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);   
    #endif

    // no additional virtual methods 

    // methods
    IMPORT void                 SaveComponent(); // serialize to active file buffer.  
    //                          Note there is no corresponding standalone LoadComponent() method, it appearntly was inlined:
    //                          LoadComponent(TESFile& file, TESForm& parentform) {script = 0; file.LoadChunkData(&script,4); Link(parentform);}
    IMPORT void                 LinkComponent(TESForm& parentForm);  // resolve formid into Script*

    // constructor, destructor
    IMPORT TESScriptableForm();
    INLINE ~TESScriptableForm() {}     // stub, inlined by game/CS
};

class IMPORTCLASS TESUsesForm : public BaseFormComponent
{// size 08/08
public:

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ UInt8      uses; // 
    MEMBER /*05/05*/ UInt8      usesPad05[3]; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // no additional virtual methods

    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()

    // constructor, destructor
    IMPORT TESUsesForm();
    INLINE ~TESUsesForm() {} // stub, inlined by game/CS
};

class IMPORTCLASS TESValueForm : public BaseFormComponent
{// size 08/08
public:    

    enum ModifiedFlags
    {
        kModified_Value     = 0x00000008,
    };

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ SInt32     goldValue;

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // additional virtual methods
    #ifdef OBLIVION
    IMPORT /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask);
    IMPORT /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask);
    IMPORT /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags);
    #endif

    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()

    // constructor, destructor
    IMPORT TESValueForm();
    INLINE ~TESValueForm() {} // stub, inlined by game/CS
};


class IMPORTCLASS TESHealthForm : public BaseFormComponent
{// size 08/08
public:

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ UInt32     health; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // additional virtual methods
    IMPORT /*010/034*/ virtual UInt32       Health();

    // methods    
    IMPORT static UInt32        GetHealthForForm(const TESForm* form); // cast form into healthform & return health if sucessful, or 0 if not

    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()

    // constructor, destructor
    IMPORT TESHealthForm();
    INLINE ~TESHealthForm() {} // stub, inlined by game/CS
};

class IMPORTCLASS TESWeightForm : public BaseFormComponent
{// size 08/08
public:

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ float      weight; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);   
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // no additional virtual methods

    // Methods
    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()
    IMPORT static float                     GetWeightForForm(const TESForm* form); // cast to TESWeightForm & return weight, or -1.0 if no weight
                                            // doesn't actually cast for performance reasons; uses a lookup table of vanilla form types with weight

    // constructor, destructor
    IMPORT TESWeightForm();
    INLINE ~TESWeightForm() {} // stub, inlined by game/CS
};

class IMPORTCLASS TESQualityForm : public BaseFormComponent
{// size 08/08
public:

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ float      quality; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // no additional virtual methods

    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()

    // constructor, destructor
    IMPORT TESQualityForm();
    INLINE ~TESQualityForm() {} // stub, inlined by game/CS
};

class IMPORTCLASS TESAttackDamageForm : public BaseFormComponent
{// size 08/08
public:

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ UInt16     damage; // 
    MEMBER /*06/06*/ UInt16     damagePad06; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // additional virtual methods
    IMPORT /*010/034*/ virtual UInt16       Damage();

    // methods
    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()
    IMPORT static UInt16    GetDamageForForm(const TESForm* form); // cast form into attackdamageform & return damage() if sucessful, or 0 if not

    // constructor, destructor
    IMPORT TESAttackDamageForm();
    INLINE ~TESAttackDamageForm() {} // stub, inlined by game/CS
};

class IMPORTCLASS TESAttributes : public BaseFormComponent
{// size C0/C0
public:

    enum ModifiedFlags
    {
        kModified_Attributes    = 0x00000008,
    };

    enum Attributes
    {
        kAttribute_Strength = 0,
        kAttribute_Intelligence,
        kAttribute_Willpower,
        kAttribute_Agility,
        kAttribute_Speed,
        kAttribute_Endurance,
        kAttribute_Personality,
        kAttribute_Luck,
        kAttribute__MAX,
    };

    // members
    //     /*00/00*/ void**     vtbl;
    MEMBER /*04/04*/ UInt8      attributes[8]; // 

    // virtual method overrides
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const ;
    #ifndef OBLIVION
    INLINE /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {return false;} //
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);    
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);     
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // additional virtual methods
    #ifdef OBLIVION
    IMPORT /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask);
    IMPORT /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask);
    IMPORT /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags);
    #endif

    // Ordinary Save/Load is done through TESForm::SaveGenericComponents() & TESForm::LoadGenericComponents()

    // constructor, destructor
    IMPORT TESAttributes();
    INLINE ~TESAttributes() {}  // stub, inlined by game/CS
};

class IMPORTCLASS TESSpellList : public BaseFormComponent
{// size 014/014
public:

    typedef BSSimpleList<SpellItem*> SpellListT;
    typedef BSSimpleList<TESLevSpell*> LevSpellListT;

    enum ModifiedFlags
    {
        kModified_SpellLists    = 0x00000020,
    };

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ SpellListT     spells;
    MEMBER /*0C/0C*/ LevSpellListT  leveledSpells;

    // virtual method overrides
    INLINE /*000/000*/ virtual void         InitializeComponent() {}
    IMPORT /*004/004*/ virtual void         ClearComponentReferences();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION
    IMPORT /*---/010*/ virtual void         BuildComponentFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/014*/ virtual void         RemoveComponentFormRef(TESForm& referencedForm) ;
    IMPORT /*---/018*/ virtual bool         ComponentFormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList) ;
    IMPORT /*---/01C*/ virtual void         GetRevisionUnmatchedComponentFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output) ;
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);                      
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);
    INLINE /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog) {}
    #endif

    // additional virtual methods
    #ifdef OBLIVION
    IMPORT /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask);
    IMPORT /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask);
    IMPORT /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags);
    #endif

    // methods - spell lists
    IMPORT bool             AddSpell(SpellItem* spell); // returns false if already in list.  if parent ref is player, updates known effects
    IMPORT bool             AddLeveledSpell(TESLevSpell* levSpell); // returns false is already in list.  does nothing if parent ref is player
    IMPORT bool             AddFormToSpellList(TESForm* form); // calls AddSpell/AddLeveledSpell via dynamic cast
    IMPORT void             ClearSpellLists(); // clear spell and levspell lists

    // methods - serialization
    IMPORT void             SaveComponent(); // serialize to active file buffer 
    IMPORT void             LinkComponent(TESForm& parentForm);  // resolve spell formids into SpellItem*/TESLevSpell*
    // NOTE: there is no LoadComponent() for this class; it is completely integrated into the LoadForm method of it's assorted descendents 

    // methods - CS dialogs
    #ifndef OBLIVION
    IMPORT void                 InitializeSpellListView(HWND listView); // clear listview, setup columns, etc.
    IMPORT void                 PopulateSpellListView(HWND listView); // populate listview with contents
    IMPORT static void CALLBACK SpellListDisplayInfo(void* displayInfo); // used to build display strings for contents in listview
                                // argument is a NMLVDISPINFO*, defined in <Commctrl.h>
    IMPORT static int CALLBACK  SpellListComparator(const TESForm& entryA, const TESForm& entryB, int compareBy); //
                                // callback for sorting content lists.  returns -1 (A<B), 0 (A==B), +1 (A>B). 
                                // compareBy is the column index in the listview, and is negative for reversed ordering
    #endif

    // constructor
    IMPORT TESSpellList();
    IMPORT ~TESSpellList(); // WARNING - does not clear lists, will cause memory leaks unless ClearSpellLists() is called first (bug?)
};

/*
class TESAIForm : public BaseFormComponent
{// size 018/018
public:
    TESAIForm();
    ~TESAIForm();

    enum AISettings
    {
        kAISetting_Aggression = 0,
        kAISetting_Confidence,
        kAISetting_Energy,
        kAISetting_Responsibility,    
        kAISetting__MAX,
    };

    enum ServiceFlags
    {
        kService_Weapons        = 1 << 0,
        kService_Armor            = 1 << 1,
        kService_Clothing        = 1 << 2,
        kService_Books            = 1 << 3,
        kService_Ingredients    = 1 << 4,
        kService_Lights            = 1 << 7,
        kService_Apparatus        = 1 << 8,
        kService_Misc            = 1 << 10,
        kService_Spells            = 1 << 11,
        kService_MagicItems     = 1 << 12,
        kService_Potions        = 1 << 13,
        kService_Training        = 1 << 14,
        kService_Recharge        = 1 << 16,
        kService_Repair            = 1 << 17,
    };

    // members
    //void**                vtbl                // 00
    UInt8                   aiSettings[4];      // 04
    UInt32                    serviceFlags;        // 08
    UInt8                    trainingSkill;        // 0C skill offset, i.e. (avCode - 12)
    UInt8                    trainingLevel;        // 0D
    UInt16                    unk00E;                // 0E
    SLLNode<TESPackage*>    packages;            // 10

    // virtual methods
    #ifdef OBLIVION
     virtual void        MarkAsModified(UInt32 modifiedMask);        // 010/--- calls MarkAsModified for parent TESForm
     virtual UInt16     GetSaveSize(UInt32 modifiedMask);           // 014/--- just save size of this component?
     virtual void        SaveGame(UInt32 modifiedMask);              // 018/---
     virtual void        LoadGame(UInt32 modifiedMask, UInt32 arg1); // 01C/---
    #else
     virtual void        unkAIForm034(UInt32 arg0);                  // ---/034
    #endif

};
*/
/*
class TESAnimation : public BaseFormComponent
{// size 0C0/014
public:
    TESAnimation();
    ~TESAnimation();

    // members
    //void**            vtbl            // 00/00
    SLLNode<char*>      animations;     // 04/04 LL of animation names
    #ifndef OBLIVION
     SLLNode<void*>     unkAnimation0C; // --/0C
    #endif

    // no additional virtual methods
};

class TESActorBaseData : public BaseFormComponent
{// size 020/020
public:
    TESActorBaseData();
    ~TESActorBaseData();

    enum NPCBaseDataFlags
    {
        kNPCBaseFlag_Female                 = 0x00000001,
        kNPCBaseFlag_Essential              = 0x00000002,
        kNPCBaseFlag_Respawn                = 0x00000008,
        kNPCBaseFlag_AutoCalcStats          = 0x00000010,
        kNPCBaseFlag_PCLevelOffset          = 0x00000080,
        kNPCBaseFlag_NoLowProc              = 0x00000200,
        kNPCBaseFlag_NoRumors               = 0x00002000,
        kNPCBaseFlag_Summonable             = 0x00004000,
        kNPCBaseFlag_NoPersuasion           = 0x00008000,
        kNPCBaseFlag_CanCorpseCheck         = 0x00100000,
    };    
    
    enum CreatureBaseDataFlags
    {
        kCreatureBaseFlag_Biped             = 0x00000001,
        kCreatureBaseFlag_Essential         = 0x00000002,
        kCreatureBaseFlag_WeaponAndShield   = 0x00000004,
        kCreatureBaseFlag_Respawn           = 0x00000008,
        kCreatureBaseFlag_Swims             = 0x00000010,
        kCreatureBaseFlag_Flies             = 0x00000020,
        kCreatureBaseFlag_Walks             = 0x00000040,
        kCreatureBaseFlag_PCLevelOffset     = 0x00000080,
        kCreatureBaseFlag_HasSounds         = 0x00000100,   // if not set has a TESCreature* soundBase
        kCreatureBaseFlag_NoLowProc         = 0x00000200,
        kCreatureBaseFlag_NoBloodParticle   = 0x00000800,
        kCreatureBaseFlag_NoBloodTexture    = 0x00001000,
        kCreatureBaseFlag_NoRumors          = 0x00002000,
        kCreatureBaseFlag_Summonable        = 0x00004000,
        kCreatureBaseFlag_NoHead            = 0x00008000,
        kCreatureBaseFlag_NoRightArm        = 0x00010000,
        kCreatureBaseFlag_NoLeftArm         = 0x00020000,
        kCreatureBaseFlag_NoCombatInWater   = 0x00040000,
        kCreatureBaseFlag_NoShadow          = 0x00080000,
        kCreatureBaseFlag_NoCorpseCheck     = 0x00100000,   // inverse of corresponding flag for NPCs
    };

    struct FactionListData
    {
        TESFaction* faction;
        UInt8        rank;
        UInt8        pad[3];
    };

    // members
    //void**            vtbl                // 000
    UInt32                actorFlags;            // 004
    UInt16                spellPoints;        // 008
    UInt16                fatigue;            // 00A
    UInt16                barterGold;            // 00C
    UInt16                level;                // 00E
    UInt16                minLevel;            // 010 if PCLevelOffset
    UInt16                maxLevel;            // 012 if PCLevelOffset
    UInt32                unk14;                // 014
    SLLNode<FactionListData*> factionList;    // 018

    // virtual methods
    // for Creature-only (NPC-only) flags & fields, the function returns the default value or does nothing for NPCs (Creatures)
    virtual bool        GetNoPersuasion();                          // 010/034
    virtual bool        GetCanCorpseCheck();                        // 014/038
    virtual bool        GetNoLeftArm();                             // 018/03C
    virtual bool        GetNoRightArm();                            // 01C/040
    virtual bool        GetNoHead();                                // 020/044
    virtual bool        GetNoShadow();                              // 024/048
    virtual bool        GetNoBloodParticle();                       // 028/04C
    virtual void        SetNoBloodParticle(bool newVal);            // 02C/050
    virtual bool        GetNoBloodTexture();                        // 030/054
    virtual void        SetNoBloodTexture(bool newVal);             // 034/058
    virtual const char*    GetBloodTexturePath();
    virtual void        SetBloodTexturePath(const char* ddsPath);
    virtual const char*    GetBloodParticlePath();                     // 040/064
    virtual void        SetBloodParticlePath(const char* nifPath);
    virtual UInt16        GetBaseSpellPoints();
    virtual UInt16        GetBaseFatigue();
    #ifdef OBLIVION
     virtual void        MarkAsModified(UInt32 modifiedMask);        // 050/--- calls MarkAsModified for parent TESForm
     virtual UInt16     GetSaveSize(UInt8 modifiedMask);            // 054/--- just save size of this component?
     virtual void        SaveGame(UInt8 modifiedMask);               // 058/---
     virtual void        LoadGame(UInt8 modifiedMask, UInt32 arg1);  // 05C/---
    #else
     virtual void        unkActorBaseData074(UInt32 arg0);           // ---/074 dialog related
     virtual void        unkActorBaseData078(UInt32 arg0);           // ---/078
    #endif
};
*/