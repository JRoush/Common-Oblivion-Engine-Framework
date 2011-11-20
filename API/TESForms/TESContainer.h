/* 
    TESContainer is a BaseFormComponent used for for forms with inventories, and on it's own
    as a way to temporarily manage collections of items.  Not all form types can be contained;
    TESBoundObjects seem to be the intended content.  Containers are not intended to contain
    other containers.  TESContainer is limited in the information it can store - just form/count
    pairs.

    ContainerExtraData is an auxiliary class, used to track additional information and changes
    to content.  Where TESContainer is associated with form classes, ContainerExtraData is 
    stored in the ExtraDataList of individual references.  It stores changes to the base inventory,
    as a list of ContainerExtraEntrys.
    The various ContainerExtraData methods automatically incorporate the base container of the parent ref.
    This means that, for example, ContainerExtraData::GetItemCount() returns the total count, including
    any items in the base inventory.
    The name "ContainerExtraData" was chosen arbitrarily.

    ContainerExtraEntry stores a form and a value indicating the total *change* in count from the base
    inventory.  The entry may have multiple "instances" of the item form, e.g. for variations in charge 
    or durability.  Each instance with extra data has it's own ExtraDataList in the entry's extra data table.  
    The number of items represented by the ExtraDataList is stored in it's ExtraCount value, or is implicitly 1
    if no ExtraCount is present.  If the entry represents items with no extra data, these collectively count as 
    an "instance".

    Example:

    -   TESContainer:
            -   5 x TESObjectWEAP (00000123)
    -   ContainerExtraData:
            -   ContainerExtraEntry with -1 x TESObjectWEAP (00000123)
                -   ExtraDataList { ExtraCount = 2, ExtraOwner = TESNPC (0000089A) }
                -   ExtraDataList { ExtraHealth = 15.0 }

    This container currently has a total of 4 of the weapon item (5 in the base container, -1 in the change entry).
    These four items are split into three "instances".  The first instance is two items that are owned by some NPC character 
    (rather than the container owner).  The second instance is a single item with an altered durability of 15.0.  
    The last instance is a single item with no extra data.

    ContentInstance* (a typedef for ContainerExtraEntry*) is often used to pass/store one or more instances from an inventory.
    Unless otherwise stated, these objects are *dynamically created partial copies* of the underlying "real" ContainerExtraEntrys.
    It is the user's responsibility to delete such objects when done with them.  They should also be treated as *read-only*.  
    Any desired changes should be made with the methods of the parent ContainerExtraData.
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/ExtraData/BSExtraData.h"
#include "API/ExtraData/ExtraDataList.h"

// argument classes
class   TESForm;        // TESForms/TESForm.h
class   TESObjectREFR;  // TESForms/TESObjectREFR.h
class   Actor;          // Actors/Actor.h
class   TESActorBase;   // Actors/TESActorBase.h
class   TESObjectARMO;
class   TESObjectCLOT;
class   TESObjectWEAP;

class IMPORTCLASS TESContainer : public BaseFormComponent
{// size 010/010
public:

    enum ContainerFlags
    {
        kContainer_Linked    = 0x01, // cleared during loading, set by LinkComponent()
    };

    struct ContentEntry
	{// size 08
		MEMBER /*00*/ SInt32		count;  // negative counts have some meaning ...
		MEMBER /*04*/ TESForm*      form;
        // use form heap
        USEFORMHEAP
	};

    typedef BSSimpleList<ContentEntry*> ContentListT;

    /// members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ UInt8          containerFlags;
    MEMBER /*05/05*/ UInt8          pad05[3];
    MEMBER /*08/08*/ ContentListT   contents;

    // virtual methods overrides:
    IMPORT /*000/000*/ virtual void         InitializeComponent();
    IMPORT /*004/004*/ virtual void         ClearComponentReferences();
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION
    IMPORT /*---/010*/ virtual void         BuildComponentFormRefList(BSSimpleList<TESForm*>* formRefs);
    IMPORT /*---/014*/ virtual void         RemoveComponentFormRef(TESForm& referencedForm);
    IMPORT /*---/018*/ virtual bool         ComponentFormRefRevisionsMatch(BSSimpleList<TESForm*>* checkinList);
    IMPORT /*---/01C*/ virtual void         GetRevisionUnmatchedComponentFormRefs(BSSimpleList<TESForm*>* checkinList, BSStringT& output);
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);                  
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);
    INLINE /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog) {} // method is a stub
    #endif

    // no additional virtual methods

    // methods - contents
    IMPORT SInt32           AddContent(TESForm& form, SInt32 count, bool replaceCurrentCount); // returns final count for form
    IMPORT void             ClearContents(); // clear content list
    IMPORT bool             HasContent(TESForm* form); // returns null if form not found
    IMPORT ContentEntry*    GetContentEntry(TESForm* form); // returns null if form not found
    IMPORT SInt32           GetContentCount(TESForm* form); // returns count for form, 0 if not found
    IMPORT void             MultiplyContents(SInt32 multiplier); // mutliply count of all contents
    #ifdef OBLIVION
    IMPORT void             CopyContentsToRef(TESObjectREFR* copyTo); // add contents to target container reference
                            // posts UI message if the target is player
    #endif

    // methods - serialization
    IMPORT void             SaveComponent(); // serialize to active file buffer 
    IMPORT void             LinkComponent(TESForm& parentForm);  // resolve content formids into TESForm*, set linked flag
    // NOTE: there is no LoadComponent() for this class; it is completely integrated into the LoadForm method of it's assorted descendents  

    // methods - aggregate values
    IMPORT float            GetContentWeight();  // total weight of contents, used for encumberance calcs
    IMPORT TESObjectARMO*   GetBestArmorForSlot(TESActorBase& wearer, UInt32 slotIndex); // uses TESActorBase::GetEquippableItemRating() to find 'best'
                            //  armor for target wearer that covers the specified slot (see TESBipedModelForm).  Returns null if no suitable armor found.
    #ifdef OBLIVION
    IMPORT TESObjectCLOT*   GetBestClothingForSlot(TESActorBase& wearer, UInt32 slotIndex); // same details as GetBestArmorForSlot
    IMPORT TESObjectWEAP*   GetBestWeapon(TESActorBase& user); // same details as GetBestArmorForSlot
    #endif
    
    // method - valid content types
    IMPORT static bool      IsInventoryItemType(UInt8 formType); // returns true for form types allowed in in-game inventories
                            // This funtion does *not* control what is allowed in a general TESContainer.

    // methods - CS dialogs
    #ifndef OBLIVION
    IMPORT static HWND          ShowContentPreviewWindow(TESContainer* container); // create & display a preview window of contents
    IMPORT void                 InitializeContentListView(HWND listView); // clear listview, setup columns, etc.
    IMPORT void                 PopulateContentListView(HWND listView); // populate listview with contents
    IMPORT static void CALLBACK GetContentEntryDisplayInfo(void* displayInfo); // used to build display strings for contents in listview
                                // argument is a NMLVDISPINFO*, defined in <Commctrl.h>
    IMPORT static int CALLBACK  ContentEntryComparator(const ContentEntry& entryA, const ContentEntry& entryB, int compareBy); //
                                // callback for sorting content lists.  returns -1 (A<B), 0 (A==B), +1 (A>B). 
                                // compareBy is the column index in the listview, and is negative for reversed ordering
    #endif

    // constuctor, destructor
    IMPORT TESContainer();
    IMPORT ~TESContainer();
};

