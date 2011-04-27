/* 
    SettingCollection is an abstract interface for managing settings from a common source (file, registery, etc.)
    INISettingCollection and it's descendents manage settings from the assorted INI files
    RegSettingCollection managed windows registry values.
    GameSettingCollection manages the configurable game settings.

    NOTE:
    SettingCollection and it's abstract descendents are actually templated to accept arbitrary data types.  The only template
    parameter used is 'Setting', so functionally the classes defined here are identical to those defined in the game/CS.
    However, as with many of the template classes reproduced (rather than imported) in COEF, dynamic casting may not work
    as expected with these types.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/NiTypes/NiTArray.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/BSTypes/BSTCaseInsensitiveStringMap.h"

// argument classes
class   Setting;    // Settings/Settings.h
class   TESFile;    // TESFiles/TESFile.h

class IMPORTCLASS SettingCollection // actually SettingCollection<Setting*>
{// size 10C
public:

    // members
    //     /*000*/ void**           vtbl;
    MEMBER /*004*/ char             filename[0x104]; // full path of ini file for ini settings, ignored for other collections
    MEMBER /*108*/ void*            fileObject; // actual type depends on class, but must be nonzero when file is open
                                    // TESFile* for gmst, HKEY for RegSetting, this* pointer for INISetting

    // virtual methods
    INLINE /*000*/ virtual void         AddSetting(Setting& setting) = 0;
    INLINE /*004*/ virtual void         RemoveSetting(Setting& setting) = 0;
    INLINE /*008*/ virtual UInt32       BuildFormattedValueArray(NiTArray<char*>& outputArray) = 0; // returns number of settings
                                        // builds a formatted string output of settings table, one array entry for each line
    INLINE /*00C*/ virtual bool         SaveSetting(Setting& setting) = 0; // opens/closes file where applicable
    INLINE /*010*/ virtual bool         LoadSetting(Setting& setting) = 0; // opens/closes file where applicable
    INLINE /*014*/ virtual bool         OpenFile(bool create) { return false; } // opens source for load/save.  returns false on failure
    INLINE /*018*/ virtual bool         CloseFile() { return true; } //  closes source after load/save.  returns false on failure
    IMPORT /*01C*/ virtual bool         SaveAllSettings(); // must open/close file manually.  default function is a stub that does nothing.
    IMPORT /*020*/ virtual bool         LoadAllSettings(); // must open/close file manually.  default function is a stub that does nothing

    // constructor, destructor
    INLINE SettingCollection() : fileObject(0) { filename[0] = 0; }
    INLINE ~SettingCollection() {}

    // use form heap
    USEFORMHEAP
};

class IMPORTCLASS SettingCollectionList : public SettingCollection  // actually SettingCollectionList<Setting*>
{ // size 114
public:

    typedef BSSimpleList<Setting*> SettingListT;

    // members
    //     /*000*/ SettingCollection
    MEMBER /*10C*/ SettingListT     settingList;

    // virtual method overrides
    IMPORT /*000*/ virtual void         AddSetting(Setting& setting);
    IMPORT /*004*/ virtual void         RemoveSetting(Setting& setting);
    IMPORT /*008*/ virtual UInt32       BuildFormattedValueArray(NiTArray<char*>& outputArray); // CTDs for empty collections
    IMPORT /*01C*/ virtual bool         SaveAllSettings(); // CTDs for empty collections
    IMPORT /*020*/ virtual bool         LoadAllSettings(); // CTDs for empty collections
    
    // constructor, destructor
    INLINE SettingCollectionList() : SettingCollection(), settingList(0) {}
    INLINE ~SettingCollectionList() { settingList.Clear(); }
};

class IMPORTCLASS INISettingCollection : public SettingCollectionList
{ // size 114
public:

    // fileName holds the ini file name
    // fileObject holds the this* pointer when file is "open", and is zero otherwise
    // file is not actually opened; all i/o uses Get/WritePrivateProfileString()

    // virtual method overrides
    IMPORT /*008*/ virtual UInt32       BuildFormattedValueArray(NiTArray<char*>& outputArray);
    IMPORT /*00C*/ virtual bool         SaveSetting(Setting& setting);
    IMPORT /*010*/ virtual bool         LoadSetting(Setting& setting);
    IMPORT /*014*/ virtual bool         OpenFile(bool create);
    IMPORT /*018*/ virtual bool         CloseFile();

