param(
    [Parameter(Mandatory=$true)]
    [string]$GameDirectory,

    [Parameter(Mandatory=$false)]
    [uint32]$RealAppId = 0
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $GameDirectory)) {
    Write-Error "Target game directory not found: $GameDirectory"
}

$binDir = Join-Path (Split-Path -Parent $PSScriptRoot) "build\bin\Release"
$sourceDll = Join-Path $binDir "steam_api64.dll"
$sourceIni = Join-Path (Split-Path -Parent $PSScriptRoot) "config\OpelaOnlineFix.ini"

if (-not (Test-Path $sourceDll)) {
    Write-Error "Compiled steam_api64.dll not found in $binDir. Run build.ps1 first."
}

# 1. Backup original game steam_api64.dll if not already backed up
$targetDll = Join-Path $GameDirectory "steam_api64.dll"
$backupDll = Join-Path $GameDirectory "steam_api64_o.dll"

if (Test-Path $targetDll) {
    if (-not (Test-Path $backupDll)) {
        Write-Host "Backing up original DLL to steam_api64_o.dll..." -ForegroundColor Yellow
        Rename-Item -Path $targetDll -NewName "steam_api64_o.dll"
    } else {
        Remove-Item -Path $targetDll -Force
    }
}

# 2. Copy proxy steam_api64.dll
Write-Host "Installing OpelaOnlineFix proxy DLL..." -ForegroundColor Cyan
Copy-Item -Path $sourceDll -Destination $targetDll -Force

# 3. Copy and configure OpelaOnlineFix.ini
$targetIni = Join-Path $GameDirectory "OpelaOnlineFix.ini"
Copy-Item -Path $sourceIni -Destination $targetIni -Force

if ($RealAppId -ne 0) {
    (Get-Content $targetIni) -replace 'RealAppId=0', "RealAppId=$RealAppId" | Set-Content $targetIni
    Write-Host "Configured RealAppId=$RealAppId in $targetIni" -ForegroundColor Green
}

Write-Host "Deployment completed successfully for $GameDirectory!" -ForegroundColor Green
