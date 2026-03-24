@echo off
REM ==========================================
REM  GITHUB PUSH SCRIPT - Easy Setup
REM ==========================================
REM  This script helps you push your code to GitHub
REM  Just fill in your info below and run!
REM ==========================================

setlocal enabledelayedexpansion

echo.
echo ╔════════════════════════════════════════╗
echo ║   SNAKE GAME - GITHUB PUSH HELPER      ║
echo ╚════════════════════════════════════════╝
echo.

REM ===== CONFIGURE THESE =====
REM Replace with YOUR actual GitHub username and repo name
set GITHUB_USERNAME=YOUR_USERNAME
set GITHUB_REPO=Snake-Game
REM ============================

REM Check if user configured values
if "!GITHUB_USERNAME!"=="YOUR_USERNAME" (
    echo ERROR: Please edit this file and set GITHUB_USERNAME
    echo.
    echo Edit line 13 with your actual GitHub username
    echo Example: set GITHUB_USERNAME=john-doe
    echo.
    pause
    exit /b 1
)

set REPO_URL=https://github.com/!GITHUB_USERNAME!/!GITHUB_REPO!.git

echo GitHub Username: !GITHUB_USERNAME!
echo Repository Name: !GITHUB_REPO!
echo Full URL: !REPO_URL!
echo.

REM Navigate to project
cd /d "%~dp0"

echo [Step 1] Checking git status...
git status >nul 2>&1
if errorlevel 1 (
    echo ERROR: This directory is not a git repository!
    pause
    exit /b 1
)
echo ✓ Git repository found
echo.

echo [Step 2] Configuring remote...
REM Remove existing remote if it exists
git remote remove origin 2>nul

REM Add new remote
git remote add origin !REPO_URL! 2>nul
if errorlevel 1 (
    echo ERROR: Could not add remote
    pause
    exit /b 1
)
echo ✓ Remote added: !REPO_URL!
echo.

echo [Step 3] Preparing to push...
REM Rename master to main (standard practice)
git branch -M main 2>nul
echo ✓ Branch renamed to main
echo.

echo [Step 4] Pushing code to GitHub...
echo This may ask for your GitHub credentials/token...
echo.
git push -u origin main

if errorlevel 1 (
    echo.
    echo ❌ PUSH FAILED
    echo.
    echo Common issues:
    echo  1. Repository doesn't exist on GitHub yet (create it first!)
    echo  2. Wrong username or URL
    echo  3. Authentication failed (use Personal Access Token)
    echo.
    echo Fix and try again!
    pause
    exit /b 1
)

echo.
echo ╔════════════════════════════════════════╗
echo ║   ✓ SUCCESS - CODE PUSHED!             ║
echo ╚════════════════════════════════════════╝
echo.
echo Your repository is now on GitHub at:
echo   https://github.com/!GITHUB_USERNAME!/!GITHUB_REPO!
echo.
echo Next step: Add collaborators on GitHub
echo  1. Go to: https://github.com/!GITHUB_USERNAME!/!GITHUB_REPO!/settings/access
echo  2. Click "Add people"
echo  3. Search for team member usernames
echo  4. Click "Add to this repository"
echo.
pause
