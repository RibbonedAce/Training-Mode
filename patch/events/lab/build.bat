SET "ASSETS=labData"
SET "SOURCEFILE=lab"
SET "OUTPUT=EvLab"

xcopy /s /y "assets\%ASSETS%.dat" "output\%OUTPUT%.dat"
"..\..\..\MexTK\MexTK.exe" -ff -i "source\%SOURCEFILE%.c" -s evFunction -dat "output\%OUTPUT%.dat" -t "..\..\..\MexTK\evFunction.txt" -q -ow -w -c -l "..\..\..\MexTK\melee.link" -op 1
"..\..\..\MexTK\MexTK.exe" -trim "output\%OUTPUT%.dat"

SET "ASSETS=importData"
SET "SOURCEFILE=lab_css"
SET "OUTPUT=EvLabCSS"

xcopy /s /y "assets\%ASSETS%.dat" "output\%OUTPUT%.dat"
"..\..\..\MexTK\MexTK.exe" -ff -i "source\%SOURCEFILE%.c" -s evFunction -dat "output\%OUTPUT%.dat" -t "..\..\..\MexTK\evFunction.txt" -q -ow -w -c -l "..\..\..\MexTK\melee.link" -op 1
"..\..\..\MexTK\MexTK.exe" -trim "output\%OUTPUT%.dat"

pause