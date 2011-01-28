/*  
    These macros define the constants used to identify various CS dialog resoruces and their controls.

    These constants were probably symbolic in the original C++ code, but being macros they where 'baked' into 
    the code as integer literals during compilation.  
    
    It appears that control identifiers for inheritable classes were intentionally chosen so that they would not 
    overlap, meaning that a class can use the unodified GetFrom/SetIn/ValidateDialog methods of it's base clases 
    to manage their respective controls.  
    
    SO - use these values where appropriate when creating custom dialogs (e.g. if a class inherits from TESFullName,
    assign the Edit control for the name to IDC_FULL_NAME).  Conversely, when designing custom dialogs that don't 
    use vanilla code, avoid using these identifiers to prevent potential conflicts.
*/

//------------------------------------ Form Components ---------------------------------------

// TESFullName
#define IDC_FULL_NAME               0x424 // EDIT

// TESDescription
#define IDC_DESC_DESCRIPTION        0x472 // EDIT

// TESTexture
#define IDC_TXTR_PATH               0x414 // BUTTON
#define IDC_TXTR_IMAGE              0x422 // BUTTON

// TESModel
#define IDC_MODL_PATH               0x413 // BUTTON

// TESScriptableForm
#define IDC_SCPF_SCRIPT             0x4CA // COMBOBOX
#define IDC_SCPF_EDITSCRIPT         0x426 // BUTTON

// TESUsesForm
#define IDC_USEF_USES               0x434 // EDIT

// TESValueForm
#define IDC_VALF_VALUE              0x405 // EDIT

// TESHealthForm
#define IDC_HTHF_HEALTH             0x400 // EDIT

// TESWeightForm
#define IDC_WGHF_WEIGHT             0x5C6 // EDIT

// TESQualityForm
#define IDC_QUAF_QUALITY            0x42B // EDIT

// TESAttackDamageForm
#define IDC_ADGF_ATTACKDAMAGE       0x3F7 // EDIT

// TESAttributes
#define IDC_ATTR_STRENGTH           0x640 // EDIT
#define IDC_ATTR_INTELLIGENCE       0x641 // EDIT
#define IDC_ATTR_WILLPOWER          0x642 // EDIT
#define IDC_ATTR_AGILITY            0x643 // EDIT
#define IDC_ATTR_SPEED              0x644 // EDIT
#define IDC_ATTR_ENDURANCE          0x645 // EDIT
#define IDC_ATTR_PERSONALITY        0x646 // EDIT
#define IDC_ATTR_LUCK               0x647 // EDIT

// TESLeveledList
#define IDC_LVLS_LEVELEDLIST        0x7F4 // SYSLISTVIEW32
#define IDC_LVLS_CHANCENONE         0x406 // EDIT
#define IDC_LVLS_CALCFORALLLEVELS   0x3EF // CHECKBOX
#define IDC_LVLS_CALCFOREACHINCOUNT 0x3EE // CHECKBOX
#define IDC_LVLS_LEVOBJECTSELECT    0x3FD // COMBOBOX
#define IDC_LVLS_LEVOBJECTLEVEL     0x427 // EDIT
#define IDC_LVLS_LEVOBJECTCOUNT     0x402 // EDIT

// TESForm
#define IDC_FORM_EDITORID           0x404 // EDIT
#define IDC_FORM_QUESTITEM          0x693 // BUTTON

// TESFormIDListView
#define IDC_FILV_FORMLIST           0x810 // SYSLISTVIEW32

//------------------------------------- TESObjectREFR --------------------------------------

