/* 
    TESActorBase is the fundamental base form class for actors.
*/
#pragma once

// base classes
#include "API/TESForms/TESObject.h"
#include "API/TESForms/BaseFormComponent.h"
#include "API/TESForms/TESAIForm.h"
#include "API/TESForms/TESAnimation.h"
#include "API/TESForms/TESActorBaseData.h"
#include "API/TESForms/TESContainer.h"
#include "API/Actors/ActorValues.h"

// argument classes
class   TESCombatStyle;

class TESActorBase : public TESBoundAnimObject, public TESActorBaseData, public TESContainer, public TESSpellList, public TESAIForm,
                    public TESHealthForm, public TESAttributes, public TESAnimation, public TESFullName, public TESModel, public TESScriptableForm
{// size 0E4/118
public:

    enum ModifiedFlags
    {
		kModified_AVModifiers       = 0x10000000,
    };

	// members
    //     /*000/000*/ TESBoundAnimObject
	//     /*024/058*/ TESActorBaseData
	//     /*044/078*/ TESContainer
	//     /*054/088*/ TESSpellList
	//     /*068/09C*/ TESAIForm
	//     /*080/0B4*/ TESHealthForm
	//     /*088/0BC*/ TESAttributes
	//     /*094/0C8*/ TESAnimation
	//     /*0A0/0DC*/ TESFullName
	//     /*0AC/0E8*/ TESModel
	//     /*0C4/10C*/ TESScriptableForm
    #ifdef OBLIVION
    MEMBER /*0D0/---*/ AVCollection     baseAVs;
    #endif

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual                  ~TESActorBase(); // also overrides TESModel::~TESModel(), for which
                                                // the compiler automatically generates a thunk redirecting to this method
    #ifdef OBLIVION
    IMPORT /*04C/---*/ virtual UInt16           ModifiedFormSize(UInt32 modifiedMask);
    IMPORT /*050/---*/ virtual void             SaveModifiedForm(UInt32 modifiedMask);
    IMPORT /*054/---*/ virtual void             LoadModifiedForm(UInt32 modifiedMask, UInt32 unkFlags);
    IMPORT /*060/---*/ virtual void             PreLoadModifiedForm(UInt32 modifiedMask);
    #endif

    // TESObject virtual methods    
    IMPORT /*0E4/12C*/ virtual bool             IsObjectAutoCalc();
    IMPORT /*0E8/130*/ virtual void             SetObjectAutoCalc(bool autoCalc);

    // Modified Component virtual methods - override corresponding methods from :
    // TESActorBaseData, TESSpellList, TESAIForm, & TESAttributes
    // These methods are all thunks, rediecting to TESActorBase::ModifiedFormSize, etc.
    #ifdef OBLIVION   
    INLINE /*054/---*/ virtual UInt16           ModifiedComponentSize(UInt32 modifiedMask) {return TESActorBase::ModifiedFormSize(modifiedMask);}
    INLINE /*058/---*/ virtual void             SaveModifiedComponent(UInt32 modifiedMask) {TESActorBase::SaveModifiedForm(modifiedMask);}
    INLINE /*05C/---*/ virtual void             LoadModifiedComponent(UInt32 modifiedMask, UInt32 unkFlags) {TESActorBase::LoadModifiedForm(modifiedMask,unkFlags);}
    #endif

    // TESActorBase virtual methods 
    INLINE /*120/168*/ virtual TESCombatStyle*  GetCombatStyle() {return 0;}
    INLINE /*124/16C*/ virtual void             SetCombatStyle(TESCombatStyle* newStyle) {}
    IMPORT /*128/170*/ virtual SInt32	        GetAViBase(UInt32 actorVal);
	IMPORT /*12C/174*/ virtual float	        GetAVfBase(UInt32 actorVal);
    IMPORT /*130/178*/ virtual void             SetAVfBase(UInt32 actorVal, float value);
	IMPORT /*134/17C*/ virtual void	            SetAViBase(UInt32 actorVal, SInt32 value);
	IMPORT /*138/180*/ virtual void	            ModAVfBase(UInt32 actorVal, float value);
	IMPORT /*13C/184*/ virtual void	            ModAViBase(UInt32 actorVal, SInt32 value);

    // methods
    IMPORT void                 SetHealth(UInt32 value); // calls MarkFormModified
    IMPORT bool                 IsFemale();
    IMPORT bool                 CanWalk();
    IMPORT bool                 CanFly();
    IMPORT bool                 CanSwim();
    IMPORT bool                 CanUseWeaponAndShield();
    IMPORT float                GetEquippableItemRating(TESForm* item); // used to determine 'best' item to equip - higher is better

    // constructor
    IMPORT TESActorBase();

    // use FormHeap for class new & delete - this is passed down to the *entire* TESForm hierarchy
    USEFORMHEAP 
};