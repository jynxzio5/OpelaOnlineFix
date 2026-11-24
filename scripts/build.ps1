param(
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$buildDir = Join-Path $root "build"

# Locate CMake
$cmakeCmd = "cmake"
if (-not (Get-Command "cmake" -ErrorAction SilentlyContinue)) {
    $vsCmake = "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
    if (Test-Path $vsCmake) {
        $cmakeCmd = $vsCmake
    } else {
        $vsCmakeX86 = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
        if (Test-Path $vsCmakeX86) {
            $cmakeCmd = $vsCmakeX86
        }
    }
}

Write-Host "==> Generating build files in $buildDir ($Configuration)..." -ForegroundColor Cyan
& $cmakeCmd -B $buildDir -S $root -DCMAKE_BUILD_TYPE=$Configuration

Write-Host "==> Compiling target OpelaOnlineFix..." -ForegroundColor Cyan
& $cmakeCmd --build $buildDir --config $Configuration

Write-Host "==> Build complete. Artifacts located in $buildDir/bin" -ForegroundColor Green
