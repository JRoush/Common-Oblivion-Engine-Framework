// Redefing macros to export class definitions
#undef IMPORT
#define IMPORT
#undef IMPORTCLASS
#define IMPORTCLASS __declspec(dllexport)

// Game vs CS
#define OBLIVION    
//#undef OBLIVION

#pragma warning (disable: 4251) // template interface needs dll instance blah blah blah

//// included classes
//#include "API/Actors/ActorValues.h"
//#include "API/BSTypes/BSSimpleList.h"
//#include "API/BSTypes/BSStringT.h"
//#include "API/CSDialogs/TESStringInputDialog.h"
//#include "API/CSDialogs/TESDialog.h"
//#include "API/CSDialogs/DialogExtraData.h"
//#include "API/CSDialogs/DialogConstants.h"
//#include "API/ExtraData/BSExtraData.h"
//#include "API/ExtraData/ExtraDataList.h"
//#include "API/GameWorld/TESObjectCELL.h"
//#include "API/Magic/Magic.h"
//#include "API/Magic/EffectSetting.h"
//#include "API/Magic/EffectItem.h"
//#include "API/Magic/MagicTarget.h"
//#include "API/Magic/MagicCaster.h"
//#include "API/Magic/MagicItem.h"
//#include "API/Magic/MagicItemForm.h"
//#include "API/Magic/MagicItemObject.h"
//#include "API/Magic/ActiveEffect.h"
//#include "API/Magic/ActiveEffects.h"
//#include "API/Magic/ValueModifierEffects.h"
//#include "API/Magic/AssociatedItemEffects.h"
//#include "API/NiTypes/NiTArray.h"
//#include "API/NiTypes/NiTList.h"
//#include "API/NiTypes/NiTMap.h"
//#include "API/Settings/Settings.h"
//#include "API/TES/DynamicCast.h"
//#include "API/TES/Mechanics.h"
//#include "API/TES/MemoryHeap.h"
//#include "API/TES/TESDataHandler.h"
//#include "API/TESFiles/TESFile.h"
//#include "API/TESForms/BaseFormComponent.h"
//#include "API/TESForms/TESLeveledList.h"
//#include "API/TESForms/TESContainer.h"
//#include "API/TESForms/TESAIForm.h"
//#include "API/TESForms/TESAnimation.h"
//#include "API/TESForms/TESActorBaseData.h"
//#include "API/TESForms/TESForm.h"
//#include "API/TESForms/TESObject.h"
//#include "API/TESForms/TESObjectREFR.h"

// testfunc
void testfunc()
{
}


