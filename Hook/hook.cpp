#include "hook.h"
#include "renderer.h"
#include <Windows.h>
#include <MinHook.h>

#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
namespace Base::Hook
{
    using PresentFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
    PresentFn oPresent = nullptr;

    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
    {
        static bool initialized = false;
        if (!initialized)
        {
            if (Base::Renderer::InitRenderer(pSwapChain))
                initialized = true;
        }

        Base::Renderer::RenderImGui();
        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    void* GetPresentAddress()
    {
        // 创建一个临时窗口
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L,
                          GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                          L"TempWindow", NULL };
        RegisterClassEx(&wc);
        HWND hWnd = CreateWindow(wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW,
            0, 0, 100, 100, NULL, NULL, wc.hInstance, NULL);

        // 创建 D3D11 设备和 SwapChain
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        ID3D11Device* pDevice = nullptr;
        ID3D11DeviceContext* pContext = nullptr;
        IDXGISwapChain* pSwapChain = nullptr;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION,
            &sd, &pSwapChain, &pDevice, nullptr, &pContext);

        void* pPresent = nullptr;

        if (SUCCEEDED(hr) && pSwapChain)
        {
            // SwapChain 的虚表第8项是 Present
            void** vtable = *reinterpret_cast<void***>(pSwapChain);
            pPresent = vtable[8];

            pSwapChain->Release();
            pDevice->Release();
            pContext->Release();
        }

        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);

        return pPresent;
    }

    void SetupHooks()
    {
        void* pPresent = GetPresentAddress();
        if (pPresent && MH_CreateHook(pPresent, &hkPresent, reinterpret_cast<void**>(&oPresent)) == MH_OK)
        {
            MH_EnableHook(pPresent);
        }
    }

    void CleanupHooks()
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_RemoveHook(MH_ALL_HOOKS);
    }

}
