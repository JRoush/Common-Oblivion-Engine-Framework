/*
    RSH implementation of the "standard" structures apparently used by MS VC++ for RTTI info
    These are based on disassembly by myself and research/experimentation done by internet sources
*/
#pragma once

// argument classes
class   type_info;

namespace RSH {
namespace RTTI {

/* 
// type_info aka Type_Descriptor - uniquely identifies each polymorphic class
// actual definition in <typeinfo.h>, see documentation
struct type_info 
{
    // members
    void**      vtbl;       // 00 vtable of type_info class  
    void*       cache;      // 04 used to keep the demangled name returned by type_info::name()
    char        name[];     // 08 mangled type name, variable size
};
*/

// BaseClassDescriptor - information which allows compiler to cast the derived class to any of the base classes
struct HierarchyDescriptor;
struct BaseDescriptor
{
    // members
    type_info*              type;                   // 00 type descriptor of the base class
    DWORD                   numBases;               // 04 number of nested classes following in the Base Class Array
    long                    mdisp;                  // 08 member displacement
    long                    pdisp;                  // 0C vbtable displacement (for virtual bases, -1 otherwise)
    long                    vdisp;                  // 10 displacement inside vbtable (for virtual bases, 0 otherwise)
    DWORD                   attributes;             // 14 flags, usually 0
    HierarchyDescriptor*    hierarchyDescriptor;    // 18

    // methods
    void        DebugDump();
    void*       UpCast(void* object);   // takes a pointer to polymorphic object associated with BaseDescriptor, returns pointer to base class part
};

// HierarchyDescriptor, aka ClassHierarchyDiscriptor - describes the inheritance hierarchy of the class
struct HierarchyDescriptor
{
    // members
    DWORD                   signature;          // 00 always zero?
    DWORD                   attributes;         // 04 bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
    DWORD                   numBaseClasses;     // 08 number of classes in Base Class Array
    BaseDescriptor**        baseClasses;        // 0C pointer to base class array, last entry is null

    // methods
    void                    DebugDump();
    void                    AdoptBase(type_info* currentBase, type_info* adoptedBase);  // replaces currentBase with adoptedBase in class hierarchy
};

// TypeLocator, aka CompleteObjectLocator - uniquely identifies each vtbl of a polymorphic class
struct ObjectLocator
{
    // members
    DWORD                   signature;          // 00 always zero ?
    DWORD                   offset;             // 04 offset of this vtable in the complete class
    DWORD                   cdOffset;           // 08 constructor displacement offset
    type_info*              type;               // 0C TypeDescriptor of the complete class
    HierarchyDescriptor*    hierarchyDescriptor;// 10 describes inheritance hierarchy

    // methods
    void                    DebugDump(); 
    static ObjectLocator*   GetObjectLocator(void* object);     // takes pointer to polymorphic object, returns ObjectLocator for that class
};


}   // end of namespace RTTI
}   // end namespace RSH