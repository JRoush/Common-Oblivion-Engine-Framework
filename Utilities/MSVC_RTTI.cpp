#include "rsh\GameInternals\MSVC_RTTI.h"

#include "obse_common\SafeWrite.h"
#include <typeinfo.h>

namespace RSH {
namespace RTTI {

/********************* BaseDescriptor ******************************/
void BaseDescriptor::DebugDump()
{
    _MESSAGE("'%-25.25s' : Base Class Desc <%08X> @ %04X,%04X,%04X w/ Bases (%02i), Attributes {%08X}",type->name(),this,mdisp,pdisp,vdisp,numBases,attributes);
}
void* BaseDescriptor::UpCast(void* object)
{
    char* pThis = (char*)object;
    pThis += mdisp;
    if (pdisp != -1)
    {
      char* vbtable = pThis + pdisp;
      pThis += *(int*)(vbtable + vdisp);
    }
    return pThis;
}
/********************* HierarchyDescriptor ******************************/
void HierarchyDescriptor::DebugDump()
{
    _MESSAGE("Class Hierarchy <%08X>",this);
    gLog.Indent();
        _MESSAGE("Bases (%02i), Signature {%08X}, Attributes {%08X}",numBaseClasses,signature,attributes);
        int indents = 0;
        for (int i = 0; i < numBaseClasses; i++) 
        {
            baseClasses[i]->DebugDump();
        }
    gLog.Outdent();
}
void HierarchyDescriptor::AdoptBase(type_info* currentBase, type_info* adoptedBase)
{     
    for (int i = 0; i < numBaseClasses; i++) 
    {
        if (baseClasses[i] && baseClasses[i]->type == currentBase) 
        {
            // RTTI info generally stored in write-protected memory
            // Need to disable write protection before modifying
            SafeWrite32((UInt32)&baseClasses[i]->type,(UInt32)adoptedBase);
        }
    }
}
/********************* ObjectLocator *****************************/
void ObjectLocator::DebugDump()
{
    _MESSAGE("'%-25.25s' : Object Locator <%08X> @ %04X w/ cdOffset %04X, Signature {%08X}, hDescriptor <%08X>",
                    type->name(),this,offset,cdOffset,signature,hierarchyDescriptor);
}
ObjectLocator* ObjectLocator::GetObjectLocator(void* object)
{   
    void* vtbl = *(void**)object;
    return *((ObjectLocator**)vtbl - 1);
}

}   // end of namespace RTTI
}   // end namespace RSH