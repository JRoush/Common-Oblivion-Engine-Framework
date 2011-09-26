/*   
    ExtendedResolution

    A complete system for managing "Context" information that maps short resource codes to universal identifiers.
*/
#pragma once
#ifndef COMPONENT_EXPORT
    #define COMPONENT_EXPORT _declspec(dllimport) 
#endif

class COMPONENT_EXPORT TESFileInterface
{
public:
    
    // Loading
    virtual bool        LoadNextRecord() = 0;   // load next record header and first chunk
    virtual UInt32      LoadRecordType() = 0;   // type code for current record
    virtual bool        LoadNextChunk() = 0;    // load next chunk header and data
    virtual UInt32      LoadChunkType() = 0;    // type code for current chunk
    virtual UInt32      LoadChunkSize() = 0;    // data size of current chunk
    virtual bool        LoadChunkData(void* buffer, UInt32 size) = 0;   // copy data from current chunk into buffer

    // Writing
    virtual bool        OpenGroup(UInt32 flags, UInt32 id) = 0;     // push new group into group stack, write group header
    virtual bool        CloseGroup() = 0;                           // pop top group from stack, write size in group header
    virtual bool        OpenRecord(UInt32 type, UInt32 flags, UInt32 id) = 0;   // set current record (close prev record if necessary), write record header
    virtual bool        CloseRecord() = 0;                          // clear current record, write size in record header
    virtual bool        WriteChunk(UInt32 type, void* buffer, UInt32 size) = 0;
};

class COMPONENT_EXPORT ResFile
{
public:

    // constructor, destructor
    static void* operator new (size_t size); 
    static void operator delete (void *obj);
    ResFile(const char* fileName, UInt16 fileCode = 0);
    ~ResFile();

    // access
    const char* Name() const;
    UInt16      Code() const;

    // serialization
    static const UInt32 kFileChunkType = 'FSER';    // 'RESF'
    bool        Load(TESFileInterface& file);       // load file from chunk
    bool        Write(TESFileInterface& file) const;      // write file to chunk

    // comparison
    bool        operator < (const ResFile& rhs) const;

protected:

    // members
    const char* fileName;
    UInt16      fileCode;
};

template <typename ValueT> class COMPONENT_EXPORT ResClass
{
public:

    // constructor, destructor
    static void* operator new (size_t size); 
    static void operator delete (void *obj);
    ResClass(const char* className, const ValueT& rangeHigh = 0, const ValueT& rangeLow = 0, UInt16 classCode = 0);
    ~ResClass();

    // access
    const char*     Name() const;
    UInt16          Code() const;
    const ValueT&   RangeLow() const;
    const ValueT&   RangeHigh() const;

    // serialization
    static const UInt32 kClassChunkType = 'CSER';    // 'RESC'
    bool        Load(TESFileInterface& file);           // load class from chunk
    bool        Write(TESFileInterface& file) const;    // write class to chunk

    // comparison
    bool        operator < (const ResClass& rhs) const;

protected:

    // members
    const char* className;
    UInt16      classCode;
    ValueT      rangeLow;
    ValueT      rangeHigh;
};

class COMPONENT_EXPORT ResValue
{
public:
    
    // value type def
    typedef UInt32 ValueT;

    // constructor, destructor
    static void* operator new (size_t size); 
    static void operator delete (void *obj);
    ResValue(const char* valueName, ResFile* resFile, ResClass* resClass, const ValueT& value = 0);
    ~ResValue();

    // access
    const char*     Name() const;
    ResClass*       Class() const;
    ResFile*        File() const;
    const ValueT&   Value() const;

    // serialization
    static const UInt32 kClassChunkType = 'VSER';    // 'RESV'
    bool        Load(TESFileInterface& file);           // load value from chunk
    bool        Write(TESFileInterface& file) const;    // write value to chunk

    // comparison
    bool        operator < (const ResValue& rhs) const;

protected:

    // members
    const char* valueName;
    ResFile*    resFile;
    ResClass*   resClass;
    ValueT      value;
};

class COMPONENT_EXPORT ResField
{
public:

    static const SInt8 kAllDepths = -1;

    // constructor, destructor
    static void* operator new (size_t size); 
    static void operator delete (void *obj);
    ResField(UInt16 offset, UInt16 size, ResClass* resClass, SInt8 depth = 0);
    ~ResField();

    // access
    UInt16      Offset() const;
    UInt16      Size() const;
    ResClass*   Class() const;
    SInt8       Depth() const;

    // comparison
    bool        operator < (const ResField& rhs) const;

protected:

    // members
    UInt16      offset;
    UInt16      size;
    ResClass*   resClass;
    SInt8       depth;
};

class COMPONENT_EXPORT ResFormat
{
public:
    
    // constructor, destructor
    static void* operator new (size_t size); 
    static void operator delete (void *obj);
    ResFormat(UInt32 recordType, UInt32 chunkType);
    ~ResFormat();

