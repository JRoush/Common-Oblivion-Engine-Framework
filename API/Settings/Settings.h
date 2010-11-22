/* 
    Setting is a common base for game settings, ini settings, etc.  Basically just name+value pairs.
    GameSetting is the class for all generic variables used in the game mechanics, e.g. "fActorLuckSkillMult"
    GameSetting derives directly from setting in the game, but also from TESForm in the CS, so it has an unusual definition
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/TESForms/TESForm.h"   // TESFormIDListView

class Setting
{// size 08/08
/*
    Partial:
    -   Type enumeration
*/
public:

    enum SettingTypes {
		kSetting_Bool           = 0x0,  // 'b' size 1
		kSetting_Char           = 0x1,  // 'c' size 1
		kSetting_UnsignedChar   = 0x2,  // 'h' size 1
		kSetting_SignedInt      = 0x3,  // 'i' size 4
		kSetting_UnsignedInt    = 0x4,  // 'u' size 4
		kSetting_Float          = 0x5,  // 'f' size 4
		kSetting_String         = 0x6,  // 'S'/'s' size indet. See note below^^
		kSetting_r              = 0x7,  // 'r' size 4
		kSetting_a              = 0x8,  // 'a' size 4
		kSetting__MAX           = 0x9
	};

    // union for various value types
    union SettingValue
	{
		bool        b;
        char        c;
        UInt8       h;
        SInt32      i;
		UInt32	    u;
		float	    f;
		const char* s;
        UInt32	    r;
        UInt32	    a;
	};

    // members
    MEMBER /*00/00*/ SettingValue       value;
    MEMBER /*04/04*/ const char*        name;   // must begin with one of the type characters

    // methods
    IMPORT void             SetStringValue(const char* newValue); // only for string settings.  See note below^^        
    IMPORT static UInt32    GetTypeFromName(const char* settingName); // parse first character in name to get type code

    // ^^ Note on string settings: the prefix 's' is for string settings with statically allocated name + value, used to initialize the setting.
    // The prefix 'S' is for string settings with name+value space allocated from the heap by SetStringValue.  Calling SetStringValue will
    // change the prefix from 's' to 'S' automatically.  Do NOT use 'S' to initialize setting names.

    // constructor, destructor - apparently completely inlined into ctor, dtor of derived classes
    INLINE Setting(const char* nName, bool nValue) : name(nName) {value.b = nValue;}
    INLINE Setting(const char* nName, char nValue) : name(nName) {value.c = nValue;}
    INLINE Setting(const char* nName, UInt8 nValue) : name(nName) {value.h = nValue;}
    INLINE Setting(const char* nName, SInt32 nValue) : name(nName) {value.i = nValue;}
    INLINE Setting(const char* nName, UInt32 nValue) : name(nName) {value.u = nValue;}
    INLINE Setting(const char* nName, float nValue) : name(nName) {value.f = nValue;}
    INLINE Setting(const char* nName, const char* nValue) : name(nName) {value.s = nValue;}
    INLINE ~Setting() {if (name && name[0] == 'S') MemoryHeap::FormHeapFree((void*)name);}

    // use FormHeap for class new & delete
    USEFORMHEAP
};

#ifdef OBLIVION

class GameSetting : public Setting
{// size 08/2C
public:

    // members
    //     /*00/--*/ Setting       

    // construction, destruction - add/remove from global gmst table
    IMPORT GameSetting(const char* nName, bool nValue);
    IMPORT GameSetting(const char* nName, char nValue);
    IMPORT GameSetting(const char* nName, UInt8 nValue);
    IMPORT GameSetting(const char* nName, SInt32 nValue);
    IMPORT GameSetting(const char* nName, UInt32 nValue);
    IMPORT GameSetting(const char* nName, float nValue);
    IMPORT GameSetting(const char* nName, const char* nValue);
    IMPORT ~GameSetting();
};

#else

class GameSetting : public TESFormIDListView, public Setting
{// size 08/2C
public:
    
    // members
    //     /*--/00*/ TESForm
    //     /*--/24*/ Setting    

    // TESForm virtual methods
    IMPORT /*010/034*/ virtual 	            ~GameSetting(); // remove from global gmst table
    IMPORT /*01C/040*/ virtual bool	        LoadForm(TESFile& file);
    IMPORT /*024/048*/ virtual void	        SaveFormChunks();
    IMPORT /*0B4/0B8*/ virtual void	        CopyFrom(TESForm& form);
	IMPORT /*0B8/0BC*/ virtual bool	        CompareTo(TESForm& compareTo);
    IMPORT /*---/114*/ virtual void         SetInDialog(HWND dialog);
    IMPORT /*---/118*/ virtual void         GetFromDialog(HWND dialog);
    IMPORT /*---/11C*/ virtual void         CleanupDialog(HWND dialog);

    // construction - add to global gmst table
    IMPORT GameSetting(const char* nName, bool nValue);
    IMPORT GameSetting(const char* nName, char nValue);
    IMPORT GameSetting(const char* nName, UInt8 nValue);
    IMPORT GameSetting(const char* nName, SInt32 nValue);
    IMPORT GameSetting(const char* nName, UInt32 nValue);
    IMPORT GameSetting(const char* nName, float nValue);
    IMPORT GameSetting(const char* nName, const char* nValue);

    // use FormHeap for class new & delete
    // necessary because it inherits overloaded new/delete from both parents, leading to ambiguity
    USEFORMHEAP
};

#endif

/*
class GameSettingsCollection
{// size 120/120  
public:
    GameSettingsCollection();
    ~GameSettingsCollection();
    typedef NiTStringPointerMap<Setting*> GmstMapT;     // should be NiTCaseInsensitiveStringMap
    // members
    //void*             vtbl;                   // 000/000
    UInt32              unk04[(0x108-0x004)/4]; // 004/004
    TESFile*            loadFile;               // 108/108  seems to be cached ptr to modfile for current load?
    GmstMapT            settingsMap;            // 10C/10C  
    // virtual methods
    virtual void        AddSetting(Setting* setting);               // 000/000
    virtual void        RemoveSetting(Setting* setting);            // 004/004
    virtual UInt32      Unk008(UInt32 arg0);                        // 008/008
    virtual bool        Unk00C(UInt32 arg0);                        // 00C/00C  does nothing, returns false
    virtual void        LoadSetting(GameSetting* gmst);             // 010/010  loads setting from file cached in loadfile
    virtual bool        ValidFile(UInt32 arg0);                     // 014/014  returns true if loadFile is non null
    virtual bool        ClearFile();                                // 018/018  nulls out loadFile, returns true
    virtual bool        Unk01C();                                   // 01C/01C  does nothing if no valid file
    virtual bool        Unk020();                                   // 020/020  does nothing if no valid file
    virtual bool        Load(TESFile* file, const char* editorID);  // 024/024  sets loadFile, looks up existing gmst, calls LoadSetting()
    // methods
    static void         Initialize();
};

// the one and only instance of GameSettingsCollection
extern GameSettingsCollection*  g_GMSTCollection;
*/