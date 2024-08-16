echo Building codeset.
cd "Build TM Codeset"
call "Build Training Mode Codeset.bat"
cd ..

echo Building Start.dol.
cd "Build TM Start.dol"
call "Build Start.Dol.bat"
cd ..

echo Building events.
cd patch
call "Build Events.bat"
cd ..

echo Done! Everything is copied to Additional ISO Files!