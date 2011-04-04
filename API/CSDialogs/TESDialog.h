/* 
    TESDialog is a 'container' classes for dealing with CS interface dialogs.
    The static methods and data here may actually belong in a number of other classes, but they are grouped together 
    in the exectuable images, and apparently had their own file in the BS repository ("TESDialog.cpp").
    At the moment, these classes are defined for convenience, to group code related to the CS interface in one place.

    TESDialogs store a BaseExtraDataList for DialogExtra*** objects in their window 'user param' (e.g. GetWindowLong(-0x15))
    See CSDialogs/DialogExtraData.h for definitions.

    There seems to be a strong distinction between dialogs that edit forms and those that edit simpler component objects or 
    provide services like import/export or text searching.  Some members are only for dialogs that edit forms, and some are
    only for dialogs that edit forms accessible in the Objects Window.
*/
#pragma once

#ifndef OBLIVION

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSSimpleList.h"
#include <Commctrl.h> // control message constants, macros for ListView & TabControl
#include <Windowsx.h> // macros for ComboBox & ListBox

// argument classes
class   TESForm;        // TESForms/TESForm.h
class   BSExtraData;    // ExtraData/BSExtraData.h
class   BaseExtraList;  // ExtraData/ExtraDataList.h
class   DialogExtraPopupMenu;   // CSDialogs/DialogExtraData.h
class   DialogExtraSubwindow;   // CSDialogs/DialogExtraData.h
class   TESObjectCELL;  // GameWorld/TESObjectCELL.h
class   TESObjectREFR;  // TESForms/TESObjectREFR.h
class   TESQuest;
class   Script;
class   TESWorldSpace;

class TESDialog
{
public:

    class IMPORTCLASS Subwindow
    {// size 20
    public:
        // members
        MEMBER /*00*/ BSSimpleList<HWND>    controls;
        MEMBER /*08*/ HWND                  hDialog; // handle of parent dialog window
        MEMBER /*0C*/ HINSTANCE             hInstance; // module instance of dialog template
        MEMBER /*10*/ POINT                 position; // position of subwindow withing parent dialog
        MEMBER /*18*/ HWND                  hContainer; // handle of container control (e.g. Tab Control)
        MEMBER /*1C*/ HWND                  hSubwindow; // handle of subwindow, if created

        // methods 
        IMPORT void         EnableControls(bool enabled); // enables/disables controls & invalidates parent rect
        IMPORT void         ClearControls(); // destroys all controls for the subwindow

        // constructor, destructor
        IMPORT Subwindow();
        IMPORT ~Subwindow(); // destroys subwindow & controls, clears control list

        // use FormHeap for class new & delete
        USEFORMHEAP
    };

    // FormEditParam - for form-editing dialogs.
    // passed as initParam to CreateDialogParam() (i.e. lParam on WM_INITDIALOG message) for form-editing dialogs
    struct FormEditParam
    {// size 0C
        MEMBER /*00*/ UInt8     formType; // see TESForm::FormTypes
        MEMBER /*01*/ UInt8     pad01[3]; 
        MEMBER /*04*/ TESForm*  form; // form object to be edited by the dialog
        // use FormHeap for class new & delete
        USEFORMHEAP
    };

