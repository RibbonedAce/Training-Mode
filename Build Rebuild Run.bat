@echo off

echo Building
call "Build Everything.bat"
echo.

echo Rebuilding
call "Rebuild ISO.bat"
echo.

echo Running
call "Run Dolphin.bat"
echo.