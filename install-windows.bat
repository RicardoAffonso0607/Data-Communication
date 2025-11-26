@echo off
REM Data-Communication Project — Automated Setup Script for Windows (Batch)
REM This script provides a simpler alternative to PowerShell for installing and running the project.
REM Requires: Windows 10/11, Administrator privileges

setlocal enabledelayedexpansion

REM Color codes (using alternative approach for older Windows)
set "RED=[91m"
set "GREEN=[92m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "NC=[0m"

REM Check for Administrator privileges
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo This script must be run as Administrator.
    echo Right-click Command Prompt and select "Run as Administrator"
    pause
    exit /b 1
)

REM Check if in correct directory
if not exist "Makefile" (
    echo Error: Makefile not found!
    echo Please navigate to the Data-Communication directory and try again.
    pause
    exit /b 1
)

echo ========================================
echo Data-Communication - Windows Setup
echo ========================================
echo.

REM Option selection
echo Choose installation method:
echo 1. WSL2 (recommended for Windows 11)
echo 2. MSYS2 (native Windows development)
echo 3. Manual setup (just compile existing environment)
echo.
set /p choice="Enter your choice (1, 2, or 3): "

if "%choice%"=="1" (
    call :SetupWSL
) else if "%choice%"=="2" (
    call :SetupMSYS2
) else if "%choice%"=="3" (
    call :SetupManual
) else (
    echo Invalid choice. Exiting.
    pause
    exit /b 1
)

goto :end

REM WSL2 Setup Function
:SetupWSL
echo.
echo ========================================
echo Setting up with WSL2
echo ========================================
echo.

REM Check if WSL is installed
wsl --version >nul 2>&1
if %errorLevel% neq 0 (
    echo WSL2 is not installed.
    echo Installing WSL2...
    echo.
    wsl --install
    echo.
    echo WSL installation initiated. You may need to restart your computer.
    echo After restart, open Ubuntu (or your preferred distro) and run:
    echo   cd /mnt/c/path/to/Data-Communication
    echo   bash install-linux.sh
    pause
    exit /b 0
)

echo WSL2 detected. Setting up in WSL...
echo.

REM Run setup inside WSL
wsl bash -c "cd /mnt/c && pwd && apt update && apt install -y build-essential git pkg-config libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"

if %errorLevel% neq 0 (
    echo.
    echo Failed to install dependencies in WSL.
    pause
    exit /b 1
)

echo.
echo Dependencies installed successfully in WSL.
pause
exit /b 0

REM MSYS2 Setup Function
:SetupMSYS2
echo.
echo ========================================
echo Setting up with MSYS2
echo ========================================
echo.

REM Check if MSYS2 is installed
if exist "C:\msys64\msys2_shell.cmd" (
    echo MSYS2 detected at C:\msys64
    echo.
    echo Opening MSYS2 shell for dependency installation...
    echo Please run these commands in the MSYS2 shell that opens:
    echo   pacman -Syu --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw mingw-w64-x86_64-pkg-config mingw-w64-x86_64-mesa
    echo   cd /c/path/to/Data-Communication
    echo   make clean ^&^& make all
    echo.
    pause
    
    REM Launch MSYS2
    call C:\msys64\msys2_shell.cmd -mingw64
    exit /b 0
) else (
    echo MSYS2 is not installed at C:\msys64
    echo.
    echo MSYS2 installation instructions:
    echo 1. Download from: https://www.msys2.org/
    echo 2. Run the installer and follow the instructions
    echo 3. Install dependencies with pacman:
    echo    pacman -Syu --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw mingw-w64-x86_64-pkg-config mingw-w64-x86_64-mesa
    echo 4. Clone the repository and run 'make all'
    echo.
    pause
    exit /b 1
)

REM Manual/Existing Environment Setup
:SetupManual
echo.
echo ========================================
echo Manual Setup (existing environment)
echo ========================================
echo.

if not exist "config.txt" (
    echo Creating default config.txt...
    (
        echo # Data-Communication Configuration
        echo RECEIVER_IP=127.0.0.1
        echo PORT=4444
    ) > config.txt
    echo Created config.txt with default settings
    echo.
)

if exist "build\meu_programa.exe" (
    echo Found compiled executable: build\meu_programa.exe
    set /p runNow="Run application now? (y/n): "
    if /i "!runNow!"=="y" (
        call build\meu_programa.exe
    )
) else (
    echo.
    echo Executable not found at build\meu_programa.exe
    echo.
    echo To compile the project, you need a development environment installed.
    echo Please use option 1 (WSL2) or option 2 (MSYS2) first.
    echo.
    pause
    exit /b 1
)

exit /b 0

:end
echo.
echo ========================================
echo Setup Complete
echo ========================================
echo.
echo To run the application:
echo   ./build/meu_programa.exe  (or just double-click it)
echo.
echo For network setup, see WINDOWS_NETWORK_GUIDE.md
echo.
pause