    // access
    UInt32          RecordType() const;
    UInt32          ChunkType() const;
    bool            AddField(ResField* field); // adopts field object
    void            RemoveField(ResField* field, bool orphan = false);
    void            RemoveAllFields(UInt8 specificDepth = ResField::kAllDepths, bool orphan = false); 
    ResField*       GetNextField(ResField* currentField = 0) const;   // returns classes in ascending order by offset, descending order by depth.  pass null to get first field    

    // serialization
    static const UInt32 kClassChunkType = 'MSER';    // 'RESM'
    bool            Load(TESFileInterface& file);           // load value from chunk
    bool            Write(TESFileInterface& file) const;    // write value to chunk

    // comparison
    bool        operator < (const ResFormat& rhs) const;

protected:

    // field list class def
    class ResFieldList;

    // members
    UInt32          recordType;
    UInt32          chunkType;
    ResFieldList*   fieldList;
};

class COMPONENT_EXPORT ResContext
{
public:

    // constructor, destructor
    ResContext();
    ~ResContext();

    // file list methods.  non-const methods will automatically update Values and ResOperation objects
    static const UInt32 kContextFileRecordType  = 'FSER';   // 'RESF'
    bool        AddFile(ResFile* file, bool autoAssignCode = false); // adopts file object.  optionally assigns next available code.
    void        RemoveFile(ResFile* file, bool orhpan = false);
    void        RemoveAllFiles(bool orhpan = false);
    ResFile*    GetFile(UInt16 fileCode) const; // fast lookup by fileCode
    ResFile*    GetFile(const char* fileName) const; // fast lookup by fileName
    ResFile*    GetNextFile(ResFile* currentFile = 0) const; // returns files in ascending order by code.  pass null to get first file
    bool        SetFileCode(ResFile* file, UInt16 code, bool swapExisting = false); // removes original file at that code or swaps with it
    bool        LoadFileList(TESFileInterface& file); // load file list from record (doesn't clear list first)
    bool        WriteFileList(TESFileInterface& file) const; // write file list in record    

    // class list methods.  non-const methods will automatically update Values, Formats, and ResOperation objects
    static const UInt32 kContextClassRecordType = 'CSER';   // 'RESC'
    bool        AddClass(ResClass* resClass, bool autoAssignCode = false); // adopts class object.  optionally assigns next available code.
    void        RemoveClass(ResClass* resClass, bool orhpan = false);
    void        RemoveAllClasses(bool orhpan = false);
    ResClass*   GetClass(UInt16 classCode) const; // fast lookup by classCode
    ResClass*   GetClass(const char* className) const; // fast lookup by className
    ResClass*   GetNextClass(ResClass* currentClass = 0) const; // returns classes in ascending order by code.  pass null to get first class
    bool        SetClassCode(ResClass* resClass, UInt16 code, bool swapExisting = false); // removes/swaps with original class at that code
    bool        LoadClassList(TESFileInterface& file); // load class list from record (doesn't clear list first)
    bool        WriteClassList(TESFileInterface& file) const; // write class list in record  

    // value list methods.  non-const methods will automatically update ResOperation objects
    static const UInt32 kContextValueRecordType = 'VSER';   // 'RESV'
    bool        AddValue(ResValue* value, bool autoAssignValue = false); // adopts value object.  optionally assigns next available value within class range.
    void        RemoveValue(ResValue* resClass, bool orhpan = false); 
    void        RemoveAllValues(bool orhpan = false);
    ResValue*   GetValue(ResClass* resClass, const ResValue::ValueT& value) const; // fast lookup by class & value
    ResValue*   GetClass(ResClass* resClass, const char* valueName) const; // fast lookup by class & valueName
    ResValue*   GetNextValue(ResValue* currentValue = 0) const; // returns values in ascending order by Value.  pass null to get first value
    bool        SetValue(ResValue* resValue, const ResValue::ValueT& value, bool swapExisting = false); // removes/swaps with original object at that value
    bool        LoadValueList(TESFileInterface& file); // load value list from record (doesn't clear list first)
    bool        WriteValueList(TESFileInterface& file) const; // write value list in record

    // format list methods
    static const UInt32 kContexFormatRecordType = 'MSER';   // 'RESM'
    bool        AddFormat(ResFormat* format); // adopts format object
    void        RemoveFormat(ResFormat* format, bool orhpan = false); 
    void        RemoveAllFormats(UInt8 specificDepth = ResField::kAllDepths, bool orhpan = false);
    ResFormat*  GetFormat(UInt32 recordType, UInt32 chunkType) const; // fast lookup by record & chunk type
    ResFormat*  GetNextFormat(ResFormat* currentFormat = 0) const; // returns formats in ascending order by record+chunk type.  pass null to get first format
    bool        LoadFormatList(TESFileInterface& file); // load format list from record (doesn't clear list first)
    bool        WriteFormatList(TESFileInterface& file, UInt8 specificDepth = ResField::kAllDepths) const; // write format list in record

    // entire context methods
    static const UInt32 kContextGroupFlags      = 'XSER';    // 'RESX'    
    void        ResetContext();    // clear all internal data
    bool        LoadContext(TESFileInterface& file);    // load complete context from group
    bool        WriteContext(TESFileInterface& file);   // write complete context in group

    // resolution methods
    

protected:

    // members
};

