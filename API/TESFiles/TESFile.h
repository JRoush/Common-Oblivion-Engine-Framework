/* 
    TESFile defines a single esp or esm file.
*/
#pragma once

// base classes
#include "API/TES/MemoryHeap.h"
#include "API/BSTypes/BSStringT.h"
#include "API/BSTypes/BSSimpleList.h"
#include "API/TESForms/TESForm.h"   // TrackingData

// argument classes
class   BSFile;  
class   TESForm;    // TESForms/TESForm.h

class IMPORTCLASS ChunkInfo
{// size 08/08 (NOTE: 06 for non-record chunks on disk, see below)
/*
    A "chunk" is the basic unit of the TES4 mod file. A chunk is headed by a type code and size field, followed by binary data
    This struct is public (not a subclass of TESFile) because it is also used by RecordInfo
*/
public:
	MEMBER /*00*/ UInt32    chunkType; // e.g. 'GRUP', 'GLOB', etc.
	MEMBER /*04*/ UInt32    chunkLength; // size from end of chunk header (NOTE: chunkLength field is a UInt16 for non-record chunks on disk)
    // ... data ...
};

class IMPORTCLASS RecordInfo : public ChunkInfo
{// size 14/14
/*
    Records are chunks whose data consists entirely of subchunks.  They have additional identifying information in the header
    During serialization, each form corresponds to a single record, but there are also a few records (e.g. GRUP,TES4) that do not correspond to any form
    This struct is public (not a subclass of TESFile) because it is also used by TESForm.h
*/
public:

    enum RecordFlags
    {
        // not sure what flags are specific to forms, references, or records, and what is shared
        kRecordFlags__FormFlags                = 0x000A8EE0, // flag bits copied froms fors for form records
        kRecordFlags__FileFlags                = 0xFF000091, // flag bits copied from files for TES4 records  
        kRecordFlags_Ignored            = /*0C*/ 0x00001000, // record is ignored during loading
        kRecordFlags_Compressed         = /*12*/ 0x00040000, // record data is compressed using ZLib.  Note that group records cannot be compressed
    };

	//     /*00*/ ChunkInfo
	MEMBER /*08*/ UInt32        recordFlags; //
                                // form records: see 'record flags' above
                                // TES4 records: fileFlags
                                // group records: contained form chunk type / parent formid / block number / grid coords
	MEMBER /*0C*/ UInt32        recordID; // 
                                // form records: formID
                                // TES4 records: 0
                                // group records: group class (0-10) indicating what kind of group record
    MEMBER /*10*/ TrackingData  trackingData; // used for internal revision control
};

class IMPORTCLASS TESFile
{// size 420
public:

    static const UInt32 kMAX_PATH = 260; // i.e. windows constant MAX_PATH

    enum FileFlags
    {
        kFileFlag__SavedInRecord       = 0xFF000091, // flag bits copied from TES4 record        
        kFileFlag_Master        = /*00*/ 0x00000001, // set for master files (as indicated by bit in TES4 record flags)
        kFileFlag_Open          = /*01*/ 0x00000002, // set in OpenBSFile()
        kFileFlag_Loaded        = /*02*/ 0x00000004, // flags file for loading, set before file is actually loaded
        kFileFlag_Active        = /*03*/ 0x00000008, // set for the currently active file
    };    

    enum FileErrorStates
    {   
        kFileState_None         = 0x0,
        kFileState_Unk2         = 0x2, // set in OpenBSFile
        kFileState_Unk9         = 0x9, // set in OpenBSFile
        kFileState_WriteError   = 0xA,
        kFileState_UnkC         = 0xC, // set in OpenBSFile
    };    

    // file header info
    struct IMPORTCLASS FileHeaderInfo
    {
        MEMBER /*00*/ float         fileVersion;
        MEMBER /*04*/ UInt32        numRecords; // number of record blocks in file
        MEMBER /*08*/ UInt32        nextFormID; // inluding file index in highest byte
    };

    // Data for Master files - compared against size in findData of masters to check if they have changed since last edit
    struct IMPORTCLASS MasterFileData
    {
        MEMBER /*00*/ DWORD nFileSizeHigh;
        MEMBER /*04*/ DWORD nFileSizeLow;
    };
    typedef BSSimpleList<MasterFileData*> MasterDataList;
    typedef BSSimpleList<const char*> MasterNameList;

