@echo off
@rem Function that checks if an environment variable is set

call if defined %1 (
    call echo %1 set to %%%1%%%.
    exit /b 0
) else (
    exit /b 1
)