    // methods - for Form-editing dialogs, only forms accessed through the Object Window.  
    // Forms accessed through the main menus are derived from TESFormIDListView, and use that dialog proc instead.
    IMPORT static INT_PTR CALLBACK      FormEditDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); // 
    IMPORT static UInt32                GetFormEditDlgTemplateID(UInt8 formType); // returns dialog template ID (e.g. IDD_SpellItem) by form type    
    IMPORT static HWND                  GetFormEditHandle(TESForm* form); // returns handle to dialog for editing form, if any are open

    // methods - for all Form-editing dialogs (all)
    IMPORT static bool                  CallFormEditDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); // 
                                        // fetches local form copy & invokes it's TESForm::DialogMessageCallback() method
    IMPORT static void                  CallFormEditCleanupDlg(HWND dialog); // destroys DialogExtraNotifyInfo, if any.
                                        // fetches local form copy & invokes it's TESForm::CleanupDialog() method
    IMPORT static TESForm*              GetFormEditParam(HWND dialog); // retrieve 'source' form from DialogExtraParam
    IMPORT static TESForm*              GetFormEditLocalCopy(HWND dialog); // retrieve local form copy from DialogExtraLocalCopy

    // methods - dialog extra data
    IMPORT static BaseExtraList*        CreateDialogExtraList(HWND dialog); // allocates a BaseExtraList & stores pointer in window 'user param' 
                                        // returns null if extra list is already present
    IMPORT static void                  DestroyDialogExtraList(HWND dialog); // clears & destroys extra list if present  
    IMPORT static BaseExtraList*        GetDialogExtraList(HWND dialog); // returns window 'user param', does no validity checks
    IMPORT static BSExtraData*          GetDialogExtraData(HWND dialog, UInt32 extraDataType);

    // methods - popup menus
    IMPORT static HMENU                 LoadPopupMenu(HWND dialog, INT menuTemplateID); // load a menu resource & associate it with this dialog
                                        // menu info is stored in a DialogExtraPopupMenu object
                                        // returns null if template is already associated with this dialog, handle of first submenu otherwise
    IMPORT static DialogExtraPopupMenu* GetDialogExtraPopupMenu(HWND dialog, INT menuTemplateID); // retrieve associated popup menu data

    // methods - subwindows
    IMPORT static DialogExtraSubwindow* AddDialogExtraSubwindow(HWND dialog, INT subwindowTemplateID, Subwindow* subwindow);
    IMPORT static Subwindow*            GetSubwindow(HWND dialog, INT subwindowTemplateID);
    IMPORT static bool                  BuildSubwindow(INT subwindowTemplateID, Subwindow* subwindow); //
                                        // creates a temporary instance of template, moves all controls to container window 
                                        // and populates control list, then destroys temporary instance.
                                        // Subwindow dialog, instance, container, and position must be properly initialized

    // methods - file selection 
    IMPORT static bool                  ShowFileSelect(HWND parent, const char* relativePath, const char* filter, 
                                            const char* title, const char* defaultExtension, LPOFNHOOKPROC proc, 
                                            bool fileMustExist, bool saveVsOpen, char* filenameBuffer, UInt32 bufferSize); //
                                        // Display an open/save file dialog.  relativePath is relative to Oblivion/ directory
                                        // returns false if canceled, or selected file is invalid
    IMPORT static bool                  ShowNIFFileSelect(HWND parent, const char* relativePath,
                                            char* filenameBuffer, UInt32 bufferSize); // for selecting model files
    IMPORT static bool                  ShowDDSFileSelect(HWND parent, const char* relativePath, const char* filter, 
                                            char* filenameBuffer, UInt32 bufferSize); // for selecting texture files
    IMPORT static bool                  ShowTXTFileSelect(HWND parent, const char* relativePath, const char* title, 
                                            bool fileMustExist, char* filenameBuffer, UInt32 bufferSize); // for selecting tab-delimited text files
    IMPORT static bool                  ShowPluginFileSave(HWND parent, const char* relativePath, bool esmVsEsp, 
                                            char* filenameBuffer, UInt32 bufferSize); // for selecting ESM/ESP files
    IMPORT static bool                  ShowSoundFileSelect(HWND parent, const char* relativePath,
                                            char* filenameBuffer, UInt32 bufferSize); // for selecting sound files
                                        // this function does not call ShowFileSelect(), it uses a different interface

    // methods - parsing floats to/from edit controls
    IMPORT static bool                  SetDlgItemTextFloat(HWND dialog, INT dlgItemID, float value, int decimalPlaces = 4);
    IMPORT static float                 GetDlgItemTextFloat(HWND dialog, INT dlgItemID);
    IMPORT static void                  SetTextFloatLimits(HWND control, float min, float max, bool noDecimalPlaces = true, UInt32 decimalPlaces = 4); // 
                                        // forces control text (current and future) within specified floating point limits

    // methods - form selection for Drag & Drop
    static const UINT   WM_CANDROPSELECTION = 0x408;    // dispatched to target window during drag & drop to determine if window is a valid drop target
                                                        // WParam = form typecode, LParam = target window handle
                                                        // LResult = 1 if primary form selection can be dropped on target
    static const UINT   WM_DROPSELECTION    = 0x407;    // dispatched to target window during drag & drop to notify window of drop
                                                        // WParam = 0, LParam = POINT* containing coordinates of drop
                                                        // Window should process TESFormSelection::primarySelection, and then clear it
    IMPORT static bool                  CanDropFormSelection(UInt8 formType, HWND targetWindow);

    // static data 
    IMPORT static BSSimpleList<HWND>    openDialogs; // LL of handles for open multiple-instance dialogs, 
                                        // used to ensure same object isn't edited in more than one window
    IMPORT static HWND                  csHandle; // handle of main CS container window
    IMPORT static HINSTANCE             csInstance; // handle of main CS module instance
};

