/* 
    TESDialogs store a BaseExtraDataList for DialogExtra*** objects in their window 'user param' (e.g. GetWindowLong(-0x15))

    TODO: A number of these objects remain to be decoded
*/
#pragma once

#ifndef OBLIVION

// base classes
#include "API/ExtraData/BSExtraData.h"
#include "API/CSDialogs/TESDialog.h" // TESDialog::FormEditParam, TESDialog::Subwindow

// argument classes
class   TESForm;

class IMPORTCLASS DialogExtraParam : public BSExtraData
{// size 0x14
/*
    For Form-editing dialogs
    Stores the form type and TESForm* of the 'source' object currently being edited by the dialog
*/
public:

    // members
    //     /*00*/ BSExtraData          
    MEMBER /*0C*/ TESDialog::FormEditParam  dialogParam;

     // virtual methods:
    INLINE /*00/00*/ virtual        ~DialogExtraParam() {}  // stub, completely inlined by CS

    // no additional virtual methods

    // constructor
    IMPORT DialogExtraParam();
};

class IMPORTCLASS DialogExtraLocalCopy : public BSExtraData
{// size 0x10
/*
    For Form-editing dialogs
    Temporary TESForm* that is directly linked to the dialog controls.  
*/
public:

    // members
    //     /*00*/ BSExtraData          
    MEMBER /*0C*/ TESForm*      localCopy;

     // virtual methods:
    IMPORT /*00/00*/ virtual        ~DialogExtraLocalCopy(); // destroys & deallocates local copy

    // no additional virtual methods

    // constructor
    IMPORT DialogExtraLocalCopy();
};

class IMPORTCLASS DialogExtraPopupMenu : public BSExtraData
{// size 0x1C
/*
    "Popup" (context) menu associated with the dialog
*/
public:

    // members
    //     /*00*/ BSExtraData          
    MEMBER /*0C*/ HMENU         menu; // parent menu resource (may contain multiple popup menus)
    MEMBER /*10*/ HMENU         popupMenu; // initialized to first submenu of parent
    MEMBER /*14*/ INT           menuTemplateID; // resource template for parent menu
    MEMBER /*18*/ INT           dialogItemID; // control ID of dialog item which was right clicked to bring up menu

     // virtual methods:
    IMPORT /*00/00*/ virtual        ~DialogExtraPopupMenu(); // destroys menu

    // no additional virtual methods

    // constructor
    IMPORT DialogExtraPopupMenu();
};

class IMPORTCLASS DialogExtraWorkingData : public BSExtraData
{// size 0x14
/*
    For dialogs editing non-TESForm objects
    Stores the 'source' object being edited and a local temporary object directly linked to the dialog controls.
    Types are 'flexible' - perhaps templated?
*/
public:

    // members
    //     /*00*/ BSExtraData          
    MEMBER /*0C*/ void*         sourceObject;
    MEMBER /*10*/ void*         localCopy;

     // virtual methods:
    INLINE /*00/00*/ virtual        ~DialogExtraWorkingData() {} // stub, completely inlined by CS

    // no additional virtual methods

    // constructor
    IMPORT DialogExtraWorkingData();
};

class IMPORTCLASS DialogExtraSubwindow : public BSExtraData
{// size 0x14
/*
    Stores information on a subwindow/child window of the dialog
*/
public:

    // members
    //     /*00*/ BSExtraData          
    MEMBER /*0C*/ TESDialog::Subwindow* subwindow;
    MEMBER /*10*/ INT                   dialogTemplateID; // dialog template ID of subwindow

     // virtual methods:
    INLINE /*00/00*/ virtual        ~DialogExtraSubwindow(); // destroys subwindow object

    // no additional virtual methods

    // constructor
    IMPORT DialogExtraSubwindow();
};

#endif