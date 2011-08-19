#include "EventManager.h"

#include "Utilities/Memaddr.h"
#include "API/TESForms/TESForm.h"

#include <list>

//----------------------------- Event Interface -------------------------------------
struct Callback
{
    void*   pFunc;
    SInt32  priority;

    Callback(void* pFunc, SInt32 priority) : pFunc(pFunc), priority(priority) {}
    bool operator < (const Callback& rhs) const { return priority < rhs.priority; }
};

class EventEx : public EventManager::Event
{
public:  
    
    typedef std::list<Callback> CallbackListT;

    // constructor, destructor
    EventEx(memaddr patchOffset, UInt8 patchSize, const char* name) : hook(patchOffset), patchSize(patchSize), overwrittenData(0), name(name) {}
    virtual ~EventEx() { if (overwrittenData) delete overwrittenData; }    

    // virtual methods
    virtual const char*     Name()
    {
        return name;
    }
    virtual bool            RegisterCallback(void* callback, SInt32 priority = 0)
    {
        // validate callback
        if (!callback)
        {
            _ERROR("Register callback for event '%s' failed: null callback pointer",Name());
            return false;
        }

        // determine insert position in list
        CallbackListT::iterator insertPos = callbacks.end(); 
        for(CallbackListT::iterator pos = callbacks.begin(); pos != callbacks.end(); pos++)
        {
            if (pos->pFunc == callback) 
            {
                _ERROR("Register callback for event '%s' failed: callback already registered w/ priority %i",Name(),pos->priority);
                return false; // callback already present in list
            }
            if (insertPos == callbacks.end() && priority < pos->priority) insertPos = pos; // insert before first callback with greater value
        }

        // register callback
        if (callbacks.empty()) Attach();  // attach event before adding first callback
        callbacks.insert(insertPos,Callback(callback,priority));
        _DMESSAGE("Registered callback <%p> w/ priority %i for event '%s'",callback,priority,Name());

        return true;
    }
    virtual bool            UnregisterCallback(void* callback)
    {
        // find callback in list
        bool found = false;
        for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end(); )
        {
            if (it->pFunc == callback)
            {
                CallbackListT::iterator toDelete = it;
                it++;
                _DMESSAGE("Unregistered callback <%p> w/ priority %i for event '%s'",callback,toDelete->priority,Name());
                callbacks.erase(toDelete);
                found = true;
            }
            else it++;
        }        
        if (callbacks.empty()) Detach(); // detach event after removing last callback
        return found;
    }
    virtual void            Attach() 
    {
        _MESSAGE("Attached Event %s", Name());
        if (patchSize && (overwrittenData == 0)) 
        {
            overwrittenData = new UInt8[patchSize];
            memcpy(overwrittenData,hook,patchSize);
        }
        // NOTE: Each subclass must override this method, call this base method, then perform the actual patch operation
    }
    virtual void            Detach() 
    {
        _MESSAGE("Detached Event %s", Name());
        if (patchSize && overwrittenData) hook.WriteDataBuf(overwrittenData,patchSize);
    }
       
    // members
    CallbackListT   callbacks;
    memaddr         hook;
    UInt8           patchSize;
    void*           overwrittenData;
    const char*     name;

    // wrappers to avoid issues with possible exceptions in naked handler functions
    CallbackListT::iterator begin() throw ()
    {
        try { return callbacks.begin(); }
        catch (...) { _ERROR(""); }
    } 
    CallbackListT::iterator end()   
    {
        try { return callbacks.end(); }
        catch (...) { _ERROR(""); }
    }

};

//----------------------------- DataHandler Events -------------------------------------
namespace _DataHandler_
{

namespace CreateDefaults
{
    // Patch data
    memaddr Hook        (0x0044CC6F,0x00480FC6);    // just before creation of formid 0x15F
    memaddr RetnA       (0x0044CC74,0x00480FCB);
    UInt8 patchLen      = 5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"DataHandler.CreateDefaults") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }
        // event handler
        static void Hndl();
        void EventT::Call()
        {
            _DMESSAGE("");
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::DataHandler::CreateDefaults_f func = (EventManager::DataHandler::CreateDefaults_f)it->pFunc;
                it++;
                func();
            }
        }
    } eventT; 

    // hook handler   
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
        }
        eventT.Call();
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

