/*
    Common definitions for COEF projects
    Requires include path pointing to the COEF project folder
    Requires definition (or intentional absence) of 'MFC' macro
*/
#pragma once

// Disable common warnings:
// 4005 - Macro redefinition
// 4018 - signed/unsigned mismatch
// 4244 - loss of data by assignment
// 4267 - possible loss of data (truncation)
// 4305 - truncation by assignment
// 4288 - disable warning for crap microsoft extension screwing up the scope of variables defined in for loops
// 4311 - pointer truncation
// 4312 - pointer extension
// 4800 - forcing bool to integer
#pragma warning(disable: 4005 4018 4244 4267 4305 4288 4312 4311 4800)

// Windows version macros: win2k and above
#define _WIN32_WINDOWS  0x0500
#define _WIN32_WINNT	0x0500
#define WINVER          0x0500

// windows/mfc headers - cannot include both
#ifdef MFC
    #define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
    #define VC_EXTRALEAN                            // Exclude rarely-used stuff from Windows headers
    #include <afx.h>
    #include <afxwin.h>                             // MFC core and standard components
    #include <afxext.h>                             // MFC extensions
    #ifndef _AFX_NO_AFXCMN_SUPPORT
        #include <afxcmn.h>                         // MFC support for Windows Common Controls
    #endif
#else
    #include <windows.h> 
#endif

// C standard libraries
#include <cstdlib>
#include <cstdio>
#include <cstring>

// standard utility includes
#include "Utilities/ITypes.h"
#include "Utilities/IErrors.h"
#include "Utilities/OutputLog.h"
#include "Utilities/ISingleton.h"

// macros for COEF API linkage states
#define IMPORTCLASS                     // Imported linkage (entire class)
#define IMPORT _declspec(dllimport)     // Import linkage (individual members)
#define _NOUSE inline                   // Pure method, not yet decoded, or otherwise unimportable.  Should not be localized
#define _LOCAL                          // Locally implemented (for customized versions of the API)
#define INLINE inline                   // Inlined code, used for basic types where game code itself is inlined, templated, or a stub
#define MEMBER                          // marks a local member variable (not imported).  Only real purpose is to break up visual studio's comment collapsing

// macro for generating decorated function names
#define DNAME(...) { __pragma( message( "DECORATED NAME " __FUNCDNAME__ " ; " __FUNCTION__)) return __VA_ARGS__;}
