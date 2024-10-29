@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
echo.

cd ..\..
call build-core.bat^
 events\pivotfsmash^
 assets\pfshData.dat^
 source\pivotfsmash.c^
 TM\EvPfsh.dat^
 evFunction