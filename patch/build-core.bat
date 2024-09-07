@echo off
@rem Function that performs the basic build mechanics
@rem %1 = Event directory. Example: events\lab
@rem %2 = Assets file location within event directory. Example: assets\labData.dat
@rem %3 = Source code file location within event directory. Example: source\lab.c
@rem %4 = Output file location within Additional ISO Files directory. Example: TM\TmDt.dat
@rem %5 = Symbol injection type for MexTK to use. Example: evFunction

set "MEX_DIR=..\MexTK"
set "ISO_DIR=..\Additional ISO Files"

echo Copying base assets file %1\%2 to %ISO_DIR%\%4.
cp %1\%2 "%ISO_DIR%\%4"

echo Injecting symbols from %1\%3 into "%ISO_DIR%\%4" using %5.
%MEX_DIR%\MexTK.exe -ff -i utils.c %1\%3 -s %5 -dat "%ISO_DIR%\%4" -t %MEX_DIR%\%5.txt -q -ow -w -c -l %MEX_DIR%\melee.link -op 1

echo Trimming "%ISO_DIR%\%4".
%MEX_DIR%\MexTK.exe -trim "%ISO_DIR%\%4"

cd %1
echo.