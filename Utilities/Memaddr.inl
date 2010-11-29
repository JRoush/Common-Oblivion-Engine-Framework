
// constructors
inline memaddr::memaddr(UInt32 address) : _addr(address) {}
inline memaddr::memaddr(UInt32 gameAddress, UInt32 csAddress)
{
    #ifdef OBLIVION
        _addr = gameAddress;   
    #else
        _addr = csAddress;
    #endif
}

// conversions
inline memaddr::operator UInt32 () {return _addr;}  // implicit conversion to UInt32    
inline memaddr::operator void* () {return (void*)_addr;} // implicit conversion to void*    

// write
inline void memaddr::WriteData8(UInt8 data)
{
    if (!_addr) return;
    UInt32    oldProtect;
    VirtualProtect((void*)_addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
    *((UInt8*)_addr) = data;
    VirtualProtect((void*)_addr, 1, oldProtect, &oldProtect);
}
inline void memaddr::WriteData32(UInt32 data)
{
    if (!_addr) return;
    UInt32    oldProtect;
    VirtualProtect((void*)_addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
    *((UInt32*)_addr) = data;
    VirtualProtect((void*)_addr, 4, oldProtect, &oldProtect);
}
inline void memaddr::WriteDataBuf(void* buf, UInt32 len)
{
    if (!_addr) return;
    UInt32    oldProtect;
    VirtualProtect((void*)_addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((void*)_addr, buf, len);
    VirtualProtect((void*)_addr, len, oldProtect, &oldProtect);
}
inline void memaddr::WriteDataNOP(UInt32 endAddress)
{
    if (!_addr || !endAddress) return;
    unsigned int len = endAddress - _addr;
    UInt32  oldProtect;
    VirtualProtect((void*)_addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset((void*)_addr, 0x90, len);    // nop
    VirtualProtect((void*)_addr, len, oldProtect, &oldProtect);
}

// write hook
inline void memaddr::WriteRelJump(void* callback)
{
    if (!_addr) return;
    UInt8   buf[5];
    buf[0] = 0xE9;  // jmp rel32
    *(UInt32*)&buf[1] = (UInt32)callback - _addr - 5; // relative to address of next instruction
    WriteDataBuf(buf,5);
}
inline void memaddr::WriteRelCall(void* callback)
{
    if (!_addr) return;
    UInt8   buf[5];
    buf[0] = 0xE8;  // call rel32
    *(UInt32*)&buf[1] = (UInt32)callback - _addr - 5; // relative to address of next instruction
    WriteDataBuf(buf,5);
}
inline void memaddr::WriteRelPaddedCall(UInt32 endAddress, void* callback)
{
    if (!_addr || !endAddress) return;
    WriteDataNOP(endAddress); // fill specified region w/ NOPs
    WriteRelCall(callback); // write a callback
}

// vtbl and RTTI locator pointers
inline void memaddr::SetObjectVtbl(void* object)
{
    if (!_addr) return;
    *(UInt32*)object = _addr;
}
inline void* memaddr::GetObjectVtbl(void* object)
{
    return (void*)(*(UInt32*)object);
}
inline void* memaddr::GetObjectLocator(void* object)
{        
    return (void*)*(*(UInt32**)object - 1);
}
inline void memaddr::SetVtblEntry(UInt32 offset, void* function)
{
    if (!_addr) return;
    _addr += offset;
    WriteData32((UInt32)function);
    _addr -= offset;
}
inline void* memaddr::GetVtblEntry(UInt32 offset)
{
    if (!_addr) return 0;
    return (void*)(*(UInt32*)(_addr + offset));
}

// pointer-to-member
template <typename TMember>
__forceinline void* memaddr::GetPointerToMember(TMember rMemberFunc)
{
    union {void* v; TMember p;} _x;
    _x.p = rMemberFunc;
    return _x.v;
}

// cdecl convention
template <typename TReturn>
__forceinline TReturn memaddr::cdeclCall()
{
    if (!_addr) return TReturn(0);
    typedef TReturn (*f)(void);
    return ((f)_addr)();
}
template <typename TReturn, typename T1>
__forceinline TReturn memaddr::cdeclCall(T1 arg1)
{
    if (!_addr) return TReturn(0);        
    typedef TReturn (*f)(T1);
    return ((f)_addr)(arg1);
}
template <typename TReturn, typename T1,typename T2>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2);
    return ((f)_addr)(arg1,arg2);
}
template <typename TReturn, typename T1,typename T2,typename T3>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3);
    return ((f)_addr)(arg1,arg2,arg3);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4);
    return ((f)_addr)(arg1,arg2,arg3,arg4);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4,T5);
    return ((f)_addr)(arg1,arg2,arg3,arg4,arg5);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4,T5,T6);
    return ((f)_addr)(arg1,arg2,arg3,arg4,arg5,arg6);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4,T5,T6,T7);
    return ((f)_addr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4,T5,T6,T7,T8);
    return ((f)_addr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4,T5,T6,T7,T8,T9);
    return ((f)_addr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
}
template <typename TReturn, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
__forceinline TReturn memaddr::cdeclCall(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9,T10 arg10)
{
    if (!_addr) return TReturn(0); 
    typedef TReturn (*f)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T10);
    return ((f)_addr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
}

// thiscall convention
template <typename TReturn, typename Tthis>
__forceinline TReturn memaddr::thisCall(Tthis _this)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(); } u = {_addr};
    return ((T*)_this->*u.m)();
}
template <typename TReturn, typename Tthis,typename T1>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1); } u = {_addr};
    return ((T*)_this->*u.m)(arg1);
}
template <typename TReturn, typename Tthis,typename T1,typename T2>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7,T8); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7,T8,T9); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
__forceinline TReturn memaddr::thisCall(Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9,T10 arg10)
{
    if (!_addr) return TReturn(0);
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T10); } u = {_addr};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
}

// thiscall convention, addressed from virtual table
template <typename TReturn, typename Tthis>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset, Tthis _this)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)();
}
template <typename TReturn, typename Tthis,typename T1>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1);
}
template <typename TReturn, typename Tthis,typename T1,typename T2>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7,T8); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7,T8,T9); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
}
template <typename TReturn, typename Tthis,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
__forceinline TReturn memaddr::thisVirtualCall(UInt32 vtblOffset,Tthis _this,T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6,T7 arg7,T8 arg8,T9 arg9,T10 arg10)
{
    if (!_addr) return TReturn(0); 
    class T {}; union { UInt32 x; TReturn (T::*m)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T10); } u = {*(UInt32*)(_addr + vtblOffset)};
    return ((T*)_this->*u.m)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
}

