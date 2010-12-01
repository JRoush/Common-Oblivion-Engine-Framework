#include "OutputLog.h"
#include <algorithm>
#include <cstdarg>
#include <regex>
#include <ctime>

#pragma warning (disable: 4996) // 'unsafe' c-style string manipulation functions

/************************** OutputStyle ************************************/
OutputStyle::OutputStyle()
    : indent(0), includeChannel(true), includeSource(true), includeTime(true)
{
}
const char* OutputStyle::GetTimeString(time_t time)
{
    static char timebuffer[20];           
    tm* Tm = localtime(&time); // convert to local time
    sprintf(timebuffer,"%04i-%02i-%02i %02i:%02i:%02i",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,Tm->tm_min,Tm->tm_sec);
    return timebuffer;
}
const char* OutputStyle::GetChannelLabel(int channel)
{
    static const char*  kChannelLabels[OutputLog::kChannelCount] = {"Fatal Error","Error","Warning","Message","Verbose Msg","Debug Msg"};
    for (int i = 0; i < OutputLog::kChannelCount; i++)
    {
        if ((1 << i) & channel) return kChannelLabels[i];
    }
    return "";
}
/************************** OutputTarget ************************************/
OutputTarget::OutputTarget() {}
OutputTarget::~OutputTarget() {}
/************************** OutputLog ************************************/
OutputLog::OutputLog()
{
    _styles.push_back(new OutputStyle); // create a default style
}
OutputLog::~OutputLog()
{    
    // delete dynamically allocated style objects in style stack
    for (StyleList::iterator it = _styles.begin(); it != _styles.end(); it++) 
    {
        if (*it) delete *it;
    }
    // clear target list
    _targets.clear();
}
void OutputLog::AttachTarget(OutputTarget& target)
{
    if (std::find(_targets.begin(),_targets.end(),&target) == _targets.end()) 
    {
        // target not found in list, append to end
        _targets.push_back(&target); 
    } 
}
void OutputLog::DetachTarget(OutputTarget& target)
{
    TargetList::iterator it = std::find(_targets.begin(),_targets.end(),&target);
    if (it != _targets.end()) 
    {
        // target found in list
        *it = _targets.back(); // copy last element to this slot
        _targets.pop_back(); // pop last element
    }
}
int OutputLog::Print(int channel, const char* source, const char* text)
{    
    int retchannel = 0;
    for (TargetList::iterator it = _targets.begin(); it != _targets.end(); it++)
    {
        int curchannel = (**it).WriteableChannels(source) & channel;
        if (curchannel) (**it).WriteOutputLine(*_styles.back(),time(0),curchannel,source,text);
        retchannel |= curchannel;
    }
    return retchannel;
}
int OutputLog::PrintF(int channel, const char* source, const char* format, ...)
{  
    // capture varargs
    va_list ap;
    va_start(ap, format);

    // Allocate a buffer on the stack that's big enough for us almost all the time.
    int     size = 1024;
    char    stackbuf[1024];
    char*   buf = stackbuf;

    // formatted print
    while (true) {
        // Try to vsnprintf into our buffer.
        int needed = vsnprintf(buf, size, format, ap);
        // NB. C99 (which modern Linux and OS X follow) says vsnprintf
        // failure returns the length it would have needed.  But older
        // glibc and current Windows return -1 for failure, i.e., not
        // telling us how much was needed.

        if (needed <= size && needed >= 0) break;  // It fit fine so we're done.
  
        // vsnprintf reported that it wanted to write more characters
        // than we allotted.  So try again using a dynamic buffer.  This
        // doesn't happen very often if we chose our initial size well.
        size = (needed > 0) ? (needed+1) : (size*2);
        if (buf != stackbuf) delete buf;
        buf = new char[size];
    }

    // pass formatted text to output
    channel = Print(channel,source,buf);

    // cleanup dynamically allocated buffer if needed, and varargs
    if (buf != stackbuf) delete buf;
    va_end (ap);

    return channel;
}
OutputStyle& OutputLog::Style()
{
    return *_styles.back();
}
void OutputLog::PushStyle()
{
    _styles.push_back(new OutputStyle);
}
void OutputLog::PushStyle(const OutputStyle& newStyle)
{
    _styles.push_back(new OutputStyle(newStyle));
}
void OutputLog::PopStyle()
{
    if (_styles.size() > 1) 
    {
        delete _styles.back();
        _styles.pop_back();
    }
}
void OutputLog::Indent()
{
    _styles.back()->indent++;
}
void OutputLog::Outdent()
{
    if (_styles.back()->indent > 0) _styles.back()->indent--;
}
/************************** RuleBasedTarget ************************************/
RuleBasedTarget::RuleBasedTarget() : OutputTarget() {}
RuleBasedTarget::~RuleBasedTarget() 
{ 
    ClearRules(); 
}
int RuleBasedTarget::WriteableChannels(const char* source)
{
    int result = OutputLog::kChannel__ALL; // allow all channels by default
    const char* end = source + strlen(source) + 1;
    for (RuleList::iterator it = _outputRules.begin(); it != _outputRules.end(); it++) 
    {
        std::tr1::regex* reg = (std::tr1::regex*)(**it)._filter;
        if (!reg || std::tr1::regex_search(source,end,*reg))
        {   
            // source matches rule filter
            if ((**it)._state == kRuleState_Block) result &= ~((**it)._channel);  // clear bits for rule channels
            else if ((**it)._state == kRuleState_Print) result |= (**it)._channel; // set bits for rule channels
        }
    }
    return result;
}
void* RuleBasedTarget::AddRule(int state, int channel, const char* filter)
{
    Rule* rule = new Rule(state, channel, filter);
    _outputRules.push_back(rule);
    return rule;
}
void RuleBasedTarget::RemoveRule(void* ruleHandle)
{
    if (_outputRules.size() > 0) 
    {
        RuleList::iterator it = std::find(_outputRules.begin(),_outputRules.end(),ruleHandle);
        if (it == _outputRules.end()) return;  // rule handle not found
        *it = _outputRules.back(); // copy last element to this slot
        _outputRules.pop_back(); // pop last element
        delete (Rule*)ruleHandle;  // delete rule object
    }
}
void RuleBasedTarget::ClearRules()
{
    // delete dynamically allocated rule objects in rule list
    for (RuleList::iterator it = _outputRules.begin(); it != _outputRules.end(); it++) 
    {
        if (*it) delete *it;
    }
    _outputRules.clear();
}
RuleBasedTarget::Rule::Rule(int state, int channel, const char* filter) 
    : _state(state), _channel(channel), _filter(0)
{    
    // allocate a new regex object
    if (filter && *filter)
    {
        try 
        {
            using namespace std::tr1::regex_constants;
            syntax_option_type flags = ECMAScript | icase | nosubs | optimize;
            _filter = new std::tr1::regex(filter,flags);
        }
        catch (...)
        {
            //WriteOutputLine(OutputStyle(),time(0),OutputLog::kChannel_Error,__FUNCTION__,"Bad source filter string");
            _filter = 0;
            throw;  // pass exception on to caller            
        }
    }
}
RuleBasedTarget::Rule::~Rule()
{
    // free regex object if necessary
    if (_filter) delete (std::tr1::regex*)_filter;
}
/************************** BufferTarget ************************************/
BufferTarget::BufferTarget() : RuleBasedTarget(), _size(0x200), _buffer(0)
{
    _buffer = new char[_size];  // allocate the starting buffer
}
BufferTarget::~BufferTarget()
{
    if (_buffer) delete [] _buffer; // dealloc the buffer
}
void BufferTarget::WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text)
{
    // fetch strings for time, channel, indent
    const char* timestr = style.includeTime ? OutputStyle::GetTimeString(time) : "";
    const char* channelstr = style.includeChannel ? OutputStyle::GetChannelLabel(channel) : "";
    char* indentstr = new char[style.indent + 1];
    for (int i = 0; i < style.indent; i++) indentstr[i] = '\t';
    indentstr[style.indent] = 0;

    // formatted print
    while (true) {
        // Try to vsnprintf into our buffer.
        int needed = sprintf_s(_buffer, _size, "%s%s%s%s%s%s%s%s%s%s%s",
            style.includeTime ? "[" : "", timestr, style.includeTime ? "] " : "", 
            style.includeChannel ? "[" : "", channelstr, style.includeChannel ? "] " : "",
            style.includeSource ? "[" : "", style.includeSource ? source : "", style.includeSource ? "] " : "",
            indentstr,text);
        // NB. C99 (which modern Linux and OS X follow) says vsnprintf
        // failure returns the length it would have needed.  But older
        // glibc and current Windows return -1 for failure, i.e., not
        // telling us how much was needed.

        if (needed <= _size && needed >= 0) break;  // It fit fine so we're done.
  
        // vsnprintf reported that it wanted to write more characters
        // than we allotted.  So try again using a dynamic buffer.  This
        // doesn't happen very often if we chose our initial size well.
        _size = (needed > 0) ? (needed+1) : (_size*2);
        if (_buffer) delete _buffer;
        _buffer = new char[_size];
    }

    delete [] indentstr;
}
const char* BufferTarget::LastOutputLine()
{
    return _buffer;
}
/************************** FunctionTarget ************************************/
FunctionTarget::FunctionTarget(WriteFunction out) : BufferTarget(), _out(out) 
{
    if (!_out) throw "FunctionTarget Error: Invalid WriteFunction pointer";
}
void FunctionTarget::WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text)
{
     BufferTarget::WriteOutputLine(style,time,channel,source,text);
    _out(LastOutputLine()); // pass output buffer to specified print function
}
/************************** OStreamTarget ************************************/
OStreamTarget::OStreamTarget(std::ostream& out) : RuleBasedTarget(), _out(out) {}
void OStreamTarget::WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text)
{
    if (style.includeTime) _out << "[" << OutputStyle::GetTimeString(time) << "] ";
    if (style.includeChannel) _out << "[" << OutputStyle::GetChannelLabel(channel) << "] ";
    if (style.includeSource) _out << "[" << source << "] ";
    for (int indent = style.indent; indent > 0; indent--) _out << "\t";
    _out << text << std::endl;
}
/************************** FileTarget ************************************/
FileTarget::FileTarget(const char* filename) : OStreamTarget(_fout)
{
    _fout.open(filename,std::ios_base::out);
    if (!_fout.is_open()) throw "FileTarget Error: Could not open target file four writing";
}
FileTarget::~FileTarget()
{
    _fout.close();
}
/************************** HTMLTarget ************************************/
HTMLTarget::HTMLTarget(const char* filename, const char* title) : FileTarget(filename), _lineParity(false)
{
    // print html opener
    _out << "<html>" << std::endl;
    _out << "<head>" << std::endl;
    // print embedded style info
    _out << "<style type='text/css'>" << std::endl;
    _out << "table {border-collapse:collapse;}" << std::endl; 
    _out << "table,th,td {border: 1px solid #BBBBBB;}" << std::endl;
    _out << "td,th {padding:4px;}" << std::endl;
    _out << "tr.title {background-color:#AAAAAA; color:#000000}" << std::endl;
    _out << "tr.channel0o {background-color:#FF8080; color:#000000}" << std::endl;
    _out << "tr.channel0e {background-color:#FFA0A0; color:#000000}" << std::endl;
    _out << "tr.channel1o {background-color:#FF8080; color:#000000}" << std::endl;
    _out << "tr.channel1e {background-color:#FFA0A0; color:#000000}" << std::endl;
    _out << "tr.channel2o {background-color:#D0D0D0; color:#C96211}" << std::endl;
    _out << "tr.channel2e {background-color:#FFFFFF; color:#C96211}" << std::endl;
    _out << "tr.channel3o {background-color:#D0D0D0; color:#0000A0}" << std::endl;
    _out << "tr.channel3e {background-color:#FFFFFF; color:#0000A0}" << std::endl;
    _out << "tr.channel4o {background-color:#D0D0D0; color:#005000}" << std::endl;
    _out << "tr.channel4e {background-color:#FFFFFF; color:#005000}" << std::endl;
    _out << "tr.channel5o {background-color:#D0D0D0; color:#000000}" << std::endl;
    _out << "tr.channel5e {background-color:#FFFFFF; color:#000000}" << std::endl;
    _out << "</style>" << std::endl;
    // print title if provided
    if (title) _out << "<title>" << title << "</title>";
    _out << "</head>" << std::endl;
    // print table opener
    _out << "<body>" << std::endl;
    _out << "<table>" << std::endl;
    _out << "<tr class='title'><th>Time</th><th>Channel</th><th>Source</th><th>Output</th></tr>" << std::endl;
}
HTMLTarget::~HTMLTarget()
{
    // print html table closer
    _out << "</body>" << std::endl << "</table>" << std::endl << "</html>";
}
void HTMLTarget::WriteOutputLine(const OutputStyle& style, time_t time, int channel, const char* source, const char* text)
{
    // determine channel index from channel mask
    int channelIdx;
    for (channelIdx = 0; channelIdx < OutputLog::kChannelCount; channelIdx++)
    {
        if ((1 << channelIdx) & channel) break;
    }
    // html table row opener
    _out << "<tr class='channel" << channelIdx << (_lineParity?"o":"e") << "'>";
    // data
    _out << "<td>" << (style.includeTime ? OutputStyle::GetTimeString(time) : "") << "</td>";
    _out << "<td>" << (style.includeChannel ? OutputStyle::GetChannelLabel(channel) : "") << "</td>";
    _out << "<td>" << (style.includeSource ? source : "") << "</td>";
    _out << "<td style='{text-indent:" << (style.indent * 25) << "px;}'>" << text << "</td>";
    // row closer
    _out << "</tr>" << std::endl;
    // increment line parity
    _lineParity = !_lineParity;
}