namespace PostCreateDefaults
{
    // Patch data
    memaddr Hook        (0x0044CD71,0x00481067);    // very end of Creation method, on final retn instruction
    UInt8 patchLen      = 5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"DataHandler.PostCreateDefaults") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }
        // event handler
        static void Hndl();
        void EventT::Call()
        {
            _DMESSAGE("");
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::DataHandler::PostCreateDefaults_f func = (EventManager::DataHandler::PostCreateDefaults_f)it->pFunc;
                it++;
                func();
            }
        }
    } eventT; 

    // hook handler   
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
        }
        eventT.Call();
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

namespace Clear
{
    // Patch data
    memaddr Hook                (0x00449A98,0x0047B0B2);    // just before leak checking on formid map
    memaddr RetnA               (0x00449A9E,0x0047B0B8);
    UInt8 patchLen              = 5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"DataHandler.Clear") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }
        // event handler
        static void Hndl();
        void EventT::Call()
        {
            _DMESSAGE("");
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::DataHandler::Clear_f func = (EventManager::DataHandler::Clear_f)it->pFunc;
                it++;
                func();
            }
        }
    } eventT;  

    // hook handler
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
        }
        eventT.Call(); 
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

namespace AddForm_
{
    // Patch data
    memaddr Hook                (0x0044D955,0x004818F5);    // just before leak checking on formid map
    memaddr RetnA               (0x0044D95A,0x004818FA);
    UInt8 patchLen              = 5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"DataHandler.AddForm") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }

        // event handler
        static void Hndl();
        bool Call(TESForm* form)
        {
            _VMESSAGE("Form %p <memaddr=%p>",form ? form->formID : 0, form);
            bool handled = false;
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::DataHandler::AddForm_f func = (EventManager::DataHandler::AddForm_f)it->pFunc;
                it++;
                if (func(form))
                {
                    _DMESSAGE("Form %p <memaddr=%p> was barred from the DataHandler by callback <%p>",form ? form->formID : 0, form,func);
                    handled = true;
                }
            }
            return handled;
        }
    } eventT;  

    // handler
    void _declspec(naked) EventT::Hndl()
    {
        __asm
        {
            // overwritten code
            push    edi 
            mov     edi, ecx
            // check for a valid form*
            test    esi,esi 
            jz      DONE 
            // call event handlers
            push    esi     // push form argument
            mov     ecx, offset eventT // set this* pointer
            call    EventT::Call
            test    al,al
            setz    al
            test    al,al   // check if event was not overridden
            // return
            DONE:
            jmp     [RetnA._addr]
        }
    }
}

} // end of namespace _DataHandler_
EventManager::Event& EventManager::DataHandler::CreateDefaults = _DataHandler_::CreateDefaults::eventT;
EventManager::Event& EventManager::DataHandler::PostCreateDefaults = _DataHandler_::PostCreateDefaults::eventT;
EventManager::Event& EventManager::DataHandler::Clear = _DataHandler_::Clear::eventT;
EventManager::Event& EventManager::DataHandler::AddForm_ = _DataHandler_::AddForm_::eventT;

//----------------------------- CS Window Events -------------------------------------
#ifndef OBLIVION
namespace _CSWindows_
{

namespace LoadMenu_
{
    // Patch data
    memaddr Patch       (0x0       ,0x009244E8);    // entry in User32.dll import table
    UInt8 patchLen      = 4;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Patch,patchLen ,"CSWindows.LoadMenu") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Patch.WriteData32((UInt32)memaddr::GetPointerToMember(&EventT::Call));
        }
        // event handler
        static HMENU __stdcall Call(HINSTANCE hInstance, LPCSTR lpMenuName);
    } eventT; 

    // event handler
    HMENU EventT::Call(HINSTANCE hInstance, LPCSTR lpMenuName)
    {
        _VMESSAGE("Menu (%p:%p)",hInstance,lpMenuName);
        HMENU overrideMenu = 0;
        for (EventT::CallbackListT::iterator it = eventT.callbacks.begin(); it != eventT.callbacks.end();)
        {
            EventManager::CSWindows::LoadMenuA_f func = (EventManager::CSWindows::LoadMenuA_f)it->pFunc;
            it++;
            if (HMENU menu = func(hInstance,lpMenuName))
            {
                _DMESSAGE("Menu (%p:%p) overloaded to (%p) by callback <%p>",hInstance,lpMenuName,menu,func);
                overrideMenu = menu;
            }
        }
        if (overrideMenu) return overrideMenu;
        typedef HMENU (__stdcall *LoadMenuT)(HINSTANCE hInstance, LPCSTR lpMenuName);
        return ((LoadMenuT)eventT.overwrittenData)(hInstance,lpMenuName);
    }
}

