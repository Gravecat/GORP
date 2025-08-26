// util/system/winver.cpp -- Some ugly code to determine what version of Windows (if any) the user is running.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef GORP_TARGET_WINDOWS
#include <windows.h>
#endif

#include "util/system/winver.hpp"

namespace gorp {
namespace winver {

#ifdef GORP_TARGET_WINDOWS

// Define the NTSTATUS and RtlGetVersion prototype
typedef LONG NTSTATUS;
typedef struct _OSVERSIONINFOEXW {
    ULONG dwOSVersionInfoSize;
    ULONG dwMajorVersion;
    ULONG dwMinorVersion;
    ULONG dwBuildNumber;
    ULONG dwPlatformId;
    WCHAR szCSDVersion[128];
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW;

extern "C" NTSTATUS NTAPI RtlGetVersion(POSVERSIONINFOEXW lpVersionInformation);

// Returns true if the user is running Windows 11.
bool is_windows_11()
{
    OSVERSIONINFOEXW osvi = {};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    if (RtlGetVersion(&osvi) == 0)  // STATUS_SUCCESS == 0
        return (osvi.dwMajorVersion == 10 && osvi.dwBuildNumber >= 22000);
    return false;
}

#else   // GORP_TARGET_WINDOWS
bool    is_windows_11() { return false; }
#endif  // GORP_TARGET_WINDOWS

}   // namespace winver
}   // namespace gorp
