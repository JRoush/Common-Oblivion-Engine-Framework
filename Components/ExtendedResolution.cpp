#include "ExtendedResolution.h"

#include <list>

/*************************************  SortedCollection *******************************************************/
template <class ObjT> class SortedCollection : private std::list<ObjT*>
{
public:

    // constructor, destructor
    SortedCollection() 
    {
        currentPos = end();
    }
    ~SortedCollection()
    {        
        RemoveAll(false); // cleanup stored objects
    }

    // access
    bool Insert(ObjT* obj)
    {
        if (!obj) return false;   // bad object
        iterator insertPos = end(); 
        for(iterator pos = begin(); pos != end(); pos++)
        {
            if (*pos == obj) return false; // object already present in list
            if (insertPos == end() && *obj < **pos) insertPos = pos; // insert before object with greater value
        }
        insert(insertPos,obj);
        return true;
    }
    void Remove(ObjT* obj, bool orphan)
    {
        if (!obj) return; // bad object
        if (currentPos != end() && *currentPos == obj)
        {
            currentPos++; // move current list position off of element to be deleted
        }
        remove(obj);   // remove object from list
        if (!orphan) delete obj;  // cleanup field object
    }
    void RemoveAll(bool orphan)
    {
        if (!orphan)
        {
            for(iterator pos = begin(); pos != end(); pos++)
            {
                delete (*pos);  // cleanup object
            }
        }
        clear();
        currentPos == end();  // reset current position
    }
    ObjT* GetNext(ObjT* current)
    {
        if (empty()) return 0;   // empty list
        else if (!current)
        {
            // first object requested
            currentPos = begin();
            return *currentPos;
        }

        if (currentPos == end() || *currentPos != current)
        {
            // internal current pos doesn't match current object arg
            for(currentPos = begin(); currentPos != end(); currentPos++)
            {
                if (*currentPos == current) break;
            }
            if (currentPos == end()) return 0; // current field not found in list
        }

        currentPos++; // increment current pos
        if (currentPos == end()) return 0; // end of list
        return *currentPos; // return next field
    }
    size_type size() const
    {
        return std::list<ObjT*>::size();
    }

protected:

    // members
    typename iterator  currentPos;
};
/*************************************  ResFile *******************************************************/
void* ResFile::operator new (size_t size) { return ::operator new(size); }
void ResFile::operator delete (void *obj) { ::operator delete(obj); }
ResFile::ResFile(const char* fileName, UInt16 fileCode) 
    : fileCode(fileCode) 
{
    // create local copy of file name
    UInt32 len = strlen(fileName);
    char* buffer = 0;
    if (len)
    {
        buffer = new char[len+1];
        strcpy_s(buffer,len+1,fileName);
    }
    else buffer = 0;
    this->fileName = buffer;
}
ResFile::~ResFile()
{
    // free string buffer
    if (fileName) delete fileName;
}
const char* ResFile::Name() const { return fileName ? fileName : ""; }
UInt16 ResFile::Code() const { return fileCode; }
bool ResFile::Load(TESFileInterface& file)
{
    return false; // TODO
}
bool ResFile::Write(TESFileInterface& file) const
{
    return false; // TODO
}
bool ResFile::operator < (const ResFile& rhs) const { return fileCode < rhs.fileCode; }
/*************************************  ResClass *******************************************************/
void* ResClass::operator new (size_t size) { return ::operator new(size); }
void ResClass::operator delete (void *obj) { ::operator delete(obj); }
ResClass::ResClass(const char* className, const ValueT& rangeHigh, const ValueT& rangeLow, UInt16 classCode)
    : classCode(classCode), rangeHigh(rangeHigh), rangeLow(rangeLow)
{
    UInt32 len = 0;
    char* buffer = 0;

    // create local copy of class name
    len = strlen(className);
    if (len)
    {
        buffer = new char[len+1];
        strcpy_s(buffer,len+1,className);
    }
    else buffer = 0;
    this->className = buffer;
}
ResClass::~ResClass()
{
    // free string buffers
    if (className) delete className;
}
const char* ResClass::Name() const { return className ? className : ""; }
UInt16 ResClass::Code() const { return classCode; }
const ResClass::ValueT& ResClass::RangeLow() const { return rangeLow; }
const ResClass::ValueT& ResClass::RangeHigh() const { return rangeHigh; }
bool ResClass::Load(TESFileInterface& file) 
{
    return false; // TODO
}
bool ResClass::Write(TESFileInterface& file) const
{
    return false; // TODO
}
bool ResClass::operator < (const ResClass& rhs) const { return classCode < rhs.classCode; }
/*************************************  ResValue *******************************************************/
void* ResValue::operator new (size_t size) { return ::operator new(size); }
void ResValue::operator delete (void *obj) { ::operator delete(obj); }
ResValue::ResValue(const char* valueName, ResFile* resFile, ResClass* resClass, const ValueT& value)
    : value(value), resFile(resFile), resClass(resClass)
{
    UInt32 len = 0;
    char* buffer = 0;

    // create local copy of value name
    len = strlen(valueName);
    if (len)
    {
        buffer = new char[len+1];
        strcpy_s(buffer,len+1,valueName);
    }
    else buffer = 0;
    this->valueName = buffer;
}
ResValue::~ResValue()
{
    // free string buffer
    if (valueName) delete valueName;
}
const char* ResValue::Name() const { return valueName ? valueName : ""; }
ResClass* ResValue::Class() const { return resClass; }
ResFile* ResValue::File() const { return resFile; }
const ResValue::ValueT& ResValue::Value() const { return value; }
bool ResValue::Load(TESFileInterface& file) 
{
    return false; // TODO
}
bool ResValue::Write(TESFileInterface& file) const
{
    return false; // TODO
}
bool ResValue::operator < (const ResValue& rhs) const { return value < rhs.value; }
/*************************************  ResField *******************************************************/
void* ResField::operator new (size_t size) { return ::operator new(size); }
void ResField::operator delete (void *obj) { ::operator delete(obj); }
ResField::ResField(UInt16 offset, UInt16 size, ResClass* resClass, SInt8 depth) 
    : offset(offset), size(size), resClass(resClass), depth(depth) 
{
    if (depth < 0) this->depth = 0;
}
ResField::~ResField() {}
UInt16 ResField::Offset() const { return offset; }
UInt16 ResField::Size() const { return size; }
ResClass* ResField::Class() const { return resClass; }
SInt8 ResField::Depth() const { return depth; }
bool ResField::operator < (const ResField& rhs) const 
{ 
    if (offset < rhs.offset) return true;
    else if (offset > rhs.offset) return false;
    else return depth > rhs.depth; 
}
/*************************************  ResFormat *******************************************************/
class ResFormat::ResFieldList : public SortedCollection<ResField> {};
void* ResFormat::operator new (size_t size) { return ::operator new(size); }
void ResFormat::operator delete (void *obj) { ::operator delete(obj); }
ResFormat::ResFormat(UInt32 recordType, UInt32 chunkType)
    : recordType(recordType), chunkType(chunkType)
{
    // construct list object
    fieldList = new ResFieldList;
}
ResFormat::~ResFormat()
{
    // clean up list object
    if (fieldList) delete fieldList;
}
UInt32 ResFormat::RecordType() const { return recordType; }
UInt32 ResFormat::ChunkType() const { return chunkType; }
bool ResFormat::AddField(ResField* field) { return fieldList->Insert(field); }
void ResFormat::RemoveField(ResField* field, bool orphan) { return fieldList->Remove(field, orphan); }
void ResFormat::RemoveAllFields(UInt8 specificDepth, bool orphan) 
{ 
    if (specificDepth < 0) fieldList->RemoveAll(orphan); // remove all fields
    
    // remove only fields at specified depth
    for (ResField* field = fieldList->GetNext(0); field; )
    {
        ResField* next = fieldList->GetNext(field);
        if (field->Depth() == specificDepth) fieldList->Remove(field, orphan); 
        field = next;
    }
}
ResField* ResFormat::GetNextField(ResField* currentField) const { return fieldList->GetNext(currentField); }
bool ResFormat::Load(TESFileInterface& file) 
{
    return false; // TODO
}
bool ResFormat::Write(TESFileInterface& file) const
{
    return false; // TODO
}
bool ResFormat::operator < (const ResFormat& rhs) const 
{ 
    if (recordType < rhs.recordType) return true;
    else if (recordType > rhs.recordType) return false;
    else return chunkType < rhs.chunkType; 
}