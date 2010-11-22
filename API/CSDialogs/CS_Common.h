/* 
    Various utilities & debugging code specific to the CS 
*/
#pragma once

namespace RSH {

// get window info for editor
HWND            CS_hWnd();          // retrieve HWND of main CS window
HINSTANCE       CS_hInstance();     // retrieve HINSTANCE for the CS

// input box for new editor id's
int             GetInputString(const char* title, char* buffer, UInt32 bufferlength);

// return a (more) human readable summary of a windows message
const char*     wmMsgDesc(UINT message, WPARAM wParam, LPARAM lParam);

// combo & list control utilities
extern const char*     NULL_ENTRY_STR;
void            PopulateComboWithForms(HWND hCtrl, UInt8 formType, bool clearFirst, bool NullEntry);
void            PopulateComboWithScripts(HWND hCtrl, bool nullEntry, bool OnlyQuestScripts, bool OnlyEffectScripts);
void            PopulateComboWithAVs(HWND hCtrl, bool clearFirst, bool NullEntry);
int             ComboLookupByData(HWND hCtrl, UInt32 itemData);
int             CList_Lookup(CListBox* list, UInt32 itemData);
int             CCombo_Lookup(CComboBox* list, UInt32 itemData);

// DataFile dialog
bool            ChooseIconFile(HWND parent, char* buffer, UInt32 buffersize);               // for choosing icon files
bool            ChooseModelFile(HWND parent, char* buffer, UInt32 buffersize);              // for choosing model files

}   //  end namespace RSH