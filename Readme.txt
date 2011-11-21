DESCRIPTION:
============
The Common Oblivion Engine Framework is an API and utility library for OBSE plugins.  It makes plugins simpler and faster
to write and more likely to be compatible with other plugins.

COEF is not a mod itself, just a resource.  It is not an OBSE plugin itself, and OBSE will not load it or even "see" it.
Only plugins that use COEF will interact with it.  You can uninstall COEF at any time, if you no longer have any plugins
that use it.


TECHNICAL DESCRIPTION (FOR MODDERS):
====================================
The main feature of COEF is it's C++ API for Oblivion and Construction Set.  For those familiar with C++ programming, this is 
basically a set of header (*.h) files that define the classes used by the engine.  Most plugins simply use the class definitions 
included with OBSE, so for convenience here are the main ways that COEF differs from the OBSE code:

-   Definition only.  No function bodys (implementations), and no classes or methods that do not exist in the Game/CS.
-   Dynamic Linkage.  Projects that use this API link *directly* to the Game/CS exectuable as if it were a dll.
    When a plugin calls an API method, it is directly calling the corresponding code in the Game/CS.
-   Modularity.  Classes are split into their own headers as much as possible, and cross dependencies are minimized.
-   Readability.  Class definitions are formated as cleanly as possible, so they can be readily browsed.
-   Completeness.  Classes that are fully defined. This means that the name, size, inheritance, and virtual method tables
    of each class is reproduced exactly, along with as many member functions as possible.  Among other things, this means
    that plugin authors can safely *derive* from the API classes, use them safely with the dynamic_cast and typeid operators.
    
This is not meant to imply the OBSE code is *bad* or poorly written, but it was written mostly for internal use and doesn't make
a good general-purpose API.

It's IMPORTANT to note that because COEF and OBSE define many of the same classes, you generally cannot include headers from both
in the same file.  This is the main drawback of COEF, especially since there are still classes that haven't been added to the API
yet (working on it!).

Aside from the API, COEF also offers a small utility library for plugin authors.  This includes the core files from the OBSE "common"
project (redistributed with permission), as well as additional classes for advanced output logging and writing patches/hooks.

Finally, COEF includes a growing set of "Components".  Each Component is basically a mini-mod that patches the Game/CS to
allow some kind of extended behavior.  Examples include registering callbacks for in-game events, allowing for new form types, or 
custom records in esp files.  These features are either (1) too difficult/complex for plugin authors to implement on their own, 
or (2) would cause incompatibilities if implemented by multiple plugins.  Each component is dormant and does absolutely nothing
until it is activated by a plugin that wants to use it.


CREDITS:
========
Most of the work for COEF was done years before it's conception by the main OBSE team: Ianpatt, Behippo, and Scruggsywuggsy the Ferret.
These three have been indirectly responsible for so many great mods that they should, by rights, be some kind of modding royalty.
Additional contributions from shadeMe and kyoma - these two are excellent decoders, and have been sharing their discoveries with me.
Special thanks to all the folks who took an interest in this project: shadeMe, DragoonWraith, kyoma, haama, and Lojack.
This project would be impossible without their support.
    