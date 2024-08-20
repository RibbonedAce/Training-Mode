@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
echo.

cd ..\..
call build-core.bat^
 events\ledgedash^
 assets\ldshData.dat^
 source\ledgedash.c^
 TM\EvLdsh.dat^
 evFunction