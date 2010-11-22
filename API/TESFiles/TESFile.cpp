#include "rsh\GameObjects\TESFile.h"
#include "rsh\Utilities.h"

namespace RSH {

/********************* TESFile ******************************/
#define SNM(method,variable) TESFile##_##method##_##variable
memaddr SNM(LoadForm,)                      (0x00447050,0x004790C0);
memaddr SNM(GetCurFormType,)                (0x004511C0,0x00488130);
memaddr SNM(GetCurChunkType,)               (0x004510B0,0x00487F90);
memaddr SNM(InitializeForm,)                (0x00451210,0x00488180);
memaddr SNM(GetChunkData,)                  (0x00450C20,0x004879D0);
memaddr SNM(GetNextChunk,)                  (0x0044FEA0,0x00486420);
// methods
bool TESFile::LoadForm(TESForm* form, TESFile* file)
{
    return (bool)SNM(LoadForm,).cdeclCall(form, file);
}
UInt32 TESFile::GetCurFormType()
{
    return SNM(GetCurFormType,).thisCall(this);
}
bool TESFile::GetCurChunkValid()
{
    // GetNextChunk() zeros out chunk type and length when it moves outside the bounds of the record.
    return (curChunk.type || curChunk.length);
}
UInt32 TESFile::GetCurChunkType()
{
    return SNM(GetCurChunkType,).thisCall(this);
}
void TESFile::InitializeForm(TESForm* form)
{
    SNM(InitializeForm,).thisCall(this, form);
}
void TESFile::GetChunkData(void* buffer, UInt32 size)
{
    SNM(GetChunkData,).thisCall(this, buffer, size);
}
bool TESFile::GetNextChunk()
{
    return (bool)SNM(GetNextChunk,).thisCall(this);
}
}   //  end namespace RSH