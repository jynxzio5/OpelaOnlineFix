# OpelaOnlineFix Setup & Deployment Guide

## Prerequisites
- Windows 10/11 x64
- Visual Studio 2019/2022 (MSVC v142/v143) with C++ Desktop Development tools
- CMake 3.16 or higher

## Compiling the Project

### Option A: Using PowerShell Script
```powershell
.\scripts\build.ps1 -Configuration Release
```

### Option B: Manual CMake
```cmd
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The compiled `steam_api64.dll`, along with `OpelaOnlineFix.ini` and `dlllist.txt`, will be placed in `build/bin/`.

## Deployment into Game Directory

1. Locate the game executable directory containing the original `steam_api64.dll`.
2. Rename the game's original DLL to `steam_api64_o.dll`.
3. Copy the compiled `steam_api64.dll` and `OpelaOnlineFix.ini` into the game directory.
4. Open `OpelaOnlineFix.ini` and set `RealAppId` to the game's actual Steam AppID (optional, for reference) and verify `FakeAppId=480`.
5. Launch the game. Diagnostics will be logged to `opela.log`.
