@echo off
REM Snake Game Build Script for Windows with GCC
REM This script compiles the Snake game

setlocal enabledelayedexpansion

echo.
echo ========================================
echo Snake Game - Windows Build
echo ========================================
echo.

REM Check for gcc
gcc --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: GCC compiler not found in PATH
    echo.
    echo Please install a C compiler:
    echo   - Download LLVM MinGW from: https://github.com/mstorsjo/llvm-mingw
    echo   - Add to PATH and try again
    echo.
    pause
    exit /b 1
)

echo [1/3] Compiler found
gcc --version | findstr /R . >nul 2>&1
echo.

REM Create build directory
if not exist build mkdir build
echo [2/3] Build directory created

REM Attempt compilation
echo [3/3] Compiling Snake game...
echo.

REM Try compiling WITHOUT Raylib first (will fail but shows what's needed)
cd /d "%~dp0"

REM Check if Raylib is available
if exist "C:\tools\raylib\include\raylib.h" (
    echo Found Raylib at C:\tools\raylib
    set RAYLIB_PATH=C:\tools\raylib
) else if exist "..\raylib\raylib-5.0_win64_gcc\include\raylib.h" (
    echo Found Raylib locally
    set RAYLIB_PATH=..\raylib\raylib-5.0_win64_gcc
) else (
    echo WARNING: Raylib not found. Compilation will fail.
    echo.
    echo To fix this:
    echo 1. Download Raylib from: https://www.raylib.com/
    echo 2. Extract to: %~dp0..\raylib\
    echo 3. Run this script again
    echo.
    pause
    exit /b 1
)

REM Compile
gcc -Wall -Wextra -std=c99 -o build\snake.exe ^
    -I!RAYLIB_PATH!\include ^
    src\main.c src\game.c ^
    -L!RAYLIB_PATH!\lib ^
    -lraylib -lm -lopengl32 -lgdi32 -lwinmm ^
    2>build\compile_errors.txt

if errorlevel 1 (
    echo.
    echo COMPILATION FAILED
    echo.
    echo Error details:
    type build\compile_errors.txt
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo SUCCESS! Game compiled
echo ========================================
echo.
echo To run:
echo   .\build\snake.exe
echo.
pause
