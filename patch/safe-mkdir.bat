@echo off
@rem Function that checks if a directory exists, and makes a new one if not
@rem %1 = Directory to check/create

if exist %1 (
  echo Found %1.
) else (
  mkdir %1
  echo Created %1.
)