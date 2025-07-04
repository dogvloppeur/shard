@echo off
setlocal enabledelayedexpansion

echo [*] Starting build...

REM Collect all .c files from subdirectories
set SOURCES=
for %%d in (cli eval lexer parser utils) do (
    for %%f in (%%d\*.c) do set SOURCES=!SOURCES! %%f
)

REM Compile all .c files into shard.exe at the root
if defined SOURCES (
    gcc !SOURCES! -o shard.exe -Wall -Wextra -O2 -I. -Icli/include -Ieval/include -Ilexer/include -Iparser/include
) else (
    echo [!] No source files found.
    pause
    exit /b 1
)

REM Check if the compilation succeeded
IF %ERRORLEVEL% NEQ 0 (
    echo [!] Build failed with errors.
    pause
    exit /b %ERRORLEVEL%
) ELSE (
    echo [✓] Build successful. Output: shard.exe
)

pause