    // methods - extracting section/key name from setting names using ':' as a delimiter
    IMPORT static void          GetSettingSectionName(Setting& setting, char* sectionNameBuffer);
    IMPORT static void          GetSettingKeyName(Setting& setting, char* keyNameBuffer);
    
    // constructor, destructor
    INLINE INISettingCollection() : SettingCollectionList() {}
    INLINE ~INISettingCollection() {}    
    
    // static global instance
    IMPORT static INISettingCollection  INISettings;
};

class IMPORTCLASS LipSynchroSettingCollection : public INISettingCollection
{ // size 114
public:
    // no additional members
    // no virtual method overrides

    // static global instance
    IMPORT static LipSynchroSettingCollection   LipSynchroSettings;
};

#ifdef OBLIVION

class IMPORTCLASS BlendSettingCollection : public INISettingCollection
{ // size 114
public:
    // no additional members
    // no virtual method overrides

    // static global instance
    IMPORT static BlendSettingCollection   BlendSettings;
};

#else

class IMPORTCLASS REGINISettingCollection : public INISettingCollection
{ // size 114
public:
    // no additional members
    // no virtual method overrides

    // static global instance
    IMPORT static REGINISettingCollection   REGINISettings;
};

#endif

class IMPORTCLASS RegSettingCollection : public SettingCollectionList
{ // size 114
public:

    // fileName holds the registry key name, from HKEY_LOCAL_MACHINE
    // fileObject holds an HKEY to the target registery key.

    // virtual method overrides
    IMPORT /*008*/ virtual UInt32       BuildFormattedValueArray(NiTArray<char*>& outputArray);
    IMPORT /*00C*/ virtual bool         SaveSetting(Setting& setting);
    IMPORT /*010*/ virtual bool         LoadSetting(Setting& setting);
    IMPORT /*014*/ virtual bool         OpenFile(bool create);  // creates/opens reg key
    IMPORT /*018*/ virtual bool         CloseFile(); // closes reg key
    
    // constructor, destructor
    INLINE RegSettingCollection() : SettingCollectionList() {}
    INLINE ~RegSettingCollection() {}    
    
    // static global instance
    IMPORT static RegSettingCollection  RegSettings;
};

class IMPORTCLASS SettingCollectionMap : public SettingCollection   // actually SettingCollectionMap<Setting*>
{// size 120
public:

    typedef BSTCaseInsensitiveStringMap<Setting*> SettingMapT;

    // members
    //     /*000*/ SettingCollection
    MEMBER /*10C*/ SettingMapT      settingMap;

    // virtual method overrides
    IMPORT /*000*/ virtual void         AddSetting(Setting& setting);
    IMPORT /*004*/ virtual void         RemoveSetting(Setting& setting);
    IMPORT /*008*/ virtual UInt32       BuildFormattedValueArray(NiTArray<char*>& outputArray);
    IMPORT /*01C*/ virtual bool         SaveAllSettings();
    IMPORT /*020*/ virtual bool         LoadAllSettings();

    // constructor, destructor
    IMPORT SettingCollectionMap(UInt32 hashSize);
    IMPORT ~SettingCollectionMap();
};


class IMPORTCLASS GameSettingCollection : public SettingCollectionMap
{// size 120 
public:

    // fileName seems to be ignored
    // fileObject holds a TESFile* for loading/saving the setting

    // virtual method overrides
    IMPORT /*008*/ virtual UInt32       BuildFormattedValueArray(NiTArray<char*>& outputArray);
    IMPORT /*00C*/ virtual bool         SaveSetting(Setting& setting);
    IMPORT /*010*/ virtual bool         LoadSetting(Setting& setting);
    IMPORT /*014*/ virtual bool         OpenFile(bool create);  // fileObject must be set manually, this method just checks that it is nonzero
    IMPORT /*018*/ virtual bool         CloseFile(); // zeros fileObject field

    // additional virtual methods
    IMPORT /*024*/ virtual bool         LoadSetting(TESFile& file, const char* settingName); // finds setting in map and loads from file

    // constructor, destructor
    INLINE GameSettingCollection(UInt32 hashSize) : SettingCollectionMap(hashSize) {}
    INLINE ~GameSettingCollection() {}    
    
    // static global instance
    IMPORT static GameSettingCollection  GameSettings;
};

