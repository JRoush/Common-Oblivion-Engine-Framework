#include "rsh\CS\CS_Common.h"
#include "rsh\GameForms\FormBase.h"
#include "rsh\GameObjects\Actor.h"
#include "rsh\Utilities.h"

#include <cstdarg>

namespace RSH {

/*************************  CS  ******************************/
#define SNM(method,variable) CS##_##method##_##variable
memaddr SNM(Instance,)                  (0x0       ,0x00A0AF1C);    // CS HINSTANCE
memaddr SNM(HWnd,)                      (0x0       ,0x00A0AF20);    // CS Parent Window HWND
memaddr SNM(PopulateComboWithForms,)    (0x0       ,0x004456F0);
memaddr SNM(PopulateComboWithScripts,)  (0x0       ,0x00445150);
memaddr SNM(PopulateComboWithAVs,)      (0x0       ,0x00442430);
memaddr SNM(ComboLookupByData,)         (0x0       ,0x00403600);
memaddr SNM(ChooseIconFile,)            (0x0       ,0x00446CA0);
memaddr SNM(ChooseModelFile,)           (0x0       ,0x00446C60);
memaddr SNM(StringInputDlgProc,)        (0x0       ,0x00441D90);
static const UInt32 StringInputDlgTemplate = 0xAE;
const char* NULL_ENTRY_STR = " [NONE]";
// CS functions
HWND CS_hWnd() 
{
    return *(HWND*)SNM(HWnd,).addr;
}
HINSTANCE CS_hInstance() 
{
    return *(HINSTANCE*)SNM(Instance,).addr;
}
// input box for new editor id's
int GetInputString(const char* title, char* buffer, UInt32 bufferlength)
{
    // make sure a valid text buffer was passed
    if (!buffer || bufferlength == 0) return false;
    // initialize lparam struct
    struct LParam
    {
        const char* title;
        UInt32      bufferlength;
        char*       buffer;
    };
    LParam lparam = {title,bufferlength,buffer};
    buffer[0] = 0;
    // show dialog
    return DialogBoxParamA(CS_hInstance(),MAKEINTRESOURCE(StringInputDlgTemplate),CS_hWnd(),(DLGPROC)SNM(StringInputDlgProc,).addr,(LPARAM)&lparam);    
}
void PopulateComboWithForms(HWND hCtrl, UInt8 formType, bool clearFirst, bool NullEntry)
{
    SNM(PopulateComboWithForms,).cdeclCall(hCtrl,formType,clearFirst,NullEntry);
}
void PopulateComboWithScripts(HWND hCtrl, bool nullEntry, bool OnlyQuestScripts, bool OnlyEffectScripts)
{
    SNM(PopulateComboWithScripts,).cdeclCall(hCtrl,nullEntry,OnlyQuestScripts,OnlyEffectScripts);
}
void PopulateComboWithAVs(HWND hCtrl, bool clearFirst, bool NullEntry)
{
    SNM(PopulateComboWithAVs,).cdeclCall(hCtrl,clearFirst,false);
    if (NullEntry)
    {
        // ensure that ' NONE ' entry to uses standard 'INVALID' avCode        
        CComboBox* cmb = (CComboBox*)CWnd::FromHandle(hCtrl);
        int idx = cmb->AddString(NULL_ENTRY_STR);
        cmb->SetItemData(idx,Actor::kActorVal__NONE);
    }
}
int ComboLookupByData(HWND hCtrl, UInt32 itemData)
{
    return (int)SNM(ComboLookupByData,).cdeclCall(hCtrl,itemData);
}

// control utilities
int CList_Lookup(CListBox* list, UInt32 itemData)
{//for looking up list items by their data
    for (int c = list->GetCount()-1; c >= 0; c--)
    {
        if (itemData == list->GetItemData(c)) return c;
    }
    return LB_ERR;
}
int CCombo_Lookup(CComboBox* list, UInt32 itemData)
{//for looking up combo items by their data
    for (int c = list->GetCount()-1; c >= 0; c--)
    {
        if (itemData == list->GetItemData(c)) return c;
    }
    return LB_ERR;
}
// DataFile dialog
bool ChooseIconFile(HWND parent, char* buffer, UInt32 buffersize)
{
    return 0 != SNM(ChooseIconFile,).cdeclCall(parent,
                        "Data\\Textures\\Menus\\Icons\\",   // path relative to Oblivion directory
                        NULL,                               // filter, in format 'Description (*.extension)'
                        buffer,buffersize);
}
bool ChooseModelFile(HWND parent, char* buffer, UInt32 buffersize)
{
    return 0 != SNM(ChooseModelFile,).cdeclCall(parent,
                        "Data\\Meshes\\",       // path relative to Oblivion directory
                        buffer,buffersize);
}
/*************************  MSGDESC  ******************************/
static const char * const MessageTypeNames[WM_USER + 1] = 
{
    "WM_NULL",
    "WM_CREATE",
    "WM_DESTROY",
    "WM_MOVE",
    "WM_SIZEWAIT",
    "WM_SIZE",
    "WM_ACTIVATE",
    "WM_SETFOCUS",
    "WM_KILLFOCUS",
    "WM_SETVISIBLE",
    "WM_ENABLE",
    "WM_SETREDRAW",
    "WM_SETTEXT",
    "WM_GETTEXT",
    "WM_GETTEXTLENGTH",
    "WM_PAINT",
    "WM_CLOSE",			/* 0x10 */
    "WM_QUERYENDSESSION",
    "WM_QUIT",
    "WM_QUERYOPEN",
    "WM_ERASEBKGND",
    "WM_SYSCOLORCHANGE",
    "WM_ENDSESSION",
    "WM_SYSTEMERROR",
    "WM_SHOWWINDOW",
    "WM_CTLCOLOR",
    "WM_WININICHANGE",
    "WM_DEVMODECHANGE",
    "WM_ACTIVATEAPP",
    "WM_FONTCHANGE",
    "WM_TIMECHANGE",
    "WM_CANCELMODE",
    "WM_SETCURSOR",		/* 0x20 */
    "WM_MOUSEACTIVATE",
    "WM_CHILDACTIVATE",
    "WM_QUEUESYNC",
    "WM_GETMINMAXINFO",
    "WM_LOGOFF",
    "WM_PAINTICON",
    "WM_ICONERASEBKGND",
    "WM_NEXTDLGCTL",
    "WM_ALTTABACTIVE",
    "WM_SPOOLERSTATUS",
    "WM_DRAWITEM",
    "WM_MEASUREITEM",
    "WM_DELETEITEM",
    "WM_VKEYTOITEM",
    "WM_CHARTOITEM",
    "WM_SETFONT",		/* 0x30 */
    "WM_GETFONT",
    "WM_SETHOTKEY",
    "WM_GETHOTKEY",
    "WM_FILESYSCHANGE",
    "WM_ISACTIVEICON",
    "WM_QUERYPARKICON",
    "WM_QUERYDRAGICON",
    "WM_WINHELP",
    "WM_COMPAREITEM",
    "WM_FULLSCREEN",
    "WM_CLIENTSHUTDOWN",
    "WM_DDEMLEVENT",
    "WM_GETOBJECT",
    NULL,
    "WM_CALCSCROLL",
    "WM_TESTING", 		        /* 0x40 */
    "WM_COMPACTING",
    "WM_OTHERWINDOWCREATED",
    "WM_OTHERWINDOWDESTROYED",
    "WM_COMMNOTIFY", 
    "WM_MEDIASTATUSCHANGE",
    "WM_WINDOWPOSCHANGING",	/* 0x0046 */
    "WM_WINDOWPOSCHANGED",	/* 0x0047 */
    "WM_POWER", 
    "WM_COPYGLOBALDATA",
    "WM_COPYDATA",
    "WM_CANCELJOURNAL",
    "WM_LOGONNOTIFY",
    "WM_KEYF1",
    "WM_NOTIFY",
    "WM_ACCESS_WINDOW",
    /* 0x0050 */
    "WM_INPUTLANGCHANGEREQUEST",
    "WM_INPUTLANGCHANGE",
    "WM_TCARD",
    "WM_HELP",
    "WM_USERCHANGED",
    "WM_NOTIFYFORMAT",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0060 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0070 */
    "WM_FINALDESTROY",
    "WM_MEASUREITEM_CLIENTDATA",
    "WM_TASKACTIVATED",
    "WM_TASKDEACTIVATED",
    "WM_TASKCREATED", 
    "WM_TASKDESTROYED",
    "WM_TASKUICHANGED",
    "WM_TASKVISIBLE",
    "WM_TASKNOTVISIBLE",
    "WM_SETCURSORINFO",
    NULL,
    "WM_CONTEXTMENU",
    "WM_STYLECHANGING",
    "WM_STYLECHANGED",
    "WM_DISPLAYCHANGE",
    "WM_GETICON",

    "WM_SETICON",		/* 0x0080 */
    "WM_NCCREATE",		/* 0x0081 */
    "WM_NCDESTROY",		/* 0x0082 */
    "WM_NCCALCSIZE",		/* 0x0083 */
    "WM_NCHITTEST",        	/* 0x0084 */
    "WM_NCPAINT",          	/* 0x0085 */
    "WM_NCACTIVATE",       	/* 0x0086 */
    "WM_GETDLGCODE",		/* 0x0087 */
    "WM_SYNCPAINT",
    "WM_SYNCTASK",
    NULL,
    "WM_KLUDGEMINRECT",
    "WM_LPKDRAWSWITCHWND",
    NULL,
    NULL,
    NULL,

    /* 0x0090 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x00A0 */
    "WM_NCMOUSEMOVE",		/* 0x00A0 */
    "WM_NCLBUTTONDOWN",		/* 0x00A1 */
    "WM_NCLBUTTONUP",		/* 0x00A2 */
    "WM_NCLBUTTONDBLCLK",	/* 0x00A3 */
    "WM_NCRBUTTONDOWN",		/* 0x00A4 */
    "WM_NCRBUTTONUP",		/* 0x00A5 */
    "WM_NCRBUTTONDBLCLK",	/* 0x00A6 */
    "WM_NCMBUTTONDOWN",		/* 0x00A7 */
    "WM_NCMBUTTONUP",		/* 0x00A8 */
    "WM_NCMBUTTONDBLCLK",	/* 0x00A9 */
    NULL, 
    "WM_NCXBUTTONDOWN",
    "WM_NCXBUTTONUP",
    "WM_NCXBUTTONDBLCLK",
    "WM_NCUAHDRAWCAPTION", /* 0x00AE */
    "WM_NCUAHDRAWFRAME", /* 0x00AF */

    /* 0x00B0 - Win32 Edit controls */
    "EM_GETSEL32",              /* 0x00b0 */
    "EM_SETSEL32",              /* 0x00b1 */
    "EM_GETRECT32",             /* 0x00b2 */
    "EM_SETRECT32",             /* 0x00b3 */
    "EM_SETRECTNP32",           /* 0x00b4 */
    "EM_SCROLL32",              /* 0x00b5 */
    "EM_LINESCROLL32",          /* 0x00b6 */
    "EM_SCROLLCARET32",         /* 0x00b7 */
    "EM_GETMODIFY32",           /* 0x00b8 */
    "EM_SETMODIFY32",           /* 0x00b9 */
    "EM_GETLINECOUNT32",        /* 0x00ba */
    "EM_LINEINDEX32",           /* 0x00bb */
    "EM_SETHANDLE32",           /* 0x00bc */
    "EM_GETHANDLE32",           /* 0x00bd */
    "EM_GETTHUMB32",            /* 0x00be */
    NULL,                       /* 0x00bf */

    NULL,                       /* 0x00c0 */
    "EM_LINELENGTH32",          /* 0x00c1 */
    "EM_REPLACESEL32",          /* 0x00c2 */
    "EM_SETFONT"                /* 0x00c3 */
    "EM_GETLINE32",             /* 0x00c4 */
    "EM_LIMITTEXT32",           /* 0x00c5 */
    "EM_CANUNDO32",             /* 0x00c6 */
    "EM_UNDO32",                /* 0x00c7 */
    "EM_FMTLINES32",            /* 0x00c8 */
    "EM_LINEFROMCHAR32",        /* 0x00c9 */
    "EM_SETWORDBREAK",          /* 0x00ca */
    "EM_SETTABSTOPS32",         /* 0x00cb */
    "EM_SETPASSWORDCHAR32",     /* 0x00cc */
    "EM_EMPTYUNDOBUFFER32",     /* 0x00cd */
    "EM_GETFIRSTVISIBLELINE32", /* 0x00ce */
    "EM_SETREADONLY32",         /* 0x00cf */

    "EM_SETWORDBREAKPROC32",    /* 0x00d0 */
    "EM_GETWORDBREAKPROC32",    /* 0x00d1 */
    "EM_GETPASSWORDCHAR32",     /* 0x00d2 */
    "EM_SETMARGINS32",          /* 0x00d3 */
    "EM_GETMARGINS32",          /* 0x00d4 */
    "EM_GETLIMITTEXT32",        /* 0x00d5 */
    "EM_POSFROMCHAR32",         /* 0x00d6 */
    "EM_CHARFROMPOS32",         /* 0x00d7 */
    "EM_SETIMESTATUS",
    "EM_GETIMESTATUS",
    "EM_MSGMAX",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x00E0 - Win32 Scrollbars */
    "SBM_SETPOS32",             /* 0x00e0 */
    "SBM_GETPOS32",             /* 0x00e1 */
    "SBM_SETRANGE32",           /* 0x00e2 */
    "SBM_GETRANGE32",           /* 0x00e3 */
    "SBM_ENABLE_ARROWS32",      /* 0x00e4 */
    NULL,
    "SBM_SETRANGEREDRAW32",     /* 0x00e6 */
    NULL,
    NULL,
    "SBM_SETSCROLLINFO32",      /* 0x00e9 */
    "SBM_GETSCROLLINFO32",      /* 0x00ea */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x00F0 - Win32 Buttons */
    "BM_GETCHECK32",            /* 0x00f0 */
    "BM_SETCHECK32",            /* 0x00f1 */
    "BM_GETSTATE32",            /* 0x00f2 */
    "BM_SETSTATE32",            /* 0x00f3 */
    "BM_SETSTYLE32",            /* 0x00f4 */
    "BM_CLICK32",               /* 0x00f5 */
    "BM_GETIMAGE32",            /* 0x00f6 */
    "BM_SETIMAGE32",            /* 0x00f7 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL, 
    "WM_INPUT",
    "WM_KEYDOWN",		/* 0x0100 */
    "WM_KEYUP",			/* 0x0101 */
    "WM_CHAR",			/* 0x0102 */
    "WM_DEADCHAR",		/* 0x0103 */
    "WM_SYSKEYDOWN",		/* 0x0104 */
    "WM_SYSKEYUP",		/* 0x0105 */
    "WM_SYSCHAR",		/* 0x0106 */
    "WM_SYSDEADCHAR",		/* 0x0107 */
    "WM_YOMICHAR",		/* 0x0108 */
    "WM_UNICHAR",
    "WM_CONVERTREQUEST",
    "WM_CONVERTRESULT",
    "WM_INTERIM",
    "WM_IME_STARTCOMPOSITION",
    "WM_IME_ENDCOMPOSITION",
    "WM_IME_COMPOSITION",
    "WM_INITDIALOG",		/* 0x0110 */
    "WM_COMMAND",		/* 0x0111 */
    "WM_SYSCOMMAND",       	/* 0x0112 */
    "WM_TIMER",			/* 0x0113 */
    "WM_HSCROLL",		/* 0x0114 */
    "WM_VSCROLL",		/* 0x0115 */
    "WM_INITMENU",              /* 0x0116 */
    "WM_INITMENUPOPUP",         /* 0x0117 */
    "WM_SYSTIMER",		/* 0x0118 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_MENUSELECT",            /* 0x011f */
    "WM_MENUCHAR",              /* 0x0120 */
    "WM_ENTERIDLE",             /* 0x0121 */
    "WM_MENURBUTTONUP",
    "WM_MENUDRAG",
    "WM_MENUGETOBJECT",
    "WM_UNINITMENUPOPUP",
    "WM_MENUCOMMAND",
    "WM_CHANGEUISTATE",
    "WM_UPDATEUISTATE",
    "WM_QUERYUISTATE",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    /* 0x0130 */
    NULL,
    "WM_LBTRACKPOINT",          /* 0x0131 */
    "WM_CTLCOLORMSGBOX",        /* 0x0132 */
    "WM_CTLCOLOREDIT",          /* 0x0133 */
    "WM_CTLCOLORLISTBOX",       /* 0x0134 */
    "WM_CTLCOLORBTN",           /* 0x0135 */
    "WM_CTLCOLORDLG",           /* 0x0136 */
    "WM_CTLCOLORSCROLLBAR",     /* 0x0137 */
    "WM_CTLCOLORSTATIC",        /* 0x0138 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0140 - Win32 Comboboxes */
    "CB_GETEDITSEL32",          /* 0x0140 */
    "CB_LIMITTEXT32",           /* 0x0141 */
    "CB_SETEDITSEL32",          /* 0x0142 */
    "CB_ADDSTRING32",           /* 0x0143 */
    "CB_DELETESTRING32",        /* 0x0144 */
    "CB_DIR32",                 /* 0x0145 */
    "CB_GETCOUNT32",            /* 0x0146 */
    "CB_GETCURSEL32",           /* 0x0147 */
    "CB_GETLBTEXT32",           /* 0x0148 */
    "CB_GETLBTEXTLEN32",        /* 0x0149 */
    "CB_INSERTSTRING32",        /* 0x014a */
    "CB_RESETCONTENT32",        /* 0x014b */
    "CB_FINDSTRING32",          /* 0x014c */
    "CB_SELECTSTRING32",        /* 0x014d */
    "CB_SETCURSEL32",           /* 0x014e */
    "CB_SHOWDROPDOWN32",        /* 0x014f */

    "CB_GETITEMDATA32",         /* 0x0150 */
    "CB_SETITEMDATA32",         /* 0x0151 */
    "CB_GETDROPPEDCONTROLRECT32",/* 0x0152 */
    "CB_SETITEMHEIGHT32",       /* 0x0153 */
    "CB_GETITEMHEIGHT32",       /* 0x0154 */
    "CB_SETEXTENDEDUI32",       /* 0x0155 */
    "CB_GETEXTENDEDUI32",       /* 0x0156 */
    "CB_GETDROPPEDSTATE32",     /* 0x0157 */
    "CB_FINDSTRINGEXACT32",     /* 0x0158 */
    "CB_SETLOCALE32",           /* 0x0159 */
    "CB_GETLOCALE32",           /* 0x015a */
    "CB_GETTOPINDEX32",         /* 0x015b */
    "CB_SETTOPINDEX32",         /* 0x015c */
    "CB_GETHORIZONTALEXTENT32", /* 0x015d */
    "CB_SETHORIZONTALEXTENT32", /* 0x015e */
    "CB_GETDROPPEDWIDTH32",     /* 0x015f */

    "CB_SETDROPPEDWIDTH32",     /* 0x0160 */
    "CB_INITSTORAGE32",         /* 0x0161 */
    NULL,
    "CB_MULTIPLEADDSTRING",
    "CB_GETCOMBOBOXINFO",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0170 - Win32 Static controls */
    "STM_SETICON32",		/* 0x0170 */
    "STM_GETICON32",		/* 0x0171 */
    "STM_SETIMAGE32",		/* 0x0172 */
    "STM_GETIMAGE32",		/* 0x0173 */
    "STM_MSGMAX",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0180 - Win32 Listboxes */
    "LB_ADDSTRING32",           /* 0x0180 */
    "LB_INSERTSTRING32",        /* 0x0181 */
    "LB_DELETESTRING32",        /* 0x0182 */
    "LB_SELITEMRANGEEX32",      /* 0x0183 */
    "LB_RESETCONTENT32",        /* 0x0184 */
    "LB_SETSEL32",              /* 0x0185 */
    "LB_SETCURSEL32",           /* 0x0186 */
    "LB_GETSEL32",              /* 0x0187 */
    "LB_GETCURSEL32",           /* 0x0188 */
    "LB_GETTEXT32",             /* 0x0189 */
    "LB_GETTEXTLEN32",          /* 0x018a */
    "LB_GETCOUNT32",            /* 0x018b */
    "LB_SELECTSTRING32",        /* 0x018c */
    "LB_DIR32",                 /* 0x018d */
    "LB_GETTOPINDEX32",         /* 0x018e */
    "LB_FINDSTRING32",          /* 0x018f */

    "LB_GETSELCOUNT32",         /* 0x0190 */
    "LB_GETSELITEMS32",         /* 0x0191 */
    "LB_SETTABSTOPS32",         /* 0x0192 */
    "LB_GETHORIZONTALEXTENT32", /* 0x0193 */
    "LB_SETHORIZONTALEXTENT32", /* 0x0194 */
    "LB_SETCOLUMNWIDTH32",      /* 0x0195 */
    "LB_ADDFILE32",             /* 0x0196 */
    "LB_SETTOPINDEX32",         /* 0x0197 */
    "LB_GETITEMRECT32",         /* 0x0198 */
    "LB_GETITEMDATA32",         /* 0x0199 */
    "LB_SETITEMDATA32",         /* 0x019a */
    "LB_SELITEMRANGE32",        /* 0x019b */
    "LB_SETANCHORINDEX32",      /* 0x019c */
    "LB_GETANCHORINDEX32",      /* 0x019d */
    "LB_SETCARETINDEX32",       /* 0x019e */
    "LB_GETCARETINDEX32",       /* 0x019f */

    "LB_SETITEMHEIGHT32",       /* 0x01a0 */
    "LB_GETITEMHEIGHT32",       /* 0x01a1 */
    "LB_FINDSTRINGEXACT32",     /* 0x01a2 */
    "LB_CARETON32",             /* 0x01a3 */
    "LB_CARETOFF32",            /* 0x01a4 */
    "LB_SETLOCALE32",           /* 0x01a5 */
    "LB_GETLOCALE32",           /* 0x01a6 */
    "LB_SETCOUNT32",            /* 0x01a7 */
    "LB_INITSTORAGE32",         /* 0x01a8 */
    "LB_ITEMFROMPOINT32",       /* 0x01a9 */
    "LB_INSERTSTRINGUPPER",
    "LB_INSERTSTRINGLOWER",
    "LB_ADDSTRINGUPPER",
    "LB_ADDSTRINGLOWER",
    "LBCB_STARTTRACK",
    "LBCB_ENDTRACK",

    /* 0x01B0 */
    NULL,
    "LB_MULTIPLEADDSTRING",
    "LB_GETLISTBOXINFO",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    
    /* 0x01C0 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x01D0 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x01E0 */
    NULL,
    NULL,
    NULL,
    "MN_SETHMENU",
    "MN_GETHMENU",
    "MN_SIZEWINDOW",
    "MN_OPENHIERARCHY",
    "MN_CLOSEHIERARCHY",
    "MN_SELECTITEM",
    "MN_CANCELMENUS",
    "MN_SELECTFIRSTVALIDITEM",
    NULL,
    NULL,
    NULL,
    "MN_FINDMENUWINDOWFROMPOINT",
    "MN_SHOWPOPUPWINDOW",
    "MN_BUTTONUP",
    "MN_SETTIMERTOOPENHIERARCHY",
    "MN_DBLCLK",
    "MN_ACTIVEPOPUP",
    "MN_ENDMENU",
    "MN_DODRAGDROP",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_MOUSEMOVE",		/* 0x0200 */
    "WM_LBUTTONDOWN",		/* 0x0201 */
    "WM_LBUTTONUP",		/* 0x0202 */
    "WM_LBUTTONDBLCLK",		/* 0x0203 */
    "WM_RBUTTONDOWN",		/* 0x0204 */
    "WM_RBUTTONUP",		/* 0x0205 */
    "WM_RBUTTONDBLCLK",		/* 0x0206 */
    "WM_MBUTTONDOWN",		/* 0x0207 */
    "WM_MBUTTONUP",		/* 0x0208 */
    "WM_MBUTTONDBLCLK",		/* 0x0209 */
    "WM_MOUSEWHEEL",
    "WM_XBUTTONDOWN",
    "WM_XBUTTONUP",
    "WM_XBUTTONDBLCLK",
    NULL,
    NULL,
    "WM_PARENTNOTIFY",		/* 0x0210 */
    "WM_ENTERMENULOOP",         /* 0x0211 */
    "WM_EXITMENULOOP",          /* 0x0212 */
    "WM_NEXTMENU", 		/* 0x0213 */
    "WM_SIZING",
    "WM_CAPTURECHANGED",
    "WM_MOVING",
    NULL,
    "WM_POWERBROADCAST",
    "WM_DEVICECHANGE",
    NULL,
    NULL, 
    NULL, 
    NULL,
    NULL,
    NULL,
    "WM_MDICREATE",             /* 0x0220 */
    "WM_MDIDESTROY",            /* 0x0221 */
    "WM_MDIACTIVATE",           /* 0x0222 */
    "WM_MDIRESTORE",            /* 0x0223 */
    "WM_MDINEXT",               /* 0x0224 */
    "WM_MDIMAXIMIZE",           /* 0x0225 */
    "WM_MDITILE",               /* 0x0226 */
    "WM_MDICASCADE",            /* 0x0227 */
    "WM_MDIICONARRANGE",        /* 0x0228 */
    "WM_MDIGETACTIVE",          /* 0x0229 */

    "WM_DROPOBJECT",
    "WM_QUERYDROPOBJECT",
    "WM_BEGINDRAG",
    "WM_DRAGLOOP",
    "WM_DRAGSELECT",
    "WM_DRAGMOVE",

    /* 0x0230*/
    "WM_MDISETMENU",            /* 0x0230 */
    "WM_ENTERSIZEMOVE",		/* 0x0231 */
    "WM_EXITSIZEMOVE",		/* 0x0232 */
    "WM_DROPFILES", 		/* 0x0233 */
    "WM_MDIREFRESHMENU",
    NULL,
    NULL,
    NULL,
    /* 0x0238*/
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0240 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0250 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0260 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0280 */
    "WM_KANJIFIRST",
    "WM_IME_SETCONTENT",
    "WM_IME_NOTIFY", 
    "WM_IME_CONTROL",
    "WM_IME_COMPOSITIONFULL",
    "WM_IME_SELECT",
    "WM_IME_CHAR",
    "WM_IME_SYSTEM",
    "WM_IME_REQUEST",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_IME_KEYDOWN",
    "WM_IME_KEYUP",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_KANJILAST",
    "WM_NCMOUSEHOVER",
    "WM_MOUSEHOVER",
    "WM_NCMOUSELEAVE",
    "WM_MOUSELEAVE",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x02c0 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,


    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    
    "WM_CUT", 			/* 0x0300 */
    "WM_COPY",
    "WM_PASTE",
    "WM_CLEAR",
    "WM_UNDO",
    "WM_RENDERFORMAT",
    "WM_RENDERALLFORMATS",
    "WM_DESTROYCLIPBOARD",
    "WM_DRAWCLIPBOARD",
    "WM_PAINTCLIPBOARD",
    "WM_VSCROLLCLIPBOARD",
    "WM_SIZECLIPBOARD",
    "WM_ASKCBFORMATNAME",
    "WM_CHANGECBCHAIN",
    "WM_HSCROLLCLIPBOARD",
    "WM_QUERYNEWPALETTE",	/* 0x030f*/

    "WM_PALETTEISCHANGING",
    "WM_PALETTECHANGED",
    "WM_HOTKEY", 		/* 0x0312 */
	  "WM_HOOKMSG",
    "WM_SYSMENU",
    "WM_EXITPROCESS",
    "WM_WAKETHREAD",
    "WM_PRINT",
    "WM_PRINTCLIENT",
    "WM_APPCOMMAND",
    "WM_THEMECHANGED",
    "WM_UAHINIT",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0340 */
    "WM_NOTIFYWOW",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_QUERYAFXWNDPROC",   /*  0x0360 */
    "WM_SIZEPARENT",        /*  0x0361 */
    "WM_SETMESSAGESTRING",  /*  0x0362 */
    "WM_IDLEUPDATECMDUI",   /*  0x0363 */
    "WM_INITIALUPDATE",     /*  0x0364 */
    "WM_COMMANDHELP",       /*  0x0365 */
    "WM_HELPHITTEST",       /*  0x0366 */
    "WM_EXITHELPMODE",      /*  0x0367 */
    "WM_RECALCPARENT",      /*  0x0368 */
    "WM_SIZECHILD",         /*  0x0369 */
    "WM_KICKIDLE",          /*  0x036A */
    "WM_QUERYCENTERWND",    /*  0x036B */
    "WM_DISABLEMODAL",      /*  0x036C */
    "WM_FLOATSTATUS",       /*  0x036D */
    "WM_ACTIVATETOPLEVEL",  /*  0x036E */
    "WM_QUERY3DCONTROLS",   /*  0x036F */
    NULL,
    NULL,
    NULL,
    "WM_SOCKET_NOTIFY",     /*  0x0373 */
    "WM_SOCKET_DEAD",       /*  0x0374 */
    "WM_POPMESSAGESTRING",  /*  0x0375 */
    "WM_OCC_LOADFROMSTREAM",     /* 0x0376 */
    "WM_OCC_LOADFROMSTORAGE",    /* 0x0377 */
    "WM_OCC_INITNEW",            /* 0x0378 */
    "WM_QUEUE_SENTINEL",         /* 0x0379 */
    "WM_OCC_LOADFROMSTREAM_EX",  /* 0x037A */
    "WM_OCC_LOADFROMSTORAGE_EX", /* 0x037B */

    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x0380 */
    "WM_PENWINFIRST",
    "WM_RCRESULT",
    "WM_HOOKRCRESULT",
    "WM_GLOBALRCCHANGE",
    "WM_SKB",
    "WM_HEDITCTL",
		NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_COALESCE_FIRST",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_COALESCE_LAST",

    /* 0x03a0 */
    "MM_JOY1MOVE",
    "MM_JOY2MOVE",
    "MM_JOY1ZMOVE",
    "MM_JOY2ZMOVE",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* 0x03b0 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "MM_JOY1BUTTONDOWN",
    "MM_JOY2BUTTONDOWN",
    "MM_JOY1BUTTONUP",
    "MM_JOY2BUTTONUP",
    "MM_MCINOTIFY",
    NULL,
    "MM_WOM_OPEN",
    "MM_WOM_CLOSE",
    "MM_WOM_DONE",
    "MM_WIM_OPEN",
    "MM_WIM_CLOSE",

    /* 0x03c0 */
    "MM_WIM_DATA",
    "MM_MIM_OPEN",
    "MM_MIM_CLOSE",
    "MM_MIM_DATA",
    "MM_MIM_LONGDATA",
    "MM_MIM_ERROR",
    "MM_MIM_LONGERROR",
    "MM_MOM_OPEN",
    "MM_MOM_CLOSE",
    "MM_MOM_DONE",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_MM_RESERVED_LAST",

    /* 0x03e0 */
    "WM_DDE_INITIATE",  /* 0x3E0 */
    "WM_DDE_TERMINATE",	/* 0x3E1 */
    "WM_DDE_ADVISE",	/* 0x3E2 */
    "WM_DDE_UNADVISE",	/* 0x3E3 */
    "WM_DDE_ACK",	/* 0x3E4 */
    "WM_DDE_DATA",	/* 0x3E5 */
    "WM_DDE_REQUEST",	/* 0x3E6 */
    "WM_DDE_POKE",	/* 0x3E7 */
    "WM_DDE_EXECUTE",	/* 0x3E8 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    /* 0x03f0 */
    "WM_CBT_RESERVED_FIRST",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "WM_CBT_RESERVED_LAST",
    "WM_USER"
};
const int _wmMsgDesc_size = 85;
char _wmMsgDesc_buffer[_wmMsgDesc_size];
const char* wmMsgDesc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message < WM_USER)
    {
        sprintf_s(_wmMsgDesc_buffer,_wmMsgDesc_size,"MSG=(0x%08X)'%25.25s', WPARAM=0x%08X, LPARAM=0x%08X", message, MessageTypeNames[message], wParam, lParam);
    }
    else if(message >= WM_USER && message <= 0x7FFF)
    {
        sprintf_s(_wmMsgDesc_buffer,_wmMsgDesc_size,"MSG=(0x%08X)'          WM_USER + %05d', WPARAM=0x%08X, LPARAM=0x%08X", message, message - WM_USER, wParam, lParam);
    }
    else if(message >= WM_APP && message <= 0xBFFF)
    {
        sprintf_s(_wmMsgDesc_buffer,_wmMsgDesc_size,"MSG=(0x%08X)'           WM_APP + %05d', WPARAM=0x%08X, LPARAM=0x%08X", message, message - WM_APP, wParam, lParam);
    }
    else if(message >= 0xC000 && message <= 0xFFFF)
    {
        CHAR szUserAtomName[MAX_PATH] = {0};
        if(GetClipboardFormatName(message, szUserAtomName, MAX_PATH))
        {
            sprintf_s(_wmMsgDesc_buffer,_wmMsgDesc_size,"MSG=(0x%08X)[REG]'%20.20s', WPARAM=0x%08X, LPARAM=0x%08X", message, szUserAtomName, wParam, lParam);
        }
        else
        {
            sprintf_s(_wmMsgDesc_buffer,_wmMsgDesc_size,"MSG=(0x%08X)[REG: BAD MESSAGE]'       ', WPARAM=0x%08X, LPARAM=0x%08X", message, wParam, lParam);
        }
    }
    else
    {
        sprintf_s(_wmMsgDesc_buffer,_wmMsgDesc_size,"MSG=(0x%08X)[UNKOWN]'                 ', WPARAM=0x%08X, LPARAM=0x%08X", message, wParam, lParam);
    }
    return _wmMsgDesc_buffer;
}

}   //  end namespace RSH