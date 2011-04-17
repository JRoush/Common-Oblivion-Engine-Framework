/* 
    'ObjectWindow' is a generic container class used to group the various global data and functions related 
    to the CS Object Window.  As such, it has only static members and methods.  It may actually exist as 
    a class in Bethesdah's source, or a namespace, etc., but it left no trace in the assembly.

    Some of this data may actually belong as static data in a more specific class.  However, it (mostly)
    appears in contiguous memory in the assembly image, which implies it was probably very close together
    in the original source.

    TODO - this code is only partially decoded; finish it as needed
*/
#pragma once

#ifndef OBLIVION

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   TESForm;    // TESForms/TESForm.h

class ObjectWindow
{
public:

    // static globals
    IMPORT static bool          isOpen;   // object window is displayed
    IMPORT static HWND          objectWindowHandle;   // object window itself
    IMPORT static HWND          listviewHandle;   // list control
    IMPORT static HWND          treeviewHandle;   // tree control
    IMPORT static HWND          seperatorHandle;  // seperator control

    // dialog proc
    static const UINT   WM_REFRESHLIST  = 0x412;    // dispatched to completely rebuild the display list from the data handler. WParam = LParam = 0
    static const UINT   WM_RESORTLIST   = 0x414;    // dispatched to resort display list. WParam = LParam = 0
    IMPORT static INT_PTR CALLBACK      DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // TreeEntryInfo (aka "FormTypeInfo")
    struct IMPORTCLASS TreeEntryInfo
    {// size 14
    public:
        
        typedef BSSimpleList<TESForm*> FormListT;

        // members
        MEMBER /*00*/ UInt8         formType;           // form type for this tree entry
        MEMBER /*01*/ UInt8         pad01[3];
        MEMBER /*04*/ UInt32        columnCount;        // number of columns in listview
        MEMBER /*08*/ UInt32        selectedIndex;      // index of currently selected item in listview (??)
        MEMBER /*0C*/ FormListT     formList;

        // use formheap for class new, delete
        USEFORMHEAP
    };
    static const UInt32 kTreeEntry__Count = 0x24;   // size of static tree entry arrays

};

#endif