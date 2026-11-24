# OpelaOnlineFix

An enterprise-grade, modular C++ Steamworks proxy and compatibility layer.

## Overview
OpelaOnlineFix is architected to intercept and virtualize Steamworks runtime API calls. It allows game clients to communicate over standard Steam matchmaking relays using standardized AppID redirection and configurable interfaces.

## Project Layout
```
OpelaOnlineFix/
├── cmake/                      # CMake helper scripts and compiler warning profiles
├── config/                     # Default runtime ini configs and loader lists
├── docs/                       # Architecture, API references, and deployment guides
├── include/                    # Header files organized by module (core, interfaces)
├── src/                        # Source implementation files (core, proxy, hooks)
├── scripts/                    # Automated build automation scripts
├── CMakeLists.txt              # Primary project build definition
└── README.md                   # Project overview
```

## Documentation
- [Architecture & Design](docs/ARCHITECTURE.md)
- [API Reference](docs/API_REFERENCE.md)
- [Setup & Deployment Guide](docs/SETUP_GUIDE.md)

## Building
```powershell
.\scripts\build.ps1 -Configuration Release
```
Artifacts are automatically produced in `build/bin/`.
