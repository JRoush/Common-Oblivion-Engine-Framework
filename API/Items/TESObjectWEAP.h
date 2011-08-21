/* 
    TESObjectWEAP is a base form for weapon items.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/TESForms/TESObject.h"
#include "API/TESForms/BaseFormComponent.h"

class IMPORTCLASS TESObjectWEAP : public TESBoundObject, public TESFullName, public TESModel, public TESIcon, public TESScriptableForm, 
                                public TESEnchantableForm, public TESValueForm, public TESWeightForm, public TESHealthForm, public TESAttackDamageForm
{// size A0/EC
public:

	enum WeaponTypes
	{
		kWeaponType_BladeOneHand		= 0x0,
		kWeaponType_BladeTwoHand        = 0x1,
		kWeaponType_BluntOneHand        = 0x2,
		kWeaponType_BluntTwoHand        = 0x3,
		kWeaponType_Staff               = 0x4,
		kWeaponType_Bow                 = 0x5,
		kWeaponType__MAX
	};

	enum WeaponFlags
	{
		kWeaponFlags_IgnoreNormalWeaponResistance		= /*00*/ 0x01,
	};

	// members
	//     /*00/00*/ TESBoundObject
	//     /*24/58*/ TESFullName
	//     /*30/64*/ TESModel
	//	   /*48/88*/ TESIcon
	//	   /*54/A0*/ TESScriptableForm
	//	   /*60/AC*/ TESEnchantableForm
	//	   /*70/BC*/ TESValueForm
	//	   /*78/C4*/ TESWeightForm
	//	   /*80/CC*/ TESHealthForm
	//	   /*88/D4*/ TESAttackDamageForm
	MEMBER /*90/DC*/ UInt8      weaponType;
	MEMBER /*91/DD*/ UInt8      pad91[3];
	MEMBER /*94/E0*/ float      weaponSpeed;
	MEMBER /*98/E4*/ float      weaponReach;
	MEMBER /*9C/E8*/ UInt8      weaponFlags;
	MEMBER /*9D/E9*/ UInt8      pad9D[3];

    // TESForm virtual method overrides
    IMPORT /*010/034*/ virtual              ~TESObjectWEAP();
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
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); 
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog); 
    IMPORT /*---/120*/ virtual const char*  GetSubtypeName();
    #endif

    // No TESObject or TESBoundObject virtual method overrides
    // No TESFullName, TESIcon, TESScriptableForm, TESWeightForm, TESHealthForm, or TESAttackDamageForm virtual method overrides

    // TESModel virtual method overrides
    //     /*010/034*/ virtual              ~TESObjectWEAP();    // automatically overriden with thunk to destructor in TESObjectWEAP.TESForm vtbl

    // TESValueForm virtual method overrides
    // These methods are all thunks to the corresponding methods in the TESObjectWEAP.TESForm vtbl
    #ifdef OBLIVION  
    INLINE /*010/---*/ virtual UInt16       ModifiedComponentSize(UInt32 modifiedMask) {return TESObjectWEAP::ModifiedFormSize(modifiedMask);}
    INLINE /*014/---*/ virtual void         SaveModifiedComponent(UInt32 modifiedMask) {TESObjectWEAP::SaveModifiedForm(modifiedMask);}
    INLINE /*018/---*/ virtual void         LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags) {TESObjectWEAP::LoadModifiedForm(modifiedMask,unkFlags);}
    #endif

    // TESEnchantableForm virtual method overrides
    IMPORT /*010/034*/ virtual UInt32       GetCastingType();

    // No additional virtual methods

    // methods
    IMPORT UInt32       GetWeaponSkillAVCode(); // returns avCode of skill associated with weapon
    #ifndef OBLIVION
    IMPORT void         PopulateComboWithWeaponTypes(HWND comboBox);
    #endif
    
    // constructor
    IMPORT TESObjectWEAP();

    // use FormHeap for class new & delete (required to resolve ambiguity from multiple inheritance)
    USEFORMHEAP 
};