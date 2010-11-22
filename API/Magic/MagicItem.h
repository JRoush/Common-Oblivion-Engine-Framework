/* 
    MagicItem is an abstract form component for objects with 'castable' effects - Spells, Enchantments, Ingredients, & Potions
    It is *not* the base for sigil stones, which cannot be cast directly.
*/ 
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h" // TESFullName
#include "API/Magic/EffectItem.h" // EffectItemList
#include "API/Magic/Magic.h" // for various contsant declarations

// argument classes
class   TESFile;

class IMPORTCLASS MagicItem : public TESFullName, public EffectItemList
{// size 1C/E8
public:

    // members
    //     /*00/00*/ TESFullName 
    //     /*0C/0C*/ EffectItemList 

    // no overrides to base class virtual methods

    // additional virtual methods
    IMPORT /*010/034*/ virtual bool         IsMagicItemAutoCalc() = 0;
    IMPORT /*014/038*/ virtual void         SetMagicItemAutoCalc(bool autoCalc) = 0;
    IMPORT /*018/03C*/ virtual UInt32       GetMagicType() = 0; // from Magic::MagicTypes
    INLINE /*01C/040*/ virtual bool         ScriptAlwaysApplies() {return false;}
    INLINE /*020/044*/ virtual bool         NoAbsorbReflect() {return false;}
    #ifndef OBLIVION
    IMPORT /*---/048*/ virtual bool	        MagicItemDlgMsgCallback(HWND dialog, int uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/04C*/ virtual void         SetMagicItemInDialog(HWND dialog);
    INLINE /*---/050*/ virtual void	        GetMagicItemFromDlg(HWND dialog) {}
    INLINE /*---/054*/ virtual void	        MagicItemDlgCleanup(HWND dialog) {}
    #endif
    IMPORT /*024/058*/ virtual UInt32       ExtraDataChunkType() = 0; // chunk type for serializing extra data
                                            // 'extra' data are the members of a derived class that don't belong to other base classes
    IMPORT /*028/05C*/ virtual void*        ExtraDataPtr() = 0; // pointer to beginning of extra data block
    IMPORT /*02C/060*/ virtual UInt32       ExtraDataSize() = 0; // size of extra data
    IMPORT /*030/064*/ virtual bool         CompareExtraData(const MagicItem& compareTo) = 0; // compares extra data
    IMPORT /*034/068*/ virtual void         CopyExtraData(const MagicItem& copyFrom) = 0; // copies extra data
    INLINE /*038/06C*/ virtual void         SaveBases() {} // calls Save() routines for other base classes
    INLINE /*03C/070*/ virtual void         LoadBases(TESFile& file, UInt32 chunkType) {} // calls Load() routines for other base classes 
    #ifndef OBLIVION
    INLINE /*---/074*/ virtual void	        UpdateMagicItemInDialog(HWND dialog) {} // update derived stats - autocalc cost, hostility, etc.
    #endif

    // methods   
        // ...

    // constructor, destructor
    IMPORT MagicItem();
    IMPORT ~MagicItem();
};