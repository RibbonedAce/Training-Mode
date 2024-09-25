@echo off

cd tmdata
call "build.bat"

set "code=%ERRORLEVEL%"
if %code% neq 0 (
  echo Error while building generic event data. See above.
  exit
)

cd ..

for /D %%e in (events\*) do (
  cd %%e
  call "build.bat"

  set "code=%ERRORLEVEL%"
  if %code% neq 0 (
    echo Error while building event %%e. See above.
    exit
  )

  cd ..\..
)

echo Copied all events into Additional ISO Files!
echo.