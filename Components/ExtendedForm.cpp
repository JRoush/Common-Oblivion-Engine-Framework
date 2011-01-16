#include "ExtendedForm.h"
#include "EventManager.h"

#include "Utilities/Memaddr.h"
#include "API/TESForms/TESForm.h"
#include "API/TESFiles/TESFile.h"
#include "API/TES/TESDataHandler.h"
#include "API/CSDialogs/TESDialog.h"

// global objects
static bool                             exFormsInitialized = false;             // whether or not extended form manager has been initialized
static const UInt32                     exFormTypes = 0x100;                    // new upper bound of form type list
static TESForm::FormTypeInfo            exFormTypeList[exFormTypes] = {{0}};    // extended Form Types list
static ExtendedForm*                    exFormInfoList[exFormTypes] = {{0}};    // extended type info, null if no exForm registered

// Patch addresses
memaddr FormTypeListInitialized                 (0x00B33C10,0x00A0E214);    // boolean used to ensure list is only init'd once
memaddr TESForm_GetFormTypeName_Hook            (0x0047E24E,0x004AC26E);    // code that handles form types > 0x3A
memaddr TESDataHandler_AddForm_Hook             (0x0044DC13,0x00481C6A);    // code that handles form types > 0x40
memaddr TESForm_Create_Hook                     (0x00448B26,0x00479E5B);    // code that handles unsupported form types
memaddr TESForm_Create_Retn                     (0x00448B4C,0x00479E94);
memaddr TESDataHandler_LoadFormRecord_Patch     (0x0044DE23,0x00483711);    // relative jump for out-of-bounds form types
memaddr TESDataHandler_LoadFormRecord_PatchVal  (0x751     ,0x939);         // new relative jump offset
memaddr TESDataHandler_LoadFormRecord_Hook      (0x0044E58A,0x0048405C);    // trap call to LoadForm(), to prevent it if form type wasn't found
memaddr TESListView_PopulateWihForms_Hook       (0x0       ,0x00445C42);    // just before form type switch
memaddr TESListView_PopulateWihForms_Retn       (0x0       ,0x00445C48);
memaddr TESComboBox_PopulateWihForms_Hook       (0x0       ,0x0044574A);    // just before form type switch
memaddr TESComboBox_PopulateWihForms_Retn       (0x0       ,0x00445750);
memaddr TESFileDetailsDialog_GetFormTypeName    (0x0       ,0x0040B280);    // function entry