    class IMPORTCLASS GroupInfo : public RecordInfo
    {// size 18/18
        //     /*00*/ RecordInfo    // for group records, the size includes the 14 bytes of the header
	    MEMBER /*14*/ UInt32        recordOffset;   // used internally to track header offsets of all open groups
    };
    typedef BSSimpleList<GroupInfo*> GroupList;

    // members
	MEMBER /*000*/ UInt32	            errorState;
	MEMBER /*004*/ UInt32	            unkFile004;
	MEMBER /*008*/ UInt32	            unkFile008;
	MEMBER /*00C*/ BSFile*	            unkFile00C; // temp file for backups?
	MEMBER /*010*/ BSFile*              bsFile; // used for actual read from / write to disk operations
	MEMBER /*014*/ UInt32	            unkFile014;
	MEMBER /*018*/ UInt32	            unkFile018;
	MEMBER /*01C*/ char	                fileName[kMAX_PATH]; 
	MEMBER /*120*/ char	                filePath[kMAX_PATH]; // relative to "Oblivion\"
	MEMBER /*224*/ void*	            unkFile224; // simple object, no destructor
	MEMBER /*228*/ UInt32	            unkFile228; // init to 0x2800
	MEMBER /*22C*/ UInt32	            unkFile22C;
    MEMBER /*230*/ UInt32	            unkFile230;
    MEMBER /*234*/ UInt32	            unkFile234;
    MEMBER /*238*/ UInt32	            unkFile238;
	MEMBER /*23C*/ RecordInfo	        currentRecord;
	MEMBER /*250*/ ChunkInfo	        currentChunk;
    MEMBER /*258*/ UInt32	            fileSize; // same as FileSizeLow in find data
    MEMBER /*25C*/ UInt32	            currentRecordOffset; // offset of current record in file
    MEMBER /*260*/ UInt32	            currentChunkOffset; // offset of current chunk in record
    MEMBER /*264*/ UInt32	            fetchedChunkDataSize; // number of bytes read in last GetChunkData() call
    MEMBER /*268*/ GroupInfo	        unkFile268; // used when saving empty form records, e.g. for deleted forms
    MEMBER /*280*/ UInt32	            unkFile280; // used when saving empty form records, e.g. for deleted forms
    MEMBER /*284*/ GroupList	        openGroups; // stack of open group records, from lowest level to highest
    MEMBER /*28C*/ bool                 headerRead; // set after header has been successfully parsed
    MEMBER /*28D*/ UInt8                padFile28D[3];
	MEMBER /*290*/ WIN32_FIND_DATA	    findData;
	MEMBER /*3D0*/ FileHeaderInfo       fileHeader;
	MEMBER /*3DC*/ UInt32	            fileFlags;
	MEMBER /*3E0*/ MasterNameList       masterNames;
	MEMBER /*3E8*/ MasterDataList       masterData;
	MEMBER /*3F0*/ UInt32	            masterCount;
	MEMBER /*3F4*/ TESFile**            masterFiles; // pointer to TESFile*[parentCount] of currently loaded masters
	MEMBER /*3F8*/ UInt32	            unkFile3F8;
	MEMBER /*3FC*/ UInt32	            unkFile3FC;
	MEMBER /*400*/ UInt8	            fileIndex; // index of this file in load order (or 0xFF if not loaded) 
	MEMBER /*401*/ UInt8	            padFile401[3];
	MEMBER /*404*/ BSStringT            authorName;
	MEMBER /*40C*/ BSStringT	        description;
	MEMBER /*414*/ void*	            currentRecordDCBuffer; // buffer for decompressed record data
    MEMBER /*418*/ UInt32	            currentRecordDCLength; // length of decompressed record data
    MEMBER /*41C*/ TESFile*             unkFile41C; // file this object was cloned from. used for local copies of network files?

