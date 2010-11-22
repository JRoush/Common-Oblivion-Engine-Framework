#include "rsh\GameObjects\Actor.h"
#include "rsh\GameForms\FormBase.h"
#include "rsh\GameInternals\Oblivion_RTTI.h"
#include "rsh\Utilities.h"

namespace RSH {

/********************* Actor ******************************/
#define SNM(method,variable) Actor##_##method##_##variable
memaddr SNM(GetBaseCalcAVi,)                (0x005F1910,0x0);    
memaddr SNM(GetBaseCalcAVf,)                (0x005EAD00,0x0);
memaddr SNM(GetCalcAV,)                     (0x005E2210,0x0); 
memaddr SNM(GetPlayerAVModifierf,)          (0x0065D270,0x0); 
memaddr SNM(GetActorValueName,)             (0x00565CC0,0x0044F570);
memaddr SNM(GetMasteryName,)                (0x0056A340,0x00455EC0);
// Actor methods
SInt16 Actor::GetAViBaseCalc(UInt32 actorVal)
{
    return SNM(GetBaseCalcAVi,).thisCall(this, actorVal);
}
float Actor::GetAVfBaseCalc(UInt32 actorVal)
{
    return SNM(GetBaseCalcAVf,).thisCallf(this, actorVal);
}
SInt32 Actor::GetAViBase(UInt32 actorVal)
{
    TESActorBase* base = (TESActorBase*)GetBaseForm();
    if (base && IsActor()) return base->GetAViBase(actorVal);
    else return 0;
}
float Actor::GetAVfBase(UInt32 actorVal)
{
    TESActorBase* base = (TESActorBase*)GetBaseForm();
    if (base && IsActor()) return base->GetAVfBase(actorVal);
    else return 0;
}
void Actor::GetAVfCalc(UInt32 actorVal, float& add, float& mult)
{
    SNM(GetCalcAV,).thisCall(this,actorVal,&add,&mult);
}
float Actor::GetAVfMax(UInt32 actorVal)
{
    // NOTE: the game doesn't seem to make a distinction between max av and current av
    // for most actors and avs - only really for PC health, magicka, and fatigue.
    float maxmod = GetAVfModifier(kAVModifier_Max,actorVal);
    if (maxmod < 0) maxmod = 0;
    return GetAVfBaseCalc(actorVal) + maxmod;
}
float Actor::GetAVfModifier(UInt32 avModifier, UInt32 actorVal)
{
    if (PlayerCharacter* pc = (PlayerCharacter*)RTTI::DynamicCast(this,0,RTTI::Actor,RTTI::PlayerCharacter,0))
    {
        // for the player, modifiers seem to be stored directly on player ref
        return SNM(GetPlayerAVModifierf,).thisCallf(pc,avModifier,actorVal);
    }
    // for non-player actors, modifiers are stored in process + actor ref
    switch (avModifier)
    {
    case kAVModifier_Max:
        // Max modifier seems to be stored in an AVCollection in the MiddleLow process at 0x94
        if (MiddleLowProcess* midproc = (MiddleLowProcess*)
            RTTI::DynamicCast(process,0,RTTI::BaseProcess,RTTI::MiddleLowProcess,0))
        {
            AVCollection* avColl = (AVCollection*)((UInt32)midproc + 0x94);
            return avColl->GetAVf(actorVal);            
        }
        break;
    case kAVModifier_Offset:
        // Offset modifier seems to be stored in an AVCollection in the Actor ref
        return avModifiers.GetAVf(actorVal);            
    case kAVModifier_Damage:
        // Damage modifier seems to be stored in an AVCollection in the Low process at 0x70
        if (LowProcess* proc = (LowProcess*)RTTI::DynamicCast(process,0,RTTI::BaseProcess,RTTI::LowProcess,0))
        {
            AVCollection* avColl = (AVCollection*)((UInt32)proc + 0x70);
            return avColl->GetAVf(actorVal);            
        }
        break;
    default:
        break;
    }
    // invalid modifier type for this actor
    return 0;
}
const char* Actor::GetActorValueName(UInt32 actorValue)
{
    return (const char*)SNM(GetActorValueName,).cdeclCall(actorValue);
}

const char* Actor::GetMasteryName(UInt32 masteryLevel)
{
    return (const char*)SNM(GetMasteryName,).cdeclCall(masteryLevel);
}
bool Actor::ResolveModActorValue(UInt32& avCode, TESFile* file)
{
    if (avCode < 0x800) return true;    // don't resolve avCodes < 0x800
    return RSH::TESForm::ResolveModFormID(avCode,file);  // resolve the avCode like a formID
}
bool Actor::ResolveSavedActorValue(UInt32& avCode)
{
    if (avCode < 0x800) return true;    // don't resolve avCodes < 0x800
    return RSH::TESForm::ResolveSavedFormID(avCode);    // resolve the avCode like a formID
}
// global pointer to player
memaddr SNM(g_playerref,)                   (0x00B333C4,0x0);
Actor** g_playerref = (Actor**)SNM(g_playerref,).addr;
}   //  end namespace RSH