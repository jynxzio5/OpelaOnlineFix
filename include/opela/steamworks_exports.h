#pragma once
#include "common.h"

extern "C" {
    OPELA_API bool OPELA_CALL SteamAPI_Init();
    OPELA_API bool OPELA_CALL SteamAPI_InitSafe();
    OPELA_API void OPELA_CALL SteamAPI_Shutdown();
    OPELA_API void OPELA_CALL SteamAPI_RunCallbacks();
    OPELA_API bool OPELA_CALL SteamAPI_IsSteamRunning();
    OPELA_API const char* OPELA_CALL SteamAPI_GetSteamInstallPath();
    OPELA_API HMODULE OPELA_CALL SteamAPI_GetHSteamUser();
    OPELA_API HMODULE OPELA_CALL SteamAPI_GetHSteamPipe();
    OPELA_API void* OPELA_CALL SteamInternal_CreateInterface(const char* version);
    OPELA_API void* OPELA_CALL SteamInternal_FindOrCreateUserInterface(int hSteamUser, const char* version);
    OPELA_API void* OPELA_CALL SteamInternal_FindOrCreateServerInterface(int hSteamUser, const char* version);
}
