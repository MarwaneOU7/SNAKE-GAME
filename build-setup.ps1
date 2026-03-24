# Complete Snake Game Build Setup for Windows
# This script downloads and configures all dependencies

param(
    [switch]$SkipDownloads
)

Write-Host "================================"
Write-Host "Snake Game Build Setup"
Write-Host "================================" -ForegroundColor Green
Write-Host ""

# Setup directories
$toolsDir = "C:\tools"
$raylib_dir = "$toolsDir\raylib"
$project_dir = Split-Path -Parent $PSScriptRoot

Write-Host "[1/4] Setting up directories..."
if (!(Test-Path $toolsDir)) { mkdir $toolsDir | Out-Null }
if (!(Test-Path $raylib_dir)) { mkdir $raylib_dir | Out-Null }
Write-Host "✓ Directories ready" -ForegroundColor Green

# Step 1: Check/Install GCC
Write-Host ""
Write-Host "[2/4] Checking C Compiler..."
$gcc = "gcc"
$gcc_version = & gcc --version 2>&1 | Select-Object -First 1
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ GCC found: $gcc_version" -ForegroundColor Green
} else {
    Write-Host "✗ GCC not found. Please install GCC first:" -ForegroundColor Red
    Write-Host "   Option A: Download from www.raylib.com (MinGW bundle)"
    Write-Host "   Option B: Use MSYS2 from www.msys2.org"
    Write-Host "   Option C: Run this script again after installing GCC"
    exit 1
}

# Step 2: Setup Raylib
Write-Host ""
Write-Host "[3/4] Setting up Raylib library..."

# Check for pre-built libraries
$raylib_lib = "$raylib_dir\lib\libraylib.a"
$raylib_include = "$raylib_dir\include\raylib.h"

if ((Test-Path $raylib_lib) -and (Test-Path $raylib_include)) {
    Write-Host "✓ Raylib already configured" -ForegroundColor Green
} else {
    Write-Host "ℹ  Raylib not fully configured. Options:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Quick Setup (Recommended):"
    Write-Host "  1. Download: https://www.raylib.com/index.html"
    Write-Host "  2. Extract to: $raylib_dir"
    Write-Host ""
    Write-Host "OR use alternative approach:"
    Write-Host "  powershell -ExecutionPolicy Bypass -File '$PSScriptRoot\setup-raylib-mingw.ps1'"
}

# Step 3: Build the project
Write-Host ""
Write-Host "[4/4] Building Snake game..."

cd $project_dir\Snake

# Try to build
Write-Host "Compiling (this may take a minute)..."
$compile_result = & make clean 2>&1
$compile_result = & make 2>&1

if (Get-ChildItem "$project_dir\Snake\build\snake.exe" -ErrorAction SilentlyContinue) {
    Write-Host "✓ Build successful!" -ForegroundColor Green
    Write-Host ""
    Write-Host "To run the game:"
    Write-Host "  cd '$project_dir\Snake'"
    Write-Host "  .\build\snake.exe"
    Write-Host ""
    Write-Host "================================"
    Write-Host "Setup Complete!" -ForegroundColor Green
    Write-Host "================================"
} else {
    Write-Host "⚠  Build output:" -ForegroundColor Yellow
    $compile_result | Write-Host
    Write-Host ""
    Write-Host "Setup issues?" -ForegroundColor Yellow
    Write-Host "1. Make sure Raylib is properly installed to: $raylib_dir"
    Write-Host "2. Run: make clean && make"
    Write-Host "3. Check: make --version"
}
