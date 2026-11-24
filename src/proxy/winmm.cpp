#include "opela/common.h"
#include "opela/winmm_exports.h"
#include "MinHook.h"
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

typedef void* (__cdecl *fnSteamInternal_CreateInterface)(const char*);
static fnSteamInternal_CreateInterface g_pfnOrigCreateInterface = nullptr;
static uint32_t g_fakeAppId = 480;

// VTable hook for ISteamUtils::GetAppID
typedef uint32_t (__thiscall *fnGetAppID)(void*);
static fnGetAppID g_pfnOrigGetAppID = nullptr;

static uint32_t __fastcall Hooked_GetAppID(void* pThis) {
    return g_fakeAppId;
}

static void* __cdecl Hooked_SteamInternal_CreateInterface(const char* version) {
    if (!g_pfnOrigCreateInterface) return nullptr;
    void* iface = g_pfnOrigCreateInterface(version);
    if (iface && version && strstr(version, "SteamUtils")) {
        // VMT hook GetAppID at index 10
        void** vtable = *reinterpret_cast<void***>(iface);
        if (vtable && vtable[10] != Hooked_GetAppID) {
            DWORD oldProtect;
            if (VirtualProtect(&vtable[10], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                g_pfnOrigGetAppID = reinterpret_cast<fnGetAppID>(vtable[10]);
                vtable[10] = reinterpret_cast<void*>(Hooked_GetAppID);
                VirtualProtect(&vtable[10], sizeof(void*), oldProtect, &oldProtect);
            }
        }
    }
    return iface;
}

static void HookSteamApi() {
    HMODULE hSteam = nullptr;
    for (int i = 0; i < 200 && !hSteam; ++i) {
        hSteam = GetModuleHandleA("steam_api64.dll");
        if (!hSteam) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    if (!hSteam) return;

    auto pfnCreate = reinterpret_cast<fnSteamInternal_CreateInterface>(
        GetProcAddress(hSteam, "SteamInternal_CreateInterface")
    );

    if (pfnCreate) {
        MH_Initialize();
        if (MH_CreateHook(reinterpret_cast<void*>(pfnCreate), 
                          reinterpret_cast<void*>(Hooked_SteamInternal_CreateInterface), 
                          reinterpret_cast<void**>(&g_pfnOrigCreateInterface)) == MH_OK) {
            MH_EnableHook(reinterpret_cast<void*>(pfnCreate));
        }
    }
}

static void InitializeOpela() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    std::wstring dir = exePath;
    size_t lastSlash = dir.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        dir = dir.substr(0, lastSlash);
    }

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

    // Diagnostics log
    std::wstring logFile = dir + L"\\opela.log";
    std::ofstream log(logFile, std::ios::app);
    if (log.is_open()) {
        log << "[Opela WinMM Hook] Initialized. AppID forced to: " << appIdStr << "\n";
        log.close();
    }

    // Start background hook worker
    std::thread(HookSteamApi).detach();
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
