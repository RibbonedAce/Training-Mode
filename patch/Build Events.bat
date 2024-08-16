@echo off

@rem TODO: fix build of tmdata
@rem cd tmdata
@rem call "build.bat"
@rem cd ..

for /D %%e in (events\*) do (
  cd %%e
  call "build.bat"
  cd ..\..
)

echo Copied all events into Additional ISO Files!
echo.