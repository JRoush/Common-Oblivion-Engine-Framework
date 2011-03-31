Requirements for Players:
=========================
1.  Oblivion Script Extender, v19 or later


Requirements for Developers:
============================
1.  Oblivion Script Extender, v19 or later
2.  Visual Studio C++, 2008 edition or later
3.  For those using VS 2008, the TR1 compliance package is strongly recommended


Basic Installation (for everyone):
==================================
1.  Extract COEF folder in the archive to Oblivion\Data\OBSE\Plugins\COEF


Advanced Installation (in an alternate directory):
==================================================
1.  Extract COEF folder in the archive to a working directory of your choice
2.  Create a Oblivion\Data\OBSE\Plugins\COEF folder, and copy these files to it:
        \COEF
            \API
                \ExportInjector.dll
                \ExportInjector.ini
            \Components
                \Components.CS.dll
                \Components.Game.dll            
3.  Open the newly copied Oblivion\Data\OBSE\Plugins\COEF\API\ExportInjector.ini and edit
    the "FileName" field to point to the API\ subfolder of your working directory. 
    For example:
        Filename = "*.eed"      
        becomes
        Filename = "C:\My Oblivion Mods\COEF\API\*.eed"      
    