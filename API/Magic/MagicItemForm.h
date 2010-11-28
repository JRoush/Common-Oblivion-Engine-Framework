/* 
    MagicItemForm is an abstract form component for objects with 'castable' effects that cannot be 
    placed as references, i.e. Spells & Enchantments.
*/ 
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/Magic/MagicItem.h"
#include "API/TESForms/TESForm.h" // TESForm

class IMPORTCLASS MagicItemForm : public TESForm, public MagicItem
{// size 034/10C
/*
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    // members
    //     /*000/000*/ TESForm
    //     /*018/024*/ MagicItem.TESFullName
    //     /*024/030*/ MagicItem.EffectItemList

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual 	            ~MagicItemForm();
    IMPORT /*01C/040*/ virtual bool	        LoadForm(TESFile& file);
	IMPORT /*024/048*/ virtual void	        SaveFormChunks(); 
    _NOUSE /*0AC/0B0*/ virtual bool	        UnkForm0AC() {return true;}
	IMPORT /*0B4/0B8*/ virtual void	        CopyFrom(TESForm& form);
	IMPORT /*0B8/0BC*/ virtual bool	        CompareTo(TESForm& compareTo);
    #ifndef OBLIVION
    _NOUSE /*---/0F8*/ virtual bool         UnkForm0F8(UInt32 arg0) {return false;}
    _NOUSE /*---/0FC*/ virtual void         UnkForm0FC(UInt32 arg0, BSStringT& output) {}
    IMPORT /*---/10C*/ virtual void         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif

    // MagicItem virtual methods
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool	        MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    IMPORT /*---/050*/ virtual void	        GetMagicItemFromDlg(HWND dialog);
    IMPORT /*---/054*/ virtual void	        MagicItemDlgCleanup(HWND dialog);
    #endif

    // no additional virtual methods

    // constructor, destructor
    IMPORT MagicItemForm();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

class IMPORTCLASS EnchantmentItem : public MagicItemForm
{// size 044/11C
public:

    enum DialogControlIDs // in addition to IDs defined in base classes and by windows
    {
        IDC_CastingType                 = 0x439, // COMBOBOX
        IDC_ChargeAmount                = 0x43D, // EDIT
        IDC_EnchantmentCost             = 0x55E, // EDIT
        IDC_Autocalc                    = 0x3EE, // BUTTON
        IDC_EnchantmentCostLabel        = 0x686, // STATIC
    };
    static const UInt32     IDD_EnchantmentItem = 0x0A6; // Dialog Tempate ID

	enum EnchantmentFlags
    {
		kEnchFlag_NoAutoCalc = 0x00000001,
	};

    // members
    //     /*000/000*/ TESForm
    //     /*018/024*/ MagicItem.TESFullName
    //     /*024/030*/ MagicItem.EffectItemList
	MEMBER /*034/10C*/ UInt32           enchType; // from Magic::CastTypes
	MEMBER /*038/110*/ SInt32           charge; // for non-auto calc
	MEMBER /*03C/114*/ SInt32           cost;
	MEMBER /*040/118*/ UInt8            enchFlags;
    MEMBER /*041/119*/ UInt8            enchPad041[3]; // saved & loaded, but not initialized

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual 	            ~EnchantmentItem();
    IMPORT /*014/038*/ virtual void	        InitializeAllComponents();
	IMPORT /*018/03C*/ virtual void	        ClearAllComponentReferences();
    #ifndef OBLIVION
    IMPORT /*---/10C*/ virtual void         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif

    // MagicItem virtual methods
    IMPORT /*010/034*/ virtual bool         IsMagicItemAutoCalc();
    IMPORT /*014/038*/ virtual void         SetMagicItemAutoCalc(bool autoCalc);
    IMPORT /*018/03C*/ virtual UInt32       GetMagicType();
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool	        MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    IMPORT /*---/050*/ virtual void	        GetMagicItemFromDlg(HWND dialog);
    #endif
    IMPORT /*024/058*/ virtual UInt32       ExtraDataChunkType();
    IMPORT /*028/05C*/ virtual void*        ExtraDataPtr();
    IMPORT /*02C/060*/ virtual UInt32       ExtraDataSize();
    IMPORT /*030/064*/ virtual bool         CompareExtraData(const MagicItem& compareTo);
    IMPORT /*034/068*/ virtual void         CopyExtraData(const MagicItem& copyFrom);
     #ifndef OBLIVION
    IMPORT /*---/074*/ virtual void	        UpdateMagicItemInDialog(HWND dialog);
    #endif

    // EffectItemList virtual methods
    #ifdef OBLIVION
    IMPORT /*000/000*/ virtual float        GetMagickaCost(const Actor* caster);
    #else
    IMPORT /*000/000*/ virtual SInt32       GetMagickaCost(const Actor* caster);
    #endif

