# OpelaOnlineFix Architecture

## High-Level Overview

OpelaOnlineFix is an enterprise-structured C++ modular proxy layer for the Steamworks API. It intercepts runtime calls between the game client and the Steam ecosystem to redirect multiplayer lobby management and authentication without requiring engine modification.

```
+-------------------------------------------------------------+
|                      Game Executable                        |
+-------------------------------------------------------------+
                              |
                              | Imports (e.g., SteamAPI_Init)
                              v
+-------------------------------------------------------------+
|                 OpelaOnlineFix Proxy Layer                  |
|                                                             |
|   +-----------------------+     +-----------------------+   |
|   |   Config Manager      |     |   Diagnostics Logger  |   |
|   |   (OpelaOnlineFix.ini)|     |   (opela.log)         |   |
|   +-----------------------+     +-----------------------+   |
|                              |                              |
|   +-----------------------------------------------------+   |
|   |             Interface Virtual Tables                |   |
|   |  - ISteamUtils (Forces GetAppID() = 480)            |   |
|   |  - ISteamUser / ISteamFriends / ISteamMatchmaking   |   |
|   +-----------------------------------------------------+   |
+-------------------------------------------------------------+
                              |
                              | Sockets / P2P Relay
                              v
+-------------------------------------------------------------+
|              Steamworks Relay Network / Peer                |
+-------------------------------------------------------------+
```

## Directory Structure

```
OpelaOnlineFix/
├── cmake/                      # Build system modules & warning presets
│   └── CompilerWarnings.cmake
├── config/                     # Production & development configuration templates
│   ├── OpelaOnlineFix.ini      # Primary runtime settings (AppIDs, feature flags)
│   └── dlllist.txt             # Sideload list for proxy loaders (e.g., winmm.dll)
├── docs/                       # Comprehensive documentation
│   ├── ARCHITECTURE.md         # System design & architecture
│   ├── API_REFERENCE.md        # Exported functions and interfaces
│   └── SETUP_GUIDE.md          # Step-by-step setup and deployment instructions
├── include/
│   └── opela/                  # Public and internal headers
│       ├── common.h            # Macros, platform includes, and calling conventions
│       ├── config.h            # Configuration loading manager
│       ├── logger.h            # Thread-safe diagnostic logging engine
│       ├── steamworks_exports.h# C ABI export signatures
│       └── interfaces/         # Emulated Steamworks COM-style interfaces
│           └── steam_utils.h   # Utils interface implementation (AppID routing)
├── src/
│   ├── core/                   # Core initialization and lifecycle management
│   │   ├── dllmain.cpp         # DLL entrypoint & module lifecycle
│   │   ├── config.cpp          # INI configuration parsing
│   │   └── logger.cpp          # Asynchronous/synchronous log streaming
│   ├── hooks/                  # Runtime hooking utilities and detours
│   └── proxy/                  # Steamworks C API forwarding and implementation
│       └── steamworks_exports.cpp
├── scripts/
│   └── build.ps1               # Automated build & compilation script
├── CMakeLists.txt              # Top-level CMake project definition
└── README.md                   # Project overview & quick start
```

## Key Mechanisms

1. **Loader / Sideloading**:
   Can be deployed as `steam_api64.dll` directly or loaded via a system proxy (such as `winmm.dll` or `version.dll`) using `config/dlllist.txt`.

2. **AppID Substitution**:
   Intercepts `SteamUtils()->GetAppID()` to return the configured `FakeAppId` (defaults to `480` for Spacewar), which routes matchmaking through Steam's public test lobbies.

3. **Logging & Diagnostics**:
   Synchronized, low-overhead logging outputs runtime calls to `opela.log` to trace compatibility and missing interface versions.
