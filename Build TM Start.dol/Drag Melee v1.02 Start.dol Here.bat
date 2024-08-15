@echo off	

echo #####################################
echo ##                                 ##
echo ##                                 ##
echo ## Training Mode Start.dol Creator ##
echo ##                                 ##
echo ##                                 ##
echo #####################################
echo.

echo Creating Training Mode Start.dol...
echo.

cd /d %~dp0

if exist "..\Additional ISO Files" (
    echo Found "Additional ISO Files" directory.
) else (
    mkdir "..\Additional ISO Files"
    echo Created "Additional ISO Files" directory.
)

if exist "..\Additional ISO Files\&&systemdata" (
    echo Found "systemdata" directory.
) else (
    mkdir "..\Additional ISO Files\&&systemdata"
    echo FouCreatednd "systemdata" directory.
)

xdelta.exe -d -f -s "..\gcr\root\&&systemdata\Start.dol" "patch.xdelta" "..\Additional ISO Files\&&SystemData\Start.dol"

echo.
echo Done!
echo The file is located in the "Additional ISO Files\&&systemdata" directory!

echo.
pause