/*
    OutputLog - flexible debug logging class
    Requires a TR1 compatibile C++ compiler (for <regex>)

    OutputLog is basically a more sophisticated printf() function.  It supports multiple "channels" 
    ("Error", "Warning", "Message", etc.) and multiple 'targets' (files, std::ostreams, etc).  
    
    Output is passed using the Print or PrintF ('Print Formatted') member functions.  Each call must specify
    the target channel(s), a string specifying the "source" (usually the local function or file name), and 
    the actual (formatted) output string.  The result is further formatted by any output targets it is passed to.
   
    Targets can be added and removed with AttachTarget() and RemoveTarget().  Included are some useful 
    target classes:
        FunctionTarget: output to a pointer-to-function with signature 'void (const char* text)'
        OStreamTarget: output to a std::ostream
        FileTarget: (plaintext) output to a file
        HTMLTarget: html-formatted output to a file
    Custom target classes may be derived from the base target class ('OutputTarget') or any of the included target
    classes.  This allows output to be directed anywhere, in any format required.
    
    All of the included targets can be filtered by channel and source using a list of 'rules'.  Each rule 
    overrides those before it, so they should get more specific as they are added.  For example, to restrict 
    output to errors from sources starting with "foo", but not including "bar":

        // add target to output log
        someOutputLog.AttachTarget(someTarget);
        // Rule #1, Block all output:
        someTarget.AddRule(kRuleState_Block, kChannel__ALL, ".*");      
        // Rule #2, Allow errors from source starting with 'foo':
        someTarget.AddRule(kRuleState_Print, kChannel_FatalError|kChannel_Error, "foo.*");
        // Rule #3, Block (again) anything containing the word 'bar':
        someTarget.AddRule(kRuleState_Block, kChannel__ALL, ".*bar.*");

    The third parameter to the rule is an ECMA regular expression to be matched against source strings.

    Print() and PrintF() return zero if the specified message + source were not printed to any targets on
    any channels.  For efficiency reasons, it is a good idea to store this value in a static "guard" variable, 
    and skip that print statement in the future if the variable is 0.  This eliminates future overhead from 
    the Print function (which is significant), and also any overhead from evaluating the arguments.  With this 
    approach it is possible to leave active debugging statements in released code with minimal performance loss.  
    The tradeoff is that the rule list can only be modified once, before any calls to Print() or PrintF() are made.
    A set of convenience macros is defined to implement this "guard" variable approach.  See their definition
    for details.
*/
#pragma once

#include <vector>
#include <ostream>
#include <fstream>

// OutputStyle - formatting variables for the entire log
class OutputStyle
{        
public:
    // style variables
    int     indent;         // indent level of message
    bool    includeChannel; // prepend channel name to output
    bool    includeSource;  // prepend source string to output
    bool    includeTime;    // prepend timestamp to output
    // methods
    OutputStyle(); // assign default values.  use compiler-generated copy constructor
    static const char*  GetTimeString(time_t time); // default formatted time string
    static const char*  GetChannelLabel(int channel); // default label for specified channel
};

// OutputTarget - output-specific formatting
class OutputTarget
{
public:
    // construction, destruction
    OutputTarget();
    virtual ~OutputTarget();
    // interface
    virtual void    WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text) = 0; // 
                    // writes complete formatted output line(s) to the specified channel(s), terminated with a newline or equivalent
    virtual int     WriteableChannels(const char* source) = 0; // returns bitmask of channels writeable to from the specified source
};

// OutputLog - general interface for output logging
class OutputLog
{
public:
    OutputLog();
    virtual ~OutputLog();

    static const unsigned int kChannelCount = 6;
    enum Channels
    {
        kChannel__NONE          = 0x00,
        kChannel_FatalError     = 0x01, // 0
        kChannel_Error          = 0x02, // 1
        kChannel_Warning        = 0x04, // 2
        kChannel_Message        = 0x08, // 3
        kChannel_VerboseMessage = 0x10, // 4
        kChannel_DebugMessage   = 0x20, // 5
        kChannel__ALL           = 0xFF,
    };    

    // manage output targets
    virtual void    AttachTarget(OutputTarget& target); 
    virtual void    DetachTarget(OutputTarget& target);

    // output functions
    virtual int     Print(int channel, const char* source, const char* text); // returns channel(s) successfully printed on
    virtual int     PrintF(int channel, const char* source, const char* format, ...); // returns channel(s) successfully printed on

    // output style
    OutputStyle&    Style(); // returns current style
    void            PushStyle(); // push current style onto stack & set new style to default
    void            PushStyle(const OutputStyle& newStyle); // push current style onto stack & set new style to copy of newStyle
    void            PopStyle(); // restore last saved style state from stack
    void            Indent(); // (for compatibility) increases indent of current style
    void            Outdent(); // (for compatibility) decreases indent of current style

protected:

