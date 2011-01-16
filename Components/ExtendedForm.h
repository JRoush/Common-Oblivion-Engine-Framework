/*
    Extended Forms Manager

    To register a custom form type:

    -   Define the form class, deriving from TESForm or one of it's descendents.  Multiple inheritance
        to include other BaseFormComponents is allowed.  Override, at a minimum, the following virtual
        methods:
        -   LoadForm
        -   SaveFormChunks
        -   CopyFrom
        -   CompareTo
        -   GetFormType
        -   {ctor}
        See the form definitions in the Advanced COEF Example project for details on each necessary override

    -   Define an instance of ExtendedForm (defined below), and initialize it's members.

    -   Call your ExtendedForm's Register() method.  It is recommended that you provide
        a unique, meaningful 4-character string as a short name.
*/
#pragma once
#ifndef COMPONENT_EXPORT
    #define COMPONENT_EXPORT _declspec(dllimport)
#endif
#pragma warning (disable: 4251) // warning about BSSimpleListT not being importable (it's not imported, don't worry)

// base classes
#include "API/BSTypes/BSSimpleList.h"   

// argument classes
class   TESForm;    // TESForms/TESForm.h

class COMPONENT_EXPORT ExtendedForm
{
public:

    typedef BSSimpleList<TESForm*> FormListT;

    // members
    const char*     pluginName; // unique name of the plugin that is registering this form type
    const char*     className; // a name that uniquely identifies this form type *within the plugin*
    const char*     displayName; // the name that is presented for the form type in, e.g., Dialog captions
    TESForm*        (*CreateForm)(); // pointer-to-function that allocates & initializes a new instance of the form type
    void            (*CreateDefaultForms)(); // pointer-to-function that creates default instances of this form type 
    void            (*ClearForms)(); // pointer-to-function called after form list has been reset      

    // methods
    bool            Register(const char* shortName = 0, UInt8 formType = 0); // register this form type 
                    // shortName - must be exactly 4 characters.  If provided, used as the record type during serialization.
                    // formType - requests this formtype, if it is available.  if a vanilla formtype is requested, the
                    // vanilla shortname is used and any value passed for the shortname is ignored.
    bool            Unregister(); // unregister this form type (destroys any forms of this type stored in the formlist)
    UInt8           FormType(); // the assigned formtype
    const char*     ShortName(); // the assigned shortname
    FormListT&      FormList(); // a list of forms of this type currently in the extended data handler

    // constructor, destructor
    ExtendedForm(const char* nPluginName, const char* nClassName, const char* nDisplayName = 0, TESForm* (*nCreateForm)() = 0);
    ~ExtendedForm();

private: // for internal use only
    char        _shortName[5];
    UInt8       _formType;
    FormListT   _formList;
};