class TESComboBox
{
public:

    // manipulation methods
    // *^ = may be a MS control macro rather than an inlined function
    INLINE static void                  Clear(HWND hCtrl) {ComboBox_ResetContent(hCtrl);} // *^
    IMPORT static void                  AddItem(HWND hCtrl, const char* text, void* data, bool resizeDroppedWidth = true);
    IMPORT static void*                 GetItemData(HWND hCtrl, INT index);
    IMPORT static void*                 GetCurSelData(HWND hCtrl);
    IMPORT static INT                   LookupByData(HWND hCtrl, void* data);
    IMPORT static void                  SetCurSel(HWND hCtrl, INT index);
    IMPORT static void                  SetCurSelByData(HWND hCtrl, void* data); // defaults to first item if data isn't found

    // populating methods - actor values
    IMPORT static void                  PopulateWithActorValues(HWND hCtrl, bool clear, bool noneEntry);
    IMPORT static void                  PopulateWithAttributes(HWND hCtrl, bool noneEntry);  
    IMPORT static void                  PopulateWithSkills(HWND hCtrl, bool noneEntry);  
    IMPORT static void                  PopulateWithSpecializations(HWND hCtrl, bool noneEntry);

    // populating methods - static constants
    IMPORT static void                  PopulateWithAxes(HWND hCtrl);  
    IMPORT static void                  PopulateWithGenders(HWND hCtrl); 
    IMPORT static void                  PopulateWithCrimes(HWND hCtrl);  
    IMPORT static void                  PopulateWithAnimGroups(HWND hCtrl, bool clear, bool noneEntry);
    IMPORT static void                  PopulateWithObjectWindowFormTypes(HWND hCtrl, bool noneEntry);  
    
    typedef bool (*RefFilterFunc)(TESObjectREFR* ref, TESObjectREFR* filterParam); // TODO - belongs with DialogExtraRefSelectControl 

    // populating methods - forms
    IMPORT static void                  PopulateWithForms(HWND hCtrl, UInt8 formType, bool clear, bool noneEntry);  //
                                        // by far the most commonly used method; the rest are for special cases
    IMPORT static void                  PopulateWithObjects(HWND hCtrl, bool noneEntry, UInt32 formType); // pass formType == 0 for all objects
    IMPORT static void                  PopulateWithInventoryItems(HWND hCtrl, bool noneEntry);  
    IMPORT static void                  PopulateWithActors(HWND hCtrl, bool noneEntry);     
    IMPORT static void                  PopulateWithCells(HWND hCtrl, bool noneEntry, bool requireEditorID);  //
                                        // exterior cells do not have editorIDs by default (they can be assigned one)
    IMPORT static void                  PopulateWithCellChildRefs(HWND hCtrl, TESObjectCELL* cell, RefFilterFunc filterFunc, 
                                            TESObjectREFR* filterParam, bool noneEntry, bool clear);    
    IMPORT static void                  PopulateWithQuestStages(HWND hCtrl, TESQuest* quest);  
    IMPORT static void                  PopulateWithRaces(HWND hCtrl, bool noneEntry);  
    IMPORT static void                  PopulateWithScripts(HWND hCtrl, bool noneEntry, bool onlyQuest, bool onlyEffect); // 
                                        // if both onlyQuest & onlyEffect are false, then only object scripts are added
    IMPORT static void                  PopulateWithScriptVariables(HWND hCtrl, Script* script);  
    IMPORT static void                  PopulateWithEnchantments(HWND hCtrl, bool noneEntry, UInt32 castingType, TESForm* enchantedForm); // 
                                        // casting type matches enchantment types, see Magic::CastingTypes.  pass kCastingType__MAX == 4 for all types
                                        // enchantments are filtered based on compatibility with enchantedForm, which is not optional
    IMPORT static void                  PopulateWithSpells(HWND hCtrl, bool clear, bool noneEntry);  
};

