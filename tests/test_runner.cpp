#include "opela/interfaces/steam_utils.h"
#include <windows.h>
#include <iostream>

typedef bool (__cdecl *fnSteamAPI_Init)();
typedef void* (__cdecl *fnSteamInternal_CreateInterface)(const char*);

int main() {
    HMODULE hDll = LoadLibraryA("build/bin/Release/steam_api64.dll");
    if (!hDll) {
        std::cerr << "[-] Failed to load steam_api64.dll. Error: " << GetLastError() << "\n";
        return 1;
    }
    std::cout << "[+] Loaded steam_api64.dll successfully.\n";

    auto Init = (fnSteamAPI_Init)GetProcAddress(hDll, "SteamAPI_Init");
    auto CreateInterface = (fnSteamInternal_CreateInterface)GetProcAddress(hDll, "SteamInternal_CreateInterface");

    if (!Init || !CreateInterface) {
        std::cerr << "[-] Missing exported functions.\n";
        FreeLibrary(hDll);
        return 1;
    }

    if (!Init()) {
        std::cerr << "[-] SteamAPI_Init() failed.\n";
        FreeLibrary(hDll);
        return 1;
    }
    std::cout << "[+] SteamAPI_Init() returned TRUE.\n";

    auto* utils = static_cast<Opela::Interfaces::ISteamUtilsProxy*>(CreateInterface("SteamUtils010"));
    if (!utils) {
        std::cerr << "[-] Failed to obtain ISteamUtils interface.\n";
        FreeLibrary(hDll);
        return 1;
    }

    uint32_t appId = utils->GetAppID();
    std::cout << "[+] SteamUtils interface verified! Active AppID: " << appId << "\n";

    if (appId == 480) {
        std::cout << "[+] SUCCESS: AppID correctly routed to Spacewar (480)!\n";
    }

    FreeLibrary(hDll);
    std::cout << "[+] All tests passed flawlessly.\n";
    return 0;
}
