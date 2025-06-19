#pragma execution_character_set("utf-8")  
#include "renderer.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include"font.h"

#include <Windowsx.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Base::Renderer
{
    
    bool g_ShowMenu = false;
    WNDPROC oWndProc = nullptr;

    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    HWND                    g_hWnd = nullptr;

    // ImGui 输入处理钩子
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {

        // 按下 F5 切换菜单状态（放在前面处理）
        if (msg == WM_KEYUP && wParam == VK_F5) {
            g_ShowMenu = !g_ShowMenu;
            return 0; // 阻止此消息继续传递
        }

        // 菜单开启时，拦截输入
        if (g_ShowMenu)
        {
            // ImGui 自己处理的消息（例如输入框、按钮等）
            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
                return true;

            // 显式拦截所有输入（防止穿透到底层窗口 CS2）
            switch (msg)
            {
            case WM_MOUSEMOVE:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_CHAR:
                return true; // 阻止这些消息继续传递
            }
        }
        // 默认处理
        return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
    }


    HWND GetHWNDFromSwapChain(IDXGISwapChain* pSwapChain)
    {
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        return desc.OutputWindow;
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView) {
            g_mainRenderTargetView->Release();
            g_mainRenderTargetView = nullptr;
        }
    }

    bool InitRenderer(IDXGISwapChain* pSwapChain)
    {
        if (!pSwapChain) return false;
        g_pSwapChain = pSwapChain;
        g_hWnd = GetHWNDFromSwapChain(pSwapChain);

        oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)))
            return false;

        g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

        CreateRenderTarget();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsLight();


        ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(font_data, font_size, 23.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        //font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());//字体设置为微软雅黑


        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        return true;
    }

    void ShutdownRenderer()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupRenderTarget();

        SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
        g_pSwapChain = nullptr;
    }

    void RenderImGui()
    {
        if (!g_ShowMenu) return;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("CS2 Base Menu");
        ImGui::Text("Hello, Counter-Strike 2!");
        ImGui::Text("A journey of a thousand miles begins with a single step. -- Baugle Project");
        ImGui::Text("Press END to unload.");
        ImGui::End();

        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
