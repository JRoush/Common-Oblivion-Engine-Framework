/* 
    DataHandler class and it's main global instance
    Default manager for forms in Oblivion
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/NiTypes/NiTArray.h"
#include "API/Actors/TESSkill.h"    // Unlike other form classes, TESSkills are statically allocated by data handler

// argument classes
class   Vector3;            // NiVector3?  currently defined in Utilities/ITypes.h
class   TESObjectListHead;  // TESForms/TESObject.h
class   TESFile;            // TESFiles/TESFile.h
class   TESForm;            // TESForms/TESForm.h
class   TESObject;          // TESForms/TESObject.h
class   TESPackage;
class   TESWorldSpace;
class   TESClimate;
class   TESWeather;
class   EnchantmentItem;    // Magic/MagicItemForm.h
class   SpellItem;          // Magic/MagicItemForm.h
class   TESHair;
class   TESEyes;
class   TESRace;
class   TESLandTexture;
class   TESClass;
class   TESFaction;
class   Script;
class   TESSound;
class   TESGlobal;
class   TESTopic;
class   TESQuest;
class   BirthSign;
class   TESCombatStyle;
class   TESLoadScreen;
class   TESWaterForm;
class   TESEffectShader;
class   TESObjectANIO;
class   TESRegionList;
class   TESObjectCELL;      // GameWorld/TESObjectCELL.h
class   TESRegionDataManager;
class   TESSoulGem;
class   TESObjectSTAT;
class   TESObjectCLOT;
class   TESObjectMISC;
class   TESObjectCONT;
class   TESObjectDOOR;
class   TESObjectREFR;      // TESForms/TESObjectREFR.h
class   ContainerExtraData; // TESForms/TESContainer.h

class IMPORTCLASS TESDataHandler
{// size 0CE0/1220
/*
    Partial:
    -   members
*/
public:

    // members
    MEMBER /*000/0000*/ TESObjectListHead*                  objects;
    MEMBER /*004/0004*/ BSSimpleList<TESPackage*>           packages;
    MEMBER /*00C/000C*/ BSSimpleList<TESWorldSpace*>        worldSpaces;
    MEMBER /*014/0014*/ BSSimpleList<TESClimate*>           climates;
    MEMBER /*01C/001C*/ BSSimpleList<TESWeather*>           weathers;
    MEMBER /*024/0024*/ BSSimpleList<EnchantmentItem*>      enchantmentItems;
    MEMBER /*02C/002C*/ BSSimpleList<SpellItem*>            spellItems;
    MEMBER /*034/0034*/ BSSimpleList<TESHair*>              hairs;
    MEMBER /*03C/003C*/ BSSimpleList<TESEyes*>              eyes;
    MEMBER /*044/0044*/ BSSimpleList<TESRace*>              races;
    MEMBER /*04C/004C*/ BSSimpleList<TESLandTexture*>       landTextures;
    MEMBER /*054/0054*/ BSSimpleList<TESClass*>             classes;
    MEMBER /*05C/005C*/ BSSimpleList<TESFaction*>           factions;
    MEMBER /*064/0064*/ BSSimpleList<Script*>               scripts;
    MEMBER /*06C/006C*/ BSSimpleList<TESSound*>             sounds;
    MEMBER /*074/0074*/ BSSimpleList<TESGlobal*>            globals;
    MEMBER /*07C/007C*/ BSSimpleList<TESTopic*>             topics;
    MEMBER /*084/0084*/ BSSimpleList<TESQuest*>             quests;
    MEMBER /*08C/008C*/ BSSimpleList<BirthSign*>            birthsigns;
    MEMBER /*094/0094*/ BSSimpleList<TESCombatStyle*>       combatStyles;
    MEMBER /*09C/009C*/ BSSimpleList<TESLoadScreen*>        loadScreens;
    MEMBER /*0A4/00A4*/ BSSimpleList<TESWaterForm*>         waterForms;
    MEMBER /*0AC/00AC*/ BSSimpleList<TESEffectShader*>      effectShaders;
    MEMBER /*0B4/00B4*/ BSSimpleList<TESObjectANIO*>        objectAnios;
    MEMBER /*0BC/00BC*/ TESRegionList*                      regionList;
    MEMBER /*0C0/00C0*/ NiTLargeArray<TESObjectCELL*>       cellArray;
    MEMBER /*0D8/00D8*/ TESSkill                            skills[0x15];
    MEMBER /*8B8/0DF8*/ BSSimpleList<void*>                 unk8B8;     // general garbage list for unsupported form types?
    MEMBER /*8C0/0E00*/ UInt32                              nextFormID; // next available formID?
    MEMBER /*8C4/0E04*/ TESFile*                            activeFile;
    MEMBER /*8C8/0E08*/ BSSimpleList<TESFile*>              fileList;   // all files in Oblivion\Data\ directory
    MEMBER /*8D0/0E10*/ UInt32                              fileCount;  // loaded files
    MEMBER /*8D4/0E14*/ TESFile*                            filesByID[0xFF]; // loaded files
    MEMBER /*CD0/1210*/ UInt8                               unkCD0;     // if true, prevents check for changes to masters during load
    MEMBER /*CD1/1211*/ UInt8                               unkCD1;     // set if there is an active file??
    MEMBER /*CD2/1212*/ UInt8                               unkCD2; 
    MEMBER /*CD3/1213*/ UInt8                               unkCD3;
    MEMBER /*CD4/1214*/ UInt8                               unkCD4;     // set when data handler cleared
    MEMBER /*CD5/1215*/ UInt8                               unkCD5;     // set after fileList is initialized?
    MEMBER /*CD6/1216*/ UInt8                               unkCD6;     // set after data handler is constructed
    MEMBER /*CD7/1217*/ UInt8                               unkCD7;     // set after files in LO have been opened, cleared after files have been loaded
    MEMBER /*CD8/1218*/ TESRegionDataManager*               regionDataManager;
    MEMBER /*CDC/121C*/ ContainerExtraData*                 unkCDC; 

    // methods - serilaization
    IMPORT bool             LoadFile(TESFile& file, bool arg1); // Deserializes file, loading all records with LoadFormRecord()
                            // returns false if any of the records failed to load
    IMPORT bool             LoadFormRecord(TESFile& file, bool arg1); // Creates a form object if necessary, using LoadForm()
                            // select form types are added to data handler with AddForm() or their own tables (settings, etc.)
                            // returns false if record could not be loaded
    IMPORT static bool      LoadForm(TESForm& form, TESFile& file); // Parses a form record into a form object, using TESForm::LoadForm() method
                            // returns false if form could not be loaded
    #ifndef OBLIVION
    IMPORT bool             SaveFile(const char* filename, bool saveAsESM); // calls SaveFormRecord() all forms in TESForm::activeFileFormList
                            // Creates the file & sets as active file if necessary.  returns false if save failed
                            // if saveAsESM is true, saves all loaded forms regardless of their "Active" status (decoded by shadeMe)
    #endif
    IMPORT void             SaveForm(TESForm& form, bool arg1); // serializes form to active file using TESForm::SaveForm()    
    
    // members - misc
    IMPORT static TESForm*  CreateForm(UInt8 formType); // form 'Factory' - creates a new form by type, for supported types
    IMPORT bool             AddFormToHandler(TESForm* form); // Adds form to handler.  Returns false for form types not handled
    IMPORT UInt32           ReserveNextUnusedFormID(); // gets next formID, and marks it as used    
    IMPORT TESSkill*        GetSkillByCode(UInt32 skillCode); // gets skill by code (performs bounds checks)
    IMPORT bool             Clear(); // destroys all objects managed by the data handler.  always returns true
    IMPORT void             CreateBuiltinObjects(); // (re)create built-in forms & objects managed by the data handler
    IMPORT TESObjectREFR*   PlaceObjectRef(TESObject* baseObject, Vector3& position, Vector3& rotation, 
                                            TESObjectCELL* cell, TESWorldSpace* worldSpace, TESObjectREFR* existingRef); // 
                            // place an object ref at the specified position in the specified cell/worldspace, with the specified base form.
                            // if existingRef is provided, it is used as the ref, otherwise a new ref is created.  returns null on  failure

    // constructor, destructor
    IMPORT TESDataHandler();
    IMPORT ~TESDataHandler();

    // use FormHeap for class new & delete
    USEFORMHEAP 

    // Static global instance
    IMPORT static TESDataHandler*   dataHandler;

    // pointer to select build-in forms (not all builtin forms have a pointer)
    IMPORT static TESObjectREFR*    playerRef; // player ref is a bit special; it seems to be managed by the datahandler in CS, but elsewhere in game
    IMPORT static TESSoulGem*       azuraStone;
    IMPORT static TESSoulGem*       blackSoulGem;
    IMPORT static TESFaction*       creatureFaction;
    IMPORT static TESWaterForm*     defaultWater;
    IMPORT static TESObjectSTAT*    divineMarker;
    IMPORT static TESObjectSTAT*    doorMarker;    
    IMPORT static TESEyes*          eyeReanimate;
    IMPORT static TESObjectSTAT*    horseMarker;
    IMPORT static TESObjectCLOT*    jailPants;
    IMPORT static TESObjectCLOT*    jailShirt;
    IMPORT static TESObjectCLOT*    jailShoes;
    IMPORT static TESObjectMISC*    lockpick;
    IMPORT static TESObjectCONT*    lootBag;
    IMPORT static TESObjectSTAT*    mapMarker;
    IMPORT static TESObjectSTAT*    northMarker;    
    IMPORT static TESObjectDOOR*    prisonMarker;
    IMPORT static TESObjectMISC*    repairHammer;
    IMPORT static TESObjectMISC*    skeletonKey;
    IMPORT static TESObjectCONT*    stolenGoods;
    IMPORT static TESObjectSTAT*    templeMarker;
    IMPORT static TESObjectSTAT*    travelMarker;
    IMPORT static TESRace*          vampireRace;
    IMPORT static TESObjectMISC*    varlaStone;
    IMPORT static TESObjectMISC*    welkyndStone;
    IMPORT static TESObjectSTAT*    xMarker;
    IMPORT static TESObjectSTAT*    xMarkerHeading;
};                                                              