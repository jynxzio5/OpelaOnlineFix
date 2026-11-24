#include "opela/steamworks_exports.h"
#include "opela/config.h"
#include "opela/logger.h"
#include "opela/interfaces/steam_utils.h"
#include <cstring>

static std::unique_ptr<Opela::Interfaces::SteamUtilsProxyImpl> g_utilsProxy;

extern "C" {

OPELA_API bool OPELA_CALL SteamAPI_Init() {
    OPELA_LOG_INFO("SteamAPI_Init called.");
    const auto& cfg = Opela::Core::ConfigManager::Instance().Get();
    if (!g_utilsProxy) {
        g_utilsProxy = std::make_unique<Opela::Interfaces::SteamUtilsProxyImpl>(cfg.fakeAppId);
    }
    return true;
}

OPELA_API bool OPELA_CALL SteamAPI_InitSafe() {
    return SteamAPI_Init();
}

OPELA_API void OPELA_CALL SteamAPI_Shutdown() {
    OPELA_LOG_INFO("SteamAPI_Shutdown called.");
    g_utilsProxy.reset();
}

OPELA_API void OPELA_CALL SteamAPI_RunCallbacks() {
}

OPELA_API bool OPELA_CALL SteamAPI_IsSteamRunning() {
    return true;
}

OPELA_API const char* OPELA_CALL SteamAPI_GetSteamInstallPath() {
    return "C:\\Program Files (x86)\\Steam";
}

OPELA_API HMODULE OPELA_CALL SteamAPI_GetHSteamUser() {
    return reinterpret_cast<HMODULE>(1);
}

OPELA_API HMODULE OPELA_CALL SteamAPI_GetHSteamPipe() {
    return reinterpret_cast<HMODULE>(1);
}

OPELA_API void* OPELA_CALL SteamInternal_CreateInterface(const char* version) {
    if (!version) return nullptr;
    OPELA_LOG_DEBUG(std::string("CreateInterface requested: ") + version);

    if (std::strstr(version, "SteamUtils")) {
        const auto& cfg = Opela::Core::ConfigManager::Instance().Get();
        if (!g_utilsProxy) {
            g_utilsProxy = std::make_unique<Opela::Interfaces::SteamUtilsProxyImpl>(cfg.fakeAppId);
        }
        return g_utilsProxy.get();
    }
    return nullptr;
}

OPELA_API void* OPELA_CALL SteamInternal_FindOrCreateUserInterface(int /*hSteamUser*/, const char* version) {
    return SteamInternal_CreateInterface(version);
}

OPELA_API void* OPELA_CALL SteamInternal_FindOrCreateServerInterface(int /*hSteamUser*/, const char* version) {
    return SteamInternal_CreateInterface(version);
}

}
