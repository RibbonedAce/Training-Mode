@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
echo.

cd ..\..
call build-core.bat^
 events\lcancel^
 assets\lclData.dat^
 source\lcancel.c^
 TM\EvLCl.dat^
 evFunction