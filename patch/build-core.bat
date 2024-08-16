@rem Function that performs the basic build mechanics

echo Copying base assets file %1 to %3.
xcopy /s /y %1 %3

echo Injecting symbols from %2 into %3.
"%4\MexTK.exe" -ff -i %2 -s evFunction -dat %3 -t "%4\evFunction.txt" -q -ow -w -c -l "%4\melee.link" -op 1

echo Trimming %3.
"%4\MexTK.exe" -trim %3

echo Copying output %3 to %5.
cp %3 %5

echo.