// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "cheat.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)cheat::cheatThread, hModule, NULL, NULL);
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

