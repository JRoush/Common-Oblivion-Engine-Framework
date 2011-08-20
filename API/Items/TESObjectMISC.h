/* 
    TESObjectMISC is a base form for miscellaneous "clutter" inventory items.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/TESForms/TESObject.h"
#include "API/TESForms/BaseFormComponent.h"

class IMPORTCLASS TESObjectMISC : public TESBoundObject, public TESFullName, public TESModel, public TESIcon, 
                                public TESScriptableForm, public TESValueForm, public TESWeightForm
{// size 70/BC
public:

	// members
	//     /*00/00*/ TESBoundObject
	//     /*24/58*/ TESFullName
	//     /*30/64*/ TESModel
	//	   /*48/88*/ TESIcon
	//	   /*54/A0*/ TESScriptableForm
	//	   /*60/AC*/ TESValueForm
	//	   /*68/B4*/ TESWeightForm
	// no additional members

    // TESForm virtual method overrides
    IMPORT /*010/034*/ virtual              ~TESObjectMISC();
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

    // No TESObject or TESBoundObject virtual method overrides
    // No TESFullName, TESIcon, TESScriptableForm, or TESWeightForm virtual method overrides

    // TESModel virtual method overrides
    //     /*010/034*/ virtual              ~TESObjectMISC();    // automatically overriden with thunk to destructor in TESObjectMISC.TESForm vtbl

    // TESValueForm virtual method overrides
    // These methods are all thunks to the corresponding methods in the TESObjectMISC.TESForm vtbl
    #ifdef OBLIVION  
    INLINE /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask) {return TESObjectMISC::ModifiedFormSize(modifiedMask);}
    INLINE /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask) {TESObjectMISC::SaveModifiedForm(modifiedMask);}
    INLINE /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags) {TESObjectMISC::LoadModifiedForm(modifiedMask,unkFlags);}
    #endif

    // No additional virtual methods
    
    // constructor
    IMPORT TESObjectMISC();

    // use FormHeap for class new & delete (required to resolve ambiguity from multiple inheritance)
    USEFORMHEAP 
};