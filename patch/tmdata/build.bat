@echo off

call "..\safe-mkdir.bat" "..\..\Additional ISO Files"
call "..\safe-mkdir.bat" "..\..\Additional ISO Files\TM"
call "..\safe-mkdir.bat" "output"
echo.

set "MEX_DIR=..\..\MexTK"

echo Copying base assets file assets\evMenu.dat to output\TmDt.dat.
cp assets\evMenu.dat output\TmDt.dat

echo Injecting symbols from source\events.c into output\TmDt.dat.
"%MEX_DIR%\MexTK.exe" -ff -i source\events.c -s tmFunction -dat output\TmDt.dat -t "%MEX_DIR%\tmFunction.txt" -q -ow -w -c -l "%MEX_DIR%\melee.link" -op 1

echo Trimming output\TmDt.dat.
"%MEX_DIR%\MexTK.exe" -trim output\TmDt.dat

echo Copying output output\TmDt.dat to "..\..\Additional ISO Files\TM\TmDt.dat".
cp output\TmDt.dat "..\..\Additional ISO Files\TM\TmDt.dat"

echo.