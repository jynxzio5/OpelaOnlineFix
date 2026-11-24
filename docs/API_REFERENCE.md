# OpelaOnlineFix API Reference

## Exported Functions (C ABI)

The module exports standard Steamworks runtime entry points:

### 1. `SteamAPI_Init`
```c
bool SteamAPI_Init();
```
- **Description**: Initializes the proxy runtime, reads `OpelaOnlineFix.ini`, initializes logging, and instantiates default interfaces.
- **Return Value**: Returns `true` if initialization succeeded.

### 2. `SteamAPI_Shutdown`
```c
void SteamAPI_Shutdown();
```
- **Description**: Flushes logs, frees allocated interfaces, and cleans up library handles.

### 3. `SteamAPI_RunCallbacks`
```c
void SteamAPI_RunCallbacks();
```
- **Description**: Invoked once per frame by the game engine to pump Steamworks callbacks and dispatch incoming events.

### 4. `SteamAPI_IsSteamRunning`
```c
bool SteamAPI_IsSteamRunning();
```
- **Description**: Reports whether the Steam background client process is active. Always returns `true` in proxy mode.

### 5. `SteamInternal_CreateInterface`
```c
void* SteamInternal_CreateInterface(const char* version);
```
- **Description**: Factory function returning an interface pointer matching the requested version string (e.g. `SteamUtils010`).
- **Return Value**: Pointer to the virtual table implementation or `nullptr` if unhandled.
