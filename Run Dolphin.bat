@echo off

call "check-var.bat" TM_DOLPHIN_EXE

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Need to set TM_DOLPHIN_EXE to the path of your Dolphin executable.
  exit
)

call "check-var.bat" TM_ISO_PATH

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Need to set TM_ISO_PATH to the path of your new ISO.
  exit
)

echo Starting Dolphin
%TM_DOLPHIN_EXE% --exec=%TM_ISO_PATH%

pause.