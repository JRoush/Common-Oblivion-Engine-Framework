/* 
    SPECIAL CREDIT: shadeMe, who originally decoded this class

    TESFormSelection (aka TESRenderControlSelection) tracks the currently selected forms/refs in the CS for drag & drop,
    both between windows and within the render window.
*/
#pragma once

#ifndef OBLIVION

// base classes
#include "API/TES/MemoryHeap.h"

// argument classes
class   Vector3;    // NiVector3?  currently defined in Utilities/ITypes.h
class   TESForm;    // TESForms/TESForm.h

class IMPORTCLASS TESFormSelection
{// size --/18
public:

    struct SelectionNode
    {// size 0C
        MEMBER /*00*/ TESForm*          form;
        MEMBER /*04*/ SelectionNode*    prev;
        MEMBER /*08*/ SelectionNode*    next;
        // use FormHeap for class new & delete
        USEFORMHEAP
    };

    // members
    MEMBER /*00*/ SelectionNode*    selections; // LL of selected forms
    MEMBER /*04*/ UInt32            selCount;   // size of LL
    MEMBER /*08*/ Vector3           refPosition;    // sum of position vectors of selected refr's, zeroed for non refs
    MEMBER /*14*/ float             refBound;       // bound radius for selected refs??

    // methods
    IMPORT void     AddSelection(TESForm* selection, bool showRefSelectionBox);
    IMPORT bool     RemoveSelection(TESForm* selection, bool clearRefSelectionBox); // returns false if selection wasn't in list
    IMPORT void     ClearSelection(bool clearRefSelectionBoxes);
    IMPORT bool     HasSelection(TESForm* selection);

    // constructor, destructor
    IMPORT TESFormSelection();
    IMPORT ~TESFormSelection();

    // use FormHeap for class new & delete
    USEFORMHEAP

    // global instances
    IMPORT static TESFormSelection*     primarySelection;
};

#endif