// TESObjectREFR - MainDialog
#define IDD_REFR                            0x08D // DIALOG
#define IDC_REFR_EDITORIDLABEL              0x62E // STATIC
#define IDC_REFR_FORMID                     0x631 // STATIC
#define IDC_REFR_BASEFORMLABEL              0x62F // STATIC
#define IDC_REFR_BASEFORM                   0x630 // STATIC
#define IDC_REFR_EDITBASE                   0x5D1 // BUTTON
#define IDC_REFR_TABS                       0x6F1 // SYSTABCONTROL32
#define IDC_REFR_PERSISTENTREFERENCE        0x3EE // BUTTON
#define IDC_REFR_TURNOFFFIRE                0x3EF // BUTTON
#define IDC_REFR_VISIBLEWHENDISTANT         0x3F0 // BUTTON
#define IDC_REFR_INITIALLYDISABLED          0xFB3 // BUTTON
#define IDC_REFR_OPENBYDEFAULT              0xFB7 // BUTTON
#define IDC_REFR_CASTSSHADOWS               0x697 // BUTTON
#define IDC_REFR_SPACELABEL                 0x632 // STATIC
#define IDC_REFR_SPACE                      0x633 // STATIC
// TESObjectREFR - Locked Tab
#define IDD_REFR_LOCKTAB                    0x101 // DIALOG
#define IDC_REFR_LOCKED                     0x3F1 // BUTTON
#define IDC_REFR_LOCKKEY                    0x428 // COMBOBOX
#define IDC_REFR_LOCKLEVELED                0x3F2 // BUTTON
#define IDC_REFR_LOCKLEVEL                  0x401 // COMBOBOX
// TESObjectREFR - Teleport Tab
#define IDD_REFR_TELEPORTTAB                0x102 // DIALOG
#define IDC_REFR_TELEPORT                   0x5D0 // BUTTON
#define IDC_REFR_SELECTTELEPORTREF          0x7BE // BUTTON
#define IDC_REFR_TELEPORTCELL               0x42A // COMBOBOX
#define IDC_REFR_TELEPORTREF                0x7BF // COMBOBOX
#define IDC_REFR_VIEWTELEPORTDOOR           0x7C1 // BUTTON
#define IDC_REFR_VIEWTELEPORTMARKER         0x7C0 // BUTTON
// TESObjectREFR - Ownership Tab
#define IDD_REFR_OWNERSHIPTAB               0x103 // DIALOG
#define IDC_REFR_OWNERNPC                   0x3FD // COMBOBOX
#define IDC_REFR_OWNERGLOBALVARIABLE        0x3FE // COMBOBOX
#define IDC_REFR_OWNERFACTION               0x3FF // COMBOBOX
#define IDC_REFR_OWNERFACTIONRANK           0x491 // COMBOBOX
// TESObjectREFR - Extra Data Tab
#define IDD_REFR_EXTRADATATAB               0x104 // DIALOG
#define IDC_REFR_EXTRACOUNTLABEL            0x62E // STATIC
#define IDC_REFR_EXTRACOUNT                 0x402 // EDIT
#define IDC_REFR_EXTRACOUNTDEFAULT          0x3FA // BUTTON
#define IDC_REFR_EXTRAHEALTHLABEL           0x62F // STATIC
#define IDC_REFR_EXTRAHEALTH                0x400 // EDIT
#define IDC_REFR_EXTRAHEALTHDEFAULT         0x3FB // BUTTON
#define IDC_REFR_EXTRACHARGELABEL           0x62D // STATIC
#define IDC_REFR_EXTRACHARGE                0x43D // EDIT
#define IDC_REFR_EXTRACHARGEDEFAULT         0x3F9 // BUTTON
#define IDC_REFR_EXTRATIMELEFTLABEL         0x630 // STATIC
#define IDC_REFR_EXTRATIMELEFT              0x435 // EDIT
#define IDC_REFR_EXTRATIMELEFTDEFAULT       0x3FD // BUTTON
#define IDC_REFR_EXTRASOUL                  0x4AD // COMBOBOX
#define IDC_REFR_EXTRASOULDEFAULT           0x3FC // BUTTON
// TESObjectREFR - Marker Tab
#define IDD_REFR_MARKERTAB                  0xC97 // DIALOG
#define IDC_REFR_MARKERDATA                 0x869 // BUTTON
#define IDC_REFR_MARKERNAME                 0x424 // EDIT
#define IDC_REFR_MARKERCAMP                 0x40A // BUTTON
#define IDC_REFR_MARKERCAVE                 0x40B // BUTTON
#define IDC_REFR_MARKERMINE                 0x40F // BUTTON
#define IDC_REFR_MARKERELVENRUIN            0x40D // BUTTON
#define IDC_REFR_MARKERFORTRUIN             0x40E // BUTTON
#define IDC_REFR_MARKERLANDMARK             0x410 // BUTTON
#define IDC_REFR_MARKERTAVERN               0x411 // BUTTON
#define IDC_REFR_MARKERCITY                 0x40C // BUTTON
#define IDC_REFR_MARKERSETTLEMENT           0x412 // BUTTON
#define IDC_REFR_MARKERSHRINE               0x413 // BUTTON
#define IDC_REFR_MARKERVISIBLE              0x86A // BUTTON
#define IDC_REFR_MARKERFASTTRAVEL           0x402 // BUTTON
#define IDC_REFR_MARKEROBLIVIONGATE         0x414 // BUTTON
// TESObjectREFR - Enable Parent Tab
#define IDD_REFR_ENABLEPARENTTAB            0xCB0 // DIALOG
#define IDC_REFR_SELECTENABLEPARENTREF      0x8D9 // BUTTON
#define IDC_REFR_ENABLEPARENTCELL           0x8DA // COMBOBOX
#define IDC_REFR_ENABLEPARENTREF            0x8DB // COMBOBOX
#define IDC_REFR_VIEWENABLEPARENTREF        0x8D8 // BUTTON
#define IDC_REFR_ENABLEPARENTLABEL          0x8DC // STATIC
#define IDC_REFR_ENABLEPARENTOPPOSITESTATE  0xFB4 // BUTTON
// TESObjectREFR - Leveled Creature Tab
#define IDD_REFR_LEVELEDCREATURETAB         0xCBB // DIALOG
#define IDC_REFR_LEVELEDCREATURELEVELMOD    0x95A // EDIT

