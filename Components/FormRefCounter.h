/*
    FormRefCounter

    The vanilla use info system is designed so that, when updating a form's reference info during CopyFrom(),
    all current references must be cleared before any new references are made (e.g. all calls to TESForm::RemoveCrossReference()
    come before any call to TESForm::AddCrossReference()).  The root of this limitation is that the use-info system
    does not track *how many times* one form references another, only that it does or does not.

    FormRefCounter is a supplementary class that can track reference changes more accurately.  
    Using it allows a CopyFrom() method to be broken down into independent parts.

    FormRefCounter does not replace the normal CS use-info system; it is just a convenient accessory 
    that makes the normal system easier to use.

    Useage: 
    -   Replace all calls to TESForm::Add/RemoveCrossReference() with calls to FormRefCounter::Add/RemoveReference()
    -   Add a call to FormRefCounter::ClearReferences() in the destructor of all form classes that use this component
    
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
    static SInt32       AddReference(TESForm* masterForm, TESForm* refForm);    // returns new count, update use-info
    static SInt32       RemoveReference(TESForm* masterForm, TESForm* refForm); // returns new count, update use-info if count == 0
    static void         ClearReferences(TESForm* masterForm);                   // clears all refs for form, does not notify use-info system
    static void         ClearAllReferences();   // clears entire ref table; does not notify use-info system

    // debugging
    static void         DumpReferences();   // dumps contents of reference table to output log
};