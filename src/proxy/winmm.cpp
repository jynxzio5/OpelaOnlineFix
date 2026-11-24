#include "opela/common.h"
#include "opela/winmm_exports.h"
#include "MinHook.h"
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

static uint32_t g_fakeAppId = 480;
static std::wstring g_logPath;

static void LogMsg(const std::string& msg) {
    if (g_logPath.empty()) return;
    std::ofstream ofs(g_logPath, std::ios::app);
    if (ofs.is_open()) {
        ofs << "[Opela] " << msg << "\n";
        ofs.close();
    }
}

// 1. SteamAPI_ISteamUtils_GetAppID
typedef uint32_t (__cdecl *fnSteamAPI_ISteamUtils_GetAppID)(void*);
static fnSteamAPI_ISteamUtils_GetAppID g_orig_GetAppID = nullptr;
static uint32_t __cdecl Hooked_SteamAPI_ISteamUtils_GetAppID(void* /*pUtils*/) {
    LogMsg("SteamAPI_ISteamUtils_GetAppID called -> returning 480");
    return g_fakeAppId;
}

// 2. SteamAPI_ISteamApps_BIsSubscribed
typedef bool (__cdecl *fnSteamAPI_ISteamApps_BIsSubscribed)(void*);
static fnSteamAPI_ISteamApps_BIsSubscribed g_orig_BIsSubscribed = nullptr;
static bool __cdecl Hooked_SteamAPI_ISteamApps_BIsSubscribed(void* /*pApps*/) {
    LogMsg("SteamAPI_ISteamApps_BIsSubscribed called -> returning true");
    return true;
}

// 3. SteamAPI_ISteamApps_BIsSubscribedApp
typedef bool (__cdecl *fnSteamAPI_ISteamApps_BIsSubscribedApp)(void*, uint32_t);
static fnSteamAPI_ISteamApps_BIsSubscribedApp g_orig_BIsSubscribedApp = nullptr;
static bool __cdecl Hooked_SteamAPI_ISteamApps_BIsSubscribedApp(void* /*pApps*/, uint32_t appId) {
    LogMsg("SteamAPI_ISteamApps_BIsSubscribedApp called for AppID " + std::to_string(appId) + " -> returning true");
    return true;
}

// 4. SteamAPI_ISteamApps_BIsDlcInstalled
typedef bool (__cdecl *fnSteamAPI_ISteamApps_BIsDlcInstalled)(void*, uint32_t);
static fnSteamAPI_ISteamApps_BIsDlcInstalled g_orig_BIsDlcInstalled = nullptr;
static bool __cdecl Hooked_SteamAPI_ISteamApps_BIsDlcInstalled(void* /*pApps*/, uint32_t appId) {
    LogMsg("SteamAPI_ISteamApps_BIsDlcInstalled called for DLC " + std::to_string(appId) + " -> returning true");
    return true;
}

// 5. SteamAPI_ISteamApps_BIsAppInstalled
typedef bool (__cdecl *fnSteamAPI_ISteamApps_BIsAppInstalled)(void*, uint32_t);
static fnSteamAPI_ISteamApps_BIsAppInstalled g_orig_BIsAppInstalled = nullptr;
static bool __cdecl Hooked_SteamAPI_ISteamApps_BIsAppInstalled(void* /*pApps*/, uint32_t appId) {
    return true;
}

// 6. SteamAPI_RestartAppIfNecessary
typedef bool (__cdecl *fnSteamAPI_RestartAppIfNecessary)(uint32_t);
static fnSteamAPI_RestartAppIfNecessary g_orig_RestartApp = nullptr;
static bool __cdecl Hooked_SteamAPI_RestartAppIfNecessary(uint32_t appId) {
    LogMsg("SteamAPI_RestartAppIfNecessary called for AppID " + std::to_string(appId) + " -> returning false");
    return false;
}

// 7. SteamInternal_CreateInterface
typedef void* (__cdecl *fnSteamInternal_CreateInterface)(const char*);
static fnSteamInternal_CreateInterface g_orig_CreateInterface = nullptr;

static uint32_t __fastcall Hooked_VMT_GetAppID(void* /*pThis*/) {
    return g_fakeAppId;
}