//------------------------------------- Magic --------------------------------------

// EffectSetting
#define IDD_MGEF                            0x0A3 // DIALOG
#define IDC_MGEF_FLAG_SPELLMAKING           0x3EE // BUTTON
#define IDC_MGEF_FLAG_ENCHANTING            0x3EF // BUTTON
#define IDC_MGEF_FLAG_HOSTILE               0x3F0 // BUTTON
#define IDC_MGEF_FLAG_DETRIMENTAL           0x3F1 // BUTTON
#define IDC_MGEF_FLAG_RECOVER               0x3F2 // BUTTON
#define IDC_MGEF_FLAG_MAGNITUDEISPERCENT    0x3F3 // BUTTON
#define IDC_MGEF_FLAG_FXPERSIST             0x3F4 // BUTTON
#define IDC_MGEF_FLAG_ONSELF                0x3F5 // BUTTON
#define IDC_MGEF_FLAG_ONTOUCH               0x3F6 // BUTTON
#define IDC_MGEF_FLAG_ONTARGET              0x3F7 // BUTTON
#define IDC_MGEF_FLAG_NODURATION            0x3F8 // BUTTON
#define IDC_MGEF_FLAG_NOMAGNITUDE           0x3F9 // BUTTON
#define IDC_MGEF_FLAG_NOAREA                0x3FA // BUTTON
#define IDC_MGEF_FLAG_NOINGREDIENT          0x3FB // BUTTON
#define IDC_MGEF_FLAG_USEWEAPON             0x3FC // BUTTON
#define IDC_MGEF_FLAG_USEARMOR              0x3FD // BUTTON
#define IDC_MGEF_FLAG_USECREATURE           0x3FE // BUTTON
#define IDC_MGEF_FLAG_USESKILL              0x3FF // BUTTON
#define IDC_MGEF_FLAG_USEATTRIBUTE          0x400 // BUTTON
#define IDC_MGEF_FLAG_NORECAST              0x402 // BUTTON
#define IDC_MGEF_FLAG_NOHITEFFECT           0x403 // BUTTON
#define IDC_MGEF_PROJECTILESPEED            0x425 // EDIT
#define IDC_MGEF_SCHOOL                     0x441 // COMBOBOX
#define IDC_MGEF_ASSOCIATEDITEM             0x443 // COMBOBOX
#define IDC_MGEF_RESISTVALUE                0x444 // COMBOBOX
#define IDC_MGEF_PROJECTILETYPE             0x445 // COMBOBOX
#define IDC_MGEF_LIGHT                      0x446 // COMBOBOX
#define IDC_MGEF_CASTINGSOUND               0x447 // COMBOBOX
#define IDC_MGEF_BOLTSOUND                  0x448 // COMBOBOX
#define IDC_MGEF_HITSOUND                   0x44A // COMBOBOX
#define IDC_MGEF_AREASOUND                  0x44B // COMBOBOX
#define IDC_MGEF_ICONIMAGEFRAME             0x460 // STATIC        
#define IDC_MGEF_ENCHANTMENTFACTOR          0x4C6 // EDIT
#define IDC_MGEF_BARTERFACTOR               0x4C7 // EDIT
#define IDC_MGEF_EFFECTSHADER               0x5E6 // COMBOBOX        
#define IDC_MGEF_COUNTEREFFECTS             0x811 // SYSLISTVIEW32
#define IDC_MGEF_BASECOST                   0x812 // EDIT
#define IDC_MGEF_ENCHANTSHADER              0x877 // COMBOBOX

// EffectItem
#define IDD_EFIT                            0x10B // DIALOG
#define IDC_EFIT_EFFECT                     0x819 // COMBOBOX
#define IDC_EFIT_ACTORVALUE                 0x816 // COMBOBOX
#define IDC_EFIT_RANGE                      0x818 // COMBOBOX
#define IDC_EFIT_AREA                       0x81A // EDIT
#define IDC_EFIT_DURATION                   0x5F4 // EDIT
#define IDC_EFIT_MAGNITUDE                  0x81B // EDIT
#define IDC_EFIT_EFFECTCOST                 0x81D // STATIC
#define IDC_EFIT_ACTORVALUELABEL            0x815 // STATIC
#define IDC_EFIT_SCRIPTEFFECTINFOFRAME      0x822 // BUTTON
#define IDC_EFIT_SCRIPTLABEL                0x81F // STATIC
#define IDC_EFIT_SCRIPT                     0x4CA // COMBOBOX
#define IDC_EFIT_EDITSCRIPT                 0x426 // BUTTON
#define IDC_EFIT_NAMELABEL                  0x820 // STATIC
#define IDC_EFIT_NAME                       0x817 // EDIT
#define IDC_EFIT_SCHOOLLABEL                0x821 // STATIC
#define IDC_EFIT_SCHOOL                     0x441 // COMBOBOX
#define IDC_EFIT_MAGICITEMCOST              0x81E // STATIC
#define IDC_EFIT_EFFECTBASECOST             0x81C // STATIC
#define IDC_EFIT_MAGICITEMMASTERYLABEL      0x826 // STATIC
#define IDC_EFIT_MAGICITEMMASTERY           0x823 // STATIC
#define IDC_EFIT_VFX                        0x824 // COMBOBOX
#define IDC_EFIT_HOSTILITY                  0x3EE // BUTTON

// EffectItemList
#define IDC_EILS_EFFECTLIST                 0x815 // SYSLISTVIEW32

// EnchantmentItem
#define IDD_ENCH                            0x0A6 // DIALOG
#define IDC_ENCH_CASTINGTYPE                0x439 // COMBOBOX
#define IDC_ENCH_CHARGEAMOUNT               0x43D // EDIT
#define IDC_ENCH_ENCHANTMENTCOST            0x55E // EDIT
#define IDC_ENCH_AUTOCALC                   0x3EE // BUTTON
#define IDC_ENCH_ENCHANTMENTCOSTLABEL       0x686 // STATIC

// SpellItem
#define IDD_SPEL                            0x117 // DIALOG
#define IDC_SPEL_PCSTARTSPELL               0x3EF // BUTTON
#define IDC_SPEL_SPELLCOST                  0x50F // EDIT
#define IDC_SPEL_AUTOCALC                   0x3EE // BUTTON
#define IDC_SPEL_SPELLCOSTLABEL             0x687 // STATIC
#define IDC_SPEL_SPELLTYPE                  0x41A // COMBOBOX
#define IDC_SPEL_MASTERYLEVEL               0x823 // COMBOBOX
#define IDC_SPEL_SCHOOL                     0x827 // STATIC
#define IDC_SPEL_IMMUNETOSILENCE            0x3F0 // BUTTON
#define IDC_SPEL_AOEIGNORESLOS              0x3F1 // BUTTON
#define IDC_SPEL_SCRIPTEFFECTALWAYSAPPLIES  0x3F2 // BUTTON
#define IDC_SPEL_NOREFLECTORABSORB          0x3F3 // BUTTON
#define IDC_SPEL_TOUCHEXPLODESWITHOUTTARGET 0x3F4 // BUTTON

// IngreditentItem
#define IDD_INGR                            0xCAF // DIALOG
#define IDC_INGR_ICONFRAME                  0x460 // STATIC        
#define IDC_INGR_AUTOCALC                   0x4BF // BUTTON
#define IDC_INGR_INGREDIENTVALUE            0x43C // EDIT
#define IDC_INGR_POISON                     0x82B // STATIC
#define IDC_INGR_EDIBLE                     0x694 // BUTTON
    
// Alchemy Item
#define IDD_ALCH                            0x0A5 // DIALOG
#define IDC_ALCH_ICONFRAME                  0x460 // STATIC        
#define IDC_ALCH_AUTOCALC                   0x4BF // BUTTON
#define IDC_ALCH_POTIONVALUE                0x43C // EDIT
#define IDC_ALCH_POISON                     0x82B // STATIC
#define IDC_ALCH_EDIBLE                     0x694 // BUTTON

//------------------------------------- Magic --------------------------------------

// TESStringInputDialog
#define IDD_STRINGINPUT                     0x0AE // DIALOG
#define IDC_STRINGINPUT_STRING              0x3E8 // EDIT
