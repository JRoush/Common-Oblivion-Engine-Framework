/* 
    TESLeveledList is a BaseFormComponent used for dynamic content generation (enemies & loot that scale with pc level).
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/BSTypes/BSStringT.h"

// argument classes
class   TESForm;        // TESForms/TESForm.h
class   TESContainer;   // TESForms/TESContainer.h

class IMPORTCLASS TESLeveledList : public BaseFormComponent
{// size 10/10
public: 

    enum LeveledListFlags
	{
		kLevListFlag_CalcAllLevels      = 0x01, // ignores max level difference, effective level is no greate than highest level in list
		kLevListFlag_CalcEachInCount    = 0x02, // for nested lists
	};

    struct ListEntry
	{// size 0C
		MEMBER /*00*/ UInt16		level;
		MEMBER /*02*/ UInt16		pad02;
		MEMBER /*04*/ TESForm*	    form;
		MEMBER /*08*/ SInt16		count;
        MEMBER /*0A*/ UInt16        pad0A;
        // use form heap
        USEFORMHEAP
	};
    typedef BSSimpleList<ListEntry*> LevListT;

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ LevListT	    levList;    // list is sorted by level
	MEMBER /*0C/0C*/ UInt8          chanceNone;	
	MEMBER /*0D/0D*/ UInt8          levListFlags;
	MEMBER /*0E/0E*/ UInt16         padLevList0E;

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
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    #endif

    // additional virtual methods
    INLINE /*010/034*/ virtual SInt32       GetMaxLevelDifference() {return 0;} // iLevCreaLevelDifferenceMax, iLevItemLevelDifferenceMax, etc.  zero default
    #ifndef OBLIVION
    IMPORT /*---/038*/ virtual void         PopulateFormSelectCombo(HWND comboBox); // populate combo for selecting entry
    #endif

    // methods
    IMPORT void             ClearLeveledList(); // destroys all list entries 
    IMPORT ListEntry*       AddLeveledForm(UInt16 level, SInt16 count, TESForm* form); // sorted insert
    IMPORT void             SimpleCalcLeveledForm(UInt16 level, TESForm*& form, SInt16& count, bool recurse); // returns a random form from the
                            // list for the specified level.  Uses 'CalcAllLevels' flag.
                            // Does *not* use the 'CalcEachInCount' flag, so it can return at most one form
    IMPORT void             CalcLeveledForm(UInt16 level, SInt16 count, TESContainer* container); // returns random form or forms from the list
                            // for the specified level, evaluating the list 'count' times.  Resulting forms are placed in container.
    IMPORT void             SaveComponent(); // serialize to active file buffer
    IMPORT void             LinkComponent(TESForm& parentForm);  // resolve entry formids into TESForm*
    // NOTE: there is no LoadComponent() for this class; it is completely integrated into the LoadForm method of it's assorted descendents

    // constuctor, destructor
    IMPORT TESLeveledList();
    IMPORT ~TESLeveledList();
};