@echo off
@rem Function that checks if a directory exists, and makes a new one if not

set "dir_to_check=%~1"

if exist "%dir_to_check%" (
  echo Found "%dir_to_check%".
) else (
  mkdir "%dir_to_check%"
  echo Created "%dir_to_check%".
)