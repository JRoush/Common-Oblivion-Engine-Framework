#include <malloc.h>

//---------------------------------------------------------------------------
template <class TVAL> 
UInt32 BSTCaseInsensitiveStringMap<TVAL>::KeyToHashIndex(const char* pKey) const
{
    unsigned int len = strlen(pKey);
    if (len == 0 || pKey == 0) return 0;
    char* buffer = (char*)alloca(len+1);  // NOTE: use of alloca() for large string buffers will cause CTDs
    
    // convert to lower case
    for (int c = 0; c < len; c++) buffer[c] = tolower(pKey[c]);
    buffer[len] = 0;

    // invoke base hash method
    return NiTStringMap<TVAL>::KeyToHashIndex(buffer);
}
//---------------------------------------------------------------------------
template <class TVAL> 
inline bool BSTCaseInsensitiveStringMap<TVAL>::IsKeysEqual(const char* pcKey1, const char* pcKey2) const
{
    return _stricmp(pcKey1, pcKey2) == 0;
}