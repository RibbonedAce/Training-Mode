@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
call "..\..\safe-mkdir.bat" "output"
echo.

call "..\..\build-core.bat"^
 assets\lclData.dat^
 source\lcancel.c^
 output\EvLCl.dat^
 ..\..\..\MexTK^
 "..\..\..\Additional ISO Files\TM\EvLCl.dat"