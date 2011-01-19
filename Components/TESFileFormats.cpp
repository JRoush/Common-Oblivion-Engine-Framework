#include "Components/TESFileFormats.h"
#include "API/TESFiles/TESFile.h"
#include "API/Actors/ActorValues.h"

bool TESFileFormats::ResolveModValue(UInt32& value, TESFile& file, UInt32 resolutionType)
{
    switch (resolutionType)
    {
    case kResolutionType_None:
        return true;
    case kResolutionType_FormID:
        return TESForm::ResolveFormID(value,file);
    case kResolutionType_MgefCode:
    {
        // returns invalid code on failure      
        if (value < 0x80000000) return true;   // code is not dynamic
        UInt32 fmid = (value << 0x18) | 0xFFFFFF;    // create a dummy formid
        if (!TESForm::ResolveFormID(fmid,file))  return false; // failed to resolve
        value = (value & 0xFFFFFF00) | (fmid >> 0x18);
        return true;
    }
    case kResolutionType_ActorValue:
    {
        if (TESForm::IsFormIDBuiltin(value) || value == ActorValues::kActorVal__UBOUND) return true; 
        else return TESForm::ResolveFormID(value,file);
    }
    default:
        return false;
    }
}