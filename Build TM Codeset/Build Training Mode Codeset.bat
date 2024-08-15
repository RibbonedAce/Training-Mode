if exist "..\Additional ISO Files" (
    echo Found "Additional ISO Files" directory.
) else (
    mkdir "..\Additional ISO Files"
    echo Created "Additional ISO Files" directory.
)

gecko build

pause