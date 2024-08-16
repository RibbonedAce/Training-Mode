@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
call "..\..\safe-mkdir.bat" "output"
echo.

call "..\..\build-core.bat"^
 assets\ldshData.dat^
 source\ledgedash.c^
 output\EvLdsh.dat^
 ..\..\..\MexTK^
 "..\..\..\Additional ISO Files\TM\EvLdsh.dat"