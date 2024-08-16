@echo off

SET "MEX_DIR=..\..\..\MexTK"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
call "..\..\safe-mkdir.bat" "output"
echo.

call "..\..\build-core.bat"^
 assets\labData.dat^
 source\lab.c^
 output\EvLab.dat^
 %MEX_DIR%^
 "..\..\..\Additional ISO Files\TM\EvLab.dat"

echo Next.

call "..\..\build-core.bat"^
 assets\importData.dat^
 source\lab_css.c^
 output\EvLabCSS.dat^
 %MEX_DIR%^
 "..\..\..\Additional ISO Files\TM\EvLabCSS.dat"