cd tmdata
call "build.bat"
cd ..

for /D %%e in (events\*) do (
  cd %%e
  call "build.bat"
  cd ..\..
)

echo Copied all events into Additional ISO Files!
echo.