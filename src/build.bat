@echo off
setlocal

:: Get the info from arguments
set "option=%~1"
set "fileName=%~2"

:: Strip the last extension
for %%f in ("%fileName%") do set "fn=%%~nf"

:: Transpiler (-t): feed the generated .c file and build executable
:: Compiler (-c): feed the generated .s file and build executable
if "%option%"=="-t" (
    gcc "%fn%.c" -o "%fn%"
) else if "%option%"=="-c" (
    gcc "%fn%.s" -o "%fn%"
) else (
    echo error: Unknown option "%option%".
    exit /b 1
)

if errorlevel 1 (
    echo error: gcc compilation failed.
    exit /b 1
)

endlocal
