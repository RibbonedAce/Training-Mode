@echo off
@rem Function that checks if an environment variable is set

set "var_name=%1"

call if defined %var_name% (
    call echo %var_name% set to %%var_name%%.
    exit /b 0
) else (
    exit /b 1
)