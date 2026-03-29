@echo off
REM ========================================
REM Snake Game - Raylib Build (Windows)
REM ========================================

setlocal enabledelayedexpansion

REM Set GCC path
if exist "C:\tools\llvm-mingw\bin\gcc.exe" (
    set "GCC_PATH=C:\tools\llvm-mingw\bin"
) else if exist "C:\Program Files\MinGW\bin\gcc.exe" (
    set "GCC_PATH=C:\Program Files\MinGW\bin"
) else (
    echo ERROR: GCC compiler not found
    echo Quick fix: Ensure GCC is in your PATH
    echo Already installed? Run: set PATH=C:\tools\llvm-mingw\bin;%%PATH%%
    pause
    exit /b 1
)

REM Add GCC to PATH
set "PATH=%GCC_PATH%;%PATH%"

REM Check if build directory exists
if not exist "build" mkdir build

REM Raylib paths (from parent directory)
set "RAYLIB_PATH=..\raylib\raylib-5.0_win64_msvc16"

REM If that doesn't exist, try alternative path
if not exist "%RAYLIB_PATH%" (
    echo WARNING: Raylib not found at %RAYLIB_PATH%
    echo Trying alternative path...
    set "RAYLIB_PATH=..\..\raylib\raylib-5.0_win64_msvc16"
)

REM Check if Raylib exists
if not exist "%RAYLIB_PATH%" (
    echo ERROR: Raylib not found
    echo Searched: ..\raylib\raylib-5.0_win64_msvc16
    echo Please ensure Raylib is installed in the PROJECT JEUX directory
    pause
    exit /b 1
)

echo Found Raylib at: %RAYLIB_PATH%

echo ========================================
echo Building Snake Game - Raylib Edition
echo ========================================
echo.

REM Compile game.c
echo Compiling game.c...
gcc -std=c99 -O2 -Wall -Wextra ^
    -I"%RAYLIB_PATH%\include" ^
    -c src/game.c -o build/game.o
if %errorlevel% neq 0 (
    echo ERROR: Failed to compile game.c
    pause
    exit /b 1
)

REM Compile main.c
echo Compiling main.c...
gcc -std=c99 -O2 -Wall -Wextra ^
    -I"%RAYLIB_PATH%\include" ^
    -c src/main.c -o build/main.o
if %errorlevel% neq 0 (
    echo ERROR: Failed to compile main.c
    pause
    exit /b 1
)

REM Link
echo Linking...
gcc -o build/snake_raylib.exe build/game.o build/main.o ^
    -L"%RAYLIB_PATH%\lib" ^
    -lraylib -lopengl32 -lwinmm -lgdi32 -std=c99
if %errorlevel% neq 0 (
    echo ERROR: Failed to link
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo Executable: build\snake_raylib.exe
echo ========================================
echo.
pause
