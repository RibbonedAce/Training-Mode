call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files"
call "..\..\safe-mkdir.bat" "..\..\..\Additional ISO Files\TM"

call "..\..\build-core.bat"^
 assets\ldshData.dat^
 source\ledgedash.c^
 output\EvLdsh.dat^
 ..\..\..\MexTK^
 "..\..\..\Additional ISO Files\TM\EvLdsh.dat"