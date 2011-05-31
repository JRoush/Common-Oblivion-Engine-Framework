/*
    NiFile: Basic NetImmerse file class 

    [Original] Notes:
    Platform-independent abstract class that defines the interface for writing to 
    and reading from files.  In addition to defining the interface it also provides
    a mechanism to replace the default concrete implementation of the File interface.
*/
#pragma once

// base classes
#include "API/NiTypes/NiBinaryStream.h"

class IMPORTCLASS NiFile : public NiBinaryStream
{// size 28
public:

    enum FileModes
    {
        kFileMode_ReadOnly      = 0,
        kFileMode_WriteOnly     = 1,
        kFileMode_AppendOnly    = 2,
    };

    // members
    //     /*00*/ NiBinaryStream
    MEMBER /*0C*/ UInt32        m_uiBufferAllocSize;  // size of buffer
    MEMBER /*10*/ UInt32        m_uiBufferReadSize;  // init to 0
    MEMBER /*14*/ UInt32        m_uiPos;  // init to 0
    MEMBER /*18*/ void*         m_pBuffer;  // allocated/deallocated on form heap by constructor/destructor
    MEMBER /*1C*/ FILE*         m_pFile;
    MEMBER /*20*/ UInt32        m_eMode; // file mode
    MEMBER /*24*/ bool          m_bGood; // true if file is open

    // virtual method overrides
    IMPORT /*000*/ virtual              ~NiFile();
    IMPORT /*004*/ virtual operator     bool() const;
    IMPORT /*008*/ virtual void         SetEndianSwap(bool bDoSwap);

    // additional virtual methods
    IMPORT /*00C*/ virtual void         Seek(int offset, int origin); // set current read/write position, same args as fseek()
    IMPORT /*010*/ virtual UInt32       GetFileSize() const;

    // methods
    IMPORT static NiFile*       GetFile(const char* pcFileName, UInt32 eMode, UInt32 uiBufferSize = 0x8000);    // create an NiFile (BSFile, in game/CS)
    IMPORT static bool          CanAccess(const char* pcFileName, UInt32 eMode); // returns false if file cannot be opened in specified mode    
    IMPORT UInt32               DirectRead(void* pvBuffer, UInt32 uiBytes); // direct read - invoked by member pointer before any byte swapping
    IMPORT UInt32               DirectWrite(const void* pvBuffer, UInt32 uiBytes); // direct write - invoked by member pointer after any byte swapping
    IMPORT bool                 Flush(); // flush write buffer to file

    // constructor
    IMPORT NiFile(const char* pcFileName, UInt32 eMode, UInt32 uiBufferSize = 0x8000);
};