call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"

call "..\..\build-core.bat"^
 assets\lclData.dat^
 source\lcancel.c^
 output\EvLCl.dat^
 ..\..\..\MexTK^
 "..\..\..\Additional ISO Files\TM\EvLCl.dat"