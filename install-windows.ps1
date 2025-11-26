# Data-Communication Project — Automated Setup Script for Windows (PowerShell)
# This script installs dependencies (via WSL or MSYS2), compiles, and runs the project.
# Requires: Windows 10/11, Administrator privileges

# Requires running as Administrator
if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Write-Host "This script must be run as Administrator. Please restart PowerShell as Admin." -ForegroundColor Red
    exit 1
}

# Color output functions
function Write-Header {
    param([string]$Message)
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host $Message -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
}

function Write-Success {
    param([string]$Message)
    Write-Host "✓ $Message" -ForegroundColor Green
}

function Write-Error {
    param([string]$Message)
    Write-Host "✗ $Message" -ForegroundColor Red
}

function Write-Warning {
    param([string]$Message)
    Write-Host "⚠ $Message" -ForegroundColor Yellow
}

function Write-Info {
    param([string]$Message)
    Write-Host "ℹ $Message" -ForegroundColor Blue
}

# Check if in correct directory
function Test-ProjectRoot {
    if ((Test-Path "Makefile") -and (Test-Path "src")) {
        return $true
    }
    return $false
}

# Detect Windows version
function Get-WindowsVersion {
    $version = [System.Environment]::OSVersion.Version
    if ($version.Major -ge 10) {
        return "Windows 10/11"
    }
    return "Windows 7/8"
}

# Check if WSL is installed
function Test-WSL {
    try {
        $wsl = wsl --version 2>$null
        if ($?) {
            return $true
        }
    } catch {}
    return $false
}

# Check if MSYS2 is installed
function Test-MSYS2 {
    if (Test-Path "C:\msys64\msys2_shell.cmd") {
        return $true
    }
    return $false
}

# Install WSL2 (Windows 11 recommended)
function Install-WSL {
    Write-Header "Installing WSL2"
    Write-Info "This requires Windows 10/11. Installing WSL..."
    
    Write-Info "Running 'wsl --install'..."
    wsl --install
    
    Write-Warning "WSL installation initiated. You may need to restart your computer."
    Write-Info "After restart, open Ubuntu (or your preferred distro) and run this script again from WSL."
    
    Read-Host "Press Enter after restarting and setting up WSL..."
    exit 0
}

# Setup project in WSL
function Setup-InWSL {
    Write-Header "Setting up in WSL"
    Write-Info "Creating script to run inside WSL..."
    
    # Create a bash script that will run inside WSL
    $wsslSetupScript = @'
#!/bin/bash
set -e

sudo apt update
sudo apt install -y build-essential git pkg-config libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

cd /mnt/c/Users/$env:USERNAME/projects/Data-Communication  # Adjust path as needed
make clean
make all

if [ -f "./build/meu_programa" ]; then
    echo "✓ Compilation successful!"
    read -p "Run application? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        ./build/meu_programa
    fi
fi
'@

    # Copy script to temporary location and run in WSL
    $tempScript = "$env:TEMP\setup-wsl.sh"
    $wsslSetupScript | Out-File -FilePath $tempScript -Encoding UTF8 -NoNewline
    
    Write-Info "Running setup inside WSL..."
    wsl bash -c "source $tempScript"
    
    Write-Success "WSL setup complete"
}

# Install MSYS2
function Install-MSYS2 {
    Write-Header "Installing MSYS2"
    
    Write-Info "Downloading MSYS2 installer..."
    $msys2Url = "https://github.com/msys2/msys2-installer/releases/download/2024-01-13/msys2-x86_64-20240113.exe"
    $installerPath = "$env:TEMP\msys2-installer.exe"
    
    try {
        Invoke-WebRequest -Uri $msys2Url -OutFile $installerPath -UseBasicParsing
        Write-Success "MSYS2 installer downloaded"
    } catch {
        Write-Error "Failed to download MSYS2. Please visit https://www.msys2.org/ and install manually."
        exit 1
    }
    
    Write-Info "Running MSYS2 installer..."
    & $installerPath
    
    Write-Warning "MSYS2 installation initiated. Complete the installer, then run this script again."
    exit 0
}

# Setup in MSYS2
function Setup-InMSYS2 {
    Write-Header "Setting up in MSYS2"
    
    $msys2Root = "C:\msys64"
    if (-not (Test-Path $msys2Root)) {
        Write-Error "MSYS2 not found at $msys2Root"
        Write-Info "Please install MSYS2 or adjust the path."
        exit 1
    }
    
    $msys2Shell = "$msys2Root\msys2_shell.cmd"
    $setupCmd = @"
@echo off
set MSYSTEM=MINGW64
call $msys2Shell -defterm -here -no-start pacman -Syu --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw mingw-w64-x86_64-pkg-config mingw-w64-x86_64-mesa 2>&1
call $msys2Shell -defterm -here -no-start bash -c "cd $(Get-Location) && make clean && make all 2>&1"
"@
    
    $setupBat = "$env:TEMP\setup-msys2.bat"
    Set-Content -Path $setupBat -Value $setupCmd
    
    Write-Info "Running MSYS2 setup..."
    & cmd.exe /c $setupBat
    
    if (Test-Path ".\build\meu_programa.exe") {
        Write-Success "Compilation successful!"
        $run = Read-Host "Run application? (y/n)"
        if ($run -eq "y" -or $run -eq "Y") {
            & ".\build\meu_programa.exe"
        }
    } else {
        Write-Error "Compilation failed. Check MSYS2 installation and try again."
        exit 1
    }
}

# Setup config.txt
function Setup-Config {
    Write-Header "Checking configuration"
    
    if (-not (Test-Path "config.txt")) {
        Write-Warning "config.txt not found, creating default..."
        $configContent = @"
# Data-Communication Configuration
RECEIVER_IP=127.0.0.1
PORT=4444
"@
        $configContent | Out-File -FilePath "config.txt" -Encoding UTF8
        Write-Success "config.txt created with defaults"
    } else {
        Write-Success "config.txt exists"
    }
}

# Main function
function Main {
    Write-Header "Data-Communication — Windows Setup Script"
    
    # Check if in correct directory
    if (-not (Test-ProjectRoot)) {
        Write-Error "This script must be run from the project root directory!"
        Write-Info "Please navigate to the Data-Communication directory and try again."
        exit 1
    }
    
    $osVersion = Get-WindowsVersion
    Write-Info "Detected: $osVersion"
    
    # Check for existing tools
    $hasWSL = Test-WSL
    $hasMSYS2 = Test-MSYS2
    
    if ($hasWSL) {
        Write-Success "WSL2 detected"
        $choice = Read-Host "Use WSL2 for setup? (y/n)"
        if ($choice -eq "y" -or $choice -eq "Y") {
            Setup-InWSL
            return
        }
    }
    
    if ($hasMSYS2) {
        Write-Success "MSYS2 detected"
        $choice = Read-Host "Use MSYS2 for setup? (y/n)"
        if ($choice -eq "y" -or $choice -eq "Y") {
            Setup-InMSYS2
            return
        }
    }
    
    # If no tool detected, ask user
    Write-Header "No development environment detected"
    Write-Info "You have two options:"
    Write-Info "1. WSL2 (recommended for Windows 11) — Linux environment on Windows"
    Write-Info "2. MSYS2 (native Windows) — MinGW toolchain"
    Write-Host ""
    
    $option = Read-Host "Choose installation method (1 for WSL2, 2 for MSYS2)"
    
    switch ($option) {
        "1" {
            Install-WSL
        }
        "2" {
            Install-MSYS2
        }
        default {
            Write-Error "Invalid option. Exiting."
            exit 1
        }
    }
}

# Run main
Main
