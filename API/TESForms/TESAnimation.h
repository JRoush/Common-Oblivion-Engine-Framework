/* 
    TESAIForm is a base component controlling animation functionality.
*/
#pragma once

// base classes
#include "API/TESForms/BaseFormComponent.h"
#include "API/BSTypes/BSSimpleList.h"

// argument classes
class   KFModel;    // (OBSE,GameTasks.h) may not be an explicitly named object

class IMPORTCLASS TESAnimation : public BaseFormComponent
{// size 0C/14
public:

    typedef BSSimpleList<char*> AnimationListT;
    typedef BSSimpleList<KFModel*> KFModelListT;

    // members
    //     /*00/00*/ void**             vtbl;
    #ifndef OBLIVION
    MEMBER /*--/04*/ KFModelListT       kfModelList; // temporary list of animation models used during dialog editing
    #endif
    MEMBER /*04/0C*/ AnimationListT     animations; // animation names, dynamically allocated    

    // virtual method overrides:
    INLINE /*000/000*/ virtual void         InitializeComponent() {}
    IMPORT /*004/004*/ virtual void         ClearComponentReferences(); // clears animation list (why ?)
    IMPORT /*008/008*/ virtual void         CopyComponentFrom(const BaseFormComponent& source);
    IMPORT /*00C/00C*/ virtual bool         CompareComponentTo(const BaseFormComponent& compareTo) const;
    #ifndef OBLIVION    
    IMPORT /*---/020*/ virtual bool         ComponentDlgMsgCallback(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);                      
    IMPORT /*---/024*/ virtual bool         IsComponentDlgValid(HWND dialog);
    IMPORT /*---/028*/ virtual void         SetComponentInDlg(HWND dialog);
    IMPORT /*---/02C*/ virtual void         GetComponentFromDlg(HWND dialog);
    IMPORT /*---/030*/ virtual void         ComponentDlgCleanup(HWND dialog);
    #endif
    // no additional virtual methods

    // methods - animation list
    IMPORT void         AddAnimation(const char* name); // adds anim to list if not already present (uses strcmp)
    IMPORT bool         HasAnimation(const char* name); // searches list for named animation (uses strcmp)

    // methods - serialization
    #ifndef OBLIVION
    IMPORT void         SaveComponent(UInt32 chunkType = 'ZFFK'); // saves animation names in a single dynamically sized chunk
                        // this method was apparently omitted from the game code; dynamically created actors can't save their anim data
    #endif
    IMPORT void         LoadComponent(TESAnimation& anim, TESFile& file); // load animation list from a 'KFFZ' chunk
                        // the anim argument is does nothing, but cannot be null - pass this*

    // constructor, destructor
    IMPORT TESAnimation();
    INLINE ~TESAnimation() {ClearComponentReferences(); animations.Clear();} // Not found in game/CS (bug? would cause memory leaks) 
};