#include <Windows.h>
#include <thread>
#include <MinHook.h>
#include "hook.h"

DWORD WINAPI EntryThread(LPVOID hModule)
{
    DisableThreadLibraryCalls((HMODULE)hModule);

    if (MH_Initialize() != MH_OK)
        return 1;

    Base::Hook::SetupHooks();

    while (!GetAsyncKeyState(VK_END))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Base::Hook::CleanupHooks();
    MH_Uninitialize();

    FreeLibraryAndExitThread((HMODULE)hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, EntryThread, hModule, 0, nullptr);
    return TRUE;
}