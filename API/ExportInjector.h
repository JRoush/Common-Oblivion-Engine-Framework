/*
    Export Injector - Module Interface

    Parameters (Multiple values allowed, must be delimited by commas without spaces):
    -   Filename: filename or filter matching file to be loaded as Extended Export Definition (.eed)
    -   Recurse: whether subdirectories should be recursively searched for target filename
    -   DEFModule: module for which to output a Module Definition file (.def)
    -   DEFFilename: target filename for module definition file

    Parameters (order of precedence):
    1.  Command line arguments
    2.  Local ini file (current working directory)
    3.  Base ini file (ExportInjector.exe directory)

    On Entry (as DLL or EXE)
    1.  Load specified eed files
    2.  Output any specified def files
    3.  Present C-linkage interface below for loading/outputting additional files
*/
#pragma once

extern "C"
{
    // Autoload (parse command line and ini files)
    void __declspec(dllexport) Autoload(int argc = 0, char* argv[] = 0);

    // Loads as Extended Export Definition (.eed) the file or files that match the filename
    // filename may contain valid windows wildcards *, ?, etc.
    // if recurse is true, all sibling subdirectories will also be searched recursively
    // Returns number of files matched
    int __declspec(dllexport) LoadExports (const char* filename, bool recurse); 

    // Output a Module Definition File (.def) for the specified module, suitable for use with
    // LIB.exe or other standard windows compiler tools.  Returns true if output is successful
    bool __declspec(dllexport) WriteModuleDef(const char* filename, const char* module);

    // Dump (to console) a summary of the export table for the specified module
    // For debugging purposes only
    void __declspec(dllexport) ModuleDebugDump(const char* module);
}