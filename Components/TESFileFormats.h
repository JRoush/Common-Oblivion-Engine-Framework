/*   
    TESFile Formats

    Common definitions for saaving, loading, and resolving data in mod (.esp/.esm) files
*/
#pragma once
#ifndef COMPONENT_EXPORT
    #define COMPONENT_EXPORT _declspec(dllimport) 
#endif

// argument classes
class   TESFile;    // API/TESFiles/TESFile.h

class COMPONENT_EXPORT TESFileFormats
{
public:

    enum ResolutionTypes
    {
        kResolutionType_None          = 0x00,   // for all values that do not depend on load order
        kResolutionType_FormID        = 0x01,   // iif >= 0x800, highest byte is index of mod in load order
        kResolutionType_MgefCode      = 0x02,   // [OBME specific] iif >= 0x80000000, lowest byte is index in load order
        kResolutionType_ActorValue    = 0x03,   // same as formIDs, but the value 0xFFFFFFFF is not resolved
    };

    // methods
    static bool     ResolveModValue(UInt32& value, TESFile& file, UInt32 resolutionType); // resolve value from mod file

};