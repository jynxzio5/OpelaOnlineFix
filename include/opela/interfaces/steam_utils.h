#pragma once
#include "../common.h"
#include <ctime>

namespace Opela {
namespace Interfaces {

class ISteamUtilsProxy {
public:
    virtual ~ISteamUtilsProxy() = default;
    virtual uint32_t GetSecondsSinceAppActive() = 0;
    virtual uint32_t GetSecondsSinceComputerActive() = 0;
    virtual uint32_t GetConnectedUniverse() = 0;
    virtual uint32_t GetServerRealTime() = 0;
    virtual const char* GetIPCountry() = 0;
    virtual bool GetImageSize(int iImage, uint32_t *pnWidth, uint32_t *pnHeight) = 0;
    virtual bool GetImageRGBA(int iImage, uint8_t *pubDest, int nDestBufferSize) = 0;
    virtual bool GetCSERIPPort(uint32_t *unIP, uint16_t *usPort) = 0;
    virtual uint8_t GetCurrentBatteryPower() = 0;
    virtual uint32_t GetAppID() = 0;
    virtual void SetOverlayNotificationPosition(int eNotificationPosition) = 0;
    virtual bool IsAPICallCompleted(uint64_t hSteamAPICall, bool *pbFailed) = 0;
};

class SteamUtilsProxyImpl : public ISteamUtilsProxy {
public:
    explicit SteamUtilsProxyImpl(uint32_t fakeAppId) : m_fakeAppId(fakeAppId) {}
    uint32_t GetSecondsSinceAppActive() override { return 0; }
    uint32_t GetSecondsSinceComputerActive() override { return 0; }
    uint32_t GetConnectedUniverse() override { return 1; }
    uint32_t GetServerRealTime() override { return static_cast<uint32_t>(::time(nullptr)); }
    const char* GetIPCountry() override { return "US"; }
    bool GetImageSize(int, uint32_t*, uint32_t*) override { return false; }
    bool GetImageRGBA(int, uint8_t*, int) override { return false; }
    bool GetCSERIPPort(uint32_t*, uint16_t*) override { return false; }
    uint8_t GetCurrentBatteryPower() override { return 255; }
    uint32_t GetAppID() override { return m_fakeAppId; }
    void SetOverlayNotificationPosition(int) override {}
    bool IsAPICallCompleted(uint64_t, bool*) override { return true; }

private:
    uint32_t m_fakeAppId;
};

} // namespace Interfaces
} // namespace Opela
