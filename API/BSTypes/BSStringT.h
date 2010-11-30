/*
    Basic dynamically-sizeable string class.

    Some methods (e.g. destructor, conversion to char*) are heavily inlined.
    They are simple enough to reconstruct directly here.

    High level string manipulation - comparison, replacement, etc. seems to be done 'manually'
    using cstdlib functions rather than through member functions.  That doesn't mean that
    member functions for comparison, etc. don't exist, but if they do they are very hard to find.
    The exception so far is 'sprintf', which has a member function wrapper.
*/
#pragma once
#include "API/TES/MemoryHeap.h"

class IMPORTCLASS BSStringT
{// size 08/08
protected:
    
    // members
    MEMBER /*00*/ char*       m_data;
    MEMBER /*04*/ SInt16      m_dataLen; 
    MEMBER /*06*/ SInt16      m_bufLen;

public:
    // methods
    IMPORT bool         Set(const char* string, SInt16 size = 0);   // size determines allocated storage? 0 to allocate automatically
    IMPORT void         Format(const char* format, ...);    
    IMPORT void         Clear();
    IMPORT SInt16       Size() const;    
    INLINE const char*  c_str() const                           {return m_data;}

    // operator overloads
    IMPORT BSStringT&   operator += (const char* string);
    INLINE BSStringT&   operator = (const char* string)         {Set(string, 0); return *this;}
    INLINE BSStringT&   operator = (const BSStringT& source)    {Set(source.c_str(), 0); return *this;}
    INLINE              operator const char* () const           {return m_data;}

    // constructors, destructor
    INLINE  BSStringT()                                         : m_data(0), m_dataLen(0), m_bufLen(0) {}
    IMPORT  BSStringT(const char* string);
    IMPORT  BSStringT(const BSStringT& source);
    INLINE  ~BSStringT()                                        {Clear();}

    // use FormHeap for class new & delete
    USEFORMHEAP 
};