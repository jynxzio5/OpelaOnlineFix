#pragma once
#include "common.h"
#include <string>

namespace Opela {
namespace Diagnostics {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static Logger& Instance();
    void Initialize(const std::wstring& logFile, bool enabled);
    void Log(LogLevel level, const std::string& message);
    void Shutdown();

private:
    Logger() = default;
    bool m_enabled = true;
    std::wstring m_logPath;
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;
    CRITICAL_SECTION m_lock;
};

#define OPELA_LOG_INFO(msg) Opela::Diagnostics::Logger::Instance().Log(Opela::Diagnostics::LogLevel::Info, msg)
#define OPELA_LOG_DEBUG(msg) Opela::Diagnostics::Logger::Instance().Log(Opela::Diagnostics::LogLevel::Debug, msg)
#define OPELA_LOG_ERROR(msg) Opela::Diagnostics::Logger::Instance().Log(Opela::Diagnostics::LogLevel::Error, msg)

} // namespace Diagnostics
} // namespace Opela
