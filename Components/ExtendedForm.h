/*
    Extended Forms Manager
    
    TESForm, as discussed in API/TESForms/TESForm.h, is a standard interface for (1) version control using formIDs, 
    (2) serialization to/from mod files, and (3) editing with dialogs in the CS.  Any new form class needs to 
    implement these basic features.  Fortunately, individual BaseFormComponent classes handle these tasks for simple
    common properties (e.g. TESFullName handles version control, serialization, and dialog editing for a name string).
    A form class can inherit from existing BaseFormComponents and gain the associated property (e.g. a name) without
    having to write much additional code.  Note that TESForm itself is a BaseFormComponent - it's 'properties' are a
    formID and an editorID, meaning that TESForm itself contains the necessary code for serializing, editing, etc. 
    those.

    New form classes use two limited resources, which can potentially cause conflicts:

    -   Form Type Code: 
        The game engine restricts form types to values between 0x00-0xFF, of which the values 0x01-0x44 are already 
        allocated to vanilla form types and 0x00 & 0x45 are special error values.  This leaves 186 available form 
        type codes, and this is the maximum number of new form classes that can be registered.  It is unlikely that
        this maximum will ever be reached in a reasonable setup, but the potential is there.
        When a new form class is registered, one of the remaining form type codes is allocated to it.  Which code may
        vary between sessions, so DO NOT assume that it will always be the same.  If no form type codes remain, the
        registration will fail.

    -   Form Short Name (aka Record Type or Group Type):
        During serialization (saving/loading), form records are marked with a 4-character string that uniquely identifies
        the form type (e.g. 'MGEF' for EffectSettings or 'SPEL' for spells).  There are many more potential short names
        available than form type codes, but unlike the type codes, short names must be unique among *all* new form classes
        introduced by *all* obse plugins.  If two new form classes use the same short name, then there will be no way for
        the CS or any third party programs to tell which class created the record in a given file.
        This may be addressed, even fixed entirely, in future versions of this library.  Such a fix would use the PluginName
        and ClassName properties to identify form records instead of the short name - much more reliable, so long as no two
        OBSE plugins share the same name.  Therefore it is required for future compatibility that all new form classes 
        be given appropriate plugin & class names in addition to a (hopefully) unique short name.

    
    To register a custom form class:

    1.  Define the form class, deriving from TESForm or one of it's descendents.  Multiple inheritance
        to include other BaseFormComponents is allowed.  Override, at a minimum, the following virtual
        methods:
        -   LoadForm
        -   SaveFormChunks
        -   CopyFrom
        -   CompareTo
        -   GetFormType
        -   {ctor}
        See the form definitions in the Advanced COEF Example project for details on each necessary override

    2.  Define an instance of ExtendedForm (defined below), and initialize it's members.

    3.  Call your ExtendedForm's Register() method.  It is recommended that you provide a unique, meaningful 
        4-character string as a short name (see notes above).
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
    bool            Register(const char* shortName = 0, UInt8 formType = 0); // register this form type.  Return false on failure.
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