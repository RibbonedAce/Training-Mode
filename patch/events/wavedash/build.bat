SET "MEX_DIR=..\..\..\MexTK"
SET "ASSETS=assets\wdshData.dat"
SET "SOURCEFILE=source\wavedash.c"
SET "OUTPUT=output\EvWdsh.dat"

echo Copying base assets file "%ASSETS%" to "%OUTPUT%".
xcopy /s /y %ASSETS% %OUTPUT%

echo Injecting symbols from "%SOURCEFILE%" into "%OUTPUT%".
"%MEX_DIR%\MexTK.exe" -ff -i %SOURCEFILE% -s evFunction -dat %OUTPUT% -t "%MEX_DIR%\evFunction.txt" -q -ow -w -c -l "%MEX_DIR%\melee.link" -op 1

echo Trimming "%OUTPUT%".
"%MEX_DIR%\MexTK.exe" -trim %OUTPUT%

echo.
pause