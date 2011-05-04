/*
    NiBinaryStream: generalized platform-independent binary i/o 

    [Original] Notes:
    This abstract base class for all binary streams, such as memory streams or files, is designed 
    to wrap platform-dependent implementations of streaming systems.
    
    Derived classes need to implement some particular behavior to support endian swapping.  Such
    details are described in the documentation about how to derive from BinaryStream.  By default,
    all files will be written in the endianness of the current platform.
*/
#pragma once
#include "API/TES/MemoryHeap.h"

class IMPORTCLASS NiBinaryStream
{// size 0C
public:

    // read/write function callbacks.  Component size and number are for custom byte ordering
    typedef UInt32 (*ReadFNT)(NiBinaryStream* pkThis, void* pvBuffer, UInt32 uiBytes, UInt32* puiComponentSizes, UInt32 uiNumComponents);
    typedef UInt32 (*WriteFNT)(NiBinaryStream* pkThis, const void* pvBuffer, UInt32 uiBytes, UInt32* puiComponentSizes, UInt32 uiNumComponents);

    // members
    //     /*00*/ void**        vtbl;
    MEMBER /*04*/ ReadFNT       m_pfnRead;  // called to actually read from stream
    MEMBER /*08*/ WriteFNT      m_pfnWrite; // called to actually write to stream

    // virtual methods
    IMPORT /*000*/ virtual              ~NiBinaryStream(); // vtbl entry is actually compiler generated "scalar deleting destructor"
    IMPORT /*004*/ virtual operator     bool() const = 0;   // returns true if stream is ready for read/write
    IMPORT /*008*/ virtual void         SetEndianSwap(bool bDoSwap) = 0;  // Set read/write pointers based on whether endian swapping is needed.

    // methods
    IMPORT static void          DoByteSwap(void* pvData, UInt32 uiBytes, UInt32* puiComponentSizes, UInt32 uiNumComponents); // swap byte ordering

    // shorthand methods for convenience
    INLINE UInt32               Read(void* pvBuffer, UInt32 uiBytes) { UInt32 x = 1; return m_pfnRead(this,pvBuffer,uiBytes,&x,1); }
    INLINE UInt32               Write(void* pvBuffer, UInt32 uiBytes) { UInt32 x = 1; return m_pfnWrite(this,pvBuffer,uiBytes,&x,1); }

    // constructor
    IMPORT NiBinaryStream();

    // use form heap
    USEFORMHEAP
};