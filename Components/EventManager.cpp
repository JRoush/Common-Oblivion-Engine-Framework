#include "EventManager.h"

#include "Utilities/Memaddr.h"
#include "API/TESForms/TESForm.h"

#include <vector>

//----------------------------- Event Interface -------------------------------------
// an abstract interface for event objects
struct Event
{
    // members
    std::vector<void*>  callbacks;
    // virtual methods
    virtual const char*     Name() = 0;     // event name, for debugging output
    virtual void            Attach() = 0;   // write hooks/patches to implement event callbacks
    virtual void            Detach() = 0;   // undo hooks/patches to restore original code
};

//----------------------------- Individual Events -------------------------------------
namespace DataHandler_CreateDefaults
{
    // Patch addresses
    memaddr Hook        (0x0044CC6F,0x00480FC6);    // just before creation of formid 0x15F
    memaddr RetnA       (0x0044CC74,0x00480FCB);
    // global objects
    UInt8   overwrittenData[0x5] = {{0}};   // buffer for storing original contents of patch address
    // event object
    struct EventT : public Event
    {        
        // static methods
        static void Hndl();
        // virtual interface
        virtual const char* Name() {return "DataHandler_CreateDefaults";}
        virtual void Attach() 
        {
            _MESSAGE("Attached Event");
            memcpy(overwrittenData,Hook,sizeof(overwrittenData));
            Hook.WriteRelJump(&Hndl);
        }
        virtual void Detach() 
        {
            _MESSAGE("Detached Event");
            Hook.WriteDataBuf(overwrittenData,sizeof(overwrittenData));
        }
    } eventT;      
    // handler
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
        }
        _DMESSAGE("");
        for (int i = 0; i < eventT.callbacks.size(); i++)
        {
            ((EventManager::DataHandler_CreateDefaults_f)eventT.callbacks[i])();
        }
        __asm
        {  
            // epilog
            mov     esp, ebp
            popad
            // overwritten code
            push    0x15F
            jmp     [RetnA._addr]
        }
    }
}
Event& EventManager::DataHandler_CreateDefaults = DataHandler_CreateDefaults::eventT;

namespace DataHandler_PostCreateDefaults
{
    // Patch addresses
    memaddr Hook        (0x0044CD71,0x00481067);    // very end of Creation method, on final retn instruction
    // global objects
    UInt8   overwrittenData[0x5] = {{0}};   // buffer for storing original contents of patch address
    // event object
    struct EventT : public Event
    {        
        // static methods
        static void Hndl();
        // virtual interface
        virtual const char* Name() {return "DataHandler_PostCreateDefaults";}
        virtual void Attach() 
        {
            _MESSAGE("Attached Event");
            memcpy(overwrittenData,Hook,sizeof(overwrittenData));
            Hook.WriteRelJump(&Hndl);
        }
        virtual void Detach() 
        {
            _MESSAGE("Detached Event");
            Hook.WriteDataBuf(overwrittenData,sizeof(overwrittenData));
        }
    } eventT;      
    // handler
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
        }
        _DMESSAGE("");
        for (int i = 0; i < eventT.callbacks.size(); i++)
        {
            ((EventManager::DataHandler_PostCreateDefaults_f)eventT.callbacks[i])();
        }
        __asm
        {  
            // epilog
            mov     esp, ebp
            popad
            // overwritten code
            retn
        }
    }
}
Event& EventManager::DataHandler_PostCreateDefaults = DataHandler_PostCreateDefaults::eventT;

namespace DataHandler_Clear
{
    // Patch addresses
    memaddr Hook                (0x00449A98,0x0047B0B2);    // just before leak checking on formid map
    memaddr RetnA               (0x00449A9E,0x0047B0B8);
    // global objects
    UInt8   overwrittenData[0x6] = {{0}};   // buffer for storing original contents of patch address
    // event object
    struct EventT : public Event
    {        
        // static methods
        static void Hndl();
        // virtual interface
        virtual const char* Name() {return "DataHandler_Clear";}
        virtual void Attach() 
        {
            _MESSAGE("Attached Event");
            memcpy(overwrittenData,Hook,sizeof(overwrittenData));
            Hook.WriteRelJump(&Hndl);
        }
        virtual void Detach() 
        {
            _MESSAGE("Detached Event");
            Hook.WriteDataBuf(overwrittenData,sizeof(overwrittenData));
        }
    } eventT;      
    // handler
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
        }
        _DMESSAGE("");
        for (int i = 0; i < eventT.callbacks.size(); i++)
        {
            ((EventManager::DataHandler_Clear_f)eventT.callbacks[i])();
        }  
        __asm
        {
            // epilog
            mov     esp, ebp
            popad
            // overwritten code
            mov     ecx, TESForm::formIDMap
            mov     ecx, [ecx + 0x4]    // extract formIDMap.bucketcount
            jmp     [RetnA._addr]
        }
    }
}
Event& EventManager::DataHandler_Clear = DataHandler_Clear::eventT;

