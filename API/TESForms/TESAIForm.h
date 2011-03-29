/* 
    TESAIForm is a base component for Actors, controlling AI functionality.
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h"
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   TESPackage;

class IMPORTCLASS TESAIForm : public BaseFormComponent
{// size 018/018
public:

    enum AIStats
    {
        kAIStat_Aggression      = 0,
        kAIStat_Confidence      = 1,
        kAIStat_Energy          = 2,
        kAIStat_Responsibility  = 3,    
        kAIStat__MAX,
    };

    enum ServiceFlags
    {
        kService_Weapons        = 1 << 0x00,
        kService_Armor          = 1 << 0x01,
        kService_Clothing       = 1 << 0x02,
        kService_Books          = 1 << 0x03,
        kService_Ingredients    = 1 << 0x04,
        kService_Lights         = 1 << 0x07,
        kService_Apparatus      = 1 << 0x08,
        kService_Misc           = 1 << 0x0A,
        kService_Spells         = 1 << 0x0B,
        kService_MagicItems     = 1 << 0x0C,
        kService_Potions        = 1 << 0x0D,
        kService_Training       = 1 << 0x0E,
        kService_Recharge       = 1 << 0x10,
        kService_Repair         = 1 << 0x11,
    };

    enum ModifiedFlags
    {
        kModified_AIStats       = 0x00000100,
    };
    
    typedef BSSimpleList<TESPackage*> PackageListT;

    // members
    //     /*00/00*/ void**         vtbl;
    MEMBER /*04/04*/ UInt8          aiStats[4];
    MEMBER /*08/08*/ UInt32         serviceFlags;
    MEMBER /*0C/0C*/ UInt8          trainingSkill;  // skill offset, i.e. (avCode - 12)
    MEMBER /*0D/0D*/ UInt8          trainingLevel;
    MEMBER /*0E/0E*/ UInt8          pad0E[2];
    MEMBER /*10/10*/ PackageListT   packages;

    // virtual method overrides:
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
    INLINE /*---/028*/ virtual void         SetComponentInDlg(HWND dialog) {}
    INLINE /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog) {}
    #endif

    // additional virtual methods    
    IMPORT /*010/034*/ virtual void         MarkComponentModified(UInt32 modifiedMask); // does nothing in CS, calls parent form method in game
    #ifdef OBLIVION
    IMPORT /*014/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask);
    IMPORT /*018/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask);
    IMPORT /*01C/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags);
    #endif

    // methods - get/set members
    IMPORT UInt8            GetAggression();
    IMPORT void             SetAggression(UInt8 value); // invokes MarkComponentModified()
    IMPORT UInt8            GetConfidence();
    IMPORT void             SetConfidence(UInt8 value); // invokes MarkComponentModified()
    IMPORT UInt8            GetEnergy();
    IMPORT void             SetEnergy(UInt8 value); // invokes MarkComponentModified()
    IMPORT UInt8            GetResponsibility();
    IMPORT void             SetResponsibility(UInt8 value); // invokes MarkComponentModified()
    IMPORT bool             OffersService(UInt32 serviceMask);
    IMPORT UInt32           GetTrainingSkillAV(); // converts stored skill offset to an avCode
    IMPORT void             SetTrainingSkill(UInt32 skillAV);  // converts avCode into a skill offset, does nothing for non-skill AVs
    IMPORT UInt8            GetTrainingLevel();
    IMPORT void             SetTrainingLevel(UInt8 level);

    // methods - serialization
    // NOTE: there is no LoadComponent() for this class; it is completely integrated into the LoadForm method of it's assorted descendents 
    #ifndef OBLIVION
    IMPORT void             SaveComponent();
    #endif
    IMPORT void             LinkComponent(TESForm& parentForm);  // resolve spell formids into SpellItem*/TESLevSpell*

    // constructor, destructor
    IMPORT TESAIForm();
    IMPORT ~TESAIForm();
};