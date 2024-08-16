@echo off	

echo #####################################
echo ##                                 ##
echo ##                                 ##
echo ## Training Mode Start.dol Creator ##
echo ##                                 ##
echo ##                                 ##
echo #####################################
echo.

call "..\check-var.bat" TM_GCR_ROOT
set "code=%ERRORLEVEL%"

if %code% gtr 0 (
  echo Need to set TM_GCR_ROOT to the directory path of your TM root.
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