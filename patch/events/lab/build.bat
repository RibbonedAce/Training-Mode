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

echo Copying base assets file assets\importData.dat to output\EvLabCSS.dat.
cp assets\importData.dat output\EvLabCSS.dat

echo Injecting symbols from source\lab_css.c into output\EvLabCSS.dat.
"%MEX_DIR%\MexTK.exe" -ff -i source\lab_css.c -s cssFunction -dat output\EvLabCSS.dat -t "%MEX_DIR%\cssFunction.txt" -q -ow -w -c -l "%MEX_DIR%\melee.link" -op 1

echo Trimming output\EvLabCSS.dat.
"%MEX_DIR%\MexTK.exe" -trim output\EvLabCSS.dat

echo Copying output output\EvLabCSS.dat to "..\..\..\Additional ISO Files\TM\EvLabCSS.dat".
cp output\EvLabCSS.dat "..\..\..\Additional ISO Files\TM\EvLabCSS.dat"

echo.