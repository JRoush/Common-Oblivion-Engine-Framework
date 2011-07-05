#include "Components/TESFileFormats.h"
#include "API/TESFiles/TESFile.h"
#include "API/TESForms/TESForm.h"
#include "API/Magic/EffectSetting.h"
#include "API/Actors/ActorValues.h"

bool TESFileFormats::ResolveModValue(UInt32& value, TESFile& file, UInt32 resolutionType)
{
    switch (resolutionType)
    {
    case kResType_None:
        return true;
    case kResType_FormID:
        if (value == 0) return true;
        else return TESForm::ResolveFormID(value,file);
    case kResType_MgefCode:
    {
        // returns invalid code on failure      
        if (value < 0x80000000) return true;   // code is not dynamic
        UInt32 fmid = (value << 0x18) | 0xFFFFFF;    // create a dummy formid
        if (!TESForm::ResolveFormID(fmid,file))  return false; // failed to resolve
        value = (value & 0xFFFFFF00) | (fmid >> 0x18);
        return true;
    }
    case kResType_ActorValue:
    {
        if (value == 0 || TESForm::IsFormIDBuiltin(value) || value == ActorValues::kActorVal__UBOUND) return true; 
        else return TESForm::ResolveFormID(value,file);
    }
    default:
        return false;
    }
}
TESForm* TESFileFormats::GetFormFromCode(UInt32 code, UInt32 resolutionType)
{
    switch (resolutionType)
    {
    case kResType_FormID:
        return TESForm::LookupByFormID(code);
    case kResType_MgefCode:
        return EffectSettingCollection::LookupByCode(code);
    case kResType_ActorValue:
        // TODO - connect to ExtendedActorValues component once it is written
        if (TESForm::IsFormIDBuiltin(code) || code == ActorValues::kActorVal__UBOUND) return 0; 
        else return TESForm::LookupByFormID(code);
    default:
        return 0;
    }
}   