#include "opela/config.h"

namespace Opela {
namespace Core {

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::Load(const std::wstring& path) {
    m_config.realAppId = GetPrivateProfileIntW(L"Main", L"RealAppId", 0, path.c_str());
    m_config.fakeAppId = GetPrivateProfileIntW(L"Main", L"FakeAppId", 480, path.c_str());
    m_config.unlockAllDLC = GetPrivateProfileIntW(L"Main", L"UnlockAllDLC", 1, path.c_str()) != 0;
    m_config.enableLogging = GetPrivateProfileIntW(L"Diagnostics", L"EnableLogging", 1, path.c_str()) != 0;

    wchar_t buf[MAX_PATH];
    GetPrivateProfileStringW(L"Diagnostics", L"LogFile", L"opela.log", buf, MAX_PATH, path.c_str());
    m_config.logFilePath = buf;

    GetPrivateProfileStringW(L"Main", L"OriginalLibrary", L"steam_api64_o.dll", buf, MAX_PATH, path.c_str());
    m_config.originalLibraryPath = buf;

    return true;
}

} // namespace Core
} // namespace Opela
