
Implementation of SWIG bindings to run Behave from C#. 
Only Surface and Crown headers (as well as enums for input) has been configured, editr behave_csharp.i if more is needed.

Generate bindings with command such as:
C:\behave\swig>swig.exe -c++ -csharp -namespace BehaveCore behave_csharp.i

This assumes you have added the swig.exe directory to PATH on windows.
The command generates C# bindings as well as the glue code, in this case behave_csharp_wrap.cxx.
Compile with LIBRARY option on to generate the BehaveCore.dll which is then used in your C# project together with the generated C# code.