namespace CreateDialogParam_
{
    // Patch data
    memaddr Patch       (0x0       ,0x009243FC);    // entry in User32.dll import table
    UInt8 patchLen      = 4;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Patch,patchLen ,"CSWindows.CreateDialogParam") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Patch.WriteData32((UInt32)memaddr::GetPointerToMember(&EventT::Call));
        }
        // event handler
        static HWND __stdcall Call(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
    } eventT; 

    // event handler
    HWND EventT::Call(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
    {
        _VMESSAGE("Dialog (%p:%p, proc=%p, param=%p)",hInstance,lpTemplateName,lpDialogFunc,dwInitParam);
        HWND overrideDialog = 0;
        for (EventT::CallbackListT::iterator it = eventT.callbacks.begin(); it != eventT.callbacks.end();)
        {
            EventManager::CSWindows::CreateDialogParamA_f func = (EventManager::CSWindows::CreateDialogParamA_f)it->pFunc;
            it++;
            if (HWND dialog = func(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam))
            {
                _DMESSAGE("Dialog (%p:%p, proc=%p, param=%p) overloaded to (%p) by callback <%p>",hInstance,lpTemplateName,lpDialogFunc,dwInitParam,dialog,func);
                overrideDialog = dialog;
            }
        }
        if (overrideDialog) return overrideDialog; // return override value
        typedef HWND (__stdcall *CreateDialogParamT)(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
        return ((CreateDialogParamT)*(void**)eventT.overwrittenData)(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam); // invoke original function
    }
 }

namespace MainW_WMCommand
{
    // Patch data
    memaddr Hook        (0x0       ,0x00419568);    // just before source control switch
    memaddr RetnA       (0x0       ,0x0041956D);    // normal execution path for unhandled event
    memaddr RetnB       (0x0       ,0x0041A94C);    // terminal execution path for handled event
    UInt8 patchLen      = 5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"CSWindows.MainWindow.WMCommand") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }
        // event handler
        static void Hndl();
        bool Call(WPARAM wparam, LPARAM lparam)
        {
            _VMESSAGE("WM_Command (wparam=%p,lparam=%p)",wparam,lparam);
            bool handled = false;
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::CSWindows::MainW_WMCommand_f func = (EventManager::CSWindows::MainW_WMCommand_f)it->pFunc;
                it++;
                if (func(wparam,lparam) == 0)
                {
                    _DMESSAGE("WM_Command (wparam=%p,lparam=%p) handled by <%p>",wparam,lparam,func);
                    handled = true;
                }
            }
            return handled;
        }
    } eventT;
     
    // hook handler
    void _declspec(naked) EventT::Hndl()
    {
        WPARAM  wparam;
        LPARAM  lparam;
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
        if (eventT.Call(wparam,lparam)) 
        {
            // event was handled, return out of message pump
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
        else
        {
            // event was not handled, continue normal execution
            __asm   
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
}

namespace ObjectW_CompareObject
{
    // Patch data
    memaddr Hook        (0x0       ,0x00415D1D);    // just before source control switch
    memaddr RetnA       (0x0       ,0x00415D27);    // normal execution path
    memaddr RetnB       (0x0       ,0x00416A48);    // terminal execution path, if valid result returned
    UInt8 patchLen      = 0xB;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"CSWindows.ObjectWindow.CompareObject") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }
        // event handler
        static void Hndl();
        bool Call(TESForm* formA, TESForm* formB, UInt32 columnID, int& result)
        {
            _VMESSAGE("Sort on (formA=%p,formB=%p,col=%i)",formA,formB,columnID);
            bool handled = false;
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::CSWindows::ObjectW_CompareObject_f func = (EventManager::CSWindows::ObjectW_CompareObject_f)it->pFunc;
                it++;
                if (func(formA,formB,columnID,result))
                {
                    _DMESSAGE("Custom sort on (formA=%p,formB=%p,col=%i) by callback <%p>",formA,formB,columnID,func);
                    handled = true;
                }
            }
            return handled;
        }
    } eventT;
    
    // hook handler
    void _declspec(naked) EventT::Hndl()
    {
        TESForm*    formA;
        TESForm*    formB;
        UInt32      colIndx;
        int*        result;
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
        if (eventT.Call(formA,formB,colIndx,*result)) 
        {
            // event was handled, jump to end of comparitor
            __asm
            {
                // epilog
                mov     esp, ebp
                popad
                // overwritten code
                jmp     [RetnB._addr]
            }
        }
        else
        {
            // event was not handled, continue normal execution
            *result = 1;
            __asm   
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
}

namespace ObjectW_GetObjectDispInfo
{
    // Patch data
    memaddr Hook        (0x0       ,0x00414E9C);    // just before source control switch
    memaddr RetnA       (0x0       ,0x00414EA1);    // normal execution path
    memaddr RetnB       (0x0       ,0x00415B45);    // terminal execution path, if valid result returned
    UInt8 patchLen      = 0x5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"CSWindows.ObjectWindow.GetObjectDispInfo") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelJump(&Hndl);
        }
        // event handler
        static void Hndl();
        bool Call(void* displayInfo)
        {
            _VMESSAGE("GetDispInfo <%p>",displayInfo);
            bool handled = false;
            for (CallbackListT::iterator it = callbacks.begin(); it != callbacks.end();)
            {
                EventManager::CSWindows::ObjectW_GetObjectDispInfo_f func = (EventManager::CSWindows::ObjectW_GetObjectDispInfo_f)it->pFunc;
                it++;
                if (func(displayInfo))
                {
                    _DMESSAGE("DisplayInfo provided for <%p> by callback <%p>",displayInfo,func);
                    handled = true;
                }
            }
            return handled;
        }
    } eventT;
    
    // hook handler
    
    void _declspec(naked) EventT::Hndl()
    {
        void*       displayInfo;
        __asm
        {
            // prolog
            pushad
            mov     ebp, esp
            sub     esp, __LOCAL_SIZE
            mov     displayInfo,esi
        }
        if (eventT.Call(displayInfo)) 
        {
            // event was handled, jump to end of comparitor
            __asm
            {
                // epilog
                mov     esp, ebp
                popad
                // overwritten code
                jmp     [RetnB._addr]
            }
        }
        else
        {
            // event was not handled, continue normal execution
            __asm   
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
}

namespace InitializeWindows
{
    // Patch data
    memaddr Hook        (0x0       ,0x0041CF40);    // call to CS InitializeWindows() method in main()
    UInt8 patchLen      = 5;

    // event object
    struct EventT : public EventEx
    {        
        // method overrides
        EventT() : EventEx(Hook,patchLen ,"CSWindows.InitializeWindows") {}
        virtual void Attach() 
        {
            EventEx::Attach();
            Hook.WriteRelCall(&Hndl);
        }
        // event handler
        static void Hndl()
        {
            // invoke CS InitializeWindows() method
            UInt32 origAddr = Hook._addr + 5 + *(UInt32*)((UInt32)eventT.overwrittenData + 1); 
            typedef void (*FuncT)(void);
            ((FuncT)origAddr)();

            // run callbacks
            _DMESSAGE("");
            for (CallbackListT::iterator it = eventT.callbacks.begin(); it != eventT.callbacks.end();)
            {
                EventManager::CSWindows::InitializeWindows_f func = (EventManager::CSWindows::InitializeWindows_f)it->pFunc;
                it++;
                
                _DMESSAGE("Invoking callback <%p> ...",func);
                func();
            }
        }
    } eventT; 
}

}   // end of namespace _CSWindows_
EventManager::Event& EventManager::CSWindows::LoadMenuA = _CSWindows_::LoadMenu_::eventT;
EventManager::Event& EventManager::CSWindows::CreateDialogParamA = _CSWindows_::CreateDialogParam_::eventT;
EventManager::Event& EventManager::CSWindows::MainW_WMCommand = _CSWindows_::MainW_WMCommand::eventT;
EventManager::Event& EventManager::CSWindows::ObjectW_CompareObject = _CSWindows_::ObjectW_CompareObject::eventT;
EventManager::Event& EventManager::CSWindows::ObjectW_GetObjectDispInfo = _CSWindows_::ObjectW_GetObjectDispInfo::eventT;
EventManager::Event& EventManager::CSWindows::InitializeWindows = _CSWindows_::InitializeWindows::eventT;
#endif

