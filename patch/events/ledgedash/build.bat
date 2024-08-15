SET "ASSETS=ldshData"
SET "SOURCEFILE=ledgedash"
SET "OUTPUT=EvLdsh"

xcopy /s /y "assets\%ASSETS%.dat" "output\%OUTPUT%.dat"
"..\..\..\MexTK\MexTK.exe" -ff -i "source\%SOURCEFILE%.c" -s evFunction -dat "output\%OUTPUT%.dat" -t "..\..\..\MexTK\evFunction.txt" -q -ow -w -c -l "..\..\..\MexTK\melee.link" -op 1
"..\..\..\MexTK\MexTK.exe" -trim "output\%OUTPUT%.dat"

pause