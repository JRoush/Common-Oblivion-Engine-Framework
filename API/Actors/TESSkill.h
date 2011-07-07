/* 
    TESSkill is the base class for actor skills.
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h" // TESDescription, TESTexture
#include "API/TESForms/TESForm.h"

// argument classes

class IMPORTCLASS TESSkill : public TESFormIDListView, public TESDescription, public TESTexture
{// size 60/A0
public:

    /// members
    //     /*00/00*/ TESFormIDListView
	//     /*18/24*/ TESDescription
	//     /*20/34*/ TESTexture
    MEMBER /*2C/4C*/ UInt32         skill;          // init to ActorValues::kActorVal__UBOUND
    MEMBER /*30/50*/ UInt32         attribute;      // init to 0
    MEMBER /*34/54*/ UInt32         specialization; // init to 0
    MEMBER /*38/58*/ float          useValue[2];    // experience earned per action (up to 2 unique actions per skill). actions are hardcoded into engine
    MEMBER /*40/60*/ TESDescription masteryDesc[4]; // description of skill perk for Apprentice mastery and above  (ActorValues::SkillMasteryLevels + 1);

    // TESFormIDListView virtual methods overrides:
    IMPORT /*010/034*/ virtual              ~TESSkill(); 
    IMPORT /*014/038*/ virtual void         InitializeAllComponents();
    IMPORT /*01C/040*/ virtual bool         LoadForm(TESFile& file); 
    IMPORT /*024/048*/ virtual void         SaveFormChunks();     
    IMPORT /*0B4/0B8*/ virtual void         CopyFrom(TESForm& form);
    IMPORT /*0B8/0BC*/ virtual bool         CompareTo(TESForm& compareTo);
    #ifndef OBLIVION    
    IMPORT /*---/10C*/ virtual bool         DialogMessageCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog); 
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog); 
    IMPORT /*---/11C*/ virtual void         CleanupDialog(HWND dialog); 
    #endif

    // no TESTexture or TESDescription virtual method overrides
    // no additional virtual methods

    // methods - mastery descriptions
    #ifdef OBLIVION
    IMPORT const char*          GetMasteryDescription(UInt32 masteryLevel); // returns sNoviceSkillLevelText for novice level, empty string for invalid levels
    #endif

    // constuctor, initializor
    IMPORT TESSkill();
    IMPORT void                 ClearTESSkill();  // init skill-specific members, clear file list and FromActiveFile flag. 
                                // called from InitializeAllComponents().  this method does more than the standard InitializeAllComponents()
                                // for a form class.  Probably needed because skills, unlike all other forms, are statically allocated in the
                                // data handler and thus are not reconstructed every time the data handler is cleared.
};