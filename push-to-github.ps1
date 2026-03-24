#!/usr/bin/env pwsh
# ==================================================
# GitHub Push Helper - PowerShell Version
# ==================================================
# Edit the variables below with your GitHub info!
# ==================================================

$ErrorActionPreference = "Stop"

# === CONFIGURE THESE ===
$GITHUB_USERNAME = "YOUR_USERNAME"
$GITHUB_REPO = "Snake-Game"
# =====================

Write-Host "`n╔════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║   SNAKE GAME - GITHUB PUSH HELPER      ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════╝`n" -ForegroundColor Cyan

# Check configuration
if ($GITHUB_USERNAME -eq "YOUR_USERNAME") {
    Write-Host "❌ ERROR: Edit this script and set your GitHub username!" -ForegroundColor Red
    Write-Host "   Line 10: `$GITHUB_USERNAME = `"YOUR_USERNAME`"`n" -ForegroundColor Yellow
    Write-Host "   Example: `$GITHUB_USERNAME = `"john-doe`"`n"
    exit 1
}

$REPO_URL = "https://github.com/$GITHUB_USERNAME/$GITHUB_REPO.git"

Write-Host "📋 Configuration:" -ForegroundColor Green
Write-Host "   GitHub Username: $GITHUB_USERNAME"
Write-Host "   Repository Name: $GITHUB_REPO"
Write-Host "   Full URL: $REPO_URL`n"

# Navigate to script directory
$projectDir = Split-Path -Parent $PSScriptRoot
if (-not (Test-Path "$projectDir\.git")) {
    Write-Host "❌ ERROR: Not in a git repository!" -ForegroundColor Red
    exit 1
}

Write-Host "[1/4] Checking git repository... ✓" -ForegroundColor Green

# Remove existing remote
git remote remove origin 2>$null
Write-Host "[2/4] Configuring remote..." -ForegroundColor Green
Write-Host "   Adding: $REPO_URL" -ForegroundColor Gray

try {
    git remote add origin $REPO_URL
} catch {
    Write-Host "❌ Failed to add remote" -ForegroundColor Red
    exit 1
}

Write-Host "[3/4] Preparing branch..." -ForegroundColor Green
git branch -M main 2>$null
Write-Host "   Branch renamed to: main" -ForegroundColor Gray

Write-Host "`n[4/4] Pushing to GitHub..." -ForegroundColor Green
Write-Host "   (You'll be prompted for credentials if needed)`n" -ForegroundColor Gray

try {
    git push -u origin main
} catch {
    Write-Host "`n❌ Push failed!" -ForegroundColor Red
    Write-Host "   Check your GitHub URL and credentials" -ForegroundColor Yellow
    exit 1
}

Write-Host "`n╔════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║   ✓ SUCCESS - CODE PUSHED!             ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════╝`n" -ForegroundColor Green

Write-Host "Repository URL:" -ForegroundColor Cyan
Write-Host "  https://github.com/$GITHUB_USERNAME/$GITHUB_REPO`n"

Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Go to GitHub: https://github.com/$GITHUB_USERNAME/$GITHUB_REPO/settings/access"
Write-Host "  2. Click 'Add people'"
Write-Host "  3. Search for team member usernames"
Write-Host "  4. Click 'Add to this repository'`n"
