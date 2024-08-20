@echo off

call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
echo.

cd ..\..
call build-core.bat^
 events\lab^
 assets\labData.dat^
 source\lab.c^
 TM\EvLab.dat^
 evFunction

echo Next.

cd ..\..
call build-core.bat^
 events\lab^
 assets\importData.dat^
 source\lab_css.c^
 TM\EvLabCSS.dat^
 cssFunction