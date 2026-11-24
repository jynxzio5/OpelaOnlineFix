#include "opela/common.h"
#include "opela/config.h"
#include "opela/logger.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID /*lpReserved*/) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);

        wchar_t modulePath[MAX_PATH];
        GetModuleFileNameW(nullptr, modulePath, MAX_PATH);
        std::wstring dir = modulePath;
        size_t lastSlash = dir.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            dir = dir.substr(0, lastSlash);
        }

        std::wstring iniPath = dir + L"\\OpelaOnlineFix.ini";
        Opela::Core::ConfigManager::Instance().Load(iniPath);
        const auto& cfg = Opela::Core::ConfigManager::Instance().Get();

        std::wstring logFile = dir + L"\\" + cfg.logFilePath;
        Opela::Diagnostics::Logger::Instance().Initialize(logFile, cfg.enableLogging);
        OPELA_LOG_INFO("OpelaOnlineFix initialized successfully.");
        break;
    }
    case DLL_PROCESS_DETACH:
        OPELA_LOG_INFO("OpelaOnlineFix unloading.");
        Opela::Diagnostics::Logger::Instance().Shutdown();
        break;
    }
    return TRUE;
}
