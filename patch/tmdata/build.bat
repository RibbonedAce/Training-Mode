call "..\safe-mkdir.bat" "..\..\Additional ISO Files"
call "..\safe-mkdir.bat" "..\..\Additional ISO Files\TM"
call "..\safe-mkdir.bat" "output"

call "..\build-core.bat"^
 assets\evMenu.dat^
 source\events.c^
 output\TmDt.dat^
 ..\..\MexTK^
 "..\..\Additional ISO Files\TM\TmDt.dat"