    // output targets
    typedef std::vector<OutputTarget*> TargetList;
    TargetList   _targets;
    
    // output styles
    typedef std::vector<OutputStyle*> StyleList;
    StyleList   _styles;
};

/* 
    Shortcut macros - 
    Print output to the specified channel, using the local function name as the source.
    Require that the global variable 'gLog' be declared, and may not work with non-microsoft compilers.
    The 'Optimizer' macro causes the result of the function call to be cached in a static "guard" variable,
    so that if the print fails once it will never be called again.  This greatly reduces the overhead of 
    embedded debugging output, but it basically means that the log rules can't be changed once loaded.  
    To disable the guards and let the print function call to proceed every time, delete everything after
    the comment in the OPTIMIZER_CHECK macro below.
*/
#define OPTIMIZER_CHECK /* optional: delete everything after this comment */ if (static int __optguard__ = 1) __optguard__ = 
extern OutputLog& gLog;
#define _FATALERROR(format, ...) OPTIMIZER_CHECK gLog.PrintF(OutputLog::kChannel_FatalError,__FUNCTION__,format,## __VA_ARGS__)
#define _ERROR(format, ...) OPTIMIZER_CHECK gLog.PrintF(OutputLog::kChannel_Error,__FUNCTION__,format,## __VA_ARGS__)
#define _WARNING(format, ...) OPTIMIZER_CHECK gLog.PrintF(OutputLog::kChannel_Warning,__FUNCTION__,format,## __VA_ARGS__)
#define _MESSAGE(format, ...) OPTIMIZER_CHECK gLog.PrintF(OutputLog::kChannel_Message,__FUNCTION__,format,## __VA_ARGS__)
#define _VMESSAGE(format, ...) OPTIMIZER_CHECK gLog.PrintF(OutputLog::kChannel_VerboseMessage,__FUNCTION__,format,## __VA_ARGS__)
#define _DMESSAGE(format, ...) OPTIMIZER_CHECK gLog.PrintF(OutputLog::kChannel_DebugMessage,__FUNCTION__,format,## __VA_ARGS__)

// RuleBasedTarget - an output target that uses a list of rules to filter output by source+channel
class RuleBasedTarget : public OutputTarget
{
public:
    // construction, destruction
    RuleBasedTarget();
    virtual ~RuleBasedTarget();    
    // interface
    virtual int     WriteableChannels(const char* source);
    // rules    
    enum RuleStates
    {
        kRuleState_Print,
        kRuleState_Block,
    };
    void*       AddRule(int state, int channel, const char* filter); // add an output control rule to the end of the rule list.  
                // returns a 'rule handle' that can be used for later manipulation
    void        RemoveRule(void* ruleHandle); // remove the rule with the specified handle from the rule list
    void        ClearRules(); // remove all rules from the rule list

protected:
    // rule data
    struct Rule
    {      
        int     _state;      // print/block matching output
        int     _channel;    // channel(s) to match
        void*   _filter;     // regex object for sources to match
        // methods
        Rule(int state, int channel, const char* sourceFilter);
        ~Rule();
    };
    typedef std::vector<Rule*> RuleList;
    RuleList    _outputRules;
};

// BufferTarget - a rule-based target that stores the last output line in a buffer
class BufferTarget : public RuleBasedTarget
{
public:
    // construction, destruction
    BufferTarget();
    virtual ~BufferTarget();
    // interface
    virtual void    WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text);
    // output access
    const char*     LastOutputLine();
protected:
    unsigned int    _size;
    char*           _buffer;
};

// FunctionTarget - a buffered target that passes output to a specified pointer-to-function 
class FunctionTarget : public BufferTarget
{
public:
    typedef void (* WriteFunction)(const char* text);
    // construction, destruction
    FunctionTarget(WriteFunction out); // throws exception if out is not a valid function pointer
    // interface
    virtual void    WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text);
protected:
    WriteFunction   _out;
};

// OStreamTarget - a rule-based target that prints unadorned text to a std::ostream
class OStreamTarget : public RuleBasedTarget
{
public:
    // construction, destruction
    OStreamTarget(std::ostream& out);
    // interface
    virtual void    WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text);
protected:
    std::ostream&   _out;
};

// FileFormatter - an ostream target that prints to an associated file
class FileTarget : public OStreamTarget
{
public:
    // construction, destruction
    FileTarget(const char* filename = 0);    // raises exception if file is write locked
    virtual ~FileTarget();
protected:
    std::fstream    _fout;
};

// HTMLTarget - a file target that prints output as an html table
class HTMLTarget : public FileTarget
{
public:
    // construction, destruction
    HTMLTarget(const char* filename, const char* title = 0);    // raises exception if file is write locked
    virtual ~HTMLTarget();
    // interface
    virtual void    WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text);
protected:
    bool    _lineParity;    // next line is even or odd
};