    // methods - master files
    IMPORT bool             HaveMastersChanged(); // checks actual master file size against MasterFileData stored in this file, for all masters
    IMPORT TESFile*         GetMasterByIndex(UInt32 index); // return master file by 1-based index.  
                            // Index 0 returns this file, and invalid indicies return null
    IMPORT UInt32           GetMasterCount();
    #ifndef OBLIVION
    IMPORT UInt32           GetMasterIndex(); // completely inlined in game code
    #endif
    // methods - open/close/seek
    IMPORT UInt32           Open(); // opens file using path+name members, loads header record & initializes.  
                            // returns non-zero error code on failure.
    IMPORT bool             Close(); // closes open file.  returns false on failure
    IMPORT UInt32           JumpToBeginningOfFile(bool getFirstRecordType); // move file pointer to beginning of file, then reloads the header
                            // optionally loads the header for the first record & returns it's type, otherwise returns 0
    // methods - loading records
    IMPORT void             InitializeFormFromRecord(TESForm& form); // set form type, id, etc. from current record header info
    IMPORT bool             GetNextRecord(bool skipIgnoredRecords); // advances file pointer to next record & reads header.  
                            // returns false on failure, usually indicating the end of file
    IMPORT UInt8            GetRecordType(); // returns type of current record, zero for invalid or no record
    IMPORT UInt32           JumpToBeginningOfRecord(); // move first chunk in current record, returns chunk type  
    IMPORT void*            GetDecompressedRecordData(); // decompresses the record if necessary, and returns a pointer to the decompressed data 
    // methods - loading chunks
    IMPORT bool             GetChunkData(void* buffer, UInt32 size); // loads data from current chunk into buffer. returns false on failure.                            
                            // number of bytes copied is stored in fetchedChunkDataSize. buffer must be large enough accept *entire* chunk, or a 
                            // warning is printed and the chunk is truncated. if the chunk is truncated, the function will still return true.
                            // compressed record data is decompressed, if it has not been already.
                            // this function may be called multiple times to load the same chunk repeatedly.
    IMPORT bool             GetChunkData2Bytes(UInt16& buffer); // shortcut for loading short ints
    IMPORT bool             GetChunkData4Bytes(UInt32& buffer); // shortcut for loading long ints
    IMPORT UInt32           GetChunkType(); // get type (e.g. 'DATA' or 'FULL') of current chunk, zero for invalid or no chunk
    IMPORT bool             GetNextChunk(); // loads next chunk in record.  returns false on failure, usually indicating end of record
    // methods - saving
    IMPORT UInt32           WriteData(void* buffer, UInt32 size); // writes data to file, starting at current file pointer
                            // returns non-zero error code on failure.
    IMPORT UInt32           WriteFormRecord(TESForm& form); // updates group records as needed, then writes contents of formRecordBuffer to file
                            // returns non-zero error code on failure.
    // methods - saving group records
    IMPORT void             PushGroupInfo(RecordInfo& groupRecord); // makes a GroupInfo from record info, and pushes onto sack of open group records   
    IMPORT void             PopGroupInfo(); // pops group from stack of open group records
    IMPORT void             OpenGroupRecord(RecordInfo& groupRecord); // push group record onto stack and write to file
    IMPORT void             CloseGroupRecord(); // finalizes group record size on disk, then pops from stack
    IMPORT void             UpdateOpenGroups(TESForm& form); // close group records that fail TESForm::MatchGroupRecord(), then open
                            // group records using TESForm::CreateGroupRecord
    // methods - file properties
    IMPORT bool             GetLastWriteTime(LPSYSTEMTIME time); // fetch last edit time of file, in system time format 
    IMPORT UInt8            GetFileIndex(); // get index of this file in current load order
    IMPORT void             SetFileIndex(UInt8 index); // sets file's internal index
                            // does *not* actually move file in load order
    IMPORT bool             IsActiveFile(); // returns true if this file is the active file
    IMPORT void             SetIsActiveFile(bool isActiveFile);
    IMPORT bool             IsMaster(); // return true if file is an esm
    #ifndef OBLIVION
    IMPORT void             SetIsMaster(bool isMaster); // completely inlined in game
    #endif
    IMPORT bool             IsLoaded(); // returns true if file is flagged for loading (set before file is actually loaded)
    IMPORT void             SetIsLoaded(bool isLoaded); // set/clear flag for file loading - takes effect when file list is processed
    // static methods
    IMPORT static UInt8     RecordTypeFromChunkType(UInt32 chunkType); // returns record type (i.e. form type) for a given chunk type
                            // e.g. 'MGEF' -> 0xC, see TESForm::FomrTypes for complete enumeration

    // constructor, destructor
    IMPORT TESFile();
    IMPORT ~TESFile();

    // use FormHeap for class new & delete
    USEFORMHEAP 

    // global static objects
    IMPORT static void*             formRecordBuffer; // static temporary buffer for the form currently being saved
    IMPORT static UInt32            formRecordBufferSize; // size of static buffer
};

