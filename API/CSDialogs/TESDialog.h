/* 
    TESDialog is a 'container' classes for dealing with CS interface dialogs.
    The static methods and data here may actually belong in a number of other classes, but they are grouped together 
    in the exectuable images, and apparently had their own file in the BS repository ("TESDialog.cpp").
    At the moment, these classes are defined for convenience, to group code related to the CS interface in one place.

    TESDialogs store a BaseExtraDataList for DialogExtra*** objects in their window 'user param' (e.g. GetWindowLong(-0x15))
    See CSDialogs/DialogExtraData.h for definitions.

    There seems to be a strong distinction between dialogs that edit forms and those that edit simpler component objects or 
    provide services like import/export or text searching.  Form-editing dialogs have their own 
*/
#pragma once

#ifndef OBLIVION

// base classes
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   TESForm;        // TESForms/TESForm.h
class   BSExtraData;    // ExtraData/BSExtraData.h
class   BaseExtraList;  // ExtraData/ExtraDataList.h
class   DialogExtraPopupMenu;   // CSDialogs/DialogExtraData.h

class TESDialog
{
public:

    // FormEditParam - passed as initParam to CreateDialogParam() (i.e. lParam on WM_INITDIALOG message) for form-editing dialogs
    struct FormEditParam
    {// size 0C
        MEMBER /*00*/ UInt8     formType; // see TESForm::FormTypes
        MEMBER /*01*/ UInt8     pad01[3]; 
        MEMBER /*04*/ TESForm*  form; // form object to be edited by the dialog
    };

    // methods - Form-editing dialogs, for forms accessed through the Object Window.  
    // Forms accessed through the main menus are derived from TESFormIDListView, and use that dialog proc instead.
    IMPORT static INT_PTR CALLBACK      FormEditDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); // 
    IMPORT static UInt32                GetFormEditDlgTemplateID(UInt8 formType); // returns dialog template ID (e.g. IDD_SpellItem) by form type    
    IMPORT static HWND                  GetFormEditHandle(TESForm* form); // returns handle to dialog for editing form, if any are open

    // methods - Form-editing dialogs (all)
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

    // static data 
    IMPORT static BSSimpleList<HWND>    openDialogs; // LL of handles for open multiple-instance dialogs, 
                                        // used to ensure same object isn't edited in more than one window
};

#endif