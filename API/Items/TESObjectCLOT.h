/* 
    TESObjectCLOT is a base form for clothing items.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/TESForms/TESObject.h"
#include "API/TESForms/BaseFormComponent.h"


class IMPORTCLASS TESObjectCLOT : public TESBoundObject, public TESFullName, public TESScriptableForm, public TESEnchantableForm, 
                                    public TESValueForm, public TESWeightForm, public TESBipedModelForm
{// size 0DC/158
public:

    // members
    //     /*000/000*/ TESBoundObject
    //     /*024/058*/ TESFullName
    //     /*030/064*/ TESScriptableForm
    //     /*03C/070*/ TESEnchantableForm
    //     /*04C/080*/ TESValueForm
    //     /*054/088*/ TESWeightForm
    //     /*05C/090*/ TESBipedModelForm
    // no additional members

    // TESForm virtual method overrides
    IMPORT /*010/034*/ virtual              ~TESObjectCLOT();
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
    #endif

    // No TESObject or TESBoundObject virtual method overrides
    // No TESFullName, TESScriptableForm, TESEnchantableForm, TESWeightForm, or TESBipedModelForm virtual method overrides

    // TESValueForm virtual method overrides
    // These methods are all thunks to the corresponding methods in the TESObjectCLOT.TESForm vtbl
    #ifdef OBLIVION  
    INLINE /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask) {return TESObjectCLOT::ModifiedFormSize(modifiedMask);}
    INLINE /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask) {TESObjectCLOT::SaveModifiedForm(modifiedMask);}
    INLINE /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags) {TESObjectCLOT::LoadModifiedForm(modifiedMask,unkFlags);}
    #endif

    // No additional virtual methods

    // constructor
    IMPORT TESObjectCLOT();

    // use FormHeap for class new & delete (required to resolve ambiguity from multiple inheritance)
    USEFORMHEAP 
};