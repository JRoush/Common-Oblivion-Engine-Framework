/*
    BSFile: basic Bethesda file class
*/
#pragma once

// base classes
#include "API/NiTypes/NiFile.h"
#include "API/NiTypes/NiTArray.h"

// argument classes
class   BSStringT;  // BSTypes/BSStringT.h

class IMPORTCLASS BSFile : public NiFile
{// size 154
/*
    Partial:
    -   members
*/
public: 

    // members
    //     /*000*/ NiFile
    MEMBER /*028*/ UInt32       unk28;
    MEMBER /*02C*/ UInt32       unk2C;
    MEMBER /*030*/ UInt32       unk30;  // init to -1
    MEMBER /*034*/ UInt32       unk34;
    MEMBER /*038*/ UInt32       unk38;
    MEMBER /*03C*/ char         fileName[0x104];
    MEMBER /*140*/ UInt32       unk140;
    MEMBER /*144*/ UInt32       unk144;
    MEMBER /*148*/ UInt32       bsPos;  // (?) tracks current offset like NiFile::m_uiPos, but not reset by Flush()
    MEMBER /*14C*/ UInt32       unk14C;
    MEMBER /*150*/ UInt32       fileSize;

    // virtual method overrides
    IMPORT /*000*/ virtual              ~BSFile();
    IMPORT /*008*/ virtual void         SetEndianSwap(bool bDoSwap);
    IMPORT /*00C*/ virtual void         Seek(int offset, int origin);
    IMPORT /*010*/ virtual UInt32       GetFileSize() const; // uses cached size

    // additional virtual methods
    IMPORT /*014*/ virtual void         BuildFormattedStatusArray(NiTArray<char*>& outputArray); //
                                        // builds a formatted string output of settings table, one array entry for each line
    IMPORT /*018*/ virtual bool         OpenFile(bool waitIfInUse = false, bool isTextFile = false); // opens file, blocks in CS if file is in use
    IMPORT /*01C*/ virtual UInt32       GetFileSizeDirect(); // gets size directly, recalculating cached value
    IMPORT /*020*/ virtual UInt32       ReadString(BSStringT& buffer, UInt32 size); // indirect read, goes through read func pointer
    IMPORT /*024*/ virtual UInt32       ReadStringW(BSStringT& buffer, UInt32 size); // same as ReadString, but for wide-character strings
    IMPORT /*028*/ virtual UInt32       ReadLine(char* buffer, UInt32 size, UInt16 deliminator); // reads until deliminator character is found
    IMPORT /*02C*/ virtual UInt32       WriteString(BSStringT& buffer);
    IMPORT /*030*/ virtual UInt32       WriteStringW(BSStringT& buffer); // same as WriteString, but for wide-character strings
    IMPORT /*034*/ virtual bool         FileExists();
    IMPORT /*038*/ virtual UInt32       DirectRead(void* buffer, UInt32 size); // direct read - invoked by read func pointer before any byte swapping
    IMPORT /*03C*/ virtual UInt32       DirectWrite(const void* buffer, UInt32 size); // direct write - invoked by write func pointer after any byte swapping

    // methods  
    IMPORT bool                 Flush(); // flush write buffer to file, hides/replaces corresponding NiFile method

    // constructor
    IMPORT BSFile(const char* fileName, UInt32 mode, UInt32 bufferSize = 0x8000, bool isTextFile = false);
};