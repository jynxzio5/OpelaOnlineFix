#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#define OPELA_API __declspec(dllexport)
#define OPELA_CALL __cdecl
