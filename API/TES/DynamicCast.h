/*
    Polymorphic type conversion for Oblivion.   

    Similarities to standard (as of VS2008) dynamic_cast<> operator:
        -   Identical signature
        -   Compares types by decorated type name
        -   Uses COM rtti scheme
    Differences with standard : None known

    **Recommendation**: use dynamic_cast<> instead, as it is much easier to read and manage.

    Useage:
        OldType* oldt;
        NewType* newt = (NewType*)OblivionDynamicCast(oldt,0,typeid(*oldt),typeid(NewType),false);
*/
#pragma once

#include <typeinfo>

IMPORT void* OblivionDynamicCast(void* srcObj, UInt32 arg1, const type_info& fromType, const type_info& toType, bool exceptionOnBadCast);  // arg1 = 0