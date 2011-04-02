/* 
    MagicItemObject is an abstract form component for objects with 'castable' effects that can be 
    placed as references, i.e. Potions & Ingredients.
*/ 
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/Magic/MagicItem.h"
#include "API/TESForms/TESObject.h" // TESBoundObject
#include "API/TESForms/BaseFormComponent.h" // TESModel, TESIcon, TESScriptableForm, TESWeightForm

class IMPORTCLASS MagicItemObject : public TESBoundObject, public MagicItem
{// size 040/140
/*
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    // members
    //     /*000/000*/ TESBoundObject
    //     /*024/058*/ MagicItem.TESFullName
    //     /*030/064*/ MagicItem.EffectItemList

    // TESBoundObject virtual methods
    IMPORT /*010/034*/ virtual              ~MagicItemObject();
    IMPORT /*01C/040*/ virtual bool         LoadForm(TESFile& file);
    IMPORT /*024/048*/ virtual void         SaveFormChunks(); 
    _NOUSE /*0AC/0B0*/ virtual bool         UnkForm0AC() {return true;}
    IMPORT /*0B4/0B8*/ virtual void         CopyFrom(TESForm& form);
    IMPORT /*0B8/0BC*/ virtual bool         CompareTo(TESForm& compareTo);    
    #ifndef OBLIVION
    _NOUSE /*---/0F8*/ virtual bool         UnkForm0F8(UInt32 arg0) {return false;}
    _NOUSE /*---/0FC*/ virtual void         UnkForm0FC(UInt32 arg0, BSStringT& output) {}
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif
    INLINE /*0E4/12C*/ virtual bool         IsObjectAutoCalc() = 0;
    INLINE /*0E8/130*/ virtual void         SetObjectAutoCalc(bool autoCalc) = 0;

    // MagicItem virtual methods
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool         MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    IMPORT /*---/050*/ virtual void         GetMagicItemFromDlg(HWND dialog);
    IMPORT /*---/054*/ virtual void         MagicItemDlgCleanup(HWND dialog);
    #endif

    // no additional virtual methods

    // constructor, destructor
    IMPORT MagicItemObject();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

class IMPORTCLASS IngredientItem : public MagicItemObject, public TESModel, public TESIcon, public TESScriptableForm, public TESWeightForm
{// size 080/198
public:

    enum IngredientFlags
    {
        kIngrFlag_NoAutocalc    = /*00*/ 0x01,
        kIngrFlag_Edible        = /*01*/ 0x02,
    };

    // members
    //     /*000/000*/ TESBoundObject
    //     /*024/058*/ MagicItem.TESFullName
    //     /*030/064*/ MagicItem.EffectItemList
    //     /*040/140*/ TESModel
    //     /*058/164*/ TESIcon
    //     /*064/17C*/ TESScriptableForm
    //     /*070/188*/ TESWeightForm
    MEMBER /*078/190*/ SInt32           baseCost; // base magicka cost / gold cost for non-autocalc
    MEMBER /*07C/194*/ UInt8            ingrFlags;
    MEMBER /*07D/195*/ UInt8            ingrPad7D[3]; // saved & loaded, but not initialized    

    // TESBoundObject virtual methods
    IMPORT /*010/034*/ virtual              ~IngredientItem();// also overrides TESModel::~TESModel(), for which
                                            // the compiler automatically generates a thunk redirecting to this method
    IMPORT /*014/038*/ virtual void         InitializeAllComponents();
    IMPORT /*018/03C*/ virtual void         ClearAllComponentReferences();
    IMPORT /*06C/070*/ virtual void         LinkForm();
    #ifndef OBLIVION
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif
    IMPORT /*0E4/12C*/ virtual bool         IsObjectAutoCalc();
    IMPORT /*0E8/130*/ virtual void         SetObjectAutoCalc(bool autoCalc);

