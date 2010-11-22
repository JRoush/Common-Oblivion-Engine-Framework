/*
    TESObjectCELL describes a physical location in the game world, tracking the scenegraph information(?)
    and all objects (TESObjectREFRs) inside it.
*/
#pragma once

// base classes
#include "API/TESForms/TESForm.h"
#include "API/TESForms/BaseFormComponent.h" // TESFullName
#include "API/ExtraData/ExtraDataList.h"

// argument classes
class   TESObjectREFR;      // TESForms/TESObjectREFR.h
class   TESObjectLAND;
class   TESPathGrid;
class   TESWorldSpace;
class   NiNode;

class TESObjectCELL : public TESForm, public TESMemContextForm, public TESFullName
{// size 58/5C
/*
    Partial:
    -   flag members
    Notes:
    -   Overrides for undecoded virtual functions (be sure to update these if the base definition changes)
*/
public:

    typedef BSSimpleList<TESObjectREFR*>    ObjectREFRList;

	enum ModifiedFlags
	{
		kModified_DetachTime        = /*1B*/ 0x08000000, // only present if version >= 0x5A
		kModified_CellFlags         = /*03*/ 0x00000008, // CHANGE_CELL_FLAGS
		kModified_SeenData          = /*1C*/ 0x10000000, // CHANGE_CELL_SEENDATA
		kModified_FullName          = /*04*/ 0x00000010, // CHANGE_CELL_FULLNAME
		kModified_Ownership         = /*05*/ 0x00000020, // CHANGE_CELL_OWNERSHIP
		kModified_Pathgrid          = /*18*/ 0x01000000, // CHANGE_CELL_PATHGRID_MODIFIED
	};

	enum CellFlags
	{
		kCellFlags_Interior             = /*00*/ 0x01, //
		kCellFlags_HasWater             = /*01*/ 0x02, // also hand changed?
		kCellFlags_InvertFastTravel     = /*02*/ 0x04, // interiors: can travel, exteriors: cannot travel
		kCellFlags_ForceHideLand        = /*03*/ 0x08, // exteriors only
		kCellFlags_OblivionInterior     = /*03*/ 0x08, // interiors only 
		kCellFlags_Unk4                 = /*04*/ 0x10,
		kCellFlags_Public               = /*05*/ 0x20,
		kCellFlags_HandChanged          = /*06*/ 0x40, // also has water?
		kCellFlags_BehaveLikeExterior   = /*07*/ 0x80,
	};

	struct CellCoordinates // what is the actual type for this?
	{
		SInt32	x;
		SInt32	y;
	};

    // members
    //     /*00/00*/ TESForm           
    //     /*18/24*/ TESFullName
	//     /*24/30*/ TESMemContextForm - empty, no members
	MEMBER /*24/30*/ UInt8			    cellFlags24;
	MEMBER /*25/31*/ UInt8			    cellFlags25;
	MEMBER /*26/32*/ UInt8			    cellFlags26;
	MEMBER /*27/33*/ UInt8			    pad27;
	MEMBER /*28/34*/ ExtraDataList	    extraData; // includes ExtraEditorID in game
	MEMBER /*3C/48*/ CellCoordinates*   coords;
	MEMBER /*40/4C*/ TESObjectLAND*     land;
	MEMBER /*44/50*/ TESPathGrid*       pathGrid;
	MEMBER /*48/54*/ ObjectREFRList	    objectList;
    #ifdef OBLIVION
	MEMBER /*50/--*/ TESWorldSpace*     worldSpace; // how is this tracked in the CS?
	MEMBER /*54/--*/ NiNode*            niNode; // smart pointer, increments/decrements ref count for node
    #endif