// ContainerExtraEntry class
class ContainerExtraEntry
{// size 0C
/*
    NOTE:
    ContainerExtraEntry* is often the return value of "GetItem"-type methods.  
    Unless otherwise stated, the returned object is *created dynamically* and orphaned by the method, making it the caller's 
    responsibility to delete it when done.  These objects are *not* the real Entries, they are simplified copies.  
    They should be treated as *read-only*.  In particular, do NOT attempt to add to, remove from, or clear the formDataTable.
    Any desired changes should be made through the methods of the parent ContainerExtraData, TESObjectREFR, Actor, etc.
*/
public:
    
    enum ModifiedFlags
    {
        kModified_ContainerEntry        = /*05*/ 0x00000020, // Common 'Modified' flag for all container entry properties:
                                                             // Count,Health,Uses,TimeLeft,Charge,Soul,Scale,Ownership,Global,Rank,Worn,WornLeft,CannotWear,
                                                             // BoundArmor,Script, LeveledItem, ReferencePointer,Poison,StartingWorldOrCell,QuickKey
    };

    typedef BSSimpleList<ExtraDataList*> FormDataTableT;

    // members        
    MEMBER /*00*/ FormDataTableT*   formDataTable;    // initialized to empty list by constructor.
    MEMBER /*04*/ SInt32            count;  // cumulative with count in base container
    MEMBER /*08*/ TESForm*          form;

    // methods
    IMPORT void                 ClearFormDataTable();   // destroy all ExtraDataLists in table, along with their contents.  Doesn't destroy table iteself.
    IMPORT void                 DestroyFormDataTable(); // destroy table, but without destroying the ExtraDataLists it contains

    // constructor, destructor
    IMPORT ContainerExtraEntry(TESForm* form, SInt32 count);
    INLINE ~ContainerExtraEntry() {DestroyFormDataTable();} // probably completely inlined.  DOES NOT DESTROY EXTRADATALISTS.  This is the responsibility
                                                            // of the owner, almost always the parent ContainerExtraData.

