/*
    Event Manager

    To register an event callback:

    -   Define a callback function with the appropriate signature.  The signatures are
        given by the typedef statements acompanying the Event declarations below.

    -   Call EventManager::RegisterEventCallback(), passing the event object and the
        callback function.  
        
    The hook for each event is set when the first callback is registered for it, and unset 
    when the last callback is unregistered.  If no callbacks are registered for an Event,
    then the hook for it is never set.
*/
#pragma once
#ifndef COMPONENT_EXPORT
    #define COMPONENT_EXPORT _declspec(dllimport) 
#endif

// argument classes
class   TESForm;    // TESForms/TESForm.h

class COMPONENT_EXPORT EventManager
{
public:   
    
    // Event interface class
    class COMPONENT_EXPORT Event
    {
    public:

        // interface methods
        virtual const char*     Name() = 0;     // event name, for debugging output
        virtual bool            RegisterCallback(void* callback, SInt32 priority = 0) = 0;  // register a callback function.
        virtual bool            UnregisterCallback(void* callback) = 0; // remove all a callback function
    };

    // Events for the global DataHandler
    struct COMPONENT_EXPORT DataHandler
    {        
        static Event&   CreateDefaults;
        typedef void (*CreateDefaults_f)();
        // Occurs when the DataHandler creates & initializes the default (or "builtin") objects
        // like BlackSoulGem, RepairHammer, the Player character, etc.  Useful for initializing
        // similar objects added by a plugin.

        static Event&   PostCreateDefaults;
        typedef void (*PostCreateDefaults_f)();
        // Occurs after the DataHandler has created & initialized all default (or "builtin") objects.
        // Useful for 'linking' references between default objects.

        static Event&   Clear;
        typedef void (*Clear_f)();
        // Occurs when the DataHandler is cleared, both when the game/CS is closed and when a
        // new set of files is selected (CS only).  Useful for cleaning up any objects managed by a plugin.

        static Event&   AddForm_;
        typedef bool (*AddForm_f)(TESForm* form);
        // Occurs just before a TESForm is added to the DataHandler. 
        // return true to prevent the form from being added
    };

    #ifndef OBLIVION
    struct COMPONENT_EXPORT CSWindows
    {

        static Event&   LoadMenuA;
        typedef HMENU (*LoadMenuA_f)(HINSTANCE hInstance, LPCSTR lpMenuName);
        // Intercepts all calls to User32::LoadMenu
        // if no callbacks return a valid HMENU, the original LoadMenu() will be invoked to get the default menu

        static Event&   CreateDialogParamA;
        typedef HWND (*CreateDialogParamA_f)(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
        // Intercepts all calls to User32::CreateDialogParam
        // if no callbacks return a valid HWND, the original CreateDialogParam() will be invoked to get the default dialog

        static Event&   MainW_WMCommand;
        typedef LRESULT (*MainW_WMCommand_f)(WPARAM wparam, LPARAM lparam);
        // Intercepts WM_COMMAND messages sent to main CS window
        // These primarily occur when the user clicks on a menu or toolbar item
        // See MS documentation on WM_COMMAND message for meaningof wparam & lparam
        // return zero if message was handled (prevents CS from handling the message itself).

        static Event&   ObjectW_CompareObject;
        typedef bool (*ObjectW_CompareObject_f)(TESForm* formA, TESForm* formB, UInt32 columnID, int& result);   
        // Intercepts calls to the Object Window sorting method, used to order objects in the window
        // result should be positive if formA > formB, negative if formA < formB, and 0 if formA == formB
        // return true to use result value, return false to use default sort comparision

        static Event&   ObjectW_GetObjectDispInfo;
        typedef bool (*ObjectW_GetObjectDispInfo_f)(void* displayInfo);   
        // Intercepts calls to the Object Window display info method, used to determine contents of object listview grid
        // displayInfo is a NMLVDISPINFO*, defined in <CommCtrl.h>
        // displayInfo->item.iSubItem is the Object Window column ID
        // displayInfo->item.lParam is the TESForm*
        // return false to use default display value
        
        static Event&   InitializeWindows;
        typedef void (*InitializeWindows_f)();
        // Occurs during startup, after the CS has finished initializing the MDI container window but *before* the 
        // CS message pump starts.  Useful for making modifications to the main window or menus.

    };
    #endif
};