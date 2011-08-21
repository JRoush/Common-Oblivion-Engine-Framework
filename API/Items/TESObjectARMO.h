/* 
    TESObjectARMO is a base form for armor items.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/TESForms/TESObject.h"
#include "API/TESForms/BaseFormComponent.h"

class IMPORTCLASS TESObjectARMO : public TESBoundObject, public TESFullName, public TESScriptableForm, public TESEnchantableForm, 
                                public TESValueForm, public TESWeightForm, public TESHealthForm, public TESBipedModelForm
{// size 0E8/164
public:

    // members
    //     /*000/000*/ TESBoundObject
    //     /*024/058*/ TESFullName
    //     /*030/064*/ TESScriptableForm
    //     /*03C/070*/ TESEnchantableForm
    //     /*04C/080*/ TESValueForm
    //     /*054/088*/ TESWeightForm
    //     /*05C/090*/ TESHealthForm
    //     /*064/098*/ TESBipedModelForm
    MEMBER /*0E4/160*/ UInt16       armorRating;
    MEMBER /*0E6/162*/ UInt16       padE6;

    // TESForm virtual method overrides
    IMPORT /*010/034*/ virtual              ~TESObjectARMO();
    IMPORT /*014/038*/ virtual void         InitializeAllComponents();
    IMPORT /*01C/040*/ virtual bool         LoadForm(TESFile& file);
    IMPORT /*024/048*/ virtual void         SaveFormChunks();
    #ifdef OBLIVION
    IMPORT /*04C/---*/ virtual UInt16       ModifiedFormSize(UInt32 modifiedMask);
    IMPORT /*050/---*/ virtual void         SaveModifiedForm(UInt32 modifiedMask);
    IMPORT /*054/---*/ virtual void         LoadModifiedForm(UInt32 modifiedMask, UInt32 unkFlags);
    #endif
    IMPORT /*06C/070*/ virtual void         LinkForm();
    IMPORT /*0B4/0B8*/ virtual void         CopyFrom(TESForm& form);
    IMPORT /*0B8/0BC*/ virtual bool         CompareTo(TESForm& compareTo);
    #ifndef OBLIVION
    IMPORT /*---/0DC*/ virtual void         GetObjectWindowTreePath(BSStringT& path);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif

    // No TESObject or TESBoundObject virtual method overrides
    // No TESFullName, TESScriptableForm, TESEnchantableForm, TESWeightForm, TESHealthForm, or TESBipedModelForm virtual method overrides

    // TESValueForm virtual method overrides
    // These methods are all thunks to the corresponding methods in the TESObjectARMO.TESForm vtbl
    #ifdef OBLIVION  
    INLINE /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask) {return TESObjectARMO::ModifiedFormSize(modifiedMask);}
    INLINE /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask) {TESObjectARMO::SaveModifiedForm(modifiedMask);}
    INLINE /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags) {TESObjectARMO::LoadModifiedForm(modifiedMask,unkFlags);}
    #endif

    // No additional virtual methods

    // methods
    IMPORT UInt32       GetArmorSkillAVCode(); // returns avCode of skill associated with armor
    IMPORT bool         IsHeavyArmor();
    
    // constructor
    IMPORT TESObjectARMO();

    // use FormHeap for class new & delete (required to resolve ambiguity from multiple inheritance)
    USEFORMHEAP 
};