class TESListBox
{
public:

    // manipulation 
    // *^ = may be a MS control macro rather than an inlined function
    INLINE static void                  Clear(HWND hCtrl) {ListBox_ResetContent(hCtrl);} // *^
    IMPORT static void                  AddItem(HWND hCtrl, const char* text, void* data);
    IMPORT static void*                 GetItemData(HWND hCtrl, INT index);
    IMPORT static void*                 GetCurSelData(HWND hCtrl);
    IMPORT static INT                   LookupByData(HWND hCtrl, void* data);
    INLINE static void                  SetCurSel(HWND hCtrl, INT index) {ListBox_SetCurSel(hCtrl,index);} // *^
    INLINE static void                  SetCurSelByData(HWND hCtrl, void* data) {SendMessage(hCtrl,LB_SETCURSEL,LookupByData(hCtrl,data),0);} // *^

    // populating methods
    IMPORT static void                  PopulateWithFormList(HWND hCtrl, BSSimpleList<TESForm*>* formList); //
                                        // includes editorid, formid, and active/deleted flags
};

class TESListView
{
public:

    // manipulation 
    // *^ = may be a MS control macro rather than an inlined function
    INLINE static void                  AllowRowSelection(HWND hCtrl) {int mask = LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT; // *^
                                                                        ListView_SetExtendedListViewStyleEx(hCtrl,mask,mask);} 
    INLINE static void                  ClearColumns(HWND hCtrl) {ClearItems(hCtrl); while (ListView_DeleteColumn(hCtrl,0)) {}} // *^
    IMPORT static INT                   InsertColumn(HWND hCtrl, INT index, const char* label, INT width, UInt32 fmt = LVCFMT_LEFT);
    IMPORT static INT                   GetColumnCount(HWND hCtrl);    
    INLINE static void                  ClearItems(HWND hCtrl) {ListView_DeleteAllItems(hCtrl);} // *^
    IMPORT static INT                   InsertItem(HWND hCtrl, void* data, bool imgCallback = false, INT index = -1);
    IMPORT static INT                   InsertItem(HWND hCtrl, const char* text, INT index, INT imgIndex, void* data);    
    IMPORT static void*                 GetItemData(HWND hCtrl, INT index);
    IMPORT static void*                 GetCurSelData(HWND hCtrl); // data of first selected item
    IMPORT static INT                   GetCurSel(HWND hCtrl); // index of first selected item
    IMPORT static INT                   LookupByData(HWND hCtrl, void* data);
    IMPORT static void                  ForceSelectItem(HWND hCtrl, INT index);
    IMPORT static void                  ScrollToItem(HWND hCtrl, INT index);
    IMPORT static bool                  SetItemData(HWND hCtrl, INT index, void* data);
    IMPORT static bool                  SetItemText(HWND hCtrl, const char* text, INT index, INT colIndex);

    // populating methods
    IMPORT static void                  PopulateWithForms(HWND hCtrl, UInt8 formType); //
                                        // actually has a third parameter, always 0, that is completely unused
    IMPORT static void                  PopulateWithCells(TESWorldSpace* worldSpace, HWND hCtrl, bool clear); // if no worldspace is provided
                                        // cells from without a worldspace (i.e. interior cells) are added  
};

class TESTabControl
{
public:

    // manipulation 
    // *^ = may be a MS control macro rather than an inlined function
    IMPORT static INT                   InsertItem(HWND hCtrl, INT index, const char* label, void* data);
    IMPORT static void*                 GetItemData(HWND hCtrl, INT index);
    INLINE static void*                 GetCurSelData(HWND hCtrl) {return GetItemData(hCtrl,GetCurSel(hCtrl));}  // *^
    INLINE static INT                   GetCurSel(HWND hCtrl) {return TabCtrl_GetCurSel(hCtrl);} // *^
               
};

#endif