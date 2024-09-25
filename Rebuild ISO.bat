@echo off

call "check-var.bat" TM_GCR_ROOT

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Need to set TM_GCR_ROOT to the directory path of your TM root.
  exit
)

call "check-var.bat" TM_GCR_EXE

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Need to set TM_GCR_EXE to the path of the GCRebuilder executable.
  exit
)

call "check-var.bat" TM_ISO_PATH

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Need to set TM_ISO_PATH to the path of your new ISO.
  exit
)

if not exist "Additional ISO Files" (
  echo Need to build your new files. Run Build Everything.bat first.
  exit
)

echo Copying Additional ISO Files into %TM_GCR_ROOT%.
xcopy /s /y "Additional ISO Files" %TM_GCR_ROOT%

echo Using GCRebuilder to rebuild ISO.
%TM_GCR_EXE% %TM_GCR_ROOT% %TM_ISO_PATH%

echo Done!
echo.