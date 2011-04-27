/*

*/
#pragma once

// base classes
#include "API/NiTypes/NiTMap.h" // NiTStringMap

template <class TVAL> class BSTCaseInsensitiveStringMap : public NiTStringMap<TVAL>
{// size 14
    
    // virtual methods           
public:     virtual                     ~BSTCaseInsensitiveStringMap() {}                                               // 000     
protected:  virtual UInt32              KeyToHashIndex(const char* key) const;                                          // 004
            virtual bool                IsKeysEqual(const char* key1, const char* key2) const;                          // 008

    // methods            
public:     BSTCaseInsensitiveStringMap(UInt32 uiHashSize = 37, bool bCopy = true) : NiTStringMap<TVAL>(uiHashSize,bCopy) {}

};

#include "API/BSTypes/BSTCaseInsensitiveStringMap.inl"