#ifndef OBLIVION

namespace CSMainWindow_WMCommand
{
    // Patch addresses
    memaddr Hook        (0x0       ,0x00419568);    // just before source control switch
    memaddr RetnA       (0x0       ,0x0041956D);    // normal execution path for unhandled event
    memaddr RetnB       (0x0       ,0x0041A94C);    // terminal execution path for handled event
    // global objects
    UInt8   overwrittenData[0x5] = {{0}};   // buffer for storing original contents of patch address
    // event object
    struct EventT : public Event
    {        
        // static methods
        static void Hndl();
        // virtual interface
        virtual const char* Name() {return "CSMainWindow_WMCommand";}
        virtual void Attach() 
        {
            _MESSAGE("Attached Event");
            memcpy(overwrittenData,Hook,sizeof(overwrittenData));
            Hook.WriteRelJump(&Hndl);
        }
        virtual void Detach() 
        {
            _MESSAGE("Detached Event");
            Hook.WriteDataBuf(overwrittenData,sizeof(overwrittenData));
        }
    } eventT;      
    // handler
    void _declspec(naked) EventT::Hndl()
    {
        WPARAM  wparam;
        LPARAM  lparam;
        bool    handled;
        __asm
        {
            // prolog
            pushad
            mov     eax, [ebp + 0xC44]  // fetch complete wparam
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
            mov     wparam, eax
            mov     lparam, edi
        }
        handled = false;
        for (int i = 0; i < eventT.callbacks.size(); i++)
        {
            if (((EventManager::CSMainWindow_WMCommand_f)eventT.callbacks[i])(wparam,lparam) == 0)
            {
                _VMESSAGE("Event '%s' (%p,%p) handled by <%p>",eventT.Name(),wparam,lparam,eventT.callbacks[i]);
                handled = true;
            }
        }
        if (handled) // event was handled, return out of message pump
        {
            __asm
            {
                // epilog
                mov     esp, ebp
                popad
                // overwritten code
                xor     eax, eax
                jmp     [RetnB._addr]
            }
        }
        _VMESSAGE("Event '%s' (%p,%p) not handled",eventT.Name(),wparam,lparam);
        __asm   // event was not handled, continue normal execution
        {
            // epilog
            mov     esp, ebp
            popad
            // overwritten code
            cmp     eax, 0x9C43
            jmp     [RetnA._addr]
        }
    }
}
Event& EventManager::CSMainWindow_WMCommand = CSMainWindow_WMCommand::eventT;

namespace CSObjectWindow_CompareObject
{
    // Patch addresses
    memaddr Hook        (0x0       ,0x00415D1D);    // just before source control switch
    memaddr RetnA       (0x0       ,0x00415D27);    // normal execution path
    memaddr RetnB       (0x0       ,0x00416A48);    // terminal execution path, if valid result returned
    // global objects
    UInt8   overwrittenData[0xB] = {{0}};   // buffer for storing original contents of patch address
    // event object
    struct EventT : public Event
    {        
        // static methods
        static void Hndl();
        // virtual interface
        virtual const char* Name() {return "CSObjectWindow_CompareObject";}
        virtual void Attach() 
        {
            _MESSAGE("Attached Event");
            memcpy(overwrittenData,Hook,sizeof(overwrittenData));
            Hook.WriteRelJump(&Hndl);
        }
        virtual void Detach() 
        {
            _MESSAGE("Detached Event");
            Hook.WriteDataBuf(overwrittenData,sizeof(overwrittenData));
        }
    } eventT;      
    // handler
    void _declspec(naked) EventT::Hndl()
    {
        TESForm*    formA;
        TESForm*    formB;
        UInt32      colIndx;
        int*        result;
        bool        handled;
        __asm
        {
            // prolog
            pushad
            lea     ecx, [ebp - 0x10]  // fetch pointer to result value on stack
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
            mov     formA,esi
            mov     formB,edi
            mov     colIndx,eax
            mov     result,ecx
        }
        handled = false;
        for (int i = 0; i < eventT.callbacks.size(); i++)
        {
            if (((EventManager::CSObjectWindow_CompareObject_f)eventT.callbacks[i])(formA,formB,colIndx,*result))
            {
                _VMESSAGE("Event '%s' (%p,%p,%i) handled by <%p>",eventT.Name(),formA,formB,colIndx,eventT.callbacks[i]);
                handled = true;
            }
        }
        if (handled) // event was handled, jump to end of comparitor
        {
            __asm
            {
                // epilog
                mov     esp, ebp
                popad
                // overwritten code
                jmp     [RetnB._addr]
            }
        }
        _VMESSAGE("Event '%s' (%p,%p,%i) not handled",eventT.Name(),formA,formB,colIndx);
        *result = 1;
        __asm   // event was not handled, continue normal execution
        {
            // epilog
            mov     esp, ebp
            popad
            // overwritten code
            cmp     eax, 0x3E
            jmp     [RetnA._addr]
        }
    }
}
Event& EventManager::CSObjectWindow_CompareObject = CSObjectWindow_CompareObject::eventT;

