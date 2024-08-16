call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"
call "..\..\safe-mkdir.bat" "output"

call "..\..\build-core.bat"^
 assets\wdshData.dat^
 source\wavedash.c^
 output\EvWdsh.dat^
 ..\..\..\MexTK^
 "..\..\..\Additional ISO Files\TM\EvWdsh.dat"