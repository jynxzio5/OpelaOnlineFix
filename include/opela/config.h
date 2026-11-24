#pragma once
#include "common.h"

namespace Opela {
namespace Core {

struct ProxyConfig {
    uint32_t realAppId = 0;
    uint32_t fakeAppId = 480;
    bool unlockAllDLC = true;
    bool enableLogging = true;
    std::wstring logFilePath = L"opela.log";
    std::wstring originalLibraryPath = L"steam_api64_o.dll";
};

class ConfigManager {
public:
    static ConfigManager& Instance();
    bool Load(const std::wstring& path);
    const ProxyConfig& Get() const { return m_config; }

private:
    ConfigManager() = default;
    ProxyConfig m_config;
};

} // namespace Core
} // namespace Opela
