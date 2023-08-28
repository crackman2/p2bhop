#include "pch.h"
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // DLL is being loaded into the address space of a calling process
            // Perform initialization tasks here
            break;

        case DLL_PROCESS_DETACH:
            // DLL is being unloaded from the address space of a calling process
            // Perform cleanup tasks here
            break;

        case DLL_THREAD_ATTACH:
            // A new thread is being created in the current process
            break;

        case DLL_THREAD_DETACH:
            // A thread is exiting normally in the current process
            break;
    }
    return TRUE;
}
