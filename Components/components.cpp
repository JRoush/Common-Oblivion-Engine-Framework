#ifdef OBLIVION
#define MODE "Game"
#else
#define MODE "CS"
#endif

// global debugging log
HTMLTarget  _gLogFile("Data\\obse\\plugins\\COEF\\Components\\Components." MODE ".log.html", "COEF Components " MODE " Log");
OutputLog   _gLog;
OutputLog&  gLog = _gLog;

// global debugging log
BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
    switch(dwReason)
    {
    case DLL_PROCESS_ATTACH:    // dll loaded
        // attach output logfile & configure rules
        gLog.AttachTarget(_gLogFile);
         // load rules for loader output from INI
        if (!_gLogFile.LoadRulesFromINI("Data\\obse\\Plugins\\COEF\\Components\\Settings.ini", MODE ".Log"))
        {
            // rules failed to load from INI; use defaults (very restrictive)
            _gLogFile.AddRule(RuleBasedTarget::kRuleState_Block, ~(OutputLog::kChannel_Error|OutputLog::kChannel_FatalError|OutputLog::kChannel_Warning),0);
        }
        _MESSAGE("Component Library loaded");
        break;
    case DLL_PROCESS_DETACH:    // dll unloaded   
        _MESSAGE("Component Library unloaded");
        gLog.DetachTarget(_gLogFile);
        break;
    }   
    return true;
}