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
class   TESFile;    // TESFiles/TESFile.h
class   TESForm;    // TESForms/TESForm.h
class   NiNode;
struct  FULL_HASH;  // Useage unknown - struct {void* unk00; void* unk04; void* unk08;}
class   Script;
class   SpellItem;  // Magic/MagicItemForm.h
class   EnchantmentItem; // Magic/MagicItemForm.h
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

    enum ModifiedFlags
    {
        kModified_FullName      = 0x00000080,
    };

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
    IMPORT void                 SaveComponent(); // serialize to active file buffer (calls static save method with default 'DESC' chunk type)
    IMPORT static void          SaveComponent(TESDescription& desc, UInt32 chunkType);  // serialize to active file buffer
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
    MEMBER /*--/0C*/ UInt32         unkTexture0C; // cached image? struct {UInt32 unkA; UInt32 unkB; void* privateObj;}
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
    IMPORT /*---/044*/ virtual void         SetTextureImageInDlg(char* FullPath); // actually "BuildDisplayObject"
                                            // allocates & constructs the object pointed to by member 0x0C
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
*/
public:

    typedef NiTList<FULL_HASH*> ModelHashList;  // probably a named class, but without it's own vtbl or RTTI info

    // members
    //     /*00/00*/ void**                 vtbl;
    MEMBER /*04/04*/ BSStringT              modelPath; // 
    MEMBER /*0C/0C*/ float                  modelBound; // 
    #ifdef OBLIVION
    MEMBER /*10/--*/ UInt8                  unkModel10; // 
    MEMBER /*11/--*/ UInt8                  unkModel11[3]; // 
    MEMBER /*14/--*/ void*                  unkModel14; // Object released by destructor, ClearComponentReferences()
    #else
    MEMBER /*--/10*/ ModelHashList          modelHashList; // texture hash list ?
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
                                            // The vtbl entry is acutally the compiler-generated 'scalar deleting destructor', which calls this method
    IMPORT /*014/038*/ virtual const char*  ModelPath();
    #ifndef OBLIVION
    IMPORT /*---/03C*/ virtual bool         GetModelPathFromDlg(HWND dialog, UInt32 pathDialogItem);   //
                                            // Sets the Model path to the text of the specified control    
    #endif
    IMPORT /*018/040*/ virtual void         SetModelPath(const char* path);
    #ifndef OBLIVION
    IMPORT /*---/044*/ virtual void         UpdateModelBound(NiNode* node, TESForm* parentForm = 0, float newBound = 0.0);
    IMPORT /*---/048*/ virtual void         UpdateModelTextureList(NiNode* node, TESForm* parentForm = 0); // invoked by the corresponding menu command
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

    enum ModifiedFlags
    {
        kModified_Health        = 0x00000004,
    };

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

    // constructor, destructor
    IMPORT TESSpellList();
    IMPORT ~TESSpellList(); // WARNING - does not clear lists, will cause memory leaks unless ClearSpellLists() is called first (bug?)
};

class IMPORTCLASS TESEnchantableForm : public BaseFormComponent
{// size 10/10
public:

    // members
    //     /*00/00*/ void**             vtbl;
    MEMBER /*04/04*/ EnchantmentItem*   enchantment;
    MEMBER /*08/08*/ UInt16             enchantmentCharge; // only valid for weapons
    MEMBER /*0A/0A*/ UInt16             enchantmentPad0A;
    MEMBER /*0C/0C*/ UInt32             castingType; // init by derived class's InitializeAllComponents().  see Magic::CastTypes

    // virtual methods:
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION
    IMPORT /*---/010*/ virtual void         BuildComponentFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/014*/ virtual void         RemoveComponentFormRef(TESForm& referencedForm);
    IMPORT /*---/018*/ virtual bool         ComponentFormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList);
    IMPORT /*---/01C*/ virtual void         GetRevisionUnmatchedComponentFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output);
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // additional virtual methods
    IMPORT /*010/034*/ virtual UInt32       GetCastingType();

    // methods
    IMPORT static EnchantmentItem*  GetEnchantmentForForm(const TESForm* form); // dynamic cast form into TESEnchantableForm. returns 0 on failure
    IMPORT static UInt16            GetEnchantmentChargeForForm(const TESForm* form); // dynamic cast form into TESEnchantableForm. returns 0 on failure
    IMPORT void                     SaveComponent(); // serialize to active file buffer.  
    //                              Note there is no corresponding standalone LoadComponent() method, it apparently 
    //                              was written directly into the TESForm::LoadForm() method of derived classes.
    IMPORT void                     LinkComponent(TESForm& parentForm); // resolve formid into EnchantmentItem* and validate for parent form

    // constructor, destructor
    IMPORT TESEnchantableForm();
    INLINE ~TESEnchantableForm() {} // stub, inlined by game/CS
};

class IMPORTCLASS TESBipedModelForm : public BaseFormComponent
{// size 80/C8
public:

