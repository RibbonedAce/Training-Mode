@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
echo.

cd ..\..
call build-core.bat^
 events\wavedash^
 assets\wdshData.dat^
 source\wavedash.c^
 TM\EvWdsh.dat^
 evFunction