    // MagicItem virtual methods
    IMPORT /*010/034*/ virtual bool         IsMagicItemAutoCalc();
    IMPORT /*014/038*/ virtual void         SetMagicItemAutoCalc(bool autoCalc);
    IMPORT /*018/03C*/ virtual UInt32       GetMagicType();
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool         MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    IMPORT /*---/050*/ virtual void         GetMagicItemFromDlg(HWND dialog);
    #endif
    IMPORT /*024/058*/ virtual UInt32       ExtraDataChunkType();
    IMPORT /*028/05C*/ virtual void*        ExtraDataPtr();
    IMPORT /*02C/060*/ virtual UInt32       ExtraDataSize();
    IMPORT /*030/064*/ virtual bool         CompareExtraData(const MagicItem& compareTo);
    IMPORT /*034/068*/ virtual void         CopyExtraData(const MagicItem& copyFrom);
    IMPORT /*038/06C*/ virtual void         SaveBases();
    IMPORT /*03C/070*/ virtual void         LoadBases(TESFile& file, UInt32 chunkType);
    #ifndef OBLIVION
    IMPORT /*---/074*/ virtual void         UpdateMagicItemInDialog(HWND dialog);
    #endif

    // EffectItemList virtual methods
    #ifdef OBLIVION
    IMPORT /*000/000*/ virtual float        GetMagickaCost(const Actor* caster);
    #else
    IMPORT /*000/000*/ virtual SInt32       GetMagickaCost(const Actor* caster);
    #endif
    IMPORT /*004/004*/ virtual UInt32       GetMaxEffectCount();

    // constructor
    IMPORT IngredientItem();

    // use FormHeap for class new & delete
    USEFORMHEAP
};

class IMPORTCLASS AlchemyItem : public MagicItemObject, public TESModel, public TESIcon, public TESScriptableForm, public TESWeightForm
{// size 080/198
public:

    enum AlchemyItemFlags
    {
        kAlchFlag_NoAutocalc    = /*00*/ 0x01,
        kAlchFlag_Edible        = /*01*/ 0x02,
    };

    // members
    //     /*000/000*/ TESBoundObject
    //     /*024/058*/ MagicItem.TESFullName
    //     /*030/064*/ MagicItem.EffectItemList
    //     /*040/140*/ TESModel
    //     /*058/164*/ TESIcon
    //     /*064/17C*/ TESScriptableForm
    //     /*070/188*/ TESWeightForm
    MEMBER /*078/190*/ SInt32           baseCost; // base magicka cost / gold cost for non-autocalc
    MEMBER /*07C/194*/ UInt8            alchFlags;
    MEMBER /*07D/195*/ UInt8            alchPad7D[3]; // saved & loaded, but not initialized    

    // TESBoundObject virtual methods
    IMPORT /*010/034*/ virtual              ~AlchemyItem();// also overrides TESModel::~TESModel(), for which
                                            // the compiler automatically generates a thunk redirecting to this method
    IMPORT /*014/038*/ virtual void         InitializeAllComponents();
    IMPORT /*018/03C*/ virtual void         ClearAllComponentReferences();
    IMPORT /*06C/070*/ virtual void         LinkForm();
    #ifndef OBLIVION
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    #endif
    IMPORT /*0E4/12C*/ virtual bool         IsObjectAutoCalc();
    IMPORT /*0E8/130*/ virtual void         SetObjectAutoCalc(bool autoCalc);

    // MagicItem virtual methods
    IMPORT /*010/034*/ virtual bool         IsMagicItemAutoCalc();
    IMPORT /*014/038*/ virtual void         SetMagicItemAutoCalc(bool autoCalc);
    IMPORT /*018/03C*/ virtual UInt32       GetMagicType();
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool         MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    IMPORT /*---/050*/ virtual void         GetMagicItemFromDlg(HWND dialog);
    #endif
    IMPORT /*024/058*/ virtual UInt32       ExtraDataChunkType();
    IMPORT /*028/05C*/ virtual void*        ExtraDataPtr();
    IMPORT /*02C/060*/ virtual UInt32       ExtraDataSize();
    IMPORT /*030/064*/ virtual bool         CompareExtraData(const MagicItem& compareTo);
    IMPORT /*034/068*/ virtual void         CopyExtraData(const MagicItem& copyFrom);
    IMPORT /*038/06C*/ virtual void         SaveBases();
    IMPORT /*03C/070*/ virtual void         LoadBases(TESFile& file, UInt32 chunkType);
    #ifndef OBLIVION
    IMPORT /*---/074*/ virtual void         UpdateMagicItemInDialog(HWND dialog);
    #endif

    // EffectItemList virtual methods
    #ifdef OBLIVION
    IMPORT /*000/000*/ virtual float        GetMagickaCost(const Actor* caster);
    #else
    IMPORT /*000/000*/ virtual SInt32       GetMagickaCost(const Actor* caster);
    #endif

    // constructor
    IMPORT AlchemyItem();

    // use FormHeap for class new & delete
    USEFORMHEAP
};