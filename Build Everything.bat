@echo off

echo Building codeset.
cd "Build TM Codeset"
call "Build Training Mode Codeset.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while building codeset. See above.
  exit
)

cd ..

echo Building Start.dol.
cd "Build TM Start.dol"
call "Build Start.Dol.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while building Start.dol. See above.
  exit
)

cd ..

echo Building events.
cd patch
call "Build Events.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while building events. See above.
  exit
)

cd ..

echo Done! Everything is copied to Additional ISO Files!