namespace CSObjectWindow_GetObjectDispInfo
{
    // Patch addresses
    memaddr Hook        (0x0       ,0x00414E9C);    // just before source control switch
    memaddr RetnA       (0x0       ,0x00414EA1);    // normal execution path
    memaddr RetnB       (0x0       ,0x00415B45);    // terminal execution path, if valid result returned
    // global objects
    UInt8   overwrittenData[0x5] = {{0}};   // buffer for storing original contents of patch address
    // event object
    struct EventT : public Event
    {        
        // static methods
        static void Hndl();
        // virtual interface
        virtual const char* Name() {return "CSObjectWindow_GetObjectDispInfo";}
        virtual void Attach() 
        {
            _MESSAGE("Attached Event");
            memcpy(overwrittenData,Hook,sizeof(overwrittenData));
            Hook.WriteRelJump(&Hndl);
        }
        virtual void Detach() 
        {
            _MESSAGE("Detached Event");
            Hook.WriteDataBuf(overwrittenData,sizeof(overwrittenData));
        }
    } eventT;      
    // handler
    void _declspec(naked) EventT::Hndl()
    {
        void*       displayInfo;
        bool        handled;
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
            mov     displayInfo,esi
        }
        handled = false;
        for (int i = 0; i < eventT.callbacks.size(); i++)
        {
            if (((EventManager::CSObjectWindow_GetObjectDispInfo_f)eventT.callbacks[i])(displayInfo))
            {
                _VMESSAGE("Event '%s' (%p) handled by <%p>",eventT.Name(),displayInfo,eventT.callbacks[i]);
                handled = true;
            }
        }
        if (handled) // event was handled, jump to end of comparitor
        {
            __asm
            {
                // epilog
                mov     esp, ebp
                popad
                // overwritten code
                jmp     [RetnB._addr]
            }
        }
        _VMESSAGE("Event '%s' (%p) not handled",eventT.Name(),displayInfo);
        __asm   // event was not handled, continue normal execution
        {
            // epilog
            mov     esp, ebp
            popad
            // overwritten code
            mov     eax, [esi + 0x14]
            mov     edx, [esi]
            jmp     [RetnA._addr]
        }
    }
}
Event& EventManager::CSObjectWindow_GetObjectDispInfo = CSObjectWindow_GetObjectDispInfo::eventT;

#endif
//----------------------------- Event Manager -------------------------------------
bool EventManager::RegisterEventCallback(Event& eventT, void* callback)
{
    // validate callback
    if (!callback)
    {
        _ERROR("Register callback for event '%s' failed: null callback pointer",eventT.Name());
        return false;
    }

    // register callback
    if (eventT.callbacks.empty()) eventT.Attach();  // attach event before adding first callback
    eventT.callbacks.push_back(callback);
    _DMESSAGE("Registered callback <%p> for event '%s'",callback,eventT.Name());

    return true;
}
bool EventManager::UnregisterEventCallback(Event& eventT, void* callback)
{
    // find callback in list
    for (std::vector<void*>::iterator it = eventT.callbacks.begin(); it != eventT.callbacks.end(); it++)
    {
        if (*it != callback) continue;
        // callback found
        eventT.callbacks.erase(it);
        if (eventT.callbacks.empty()) eventT.Detach(); // detach event after removing last callback
        _DMESSAGE("Unegistered callback <%p> for event '%s'",callback,eventT.Name());
        return true;
    }
    // callback not found in list
    return false;
}