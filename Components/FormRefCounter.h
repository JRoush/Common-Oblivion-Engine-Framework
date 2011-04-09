/*
    FormRefCounter

    The vanilla use info system is designed so that, when updating a form's reference info during CopyFrom(),
    all current references must be cleared before any new references are made (e.g. all calls to TESForm::RemoveCrossReference()
    come before any call to TESForm::AddCrossReference()).  The root of this limitation is that the use-info system
    does not track *how many times* one form references another, only that it does or does not.

    FormRefCounter is a supplementary class that can track reference changes more accurately.  Using it allows a CopyFrom() method 
    to be broken down into independent parts.

    FormRefCounter makes *no changes* to existing game or CS code; it is just a convenient accessory that makes the existing
    system easier to use.

    Useage:
    1.  Call FormRefCounter::ClearRefences() to initialize the global data structure
    2.  For each form member that points to another form:
        -   call TESForm::RemoveCrossReference() on the old value, 
            immediately clearing the relevanty use info for that form
        -   call FormRefCounter::AddReference() on the new value, 
            storing the ref for later use
    3.  After all pointer members have been updated, call FormRefCounter::CommitReferences() 
        to move changes to the regular use info system.

    NOTE: to simplify code that uses it, the interface for this class is defined in both the game and CS.
           If called in game mode, the methods will have absolutely no effect.
*/
#pragma once
#ifndef COMPONENT_EXPORT
    #define COMPONENT_EXPORT _declspec(dllimport) 
#endif

// argument classes
class   TESForm;    // TESForms/TESForm.h

class COMPONENT_EXPORT FormRefCounter
{
public:

    // reference management
    static SInt32       AddReference(TESForm* masterForm, TESForm* refForm); // returns new count
    static SInt32       RemoveReference(TESForm* masterForm, TESForm* refForm); // returns new count
    static void         ClearRefences();    // clears all tracked changes
    static void         CommitReferences(); // commit all changes to CS use info system and clear table
};