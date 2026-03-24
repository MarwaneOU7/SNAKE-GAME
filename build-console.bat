@echo off
REM Snake Game - Console Version Build
REM This version compiles without external dependencies

setlocal enabledelayedexpansion

echo.
echo ========================================
echo Snake Game - Console Build (No Dependencies)
echo ========================================
echo.

REM Check for gcc
gcc --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: GCC compiler not found!
    echo.
    echo Quick fix: Ensure GCC is in your PATH
    echo Already installed? Run: set PATH=C:\tools\llvm-mingw\bin;%%PATH%%
    echo.
    pause
    exit /b 1
)

echo [1/2] Compiler verified ✓
echo.

REM Create build directory
if not exist build (
    mkdir build
    echo [2/2] build\ directory created
) else (
    echo [2/2] build\ directory exists
)

REM Compile console version
echo.
echo [3/2] Compiling...
cd /d "%~dp0"

gcc -Wall -Wextra -std=c99 -O2 ^
    -o build\snake_console.exe ^
    src\game_console.c src\main_console.c ^
    2>build\compile_log.txt

if errorlevel 1 (
    echo.
    echo ╔════════════════════════════════╗
    echo ║  COMPILATION FAILED            ║
    echo ╚════════════════════════════════╝
    echo.
    echo Error log:
    type build\compile_log.txt
    echo.
    pause
    exit /b 1
)

if exist build\snake_console.exe (
    echo.
    echo ╔════════════════════════════════╗
    echo ║  BUILD SUCCESSFUL! ✓           ║
    echo ╚════════════════════════════════╝
    echo.
    echo Executable: build\snake_console.exe
    echo Size: 
    for %%A in (build\snake_console.exe) do echo %%~zA bytes
    echo.
    echo To play the game:
    echo   .\build\snake_console.exe
    echo.
    echo Or from any CMD:
    echo   "%~dp0build\snake_console.exe"
    echo.
) else (
    echo.
    echo ERROR: Executable not created
    echo.
    type build\compile_log.txt
)

echo.
pause
