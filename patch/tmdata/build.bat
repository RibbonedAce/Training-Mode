@echo off

call "..\safe-mkdir.bat" "..\..\Additional ISO Files"
call "..\safe-mkdir.bat" "..\..\Additional ISO Files\TM"
echo.

cd ..
call build-core.bat^
 tmdata^
 assets\evMenu.dat^
 source\events.c^
 TM\TmDt.dat^
 tmFunction