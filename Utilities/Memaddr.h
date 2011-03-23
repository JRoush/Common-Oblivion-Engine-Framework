/* 
    Memaddr - wrapper class for a raw memory address, with methods for common patching tasks

    Memaddr can be initialized with a pair of addresses as (gameAddress, csAddress).  Depending on
    the state of the OBLIVION macro, one address will be stored and the other discarded.

    Use the address 0x0 as a special 'invalid' or 'not present' value.  Memaddr methods that operate on
    the stored address will do nothing if the address has this value.
*/
#pragma once

class memaddr
{
public:

    // members
    UInt32      _addr;  // stored address

    // constructors
    inline memaddr(UInt32 address);
    inline memaddr(UInt32 gameAddress, UInt32 csAddress); // actual address chosen using state of OBLIVION def

    // conversions
    inline operator UInt32 ();  // implicit conversion to UInt32    
    inline operator void* (); // implicit conversion to void*    

    // write
    inline void WriteData8(UInt8 data);
    inline void WriteData32(UInt32 data);
    inline void WriteDataBuf(void* buf, UInt32 len);
    inline void WriteDataNOP(UInt32 endAddress); // fills region from stored addr to end addr with nop instructions

    // write hook
    inline void WriteRelJump(void* callback);
    inline void WriteRelCall(void* callback);
    inline void WriteRelPaddedCall(UInt32 endAddress, void* callback); // writes a rel call, & pads remaining space w/ nops

    // vtbl and RTTI related pointers
    inline void         SetObjectVtbl(void* object); // sets vtbl of object to stored address
    inline static void* GetObjectVtbl(void* object); // returns vtbl from object pointer
    inline static void* GetObjectLocator(void* object); // returns RTTI object locator from object pointer
    inline void         SetVtblEntry(UInt32 offset, void* function); // overwrites an entry in vtbl pointed to by stored address
    inline void*        GetVtblEntry(UInt32 offset); // retrieves an entry in vtbl pointed to by stored address
    
    // retrieve pointer-to-member (use caution with virtual methods)
    template <typename TMember>
    inline static void* GetPointerToMember(TMember rMemberFunc);

    // cdecl convention, for static member functions:  memaddr.cdeclCall<returntype>(args...)
    template <typename TReturn> 
    inline TReturn cdeclCall();
    template <typename TReturn, typename T1>
    inline TReturn cdeclCall(T1 arg1);
    template <typename TReturn, typename T1,typename T2>
    inline TReturn cdeclCall(T1 arg1,T2 arg2);
    template <typename TReturn, typename T1,typename T2,typename T3>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9);
    template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
    inline TReturn cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9,T10 arg10);

    // thiscall convention, for ordinary member functions: memaddr.thisCall<returntype>(object*,args...)
    template <typename TReturn, typename Tthis>
    inline TReturn thisCall(Tthis _this);
    template <typename TReturn, typename Tthis,typename T1>
    inline TReturn thisCall(Tthis _this,T1 arg1);
    template <typename TReturn, typename Tthis,typename T1,typename T2>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
    inline TReturn thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9,T10 arg10);

    // thiscall convention addressed from virtual table, for virtual member functions using address of vtbl:
    //      memaddr.thisVirtualCall<returntype>(offsetInVtbl, object*, args...)
    template <typename TReturn, typename Tthis>
    inline TReturn thisVirtualCall(UInt32 vtblOffset, Tthis _this);
    template <typename TReturn, typename Tthis,typename T1>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1);
    template <typename TReturn, typename Tthis,typename T1,typename T2>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9);
    template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
    inline TReturn thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9,T10 arg10);
};

#include "Utilities/Memaddr.inl"
