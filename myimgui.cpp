#include "myimgui.h"
#include <Windows.h>

// Creates an overlay window with ImGui rendering
// Parameters:
// className - Window class name to hook to
// titleName - Window title to hook to  
// callback - Function to render ImGui UI
// fontPath - Path to font file
// fontSize - Font size to use
// vsync - Enable vertical sync
// style - 0 for dark theme, 1 for light theme
int myimgui::CreateWindow_Violet(LPCSTR className, LPCSTR titleName, myimgui::myFun callback, 
                                const char* fontPath, float fontSize, bool vsync, int style)
{
    gamewindow.ClassName = className;
    gamewindow.TitleName = titleName;

    // Register window class
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), 
                       nullptr, nullptr, nullptr, nullptr, L"Violet", nullptr };
    ::RegisterClassExW(&wc);

    // Create transparent overlay window
    mywindow.hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        wc.lpszClassName, L"ImGui Overlay", WS_POPUP, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!myimgui::CreateDeviceD3D(mywindow.hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(mywindow.hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(mywindow.hwnd);

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     

    // Set theme
    style == 0 ? ImGui::StyleColorsDark() : ImGui::StyleColorsLight();
    
    // Initialize ImGui backends
    ImGui_ImplWin32_Init(mywindow.hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load font
    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, fontSize, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    ImVec4 clearColor = ImVec4(0.f, 0.f, 0.f, 0.f);
    bool done = false;
    
    // Start cheat thread
    std::thread cheatThread(作弊线程1);

    // Main loop
    while (!done)
    {
        // Handle Windows messages
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            exit(0);

        // Update window position/size
        UpdateWindow();

        // Handle window resize
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        // Execute callback to render UI
        callback(); 
        
        // Render ImGui
        ImGui::Render();
        const float clearColorWithAlpha[4] = { clearColor.x * clearColor.w, clearColor.y * clearColor.w, 
                                             clearColor.z * clearColor.w, clearColor.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColorWithAlpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present frame
        g_pSwapChain->Present(vsync ? 1 : 0, 0);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(mywindow.hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}

// Updates overlay window position and size to match target window
void myimgui::UpdateWindow()
{
    POINT point{};
    RECT rect{};

    // Find target window
    gamewindow.hwnd = FindWindowA(gamewindow.ClassName, gamewindow.TitleName);

    // Get window position and size
    GetClientRect(gamewindow.hwnd, &rect);
    ClientToScreen(gamewindow.hwnd, &point);

    // Update overlay window
    mywindow.pos = gamewindow.pos = ImVec2((float)point.x, (float)point.y);
    mywindow.size = gamewindow.size = ImVec2((float)rect.right, (float)rect.bottom);
    SetWindowPos(mywindow.hwnd, HWND_TOPMOST, (int)mywindow.pos.x, (int)mywindow.pos.y, 
                (int)mywindow.size.x, (int)mywindow.size.y, SWP_SHOWWINDOW);

    // Update ImGui mouse position
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(mywindow.hwnd, &mousePos);
    ImGui::GetIO().MousePos.x = (float)mousePos.x;
    ImGui::GetIO().MousePos.y = (float)mousePos.y;

    // Handle mouse click-through
    if (ImGui::GetIO().WantCaptureMouse)
    {
        // Disable click-through when mouse is over UI
        SetWindowLong(mywindow.hwnd, GWL_EXSTYLE, GetWindowLong(mywindow.hwnd, GWL_EXSTYLE) & (~WS_EX_LAYERED));
    }
    else
    {
        // Enable click-through when mouse is not over UI
        SetWindowLong(mywindow.hwnd, GWL_EXSTYLE, GetWindowLong(mywindow.hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    }
}

// Initialize Direct3D device and swap chain
bool myimgui::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    // Try to create hardware device
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

    // Fall back to WARP device if hardware is not available
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

// Clean up Direct3D resources
void myimgui::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

// Create render target view
void myimgui::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

// Clean up render target
void myimgui::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Win32 window procedure
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI myimgui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
    {
        MARGINS margin = { -1 };
        DwmExtendFrameIntoClientArea(hWnd, &margin);
        break;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