static void* __cdecl Hooked_SteamInternal_CreateInterface(const char* version) {
    if (!g_orig_CreateInterface) return nullptr;
    void* iface = g_orig_CreateInterface(version);
    if (iface && version && strstr(version, "SteamUtils")) {
        void** vtable = *reinterpret_cast<void***>(iface);
        if (vtable && vtable[10] != Hooked_VMT_GetAppID) {
            DWORD oldProtect;
            if (VirtualProtect(&vtable[10], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                vtable[10] = reinterpret_cast<void*>(Hooked_VMT_GetAppID);
                VirtualProtect(&vtable[10], sizeof(void*), oldProtect, &oldProtect);
                LogMsg("Hooked VMT ISteamUtils::GetAppID successfully.");
            }
        }
    }
    return iface;
}

static void HookTargetFunctions(HMODULE hSteam) {
    LogMsg("HookTargetFunctions: Attaching MinHook to steam_api64 exports...");
    MH_Initialize();

    auto pfnGetAppID = GetProcAddress(hSteam, "SteamAPI_ISteamUtils_GetAppID");
    if (pfnGetAppID) {
        MH_CreateHook(pfnGetAppID, (void*)Hooked_SteamAPI_ISteamUtils_GetAppID, (void**)&g_orig_GetAppID);
    }

    auto pfnSub = GetProcAddress(hSteam, "SteamAPI_ISteamApps_BIsSubscribed");
    if (pfnSub) {
        MH_CreateHook(pfnSub, (void*)Hooked_SteamAPI_ISteamApps_BIsSubscribed, (void**)&g_orig_BIsSubscribed);
    }

    auto pfnSubApp = GetProcAddress(hSteam, "SteamAPI_ISteamApps_BIsSubscribedApp");
    if (pfnSubApp) {
        MH_CreateHook(pfnSubApp, (void*)Hooked_SteamAPI_ISteamApps_BIsSubscribedApp, (void**)&g_orig_BIsSubscribedApp);
    }

    auto pfnDlc = GetProcAddress(hSteam, "SteamAPI_ISteamApps_BIsDlcInstalled");
    if (pfnDlc) {
        MH_CreateHook(pfnDlc, (void*)Hooked_SteamAPI_ISteamApps_BIsDlcInstalled, (void**)&g_orig_BIsDlcInstalled);
    }

    auto pfnAppInst = GetProcAddress(hSteam, "SteamAPI_ISteamApps_BIsAppInstalled");
    if (pfnAppInst) {
        MH_CreateHook(pfnAppInst, (void*)Hooked_SteamAPI_ISteamApps_BIsAppInstalled, (void**)&g_orig_BIsAppInstalled);
    }

    auto pfnRestart = GetProcAddress(hSteam, "SteamAPI_RestartAppIfNecessary");
    if (pfnRestart) {
        MH_CreateHook(pfnRestart, (void*)Hooked_SteamAPI_RestartAppIfNecessary, (void**)&g_orig_RestartApp);
    }

    auto pfnCreate = GetProcAddress(hSteam, "SteamInternal_CreateInterface");
    if (pfnCreate) {
        MH_CreateHook(pfnCreate, (void*)Hooked_SteamInternal_CreateInterface, (void**)&g_orig_CreateInterface);
    }

    MH_EnableHook(MH_ALL_HOOKS);
    LogMsg("All Steamworks hooks enabled successfully!");
}

// Hook LoadLibraryW to catch steam_api64 synchronously
typedef HMODULE (WINAPI *fnLoadLibraryW)(LPCWSTR);
static fnLoadLibraryW g_orig_LoadLibraryW = nullptr;

static HMODULE WINAPI Hooked_LoadLibraryW(LPCWSTR lpLibFileName) {
    HMODULE hMod = g_orig_LoadLibraryW(lpLibFileName);
    if (hMod && lpLibFileName && wcsstr(lpLibFileName, L"steam_api64")) {
        LogMsg("Synchronous catch: steam_api64.dll loaded by engine!");
        HookTargetFunctions(hMod);
    }
    return hMod;
}

static void InitializeOpela() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    std::wstring dir = exePath;
    size_t lastSlash = dir.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        dir = dir.substr(0, lastSlash);
    }

    g_logPath = dir + L"\\opela.log";

    std::wstring iniPath = dir + L"\\OpelaOnlineFix.ini";
    g_fakeAppId = GetPrivateProfileIntW(L"Main", L"FakeAppId", 480, iniPath.c_str());

    std::string appIdStr = std::to_string(g_fakeAppId);
    SetEnvironmentVariableA("SteamAppId", appIdStr.c_str());
    SetEnvironmentVariableA("SteamGameId", appIdStr.c_str());

    // Write steam_appid.txt in game directory
    std::wstring appIdFile = dir + L"\\steam_appid.txt";
    std::ofstream ofs(appIdFile);
    if (ofs.is_open()) {
        ofs << appIdStr;
        ofs.close();
    }

    LogMsg("Opela WinMM Proxy attached. FakeAppId=" + appIdStr);

    // Check if steam_api64 is already loaded; otherwise hook LoadLibraryW
    HMODULE hSteam = GetModuleHandleA("steam_api64.dll");
    if (hSteam) {
        HookTargetFunctions(hSteam);
    } else {
        MH_Initialize();
        MH_CreateHook(&LoadLibraryW, (void*)Hooked_LoadLibraryW, (void**)&g_orig_LoadLibraryW);
        MH_EnableHook(&LoadLibraryW);
        LogMsg("Installed synchronous LoadLibraryW watcher.");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID /*lpReserved*/) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        InitializeOpela();
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        MH_Uninitialize();
    }
    return TRUE;
}
