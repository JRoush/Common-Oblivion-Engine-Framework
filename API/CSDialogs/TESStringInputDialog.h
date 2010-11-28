/* 
    TESStringInputDialog - very simple dialog requesting a string from the user

    Note: class name is not certain
*/
#pragma once

#ifndef OBLIVION

class IMPORTCLASS TESStringInputDialog
{
public:
    
    enum DialogControlIDs 
    {
        IDC_StringInput     = 0x3E8, // EDIT
    };

    struct StringInputInfo // ptr passed as initParam when opening dialog (i.e. lParam on WM_INITDIALOG message)
    {// size 0C
        MEMBER /*00*/ const char* title; // dialog title, serves as prompt string
        MEMBER /*04*/ UInt32      bufferSize; // size of input buffer
        MEMBER /*08*/ char*       buffer; // buffer for user input string
    };

    // dialog template ID
    static const UInt32 IDD_StringInputDialog = 0x0AE;

    // methods
    IMPORT static INT_PTR CALLBACK      DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif