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
#define IDC_FULL_NAME               1060 // EDIT

// TESDescription
#define IDC_DESC_DESCRIPTION        1138 // EDIT

// TESTexture
#define IDC_TXTR_PATH               1044 // BUTTON
#define IDC_TXTR_IMAGE              1058 // BUTTON

// TESModel
#define IDC_MODL_PATH               1043 // BUTTON

// TESScriptableForm
#define IDC_SCPF_SCRIPT             1226 // COMBOBOX
#define IDC_SCPF_EDITSCRIPT         1062 // BUTTON

// TESUsesForm
#define IDC_USEF_USES               1076 // EDIT

// TESValueForm
#define IDC_VALF_VALUE              1029 // EDIT

// TESHealthForm
#define IDC_HTHF_HEALTH             1024 // EDIT

// TESWeightForm
#define IDC_WGHF_WEIGHT             1478 // EDIT

// TESQualityForm
#define IDC_QUAF_QUALITY            1067 // EDIT

// TESAttackDamageForm
#define IDC_ADGF_ATTACKDAMAGE       1015 // EDIT

// TESAttributes
#define IDC_ATTR_STRENGTH           1600 // EDIT
#define IDC_ATTR_INTELLIGENCE       1601 // EDIT
#define IDC_ATTR_WILLPOWER          1602 // EDIT
#define IDC_ATTR_AGILITY            1603 // EDIT
#define IDC_ATTR_SPEED              1604 // EDIT
#define IDC_ATTR_ENDURANCE          1605 // EDIT
#define IDC_ATTR_PERSONALITY        1606 // EDIT
#define IDC_ATTR_LUCK               1607 // EDIT

// TESLeveledList
#define IDC_LVLS_LEVELEDLIST        2036 // SYSLISTVIEW32
#define IDC_LVLS_CHANCENONE         1030 // EDIT
#define IDC_LVLS_CALCFORALLLEVELS   1007 // CHECKBOX
#define IDC_LVLS_CALCFOREACHINCOUNT 1006 // CHECKBOX
#define IDC_LVLS_LEVOBJECTSELECT    1021 // COMBOBOX
#define IDC_LVLS_LEVOBJECTLEVEL     1063 // EDIT
#define IDC_LVLS_LEVOBJECTCOUNT     1026 // EDIT

// TESContainer
#define IDC_CONT_CONTENTS           2035 // SYSLISTVIEW32
#define IDC_CONT_PREVIEWLVL         1064 // EDIT
#define IDC_CONT_PREVIEW            2331 // BUTTON

// TESForm
#define IDC_FORM_EDITORID           1028 // EDIT
#define IDC_FORM_QUESTITEM          1683 // BUTTON

// TESFormIDListView
#define IDC_FILV_FORMLIST           2064 // SYSLISTVIEW32

//------------------------------------- TESObjectREFR --------------------------------------

// TESObjectREFR - MainDialog
#define IDD_REFR                             141 // DIALOG
#define IDC_REFR_EDITORIDLABEL              1582 // STATIC
#define IDC_REFR_FORMID                     1585 // STATIC
#define IDC_REFR_BASEFORMLABEL              1583 // STATIC
#define IDC_REFR_BASEFORM                   1584 // STATIC
#define IDC_REFR_EDITBASE                   1489 // BUTTON
#define IDC_REFR_TABS                       1777 // SYSTABCONTROL32
#define IDC_REFR_PERSISTENTREFERENCE        1006 // BUTTON
#define IDC_REFR_TURNOFFFIRE                1007 // BUTTON
#define IDC_REFR_VISIBLEWHENDISTANT         1008 // BUTTON
#define IDC_REFR_INITIALLYDISABLED          4019 // BUTTON
#define IDC_REFR_OPENBYDEFAULT              4023 // BUTTON
#define IDC_REFR_CASTSSHADOWS               1687 // BUTTON
#define IDC_REFR_SPACELABEL                 1586 // STATIC
#define IDC_REFR_SPACE                      1587 // STATIC
// TESObjectREFR - Locked Tab
#define IDD_REFR_LOCKTAB                     257 // DIALOG
#define IDC_REFR_LOCKED                     1009 // BUTTON
#define IDC_REFR_LOCKKEY                    1064 // COMBOBOX
#define IDC_REFR_LOCKLEVELED                1010 // BUTTON
#define IDC_REFR_LOCKLEVEL                  1025 // COMBOBOX
// TESObjectREFR - Teleport Tab
#define IDD_REFR_TELEPORTTAB                 258 // DIALOG
#define IDC_REFR_TELEPORT                   1488 // BUTTON
#define IDC_REFR_SELECTTELEPORTREF          1982 // BUTTON
#define IDC_REFR_TELEPORTCELL               1066 // COMBOBOX
#define IDC_REFR_TELEPORTREF                1983 // COMBOBOX
#define IDC_REFR_VIEWTELEPORTDOOR           1985 // BUTTON
#define IDC_REFR_VIEWTELEPORTMARKER         1984 // BUTTON
// TESObjectREFR - Ownership Tab
#define IDD_REFR_OWNERSHIPTAB                259 // DIALOG
#define IDC_REFR_OWNERNPC                   1021 // COMBOBOX
#define IDC_REFR_OWNERGLOBALVARIABLE        1022 // COMBOBOX
#define IDC_REFR_OWNERFACTION               1023 // COMBOBOX
#define IDC_REFR_OWNERFACTIONRANK           1169 // COMBOBOX
// TESObjectREFR - Extra Data Tab
#define IDD_REFR_EXTRADATATAB                260 // DIALOG
#define IDC_REFR_EXTRACOUNTLABEL            1582 // STATIC
#define IDC_REFR_EXTRACOUNT                 1026 // EDIT
#define IDC_REFR_EXTRACOUNTDEFAULT          1018 // BUTTON
#define IDC_REFR_EXTRAHEALTHLABEL           1583 // STATIC
#define IDC_REFR_EXTRAHEALTH                1024 // EDIT
#define IDC_REFR_EXTRAHEALTHDEFAULT         1019 // BUTTON
#define IDC_REFR_EXTRACHARGELABEL           1581 // STATIC
#define IDC_REFR_EXTRACHARGE                1085 // EDIT
#define IDC_REFR_EXTRACHARGEDEFAULT         1017 // BUTTON
#define IDC_REFR_EXTRATIMELEFTLABEL         1584 // STATIC
#define IDC_REFR_EXTRATIMELEFT              1077 // EDIT
#define IDC_REFR_EXTRATIMELEFTDEFAULT       1021 // BUTTON
#define IDC_REFR_EXTRASOUL                  1197 // COMBOBOX
#define IDC_REFR_EXTRASOULDEFAULT           1020 // BUTTON
// TESObjectREFR - Marker Tab
#define IDD_REFR_MARKERTAB                  3223 // DIALOG
#define IDC_REFR_MARKERDATA                 2153 // BUTTON
#define IDC_REFR_MARKERNAME                 1060 // EDIT
#define IDC_REFR_MARKERCAMP                 1034 // BUTTON
#define IDC_REFR_MARKERCAVE                 1035 // BUTTON
#define IDC_REFR_MARKERMINE                 1039 // BUTTON
#define IDC_REFR_MARKERELVENRUIN            1037 // BUTTON
#define IDC_REFR_MARKERFORTRUIN             1038 // BUTTON
#define IDC_REFR_MARKERLANDMARK             1040 // BUTTON
#define IDC_REFR_MARKERTAVERN               1041 // BUTTON
#define IDC_REFR_MARKERCITY                 1036 // BUTTON
#define IDC_REFR_MARKERSETTLEMENT           1042 // BUTTON
#define IDC_REFR_MARKERSHRINE               1043 // BUTTON
#define IDC_REFR_MARKERVISIBLE              2154 // BUTTON
#define IDC_REFR_MARKERFASTTRAVEL           1026 // BUTTON
#define IDC_REFR_MARKEROBLIVIONGATE         1044 // BUTTON
// TESObjectREFR - Enable Parent Tab
#define IDD_REFR_ENABLEPARENTTAB            3248 // DIALOG
#define IDC_REFR_SELECTENABLEPARENTREF      2265 // BUTTON
#define IDC_REFR_ENABLEPARENTCELL           2266 // COMBOBOX
#define IDC_REFR_ENABLEPARENTREF            2267 // COMBOBOX
#define IDC_REFR_VIEWENABLEPARENTREF        2264 // BUTTON
#define IDC_REFR_ENABLEPARENTLABEL          2268 // STATIC
#define IDC_REFR_ENABLEPARENTOPPOSITESTATE  4020 // BUTTON
// TESObjectREFR - Leveled Creature Tab
#define IDD_REFR_LEVELEDCREATURETAB         3259 // DIALOG
#define IDC_REFR_LEVELEDCREATURELEVELMOD    2394 // EDIT

//------------------------------------- Magic --------------------------------------

// EffectSetting
#define IDD_MGEF                             163 // DIALOG
#define IDC_MGEF_FLAG_SPELLMAKING           1006 // BUTTON
#define IDC_MGEF_FLAG_ENCHANTING            1007 // BUTTON
#define IDC_MGEF_FLAG_HOSTILE               1008 // BUTTON
#define IDC_MGEF_FLAG_DETRIMENTAL           1009 // BUTTON
#define IDC_MGEF_FLAG_RECOVER               1010 // BUTTON
#define IDC_MGEF_FLAG_MAGNITUDEISPERCENT    1011 // BUTTON
#define IDC_MGEF_FLAG_FXPERSIST             1012 // BUTTON
#define IDC_MGEF_FLAG_ONSELF                1013 // BUTTON
#define IDC_MGEF_FLAG_ONTOUCH               1014 // BUTTON
#define IDC_MGEF_FLAG_ONTARGET              1015 // BUTTON
#define IDC_MGEF_FLAG_NODURATION            1016 // BUTTON
#define IDC_MGEF_FLAG_NOMAGNITUDE           1017 // BUTTON
#define IDC_MGEF_FLAG_NOAREA                1018 // BUTTON
#define IDC_MGEF_FLAG_NOINGREDIENT          1019 // BUTTON
#define IDC_MGEF_FLAG_USEWEAPON             1020 // BUTTON
#define IDC_MGEF_FLAG_USEARMOR              1021 // BUTTON
#define IDC_MGEF_FLAG_USECREATURE           1022 // BUTTON
#define IDC_MGEF_FLAG_USESKILL              1023 // BUTTON
#define IDC_MGEF_FLAG_USEATTRIBUTE          1024 // BUTTON
#define IDC_MGEF_FLAG_NORECAST              1026 // BUTTON
#define IDC_MGEF_FLAG_NOHITEFFECT           1027 // BUTTON
#define IDC_MGEF_PROJECTILESPEED            1061 // EDIT
#define IDC_MGEF_SCHOOL                     1089 // COMBOBOX
#define IDC_MGEF_ASSOCIATEDITEM             1091 // COMBOBOX
#define IDC_MGEF_RESISTVALUE                1092 // COMBOBOX
#define IDC_MGEF_PROJECTILETYPE             1093 // COMBOBOX
#define IDC_MGEF_LIGHT                      1094 // COMBOBOX
#define IDC_MGEF_CASTINGSOUND               1095 // COMBOBOX
#define IDC_MGEF_BOLTSOUND                  1096 // COMBOBOX
#define IDC_MGEF_HITSOUND                   1098 // COMBOBOX
#define IDC_MGEF_AREASOUND                  1099 // COMBOBOX
#define IDC_MGEF_ICONIMAGEFRAME             1120 // STATIC        
#define IDC_MGEF_ENCHANTMENTFACTOR          1222 // EDIT
#define IDC_MGEF_BARTERFACTOR               1223 // EDIT
#define IDC_MGEF_EFFECTSHADER               1510 // COMBOBOX        
#define IDC_MGEF_COUNTEREFFECTS             2065 // SYSLISTVIEW32
#define IDC_MGEF_BASECOST                   2066 // EDIT
#define IDC_MGEF_ENCHANTSHADER              2167 // COMBOBOX

// EffectItem
#define IDD_EFIT                             267 // DIALOG
#define IDC_EFIT_EFFECT                     2073 // COMBOBOX
#define IDC_EFIT_ACTORVALUE                 2070 // COMBOBOX
#define IDC_EFIT_RANGE                      2072 // COMBOBOX
#define IDC_EFIT_AREA                       2074 // EDIT
#define IDC_EFIT_DURATION                   1524 // EDIT
#define IDC_EFIT_MAGNITUDE                  2075 // EDIT
#define IDC_EFIT_EFFECTCOST                 2077 // STATIC
#define IDC_EFIT_ACTORVALUELABEL            2069 // STATIC
#define IDC_EFIT_SCRIPTEFFECTINFOFRAME      2082 // BUTTON
#define IDC_EFIT_SCRIPTLABEL                2079 // STATIC
#define IDC_EFIT_SCRIPT                     1226 // COMBOBOX
#define IDC_EFIT_EDITSCRIPT                 1062 // BUTTON
#define IDC_EFIT_NAMELABEL                  2080 // STATIC
#define IDC_EFIT_NAME                       2071 // EDIT
#define IDC_EFIT_SCHOOLLABEL                2081 // STATIC
#define IDC_EFIT_SCHOOL                     1089 // COMBOBOX
#define IDC_EFIT_MAGICITEMCOST              2078 // STATIC
#define IDC_EFIT_EFFECTBASECOST             2076 // STATIC
#define IDC_EFIT_MAGICITEMMASTERYLABEL      2086 // STATIC
#define IDC_EFIT_MAGICITEMMASTERY           2083 // STATIC
#define IDC_EFIT_VFX                        2084 // COMBOBOX
#define IDC_EFIT_HOSTILITY                  1006 // BUTTON

// EffectItemList
#define IDC_EILS_EFFECTLIST                 2069 // SYSLISTVIEW32

// EnchantmentItem
#define IDD_ENCH                             166 // DIALOG
#define IDC_ENCH_CASTINGTYPE                1081 // COMBOBOX
#define IDC_ENCH_CHARGEAMOUNT               1085 // EDIT
#define IDC_ENCH_ENCHANTMENTCOST            1374 // EDIT
#define IDC_ENCH_AUTOCALC                   1006 // BUTTON
#define IDC_ENCH_ENCHANTMENTCOSTLABEL       1670 // STATIC

// SpellItem
#define IDD_SPEL                            279 // DIALOG
#define IDC_SPEL_PCSTARTSPELL               1007 // BUTTON
#define IDC_SPEL_SPELLCOST                  1295 // EDIT
#define IDC_SPEL_AUTOCALC                   1006 // BUTTON
#define IDC_SPEL_SPELLCOSTLABEL             1671 // STATIC
#define IDC_SPEL_SPELLTYPE                  1050 // COMBOBOX
#define IDC_SPEL_MASTERYLEVEL               2083 // COMBOBOX
#define IDC_SPEL_SCHOOL                     2087 // STATIC
#define IDC_SPEL_IMMUNETOSILENCE            1008 // BUTTON
#define IDC_SPEL_AOEIGNORESLOS              1009 // BUTTON
#define IDC_SPEL_SCRIPTEFFECTALWAYSAPPLIES  1010 // BUTTON
#define IDC_SPEL_NOREFLECTORABSORB          1011 // BUTTON
#define IDC_SPEL_TOUCHEXPLODESWITHOUTTARGET 1012 // BUTTON

// IngreditentItem
#define IDD_INGR                            3247 // DIALOG
#define IDC_INGR_ICONFRAME                  1120 // STATIC        
#define IDC_INGR_AUTOCALC                   1215 // BUTTON
#define IDC_INGR_INGREDIENTVALUE            1084 // EDIT
#define IDC_INGR_POISON                     2091 // STATIC
#define IDC_INGR_EDIBLE                     1684 // BUTTON
    
// Alchemy Item
#define IDD_ALCH                             165 // DIALOG
#define IDC_ALCH_ICONFRAME                  1120 // STATIC        
#define IDC_ALCH_AUTOCALC                   1215 // BUTTON
#define IDC_ALCH_POTIONVALUE                1084 // EDIT
#define IDC_ALCH_POISON                     2091 // STATIC
#define IDC_ALCH_EDIBLE                     1684 // BUTTON

//------------------------------------- Magic --------------------------------------

// TESStringInputDialog
#define IDD_STRINGINPUT                      174 // DIALOG
#define IDC_STRINGINPUT_STRING              1000 // EDIT