// Patch & Hook functions
void _declspec(naked) TESForm_GetFormTypeName_Hndl()
{
    UInt8       formType;
    const char* result;
    __asm
    {
        // prolog
        pushad
        mov     ebp, esp
        sub     esp, __LOCAL_SIZE
        mov     formType, cl
    }
    if (exFormInfoList[formType] && exFormInfoList[formType]->displayName)
    {        
        result = exFormInfoList[formType]->displayName; // use display name        
        _VMESSAGE("formType 0x%02X -> '%s'",formType,result);
    }
    else if (exFormTypeList[formType].shortName)
    {
        result = exFormTypeList[formType].shortName; // use short name        
        _VMESSAGE("formType 0x%02X -> '%s'",formType,result);
    }
    else 
    {
        result = exFormTypeList[0].shortName; // use "NONE"
        _ERROR("No name available for FormType 0x%02X",formType);
    }
    __asm
    {
        // write result into stack were it will be popped into eax
        mov     eax, result
        mov     [ebp + 0x1C], eax    
        // epilog
        mov     esp, ebp
        popad
        // overwritten code
        retn
    }
}
UInt32 GetFormTypeFromChunkType(UInt32 chunkType)
{
    // check cached chunk type (for efficiency)
    static UInt8 lastFormType = 0;
    if (chunkType == 0) return 0;
    else if (chunkType == exFormTypeList[lastFormType].chunkType) return lastFormType;

    for (int i = 0; i < exFormTypes; i++) 
    {
        if (chunkType == exFormTypeList[i].chunkType)
        {
            _VMESSAGE("Chunk type %08X %4.4s found -> form type 0x%02X",chunkType,&chunkType,i);
            lastFormType = i;
            return i;        
        }
    }

    _WARNING("Chunk type %08X %4.4s not found",chunkType,&chunkType);
    return 0;   // form not found
}
void _declspec(naked) TESDataHandler_AddForm_Hndl()
{
    TESForm*    form;
    bool        success;
    __asm
    {
        // prolog
        pushad
        mov     ebp, esp
        sub     esp, __LOCAL_SIZE
        mov     form, esi
    }
    if (exFormInfoList[form->formType])
    {        
        exFormInfoList[form->formType]->FormList().PushFront(form); // add form to list
        success = true;
        _VMESSAGE("Added Form %s @ <%p> to data handler",exFormTypeList[form->formType].shortName,form);
    }
    else 
    {
        success = false;
        _ERROR("The data handler does not support FormType 0x%02X",form->formType);
    }
    __asm
    {
        // write result into stack were it will be popped into eax
        movzx   eax, success
        mov     [ebp + 0x1C], eax    
        // epilog
        mov     esp, ebp
        popad
        // overwritten code
        pop     ebx
        pop     edi
        pop     esi
        retn    4
    }
}
void TESDataHandler_Clear_Hndl()
{
    for (int i = 0; i < exFormTypes; i++) 
    {
        if (!exFormInfoList[i]) continue; 
        _DMESSAGE("Clearing %s forms in extended data handler",exFormTypeList[i].shortName);
        // destroy any existing forms of this type
        for (ExtendedForm::FormListT::Node* node = &exFormInfoList[i]->FormList().firstNode; node; node = node->next)
        {
            if (node->data) delete node->data;
        }
        exFormInfoList[i]->FormList().Clear();
        // notify user that forms have been destroyed
        if (exFormInfoList[i]->ClearForms) exFormInfoList[i]->ClearForms();        
    }  
}
void TESDataHandler_CreateBuiltins_Hndl()
{
    for (int i = 0; i < exFormTypes; i++) 
    {
        if (!exFormInfoList[i]) continue; 
        _DMESSAGE("Creating Builtin %s forms in extended data handler",exFormTypeList[i].shortName);
        // notify user to create builtin forms
        if (exFormInfoList[i]->CreateDefaultForms) exFormInfoList[i]->CreateDefaultForms();        
    }  
}
void TESDataHandler_LoadFormRecord_Hndl(TESForm* form, TESFile* file)
{
    if (form && file) TESDataHandler::dataHandler->LoadForm(*form,*file);
}
void _declspec(naked) TESForm_Create_Hndl()
{
    TESForm*    form;
    UInt8       formType;
    __asm
    {
        // prolog
        pushad
        mov     ebp, esp
        sub     esp, __LOCAL_SIZE
        #ifdef OBLIVION
        mov     formType, dl
        #else
        mov     formType, bl
        #endif
    }
    if (exFormInfoList[formType] && exFormInfoList[formType]->CreateForm)
    {
        form = exFormInfoList[formType]->CreateForm();
        _VMESSAGE("Dynamically created %s form @ <%p>",exFormTypeList[formType].shortName,form);
    }
    else
    {
        _ERROR("TESForm does not support dynamic creation of FormType 0x%02X",formType);
        form = 0;
    }
    __asm
    {
        // write result into stack were it will be popped into eax
        mov     eax, form
        mov     [ebp + 0x1C], eax    
        // epilog
        mov     esp, ebp
        popad
        // overwritten code
        jmp     [TESForm_Create_Retn._addr]
    }
}
#ifndef OBLIVION
void _declspec(naked) TESListView_PopulateWihForms_Hndl()
{
    UInt8       formType;
    HWND        dialog;
    __asm
    {
        // prolog
        pushad
        mov     eax, ebp    // save dialog handle stored in ebp
        mov     ebp, esp
        sub     esp, __LOCAL_SIZE
        mov     formType, bl
        mov     dialog, eax
    }
    if (exFormInfoList[formType])
    {
        _VMESSAGE("Populating ListView {%p} with %s forms from extended data handler ...",dialog,exFormTypeList[formType].shortName);
        for (ExtendedForm::FormListT::Node* node = &exFormInfoList[formType]->FormList().firstNode; node; node = node->next)
        {
            if (!node->data) continue;
            TESListView::InsertItem(dialog,node->data,false,0);
        }
    }
    __asm
    { 
        // epilog
        mov     esp, ebp
        popad
        // overwritten code
        movzx   eax, bl
        add     eax, 0FFFFFFFDh
        jmp     [TESListView_PopulateWihForms_Retn._addr]
    }
}
void _declspec(naked) TESComboBox_PopulateWihForms_Hndl()
{
    UInt8       formType;
    HWND        dialog;
    __asm
    {
        // prolog
        pushad
        mov     eax, ebp    // save dialog handle stored in ebp
        mov     ebp, esp
        sub     esp, __LOCAL_SIZE
        mov     formType, bl
        mov     dialog, eax
    }
    if (exFormInfoList[formType])
    {
        _VMESSAGE("Populating Combo {%p} with %s forms from extended data handler ...",dialog,exFormTypeList[formType].shortName);
        for (ExtendedForm::FormListT::Node* node = &exFormInfoList[formType]->FormList().firstNode; node; node = node->next)
        {
            if (!node->data) continue;
            TESComboBox::AddItem(dialog,node->data->GetEditorID(),node->data);
        }
    }
    __asm
    { 
        // epilog
        mov     esp, ebp
        popad
        // overwritten code
        movzx   eax, bl
        add     eax, 0xFFFFFFFC
        jmp     [TESComboBox_PopulateWihForms_Retn._addr]
    }
}
const char* TESFileDetailsDialog_GetFormTypeName_Hndl(UInt8 formType)
{
    if (exFormTypeList[formType].shortName)
    {           
        _VMESSAGE("formType 0x%02X -> '%s'",formType,exFormTypeList[formType].shortName);
        return exFormTypeList[formType].shortName; // use short name     
    }
    else 
    {
        _ERROR("No name available for FormType 0x%02X",formType);
        return "";
    }
}
#endif
void PatchFormTypeListRef(memaddr addr, UInt8 formType, UInt32 member)
{
    // replace address of old list entry with corresponding address in new list
    void* newaddr;
    switch (member)
    {
    case 0:
        newaddr = &exFormTypeList[formType].formType;
        break;
    case 4:
        newaddr = &exFormTypeList[formType].shortName;
        break;
    case 8:
        newaddr = &exFormTypeList[formType].chunkType;
        break;
    default:
        return;
    };
    addr.WriteData32((UInt32)newaddr);
}