    // no TESFullName virtual method overrides

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual 	            ~TESObjectCELL();
    IMPORT /*018/03C*/ virtual void	        ClearAllComponentReferences();
	IMPORT /*01C/040*/ virtual bool	        LoadForm(TESFile& file);
    IMPORT /*020/044*/ virtual bool	        SaveFormRecord(TESFile& file);
	IMPORT /*024/048*/ virtual void	        SaveFormChunks();
    IMPORT /*02C/050*/ virtual bool	        SaveForm(TESFile& file);
    IMPORT /*030/054*/ virtual bool	        LessThanGroup(const RecordInfo& groupRecord);
	IMPORT /*034/058*/ virtual bool	        LessThan(TESForm& form);
    IMPORT /*038/05C*/ virtual TESForm*     Clone(bool autoEditorID, NiTMap<TESForm*,TESForm*>* cloneMap);
    #ifdef OBLIVION
    IMPORT /*04C/---*/ virtual UInt16	    ModifiedFormSize(UInt32 modifiedMask);
    IMPORT /*050/---*/ virtual void	        SaveModifiedForm(UInt32 modifiedMask);
    IMPORT /*054/---*/ virtual void	        LoadModifiedForm(UInt32 modifiedMask, UInt32 unkFlags);
    INLINE /*058/---*/ virtual void	        LinkModifiedForm(UInt32 modifiedMask, UInt32 arg1);
    INLINE /*060/---*/ virtual void	        PreLoadModifiedForm(UInt32 modifiedMask);
    _NOUSE /*068/---*/ virtual bool	        UnkForm068(UInt32 arg0);
    #endif
    IMPORT /*06C/070*/ virtual void	        LinkForm();
    IMPORT /*090/094*/ virtual void	        SetFromActiveFile(bool fromActiveFile);
    IMPORT /*0B4/0B8*/ virtual void	        CopyFrom(TESForm& form);
	IMPORT /*0B8/0BC*/ virtual bool	        CompareTo(TESForm& compareTo);
	IMPORT /*0BC/0C0*/ virtual bool	        MatchGroupRecord(const RecordInfo& groupRecord, bool matchAllLevels, bool arg2);
    IMPORT /*0C0/0C4*/ virtual void	        CreateGroupRecord(RecordInfo& newGroupRecord, RecordInfo& currentGroupRecord);
    #ifdef OBLIVION
    _NOUSE /*0C4/---*/ virtual bool	        UnkForm0C4();
    _NOUSE /*0C8/---*/ virtual bool	        UnkForm0C8(UInt32 arg0);
    #endif
    IMPORT /*0D4/0C8*/ virtual const char*  GetEditorID();
    #ifdef OBLIVION
    IMPORT /*0D8/---*/ virtual bool	        SetEditorID(const char* newEditorID);
    #else
    _NOUSE /*---/0CC*/ virtual bool         UnkForm0CC();
    _NOUSE /*---/0D0*/ virtual bool         UnkForm0D0(UInt32 arg0);
    _NOUSE /*---/0D8*/ virtual bool         UnkForm0D8();
    _NOUSE /*---/0E0*/ virtual bool         UnkForm0E0(UInt32 arg0);
    _NOUSE /*---/0E4*/ virtual bool         UnkForm0E4();
    _NOUSE /*---/0E8*/ virtual bool         UnkForm0E8(UInt32 arg0);
    _NOUSE /*---/0EC*/ virtual bool         UnkForm0EC(UInt32 arg0, UInt32 arg1);
    _NOUSE /*---/0F0*/ virtual void         UnkForm0F0(void* arg0);
    IMPORT /*---/0F4*/ virtual void         RemoveFormReference(TESForm& form);
    _NOUSE /*---/0F8*/ virtual bool         UnkForm0F8(UInt32 arg0);
    _NOUSE /*---/0FC*/ virtual void         UnkForm0FC(UInt32 arg0, BSStringT& output);
    IMPORT /*---/10C*/ virtual void         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    IMPORT /*---/11C*/ virtual void         CleanupDialog(HWND dialog);
    #endif

    // No additional virtual methods

    // constructor
    IMPORT TESObjectCELL();
};

class TESChildCell
{// size 04/04
/*
    Todo:
    -   possible inlined destructor 
*/
public:

    // no members, other than vtbl

    // virtual methods
    IMPORT /*00/00*/ virtual TESObjectCELL*	    GetChildCell() = 0;

    // no constructor, destructor
};

