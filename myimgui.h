#pragma once
#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"
#include "Tools.h"
#include "ESP.h"

#include <iostream>
#include <Windows.h>
#include <thread>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

namespace myimgui {

    // Function pointer type for UI callback
    typedef void (*UICallback)(void);

    // Structure to store window information
    struct WindowInfo {
        HWND hwnd = nullptr;           // Window handle
        LPCSTR className = nullptr;    // Window class name 
        LPCSTR titleName = nullptr;    // Window title
        ImVec2 position = {0,0};       // Window position
        ImVec2 size = {0,0};          // Window size
    };

    // Global window instances
    inline WindowInfo overlayWindow;    // Overlay window info
    inline WindowInfo targetWindow;     // Target game window info

    // DirectX resources
    inline static ID3D11Device* g_pd3dDevice = nullptr;
    inline static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    inline static IDXGISwapChain* g_pSwapChain = nullptr;
    inline static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
    inline static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    // Creates an overlay window with ImGui rendering
    // Parameters:
    // className - Window class name to hook to
    // titleName - Window title to hook to
    // callback - Function to render ImGui UI
    // fontPath - Path to font file
    // fontSize - Font size to use
    // vsync - Enable vertical sync
    // style - 0 for dark theme, 1 for light theme
    int CreateWindow_Violet(LPCSTR className, LPCSTR titleName, UICallback callback, 
                          const char* fontPath, float fontSize, bool vsync, int style);

    void UpdateWindow();               // Updates overlay window position and size
    bool CreateDeviceD3D(HWND hWnd);  // Initialize Direct3D device
    void CleanupDeviceD3D();          // Cleanup Direct3D resources
    void CreateRenderTarget();         // Create render target view
    void CleanupRenderTarget();        // Cleanup render target view
    
    // Win32 window procedure
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