    // constructor
    IMPORT EnchantmentItem();
};

class IMPORTCLASS SpellItem : public MagicItemForm
{// size 044/11C
public:

    enum DialogControlIDs // in addition to IDs defined in base classes and by windows
    {
        IDC_PCStartSpell                = 0x3EF, // BUTTON
        IDC_SpellCost                   = 0x50F, // EDIT
        IDC_Autocalc                    = 0x3EE, // BUTTON
        IDC_SpellCostLabel              = 0x687, // STATIC
        IDC_SpellType                   = 0x41A, // COMBOBOX
        IDC_MasteryLevel                = 0x823, // COMBOBOX
        IDC_School                      = 0x827, // STATIC
        IDC_ImmuneToSilence             = 0x3F0, // BUTTON
        IDC_AOEIgnoresLOS               = 0x3F1, // BUTTON
        IDC_ScriptEffectAlwaysApplies   = 0x3F2, // BUTTON
        IDC_NoReflectOrAbsorb           = 0x3F3, // BUTTON
        IDC_TouchExplodesWithoutTarget  = 0x3F4, // BUTTON
    };
    static const UInt32     IDD_EnchantmentItem = 0x117; // Dialog Tempate ID

	enum SpellFlags
    {
		kSpellFlag_NoAutoCalc                   = /*00*/ 0x01,
        kSpellFlag_ImmuneToSilence1             = /*01*/ 0x02,
        kSpellFlag_PlayerStartSpell             = /*02*/ 0x04,
        kSpellFlag_ImmuneToSilence2             = /*03*/ 0x08,
        kSpellFlag_AOEIgnoresLOS                = /*04*/ 0x10,
        kSpellFlag_SEFFAlwaysApplies            = /*05*/ 0x20,
        kSpellFlag_NoAbsorbOrReflect            = /*06*/ 0x40,
		kSpellFlag_TouchExplodesWithNoTarget    = /*07*/ 0x80,
	};

    // members
    //     /*000/000*/ TESForm
    //     /*018/024*/ MagicItem.TESFullName
    //     /*024/030*/ MagicItem.EffectItemList
	MEMBER /*034/10C*/ UInt32           spellType; // from Magic::MagicTypes, from Spell - Ability
	MEMBER /*038/110*/ SInt32           magickaCost; // for non-auto calc
	MEMBER /*03C/114*/ UInt32           masteryLevel; // for non-autocalc
	MEMBER /*040/118*/ UInt8            spellFlags;
    MEMBER /*041/119*/ UInt8            spellPad041[3]; // saved & loaded, but not initialized

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual 	            ~SpellItem();
    IMPORT /*014/038*/ virtual void	        InitializeAllComponents();
	IMPORT /*018/03C*/ virtual void	        ClearAllComponentReferences();
    #ifndef OBLIVION
    IMPORT /*---/0DC*/ virtual void         GetResourcePath(BSStringT& path);
    IMPORT /*---/10C*/ virtual void         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif

    // MagicItem virtual methods
    IMPORT /*010/034*/ virtual bool         IsMagicItemAutoCalc();
    IMPORT /*014/038*/ virtual void         SetMagicItemAutoCalc(bool autoCalc);
    IMPORT /*018/03C*/ virtual UInt32       GetMagicType();
    IMPORT /*01C/040*/ virtual bool         ScriptAlwaysApplies();
    IMPORT /*020/044*/ virtual bool         NoAbsorbReflect();
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool	        MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    IMPORT /*---/050*/ virtual void	        GetMagicItemFromDlg(HWND dialog);
    #endif
    IMPORT /*024/058*/ virtual UInt32       ExtraDataChunkType();
    IMPORT /*028/05C*/ virtual void*        ExtraDataPtr();
    IMPORT /*02C/060*/ virtual UInt32       ExtraDataSize();
    IMPORT /*030/064*/ virtual bool         CompareExtraData(const MagicItem& compareTo);
    IMPORT /*034/068*/ virtual void         CopyExtraData(const MagicItem& copyFrom);
     #ifndef OBLIVION
    IMPORT /*---/074*/ virtual void	        UpdateMagicItemInDialog(HWND dialog);
    #endif

    // EffectItemList virtual methods
    #ifdef OBLIVION
    IMPORT /*000/000*/ virtual float        GetMagickaCost(const Actor* caster);
    #else
    IMPORT /*000/000*/ virtual SInt32       GetMagickaCost(const Actor* caster);
    #endif
    IMPORT /*008/008*/ virtual UInt32       GetMasteryLevel();

    // constructor
    IMPORT SpellItem();
};
