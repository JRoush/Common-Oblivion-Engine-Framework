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
struct Event;

class COMPONENT_EXPORT EventManager
{
public:   
        
    static Event&   DataHandler_CreateDefaults;
    typedef void (*DataHandler_CreateDefaults_f)();
    // Occurs when the DataHandler creates & initializes the default (or "builtin") objects
    // like BlackSoulGem, RepairHammer, the Player character, etc.  Useful for initializing
    // similar objects added by a plugin.

    static Event&   DataHandler_PostCreateDefaults;
    typedef void (*DataHandler_PostCreateDefaults_f)();
    // Occurs after the DataHandler has created & initialized all default (or "builtin") objects.
    // Useful for 'linking' references between default objects.

    static Event&   DataHandler_Clear;
    typedef void (*DataHandler_Clear_f)();
    // Occurs when the DataHandler is cleared, both when the game/CS is closed and when a
    // new set of files is selected (CS only).  Useful for cleaning up any objects managed 
    // by a plugin.

    #ifndef OBLIVION

    static Event&   CSMainWindow_WMCommand;
    typedef LRESULT (*CSMainWindow_WMCommand_f)(WPARAM wparam, LPARAM lparam);
    // Intercepts WM_COMMAND messages sent to main CS window
    // These primarily occur when the user clicks on a menu or toolbar item
    // See MS documentation on WM_COMMAND message for meaningof wparam & lparam
    // Callback should return zero if message was handled (prevents CS from handling
    // the message itself).

    #endif
    
    // methods
    static bool     RegisterEventCallback(Event& eventT, void* callback);
    static bool     UnregisterEventCallback(Event& eventT, void* callback);
};