/* 
    Memory heap class and it's main global instance, the FormHeap.
    This appears to be Bethesda's answer to the global new & delete operators.
*/
#pragma once

class IMPORTCLASS MemoryHeap
{// size 170/170    (actually 16E, but aligned to word boundary)
/*
    Partial:
    -   members
    -   virtual methods
*/
public:

    // memory panic callback
    typedef void (*Unk164Callback)(UInt32 unk0, UInt32 unk1, UInt32 unk2);

    // members
    //     /*000*/ void**               vtbl;    
    MEMBER /*004*/ UInt32               unk004; // alignment
    MEMBER /*008*/ UInt32               unk008;
    MEMBER /*00C*/ UInt32               unk00C; // size of main memory block
    MEMBER /*010*/ UInt32               unk010;
    MEMBER /*014*/ UInt32               unk014;
    MEMBER /*018*/ void*                unk018; // main memory block
    MEMBER /*01C*/ UInt32               unk01C;
    MEMBER /*020*/ UInt32               unk020;
    MEMBER /*024*/ UInt32               unk024;
    MEMBER /*028*/ UInt32               unk028;
    MEMBER /*02C*/ UInt32               unk02C;
    MEMBER /*030*/ UInt32               unk030; // size of unk034 / 8
    MEMBER /*034*/ void*                unk034; // 0x2000 byte buffer
    MEMBER /*038*/ void*                unk038; // end of unk034
    MEMBER /*03C*/ UInt32               unk03C;
    MEMBER /*040*/ UInt32               unk040;
    MEMBER /*044*/ void*                unk044;
    MEMBER /*048*/ UInt32               unk048;
    MEMBER /*04C*/ UInt32               unk04C;
    MEMBER /*050*/ UInt32               unk050;
    MEMBER /*054*/ UInt32               unk054; // available memory at startup
    MEMBER /*058*/ UInt32               unk058;
    MEMBER /*05C*/ UInt32               unk05C;
    MEMBER /*060*/ UInt32               unk060;
    MEMBER /*064*/ UInt32               unk064;
    MEMBER /*068*/ UInt8                unk068[0x164-0x068];
    MEMBER /*164*/ Unk164Callback       unk164;
    MEMBER /*168*/ UInt32               unk168; // used memory at startup
    MEMBER /*16C*/ UInt8                unk16C;
    MEMBER /*16D*/ UInt8                unk16D;
    MEMBER /*16E*/ UInt8                pad16E[2];

    // virtual methods
    _NOUSE /*000/000*/ virtual void     Unk000(UInt32 arg0) {}
    IMPORT /*000/000*/ virtual void*    AllocatePage(UInt32 size);  
    IMPORT /*000/000*/ virtual void*    RawAlloc(UInt32 arg0);
    IMPORT /*000/000*/ virtual void*    RawAlloc2(UInt32 arg0);
    IMPORT /*000/000*/ virtual void     FreeMemory(void* buffer);
    IMPORT /*000/000*/ virtual void     RawFree(void* buffer);
    IMPORT /*000/000*/ virtual void     RawFree2(void* buffer);
    IMPORT /*000/000*/ virtual UInt32   RawSize(void* buffer);

    // methods
    IMPORT void*            Allocate(UInt32 size, bool arg1);       // arg1 is true for formheap allocs
    IMPORT void             Free(void* object);
    IMPORT static void*     FormHeapAlloc(UInt32 size);             // Allocate, deallocate using singleton
    IMPORT static void      FormHeapFree(void *object);

    // constructor, destructor
    _NOUSE MemoryHeap();
    _NOUSE ~MemoryHeap();

    // Static singleton
    IMPORT static MemoryHeap FormHeap;
};

// macro for overloading new & delete operators to use the FormHeap
#define USEFORMHEAP inline void* operator new (size_t size) {return MemoryHeap::FormHeapAlloc(size);} \
                    inline void* operator new[] (size_t size) {return MemoryHeap::FormHeapAlloc(size);} \
                    inline void operator delete (void *object) {MemoryHeap::FormHeapFree(object);} \
                    inline void operator delete[] (void *object) {MemoryHeap::FormHeapFree(object);}