// Initialization function
void InitializeExtendedForms()
{
    if (exFormsInitialized) return; // manager already initialized
    _MESSAGE("Initializing Extended Form Manager ...");

    // initialize & copy vanilla form type list
    _DMESSAGE("Initializing Form Type List ...");
    for (int i = 0; i < TESForm::kFormType__MAX; i++) 
    {
        // reverse short name to produce chunk type
        TESForm::formTypeList[i].chunkType = *(UInt32*)TESForm::formTypeList[i].shortName;
        // copy entry to new form list
        exFormTypeList[i] = TESForm::formTypeList[i];
    }
    // mark form type list as initialized
    FormTypeListInitialized.WriteData8(true);

    // hook TESForm::GetFormTypeName
    TESForm_GetFormTypeName_Hook.WriteRelJump(&TESForm_GetFormTypeName_Hndl);

    // patch TESForm::GetFormTypeFromChunkType (actually listed under TESFile in API)
    memaddr GetFormTypeFromChunkType_Hook((UInt32)&TESFile::RecordTypeFromChunkType);
    GetFormTypeFromChunkType_Hook.WriteRelJump(&GetFormTypeFromChunkType);

    // hook TESDataHandler::AddForm, Clear, and CreateDefaultForms
    TESDataHandler_AddForm_Hook.WriteRelJump(&TESDataHandler_AddForm_Hndl);
    EventManager::RegisterEventCallback(EventManager::DataHandler_Clear,&TESDataHandler_Clear_Hndl);
    EventManager::RegisterEventCallback(EventManager::DataHandler_CreateDefaults,&TESDataHandler_CreateBuiltins_Hndl);

    // patch TESDataHandler::LoadFormRecord to create,load, & add to DH for out-of-bounds forms
    TESDataHandler_LoadFormRecord_Patch.WriteData32(TESDataHandler_LoadFormRecord_PatchVal);   // new relative jump offset
    TESDataHandler_LoadFormRecord_Hook.WriteRelCall(&TESDataHandler_LoadFormRecord_Hndl);

    // hook TESForm::Create
    TESForm_Create_Hook.WriteRelJump(&TESForm_Create_Hndl);

    #ifndef OBLIVION

    // hook TESDialog control populators
    TESListView_PopulateWihForms_Hook.WriteRelJump(&TESListView_PopulateWihForms_Hndl);
    TESComboBox_PopulateWihForms_Hook.WriteRelJump(&TESComboBox_PopulateWihForms_Hndl);

    // hook TESFileDetailsDialog methods
    TESFileDetailsDialog_GetFormTypeName.WriteRelJump(&TESFileDetailsDialog_GetFormTypeName_Hndl);

    #endif

    // patch (indexed) references into the FormTypeList to point to new expanded list
    _DMESSAGE("Patching refs to old Form Type List ...");
    #ifdef OBLIVION
        PatchFormTypeListRef(0x0046A698, 0, 0); // TESDescription_LoadToCache
        PatchFormTypeListRef(0x0052AFCB, 0, 0);
        PatchFormTypeListRef(0x00530CD0, 0, 0);
        PatchFormTypeListRef(0x00531349, 0, 0);
        PatchFormTypeListRef(0x0042B7D2, 0, 4);
        PatchFormTypeListRef(0x00448B31, 0, 4); // TESForm_CreateDynamic
        PatchFormTypeListRef(0x00449B8C, 0, 4); // TESDataHandler_Clear
        PatchFormTypeListRef(0x0044DC23, 0, 4); // TESDataHandler_AddForm
        PatchFormTypeListRef(0x0044DDD4, 0, 4); // TESDataHandler_LoadFormRecord
        PatchFormTypeListRef(0x0044DDDF, 0, 4); // TESDataHandler_LoadFormRecord
        PatchFormTypeListRef(0x00450409, 0, 4); // TESFile_UpdateOpenGroups
        PatchFormTypeListRef(0x0045AF60, 0, 4); // TESSaveLoadGame_PrintChangeRecords?
        PatchFormTypeListRef(0x0046389D, 0, 4); // TESSaveLoadGame_LoadForm
        PatchFormTypeListRef(0x004638AE, 0, 4); // TESSaveLoadGame_LoadForm
        PatchFormTypeListRef(0x0046642A, 0, 4); // TESSaveLoadGame_LoadGame?
        PatchFormTypeListRef(0x00466438, 0, 4); // TESSaveLoadGame_LoadGame?
        PatchFormTypeListRef(0x00466470, 0, 4); // TESSaveLoadGame_LoadGame?
        PatchFormTypeListRef(0x00466599, 0, 4); // TESSaveLoadGame_LoadGame?
        PatchFormTypeListRef(0x0046B7B0, 0, 4); // TESForm_CopyFrom
        PatchFormTypeListRef(0x0046B7E0, 0, 4); // TESForm_CompareTo
        PatchFormTypeListRef(0x0046C49B, 0, 4); // TESForm_constr - redundant, since this code is now supersceded
        PatchFormTypeListRef(0x0046C70E, 0, 4); // TESForm_GetDebugDesc
        PatchFormTypeListRef(0x0047E259, 0, 4); // TESForm_GetFormTypeName
        PatchFormTypeListRef(0x0047E276, 0, 4); // TESForm_GetFormTypeName
        PatchFormTypeListRef(0x004DD33F, 0, 4);
        PatchFormTypeListRef(0x004DF6E5, 0, 4); // TESObjectREFR_GetDebugDesc
        PatchFormTypeListRef(0x004DF7E9, 0, 4); // TESObjectREFR_GetDebugDesc
        PatchFormTypeListRef(0x004E3643, 0, 4);
        PatchFormTypeListRef(0x00530F78, 0, 4);
        PatchFormTypeListRef(0x0056AF63, 0, 4);
        PatchFormTypeListRef(0x0056B0F2, 0, 4);
        PatchFormTypeListRef(0x004509A7, 0, 8); // TESFile_WriteEmptyFormRecord
        PatchFormTypeListRef(0x004519B7, 0, 8);
        PatchFormTypeListRef(0x0046AF85, 0, 8); // TESForm_GetFormTypeFromChunkType
        PatchFormTypeListRef(0x0046B939, 0, 8); // TESForm_MatchGroupRecord
        PatchFormTypeListRef(0x0046B976, 0, 8); // TESForm_CreateGroupRecord
        PatchFormTypeListRef(0x0046B9D9, 0, 8); // TESForm_InitializeFormRecord
        PatchFormTypeListRef(0x0046C4F9, 0, 8); // TESForm_constr - redundant, since this code is now supersceded
    #else
        PatchFormTypeListRef(0x0040B291, 0, 4); // TESForm_GetRecordNameFromFormType
        //PatchFormTypeListRef(0x0040B6F8, 0, 4); // FileDetailsDialog_MoreInfoMessage - this popup is essentially useless anyway, not worth supporting
        PatchFormTypeListRef(0x0040B992, 0, 4); // FileDetailsDialog_CompareFunc
        PatchFormTypeListRef(0x0040B99F, 0, 4); // FileDetailsDialog_CompareFunc
        PatchFormTypeListRef(0x0040BECC, 0, 4); // FileDetailsDialog_GetDispInfo???
        PatchFormTypeListRef(0x00433B34, 0, 4); // UseReportDialog_ObjectCompareFunc
        PatchFormTypeListRef(0x00433B3E, 0, 4); // UseReportDialog_ObjectCompareFunc
        PatchFormTypeListRef(0x00433E7C, 0, 4); // UseReportDialog_GetObjectDispInfo
        PatchFormTypeListRef(0x0043607F, 0, 4); // FileDetailsDialog_SuccessfulUpdateGroup
        PatchFormTypeListRef(0x00437076, 0, 4); // FileDetailsDialog_ValidateBitArrayFiles
        PatchFormTypeListRef(0x004370CB, 0, 4); // FileDetailsDialog_ValidateBitArrayFiles
        PatchFormTypeListRef(0x004371E8, 0, 4); // FileDetailsDialog_ValidateBitArrayFiles
        PatchFormTypeListRef(0x00437255, 0, 4); // FileDetailsDialog_ValidateBitArrayFiles
        PatchFormTypeListRef(0x004386F0, 0, 4);
        PatchFormTypeListRef(0x00439F87, 0, 4); // FileDetailsDialog_SortForms
        PatchFormTypeListRef(0x0043A5D0, 0, 4);
        PatchFormTypeListRef(0x0043A64A, 0, 4);
        PatchFormTypeListRef(0x0043EE82, 0, 4); // TESVersionControl_DlgProc
        PatchFormTypeListRef(0x0043FA17, 0, 4); // TESVersionControl_DlgProc
        PatchFormTypeListRef(0x00442276, 0, 4); // TESDialog_GetFormEditDlgTemplateID
        PatchFormTypeListRef(0x004582F3, 0, 4);
        PatchFormTypeListRef(0x00458488, 0, 4);
        PatchFormTypeListRef(0x0046275B, 0, 4);
        PatchFormTypeListRef(0x00479E79, 0, 4); // TESForm_CreateDynamic
        PatchFormTypeListRef(0x0047B19C, 0, 4); // TESDataHandler_Clear
        PatchFormTypeListRef(0x0047B8A3, 0, 4);
        PatchFormTypeListRef(0x00481C8E, 0, 4); // TESDataHandler_AddForm
        PatchFormTypeListRef(0x00482368, 0, 4); // TESDataHandler_RemapFormIDs
        PatchFormTypeListRef(0x00483504, 0, 4); // TESDataHandler_LoadFormRecord
        PatchFormTypeListRef(0x0048350F, 0, 4); // TESDataHandler_LoadFormRecord
        PatchFormTypeListRef(0x00483614, 0, 4); // TESDataHandler_LoadFormRecord
        PatchFormTypeListRef(0x00487705, 0, 4); // TESFile_UpdateOpenGroups
        PatchFormTypeListRef(0x00494D50, 0, 4); // TESForm_CopyFrom
        PatchFormTypeListRef(0x00494D80, 0, 4); // TESForm_CompareTo
        PatchFormTypeListRef(0x00496ED0, 0, 4);
        PatchFormTypeListRef(0x0049777E, 0, 4); // TESForm_SetEditorID
        PatchFormTypeListRef(0x00497EF3, 0, 4); // TESForm_SetFormID
        PatchFormTypeListRef(0x004980C5, 0, 4); // TESForm_Clone
        PatchFormTypeListRef(0x004981AF, 0, 4); // TESForm_GetDebugDesc
        PatchFormTypeListRef(0x004982EF, 0, 4); // ??0TESForm@@QAE@XZ - redundant, since this code is now supersceded
        PatchFormTypeListRef(0x004990A8, 0, 4); // TESCS_ExportNames?
        PatchFormTypeListRef(0x004AC279, 0, 4); // TESForm_GetFormTypeName
        PatchFormTypeListRef(0x004AC296, 0, 4); // TESForm_GetFormTypeName
        PatchFormTypeListRef(0x004F73E6, 0, 4);
        PatchFormTypeListRef(0x00512910, 0, 4);
        PatchFormTypeListRef(0x00514C56, 0, 4);
        PatchFormTypeListRef(0x005191ED, 0, 4); // TESLoadScreen_GetSpecificFormDispInfo
        PatchFormTypeListRef(0x0054778E, 0, 4);
        PatchFormTypeListRef(0x005485D6, 0, 4); // TESObjectREFR_GetDebugDesc
        PatchFormTypeListRef(0x005486E0, 0, 4); // TESObjectREFR_GetDebugDesc
        PatchFormTypeListRef(0x0054A7B7, 0, 4);
        PatchFormTypeListRef(0x0054A7F0, 0, 4);
        PatchFormTypeListRef(0x004364AB, 0, 8);
        PatchFormTypeListRef(0x004366DC, 0, 8);
        PatchFormTypeListRef(0x00436965, 0, 8);
        PatchFormTypeListRef(0x00438A5C, 0, 8); // TESVersionControl_CompareFunc
        PatchFormTypeListRef(0x00438A89, 0, 8); // TESVersionControl_CompareFunc
        PatchFormTypeListRef(0x00487D80, 0, 8); // TESFile_WriteEmptyFormRecord
        PatchFormTypeListRef(0x0048916E, 0, 8);
        PatchFormTypeListRef(0x00494F59, 0, 8); // TESForm_MatchGroupRecord
        PatchFormTypeListRef(0x00494F95, 0, 8); // TESForm_CreateGroupRecord
        PatchFormTypeListRef(0x00495A75, 0, 8); // TESForm_GetFormTypeFromChunkType
        PatchFormTypeListRef(0x0049628D, 0, 8); // TESForm_InitializeFormRecord
        PatchFormTypeListRef(0x00498349, 0, 8); // ??0TESForm@@QAE@XZ - redundant, since this code is now supersceded
    #endif

    // initialization complete
    exFormsInitialized = true;
    _DMESSAGE("Initialization Complete");
}

