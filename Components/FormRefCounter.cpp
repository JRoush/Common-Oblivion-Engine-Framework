#include "FormRefCounter.h"
#include "EventManager.h"

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
        if (!refTable) 
        {
            // construct a new ref table
            refTable = new RefTableT(1000);
            // register event to destroy table when data handler is cleared
            EventManager::DataHandler::Clear.RegisterCallback(&FormRefCounter::ClearAllReferences);
        }
        return *refTable;
    }

    // reference management
    SInt32          ModifyCount(TESForm* masterForm, TESForm* refForm, SInt32 countChange)   // updates use-info system, returns new count
    {
        if (!refTable && countChange <= 0) return 0;  // table is empty
        if (!masterForm || masterForm->formFlags & TESForm::kFormFlags_Temporary) return 0; // invalid or temporary master form
        if (!refForm || refForm->formFlags & TESForm::kFormFlags_Temporary) return 0; // invalid or temporary referenced form

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
            if (countChange > 0)
            {
                // non-zero change to count; create a new list entry
                lastListHead = new RefChange(refForm,countChange);
                GetRefTable().SetAt(masterForm,lastListHead);
                refForm->AddCrossReference(masterForm);
                return countChange;
            }
            else return 0;
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
            if (countChange > 0)   
            {
                // non-zero change to count; create a new list entry
                change = new RefChange(refForm,countChange);
                if (prevChange) prevChange->next = change;
                refForm->AddCrossReference(masterForm);
                return countChange;   
            }  
            else return 0;
        }

        // modify count for existing entry
        change->count += countChange;
        if (change->count <= 0)
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
            refForm->RemoveCrossReference(masterForm);
            return 0;
        }
        return change->count;
    }
    void            ClearCount(TESForm* masterForm)   // clears ref info for master form, update use-info system
    {
        if (!refTable) return;  // table is empty
        if (!masterForm) return; // invalid master form

        // lookup list head in map 
        RefChange* listHead = 0;
        if (masterForm == lastKey) 
        {
            // lookup result cached
            listHead = lastListHead;
            lastListHead = 0;
            lastKey = 0;
        }
        else if (!GetRefTable().GetAt(masterForm,listHead)) listHead = 0;    // lookup result not cached
        if (!listHead) return;  // no ref data for this master form

        // destroy ref list
        GetRefTable().RemoveAt(masterForm);
        while (listHead)
        {
            RefChange* n = listHead->next;
            delete listHead;        
            listHead = n;
        }
    }
    void            ClearTable()        // clear all tracked changes, does *not* update use-info system
    {
        if (!refTable) return;  // table already empty

        // clear table lists
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
        // clear & destroy table itself
        GetRefTable().RemoveAll();
        delete refTable;
        refTable = 0;
        // unregister cleanup event
        EventManager::DataHandler::Clear.UnregisterCallback(&FormRefCounter::ClearAllReferences);
        // clear cached search results
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
        ClearTable();
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
void FormRefCounter::ClearReferences(TESForm* masterForm)
{
    #ifndef OBLIVION
    FormRefCounterInstance::instance.ClearCount(masterForm);
    #endif
}
void FormRefCounter::ClearAllReferences()
{
    #ifndef OBLIVION
    FormRefCounterInstance::instance.ClearTable();
    #endif
}
void FormRefCounter::DumpReferences()
{
    #ifndef OBLIVION
    FormRefCounterInstance::instance.DumpTable();
    #endif
}
