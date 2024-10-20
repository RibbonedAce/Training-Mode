@echo off
@rem Function that performs the basic build mechanics

set "event_dir=%1"
set "assets_path=%2"
set "source_path=%3"
set "output_path=%4"
set "mex_symbol=%5"

set "MEX_DIR=..\MexTK"
set "ISO_DIR=..\Additional ISO Files"

echo Copying base assets file %event_dir%\%assets_path% to %ISO_DIR%\%output_path%.
cp %event_dir%\%assets_path% "%ISO_DIR%\%output_path%"

echo Injecting symbols from %event_dir%\%source_path% into "%ISO_DIR%\%output_path%" using %mex_symbol%.
%MEX_DIR%\MexTK.exe -ff -i utils.c cpu.c %event_dir%\%source_path% -s %mex_symbol% -dat "%ISO_DIR%\%output_path%" -t %MEX_DIR%\%mex_symbol%.txt -q -ow -w -c -l %MEX_DIR%\melee.link -op 1

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while injecting symbols for %source_path%. See above.
  exit
)

echo Trimming "%ISO_DIR%\%output_path%".
%MEX_DIR%\MexTK.exe -trim "%ISO_DIR%\%output_path%"

cd %event_dir%
echo.