    enum Genders
    {   
        kGender_Male        = 0x0,
        kGender_Female      = 0x1,
        kGender__MAX
    };

    enum SlotIndexes
    {
        kSlotIndex_Head             = 0x00,
        kSlotIndex_Hair             = 0x01,
        kSlotIndex_UpperBody        = 0x02,
        kSlotIndex_LowerBody        = 0x03,
        kSlotIndex_Hand             = 0x04,
        kSlotIndex_Foot             = 0x05,
        kSlotIndex_RightRing        = 0x06,
        kSlotIndex_LeftRing         = 0x07,
        kSlotIndex_Amulet           = 0x08,
        kSlotIndex_Weapon           = 0x09,
        kSlotIndex_BackWeapon       = 0x0A,
        kSlotIndex_SideWeapon       = 0x0B,
        kSlotIndex_Quiver           = 0x0C,
        kSlotIndex_Shield           = 0x0D,
        kSlotIndex_Torch            = 0x0E,
        kSlotIndex_Tail             = 0x0F,
        kSlotIndex__MAX
    };

    enum SlotMasks
    {
        kSlotMask_Head              = /*00*/ 0x0001,
        kSlotMask_Hair              = /*01*/ 0x0002,
        kSlotMask_UpperBody         = /*02*/ 0x0004,
        kSlotMask_LowerBody         = /*03*/ 0x0008,
        kSlotMask_Hand              = /*04*/ 0x0010,
        kSlotMask_Foot              = /*05*/ 0x0020,
        kSlotMask_RightRing         = /*06*/ 0x0040,
        kSlotMask_LeftRing          = /*07*/ 0x0080,
        kSlotMask_Amulet            = /*08*/ 0x0100,
        kSlotMask_Weapon            = /*09*/ 0x0200,
        kSlotMask_BackWeapon        = /*0A*/ 0x0400,
        kSlotMask_SideWeapon        = /*0B*/ 0x0800,
        kSlotMask_Quiver            = /*0C*/ 0x1000,
        kSlotMask_Shield            = /*0D*/ 0x2000,
        kSlotMask_Torch             = /*0E*/ 0x4000,
        kSlotMask_Tail              = /*0F*/ 0x8000,
    };

    enum BipedModelFlags
    {
        kBipedModelFlag_HidesRings      = /*00*/ 0x0001,
        kBipedModelFlag_HidesAmulets    = /*01*/ 0x0002,
        kBipedModelFlag_Unk02           = /*02*/ 0x0004,
        kBipedModelFlag_Unk03           = /*03*/ 0x0008,
        kBipedModelFlag_Unk04           = /*04*/ 0x0010,
        kBipedModelFlag_Unk05           = /*05*/ 0x0020,
        kBipedModelFlag_NotPlayable     = /*06*/ 0x0040,
        kBipedModelFlag_HeavyArmor      = /*07*/ 0x0080,
        kBipedModelFlag_Unk08           = /*08*/ 0x0100
    };

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ UInt16         bipedSlotMask;
    MEMBER /*06/06*/ UInt8          bipedModelFlags;
    MEMBER /*07/07*/ UInt8          pad07;
    MEMBER /*08/08*/ TESModel       bipedModel[kGender__MAX];
    MEMBER /*38/50*/ TESModel       worldModel[kGender__MAX];
    MEMBER /*68/98*/ TESIcon        bipedIcon[kGender__MAX];

    // virtual methods:
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*004/004*/ virtual void         ClearComponentReferences();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);              
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    IMPORT /*---/030*/ virtual void         ComponentDlgCleanup(HWND dialog);
    #endif

    // methods - flags & slots
    IMPORT bool             HasBipedSlotOverlap(TESBipedModelForm& otherBipedModel);
    IMPORT bool             CoversBipedSlot(UInt32 slotIndex, bool headCantCoverHair = false);
    IMPORT void             SetCoversBipedSlot(UInt32 slotIndex, bool covers);
    IMPORT bool             IsPlayable();
    #ifndef OBLIVION
    IMPORT void             SetIsPlayable(bool playable); // apparently never used in the game code
    #endif

    // methods - models & icons
    IMPORT TESModel&        GetBipedModel(UInt32 gender);
    IMPORT const char*      GetBipedModelPath(UInt32 gender);
    IMPORT void             SetBipedModelPath(UInt32 gender, const char* path);
    IMPORT TESModel&        GetWorldModel(UInt32 gender);
    IMPORT const char*      GetWorldModelPath(UInt32 gender);
    IMPORT void             SetWorldModelPath(UInt32 gender, const char* path);
    IMPORT const char*      GetBipedIconPath(UInt32 gender);

    // methods - serialization
    IMPORT void             SaveComponent();   // serialize to active file buffer  
    //                      Note there is no corresponding standalone LoadComponent() method, it apparently 
    //                      was written directly into the TESForm::LoadForm() method of derived classes.

    // constructor, destructor
    IMPORT TESBipedModelForm();
    IMPORT ~TESBipedModelForm();

};