    // use form heap
    USEFORMHEAP
};
typedef ContainerExtraEntry ContentInstance; // purely for aesthetics, to differentiate "instances" from true entries.  see notes above.

class IMPORTCLASS ContainerExtraData
{ // size 10
public:  
    typedef BSSimpleList<ContainerExtraEntry*> EntryDataListT;  

    // members        
    MEMBER /*00*/ EntryDataListT*   entryDataList;  // initialized to empty list by constructor
    MEMBER /*04*/ TESObjectREFR*    parent;         // container ref to which this extra data belongs
	MEMBER /*08*/ float			    totalWeight;	// cached total weight of contents (includes perks), -1 if needs to be recalculated
	MEMBER /*0C*/ float			    armorWeight;	// cached total weight of equipped armor (includes perks), -1 if needs to be recalculated

    // methods - manipulate contents
    #ifdef OBLIVION
    IMPORT void                         AddContent(TESForm& form, ExtraDataList* contentExtraData, SInt32 count); // add instance to container
                                        // NOTE: container adopts ExtraDataList, and may immediately destroy it if it duplicates one already present
    IMPORT TESObjectREFR*               RemoveContent(TESObjectREFR* dropRef, // "self" ref for OnDrop script blocks, origin of dropped item placement
                                            TESForm& form, ExtraDataList* entryExtra, SInt32 count, // instance & count to remove                                              
                                            bool useContainerOwnership = false, // copies this container owner to transfered items w/o an owner
                                            bool drop = false, // items dropped in game world, rather than transfered or destroyed
                                            TESObjectREFR* destRef = 0, // target container for transfer
                                            Vector3* destPosition = 0, Vector3* destRotation = 0, // coordinates for drop (default is near onDropThisRef)
                                            bool arg8 = true, // unk?
                                            bool useExistingEntryExtra = false // transfer existing entry ExtraDataList if entryExtra == 0
                                        ); // returns dropped item, if dropped
    IMPORT void                         EquipContent(TESForm& form, SInt32 count, Actor& parent, ExtraDataList* entryExtra, 
                                                    bool wornLeft = false, bool cantUnequip = false); //
                                        // parent actor needed to update relevant behavior
                                        // wornLeft applies ExtraWornLeft instead of ExtraWorn (only used for rings)
    IMPORT bool                         UnequipContent(bool& itemFound, TESForm& form, SInt32 count, Actor& parent, ExtraDataList* entryExtra, 
                                                    bool wornLeft = false, bool arg6 = false); // 
                                        // parent actor needed to update relevant behavior.  arg6 is not used.
                                        // ExtraDataList will be destroyed (merging with another in the container), in which case method returns false.
    IMPORT void                         UnequipAll(bool respectNoUnequipFlag = false);
    #endif

    // methods - evaluate contents
    #ifdef OBLIVION                                         
    IMPORT SInt32                       GetContentCount(TESForm& form); // returns 1 if an entry is present for item and base count = entry count = 0.
    IMPORT float                        GetContentWeight(); // returns cached value if >= 0.0, otherwise recalculates
    IMPORT float                        GetWornArmorWeight(Actor& parent); // returns cached value if >= 0.0, otherwise recalculates.  
                                        // parent actor used for perk calc
    IMPORT UInt32                       GetInstanceCount(); // total number of unique item instances in inventory
    IMPORT const ContentInstance*       GetInstance(UInt32 index); // get content instance by index.  
                                        // instances may share an index if they have identical apperances in the inventory menu
    IMPORT const ContentInstance*       GetEquippedInstance(UInt32 slotIndex, bool armorOnly = false);
    IMPORT const ContentInstance*       GetBestApparatusInstance(UInt32 apparatusType); // returns single highest quality apparatus of specified type
    #endif

    // constructor, destructor, creation
    IMPORT static ContainerExtraData&   GetForRef(TESObjectREFR& ref, TESContainer* refContainer = 0); // returns existing ContainerExtraData 
                                        // if one is already attached to the ref, otherwise creates & attaches a new one.  
                                        // refContainer is the result of TESObjectREFR::GetContainer(), and is not used.
    IMPORT ContainerExtraData(TESObjectREFR* owner);
    IMPORT ~ContainerExtraData();  // marks owner ref as modified

    // use form heap
    USEFORMHEAP    
};

class IMPORTCLASS ExtraContainerChanges : public BSExtraData
{// size 0x10
public:

    // members
    //     /*00*/ BSExtraData          
    MEMBER /*0C*/ ContainerExtraData*   data;

     // virtual methods:
    INLINE /*00/00*/ virtual        ~ExtraContainerChanges(); // destroys subwindow object
    // no additional virtual methods

    // constructor
    IMPORT ExtraContainerChanges(ContainerExtraData* data);  
}; 