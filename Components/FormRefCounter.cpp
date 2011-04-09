#include "FormRefCounter.h"

#include "API/TES/MemoryHeap.h"
#include "API/NiTypes/NiTMap.h"
#include "API/TESForms/TESForm.h"

#ifndef OBLIVION

// instance class definition
class FormRefCounterInstance
{
public:

    // form ref info, part of a LL of referenced *from* a single master form
    struct RefChange
    {
        // members
        TESForm*        form;   // form being referenced
        SInt32          count;  // change in ref count
        RefChange*      next;   // next form being referenced
        // methods
        RefChange(TESForm* nForm, SInt32 nCount) : form(nForm), count(nCount), next(0) {}
        // use form heap for new,delete
        USEFORMHEAP
    };

    // map of referencing forms to their LL of references
    typedef NiTPointerMap<TESForm*,RefChange*> RefTableT;
    RefTableT*      refTable;
    TESForm*        lastKey;
    RefChange*      lastListHead;
    RefTableT&      GetRefTable()       // return ref table for this instance, creating one if necessary  
    {
        if (!refTable) refTable = new RefTableT;
        return *refTable;
    }

    // reference management
    SInt32          ModifyCount(TESForm* masterForm, TESForm* refForm, SInt32 countChange)   // returns new count
    {
        if (!masterForm || !refForm) return 0;  // bad form*

        // lookup list head in map 
        if (masterForm != lastKey) 
        {
            // lookup result not cached
            lastKey = masterForm; 
            if (!GetRefTable().GetAt(lastKey,lastListHead)) lastListHead = 0;
        }
        if (!lastListHead)
        {
            // no list found for master form
            if (countChange != 0)
            {
                // non-zero change to count; create a new list entry
                lastListHead = new RefChange(refForm,countChange);
                GetRefTable().SetAt(masterForm,lastListHead);
            }
            return countChange;
        }

        // search list for referenced form
        RefChange* prevChange = 0;
        RefChange* change = lastListHead;
        while (change && change->form != refForm) 
        {
            prevChange = change;
            change = change->next;
        }
        if (!change)
        {
            // no entry found for referenced form
            if (countChange != 0)   
            {
                // non-zero change to count; create a new list entry
                change = new RefChange(refForm,countChange);
                if (prevChange) prevChange->next = change;
            }
            return countChange;        
        }

        // modify count for existing entry
        change->count += countChange;
        if (change->count == 0)
        {
            // change record now empty, remove from list
            if (prevChange)
            {
                // delete this entry
                prevChange->next = change->next;   
                delete change;
            }
            else if (change->next)
            {
                // this entry is list header, stored in map
                // copy contents of next entry, then delete that
                RefChange* n = change->next;
                change->form = n->form;
                change->count = n->count;
                change->next = n->next; 
                delete n;
            }
            else
            {
                // this list is now empty, remove from map
                GetRefTable().RemoveAt(masterForm);
                lastKey = 0;
                lastListHead = 0;
                delete change;
            }
            return 0;
        }
        return change->count;
    }
    void            ClearTable()        // clear all tracked changes
    {
        NiTMapIterator pos = GetRefTable().GetFirstPos();
        while (pos)
        {
            TESForm*    masterForm;
            RefChange*  refChange;
            GetRefTable().GetNext(pos,masterForm,refChange);
            while (refChange)
            {
                RefChange* n = refChange->next;
                delete refChange;
                refChange = n;
            }
        }
        GetRefTable().RemoveAll();
        lastKey = 0;
        lastListHead = 0;
    }
    void            CommitChanges()     // commit all changes to CS use info system and clear table
    {
        NiTMapIterator pos = GetRefTable().GetFirstPos();
        while (pos)
        {
            TESForm*    masterForm;
            RefChange*  refChange;
            GetRefTable().GetNext(pos,masterForm,refChange);
            while (refChange)
            {
                RefChange* n = refChange->next;
                if (refChange->form && refChange->count > 0) refChange->form->AddCrossReference(masterForm);
                else if (refChange->form && refChange->count < 0) refChange->form->RemoveCrossReference(masterForm);
                delete refChange;
                refChange = n;
            }
        }
        GetRefTable().RemoveAll();
        lastKey = 0;
        lastListHead = 0;
    }
    void            DumpTable()         // dump to output log for debugging
    {
        _MESSAGE("Dumping ref table with %i lists ...",GetRefTable().GetCount());
        gLog.Indent();
        NiTMapIterator pos = GetRefTable().GetFirstPos();
        while (pos)
        {
            TESForm*    masterForm;
            RefChange*  refChange;
            BSStringT   desc;
            GetRefTable().GetNext(pos,masterForm,refChange);
            // 
            masterForm->GetDebugDescription(desc);
            _MESSAGE("Master Form <%p> %s :",masterForm,desc.c_str());
            gLog.Indent();
            while (refChange)
            {
                refChange->form->GetDebugDescription(desc);
                _MESSAGE("Referenced Form <%p> (x%i) %s",refChange->form,refChange->count,desc.c_str());
                refChange = refChange->next;
            }
            gLog.Outdent();
        }
        gLog.Outdent();
    }
    
    // constructor, destructor
    FormRefCounterInstance(): refTable(0), lastKey(0), lastListHead(0) {}
    ~FormRefCounterInstance()
    {
        // cleanup map if present
        if (refTable) 
        {
            ClearTable();
            delete refTable;
        }
    }

    // global static instance
    static FormRefCounterInstance  instance;
};

// global static instance
FormRefCounterInstance  FormRefCounterInstance::instance;

#endif

// public reference management
SInt32 FormRefCounter::AddReference(TESForm* masterForm, TESForm* refForm)
{
    #ifndef OBLIVION
    return FormRefCounterInstance::instance.ModifyCount(masterForm,refForm,1);
    #else
    return 0;
    #endif
}
SInt32 FormRefCounter::RemoveReference(TESForm* masterForm, TESForm* refForm)
{
    #ifndef OBLIVION
    return FormRefCounterInstance::instance.ModifyCount(masterForm,refForm,-1);
    #else
    return 0;
    #endif
}
void FormRefCounter::ClearReferences()
{
    #ifndef OBLIVION
    FormRefCounterInstance::instance.ClearTable();
    #endif
}
void FormRefCounter::CommitReferences()
{
    #ifndef OBLIVION
    FormRefCounterInstance::instance.CommitChanges();
    #endif
}
