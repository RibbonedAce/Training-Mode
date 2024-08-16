@echo off	

echo #####################################
echo ##                                 ##
echo ##                                 ##
echo ## Training Mode Start.dol Creator ##
echo ##                                 ##
echo ##                                 ##
echo #####################################
echo.

if exist %TM_GCR_ROOT% (
    echo TM_GCR_ROOT = %TM_GCR_ROOT%
) else (
    echo You must set the TM_GCR_ROOT variable to the location of the root of your TM ISO.
    exit
)

echo Creating Training Mode Start.dol...
echo.

cd /d %~dp0

call "..\patch\safe-mkdir.bat" "..\Additional ISO Files"
call "..\patch\safe-mkdir.bat" "..\Additional ISO Files\&&systemdata"

xdelta.exe -d -f -s "%TM_GCR_ROOT%\&&systemdata\Start.dol" "patch.xdelta" "..\Additional ISO Files\&&systemdata\Start.dol"

echo.
echo Done!
echo The file is located in the "Additional ISO Files\&&systemdata" directory!

echo.