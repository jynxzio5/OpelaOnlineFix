#include "opela/logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace Opela {
namespace Diagnostics {

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

void Logger::Initialize(const std::wstring& logFile, bool enabled) {
    m_enabled = enabled;
    m_logPath = logFile;
    InitializeCriticalSection(&m_lock);
    if (!m_enabled) return;

    m_fileHandle = CreateFileW(
        m_logPath.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (!m_enabled || m_fileHandle == INVALID_HANDLE_VALUE) return;

    EnterCriticalSection(&m_lock);

    const char* lvlStr = "[INFO]";
    switch (level) {
        case LogLevel::Debug: lvlStr = "[DEBUG]"; break;
        case LogLevel::Warning: lvlStr = "[WARN]"; break;
        case LogLevel::Error: lvlStr = "[ERROR]"; break;
        default: break;
    }

    std::string line = std::string(lvlStr) + " " + message + "\r\n";
    DWORD written = 0;
    WriteFile(m_fileHandle, line.data(), static_cast<DWORD>(line.size()), &written, nullptr);

    LeaveCriticalSection(&m_lock);
}

void Logger::Shutdown() {
    if (m_fileHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
    }
    DeleteCriticalSection(&m_lock);
}

} // namespace Diagnostics
} // namespace Opela