// ExtendedForm
ExtendedForm::ExtendedForm(const char* nPluginName, const char* nClassName, const char* nDisplayName, TESForm* (*nCreateForm)())
:   pluginName(nPluginName), className(nClassName), displayName(nDisplayName), 
    CreateForm(nCreateForm), CreateDefaultForms(0), ClearForms(0), 
    _formType(0), _formList()
{
    memset(_shortName,0,sizeof(_shortName));
}
ExtendedForm::~ExtendedForm() 
{
    // unregister this form type
    // this may cause an error if 
    //  - the global output log is destroyed before this destructor is called
    //  - the module containing the registered 'Clear' callback has been unloaded
    // for this reason, it is a good idea for users to unregister their forms in the course of normal cleanup
    Unregister();
    // clear form list to free up any memory alloated for the nodes
    _formList.Clear();
}
bool ExtendedForm::Register(const char* shortName, UInt8 formType)
{
    // validate form type lists
    if (exFormsInitialized == false)
    {
        // extended form manager has not yet been initialized
        InitializeExtendedForms();
    }

    // validate plugin+class names
    if (!pluginName || !className || !pluginName[0] || !className[0])
    {
        // null or empty names
        _ERROR("Could not register extended form type [%s::%s] - class+plugin names cannot be null or empty",pluginName,className);
        return false;
    }
    for (int i = 0; i < exFormTypes; i++) 
    {
        if (exFormInfoList[i] && strcmp(pluginName,exFormInfoList[i]->pluginName) == 0 && strcmp(className,exFormInfoList[i]->className) == 0)
        {
            _ERROR("Could not register extended form type [%s::%s] - the requested plugin and class names are already in use for form type 0x%02X",
                    pluginName,className,i);
            return 0;
        }
    }

    // determine form type code
    if (formType == 0)
    {
        // find an unused extended form type
        UInt32 i;
        for (i = TESForm::kFormType__MAX+1; i < exFormTypes; i++) { if (!exFormInfoList[i]) break; }
        if (i >= exFormTypes)
        {
            // all extended form types are used
            _ERROR("Could not register extended form type [%s::%s] - the extended form table is full",pluginName,className);
            return false;
        }
        formType = i;
    }
    else if (formType == TESForm::kFormType__MAX)
    {
        // requested type code is reserved, as it has special meaning in some places
        _ERROR("Could not register extended form type [%s::%s] - the requested code 0x%02X is reserved for internal use", pluginName,className,formType);
        return false;
    }
    else if (exFormInfoList[formType])
    {
        // requested type code is currently registered to another form type
        _ERROR("Could not register extended form type [%s::%s] - the requested code 0x%02X is already registered to [%s::%s]",
            pluginName,className,formType,exFormInfoList[formType]->pluginName,exFormInfoList[formType]->className);
        return false;
    }

    // determine short name
    if (formType < TESForm::kFormType__MAX)
    {
        strcpy_s(_shortName,sizeof(_shortName),exFormTypeList[formType].shortName); // copy existing short name to local buffer
    }
    else if (shortName == 0)
    {
        // generate short name from type code
        sprintf_s(_shortName,sizeof(_shortName),"EF%02X",formType);
    }
    else if (strncmp(shortName,"EF",2) == 0)
    {
        // requested short name begins with reserved prefix 'EF'
        _ERROR("Could not register extended form type [%s::%s] - the requested short name '%s' begins with reserved prefix 'EF'",
                pluginName,className,shortName);
        return false;
    }
    else if (strlen(shortName)!= 4)
    {
        // requested short name has wrong length
        _ERROR("Could not register extended form type [%s::%s] - the requested short name '%s' does not have the required length (4 characters)",
                pluginName,className,shortName);
        return false;
    }
    else
    {
        for (int i = 0; i < exFormTypes; i++) 
        { 
            if (exFormInfoList[i] && strncmp(shortName,exFormInfoList[i]->_shortName,sizeof(_shortName)) == 0)
            {
                // requested short name is already in use
                _ERROR("Could not register extended form type [%s::%s] - the requested short name '%s' is already registered to [%s::%s]",
                        pluginName,className,shortName,exFormInfoList[i]->pluginName,exFormInfoList[i]->className);
                return false;
            }
        }
        strcpy_s(_shortName,sizeof(_shortName),shortName); // copy requested short name to local buffer
    }
       
    // setup form type entry
    if (formType > TESForm::kFormType__MAX) 
    {
        exFormTypeList[formType].formType = formType;    
        exFormTypeList[formType].shortName = _shortName;
        exFormTypeList[formType].chunkType = *(UInt32*)_shortName;
    }
    exFormInfoList[formType] = this;
    _formType = formType;   
    _MESSAGE("Registered extended form type [%s::%s] at codepoint %02X, short name %4.4s", pluginName,className,_formType,_shortName);

    // if data handler has already been initialized, notify user to create built-in forms of this type
    if (TESDataHandler::dataHandler && CreateDefaultForms) CreateDefaultForms();

    return true;
}
bool ExtendedForm::Unregister()
{       
    if (_formType > 0)
    {
        // destroy any existing forms of this type
        for (FormListT::Node* node = &_formList.firstNode; node; node = node->next)
        {
            if (node->data) delete node->data;
        }
        _formList.Clear();
        // notify user that forms have been destroyed
        if (ClearForms) ClearForms();
        // clear form type list entry
        if (_formType > TESForm::kFormType__MAX) 
        {
            exFormTypeList[_formType].shortName = 0;
            exFormTypeList[_formType].formType = 0;
            exFormTypeList[_formType].chunkType = 0;
        }
        // clear extended form info entry
        exFormInfoList[_formType] = 0;
        // reset form type
        _MESSAGE("Unregistered extended form type [%s::%s] from codepoint %02X",pluginName,className,_formType);
        _formType = 0;
        return true;
    }
    else return false;
}
UInt8 ExtendedForm::FormType() { return _formType; }
const char* ExtendedForm::ShortName() { return _shortName; }
ExtendedForm::FormListT& ExtendedForm::FormList() { return _formList; }

