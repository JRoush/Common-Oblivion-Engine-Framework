/* 
    DataHandler class and it's main global instance
    Default manager for forms in Oblivion

    TODO: layout real definition for TESSkill
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/NiTypes/NiTArray.h"

// argument classes
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

// 'faked' base: TESSkill
class TESSkill
{// size 60/A0
    #ifdef OBLIVION
    UInt8   unk[0x60];
    #else
    UInt8   unk[0xA0];
    #endif
};

class TESFileManager
{// size 420/420  TODO - move to TESFile.h
// (as seen in editor by obse team) Not sure if this is a distinct named object, or just a logical grouping of members
public:
    // members
    MEMBER /*000*/ BSSimpleList<void*>        unk00; // general garbage list for unsupported form types?
    MEMBER /*008*/ UInt32                    nextFormID; // next available formID?
    MEMBER /*00C*/ TESFile*                    activeFile;
    MEMBER /*010*/ BSSimpleList<TESFile*>   fileList; // all files in Oblivion\Data\ directory
    MEMBER /*018*/ UInt32                    fileCount; // loaded files
    MEMBER /*01C*/ TESFile*                 filesByID[0xFF]; // loaded files
    MEMBER /*418*/ UInt8                    unk418; // if true, prevents check for changes to masters during load
    MEMBER /*419*/ UInt8                    unk419; // set if there is an active file??
    MEMBER /*41A*/ UInt8                    unk41A; 
    MEMBER /*41B*/ UInt8                    unk41B;
    MEMBER /*41C*/ UInt8                    unk41C; // set when data handler cleared
    MEMBER /*41D*/ UInt8                    unk41D; // set after fileList is initialized?
    MEMBER /*41E*/ UInt8                    unk41E; // set after data handler is constructed
    MEMBER /*41F*/ UInt8                    unk41F; // set after files in LO have been opened, cleared after files have been loaded
};

class IMPORTCLASS TESDataHandler
{// size 0CE0/1220
/*
    Partial:
    -   members
*/
public:

    // members
    MEMBER /*000/0000*/ TESObjectListHead*                  objects;
    MEMBER /*004/0004*/ BSSimpleList<TESPackage*>            packages;
    MEMBER /*00C/000C*/ BSSimpleList<TESWorldSpace*>        worldSpaces;
    MEMBER /*014/0014*/ BSSimpleList<TESClimate*>            climates;
    MEMBER /*01C/001C*/ BSSimpleList<TESWeather*>            weathers;
    MEMBER /*024/0024*/ BSSimpleList<EnchantmentItem*>        enchantmentItems;
    MEMBER /*02C/002C*/ BSSimpleList<SpellItem*>            spellItems;
    MEMBER /*034/0034*/ BSSimpleList<TESHair*>                hairs;
    MEMBER /*03C/003C*/ BSSimpleList<TESEyes*>                eyes;
    MEMBER /*044/0044*/ BSSimpleList<TESRace*>                races;
    MEMBER /*04C/004C*/ BSSimpleList<TESLandTexture*>        landTextures;
    MEMBER /*054/0054*/ BSSimpleList<TESClass*>                classes;
    MEMBER /*05C/005C*/ BSSimpleList<TESFaction*>            factions;
    MEMBER /*064/0064*/ BSSimpleList<Script*>                scripts;
    MEMBER /*06C/006C*/ BSSimpleList<TESSound*>                sounds;
    MEMBER /*074/0074*/ BSSimpleList<TESGlobal*>            globals;
    MEMBER /*07C/007C*/ BSSimpleList<TESTopic*>                topics;
    MEMBER /*084/0084*/ BSSimpleList<TESQuest*>                quests;
    MEMBER /*08C/008C*/ BSSimpleList<BirthSign*>            birthsigns;
    MEMBER /*094/0094*/ BSSimpleList<TESCombatStyle*>        combatStyles;
    MEMBER /*09C/009C*/ BSSimpleList<TESLoadScreen*>        loadScreens;
    MEMBER /*0A4/00A4*/ BSSimpleList<TESWaterForm*>            waterForms;
    MEMBER /*0AC/00AC*/ BSSimpleList<TESEffectShader*>        effectShaders;
    MEMBER /*0B4/00B4*/ BSSimpleList<TESObjectANIO*>        objectAnios;
    MEMBER /*0BC/00BC*/ TESRegionList*                      regionList;
    MEMBER /*0C0/00C0*/ NiTLargeArray<TESObjectCELL*>        cellArray;
    MEMBER /*0D8/00D8*/ TESSkill                            skills[0x15];
    MEMBER /*8B8/0DF8*/ TESFileManager                        fileManager;
    MEMBER /*CD8/1218*/ TESRegionDataManager*               regionDataManager;
    MEMBER /*CDC/121C*/ void*                                unkCDC; // pointer to same object type as an ExtraContainerChanges +0xC

    // methods - serilaization
    IMPORT bool         LoadFile(TESFile& file, bool arg1); // Deserializes file, loading all records with LoadFormRecord()
                        // returns false if any of the records failed to load
    IMPORT bool         LoadFormRecord(TESFile& file, bool arg1); // Creates a form object if necessary, using LoadForm()
                        // select form types are added to data handler with AddForm() or their own tables (settings, etc.)
                        // returns false if record could not be loaded
    IMPORT static bool  LoadForm(TESForm& form, TESFile& file); // Parses a form record into a form object, using TESForm::LoadForm() method
                        // returns false if form could not be loaded
    #ifndef OBLIVION
    IMPORT bool         SaveFile(const char* filename, bool arg1); // calls SaveFormRecord() all forms in TESForm::activeFileFormList
                        // Creates the file & sets as active file if necessary.  returns false if save failed    
    #endif
    IMPORT void         SaveForm(TESForm& form, bool arg1); // serializes form to active file using TESForm::SaveForm()    
    // members - misc
    IMPORT bool         AddFormToHandler(TESForm* form); // Adds form to handler.  Returns false for form types not handled
    IMPORT bool         Clear(); // destroys all objects managed by the data handler.  always returns true
    IMPORT void         CreateBuiltinObjects(); // (re)create built-in forms & objects managed by the data handler

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