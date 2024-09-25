@echo off

echo Building
call "Build Everything.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while building code and assets. See above.
  exit
)

echo.

echo Rebuilding
call "Rebuild ISO.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while rebuilding ISO. See above.
  exit
)

echo.

echo Running
call "Run Dolphin.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while starting Dolphin. See above.
  exit
)

echo.