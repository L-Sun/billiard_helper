#include "ImageAnalyser.hpp"

#include "Application.hpp"

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <array>

int Application::Initialize() {
    // high dpi
    SetProcessDPIAware();

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEX wc    = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"BilliardHelper";
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.lpfnWndProc   = WindowProc;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    RegisterClassEx(&wc);

    m_Window = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        L"BilliardHelper",
        L"BilliardHelper",
        WS_POPUP,
        0,
        0,
        0,
        0,
        nullptr,
        nullptr,
        hInstance,
        this);

    SetLayeredWindowAttributes(m_Window, RGB(255, 255, 255), 0, LWA_COLORKEY);

    if (m_Window == nullptr) {
        std::cerr << "Create window fail." << std::endl;
        return 1;
    }

    ShowWindow(m_Window, SW_SHOW);
    // Activate key
    RegisterHotKey(m_Window, m_HotKeyD, MOD_ALT, 'D');
    RegisterHotKey(m_Window, m_HotKeyF, MOD_ALT, 'F');

    m_Init = true;
    return 0;
}

void Application::Tick() {
    assert(m_Init);

    MSG msg;
    if (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Application::Finalize() {
    assert(m_Init);
}

void Application::UpdateCapture(HWND hwnd) {
    if (!hwnd) return;

    // get the device context of the screen
    HDC hDC = GetDC(hwnd);
    // and a device context to put it in
    HDC hMemoryDC = CreateCompatibleDC(hDC);

    RECT rc = {};
    GetClientRect(hwnd, &rc);
    int width    = rc.right - rc.left;
    int height   = rc.bottom - rc.top;
    int bitCount = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    assert(bitCount == 32 && "bit count is not 32!");

    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);
    if (!hBitmap) {
        DeleteObject(hBitmap);
        DeleteDC(hDC);
        DeleteDC(hMemoryDC);

        throw std::runtime_error("Create compatible bitmap fail!");
    }

    // get a new bitmap
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));

    if (!BitBlt(hMemoryDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY)) {
        DeleteObject(hBitmap);
        DeleteDC(hMemoryDC);
        DeleteDC(hDC);

        throw std::runtime_error("BitBlt has failed");
    }

    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));

    BITMAP bmpScreen;
    // Get the BITMAP from the HBITMAP
    GetObject(hBitmap, sizeof(BITMAP), &bmpScreen);

    // the size of color infomation
    auto dataSize = ((bmpScreen.bmWidth * bitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
    // size of data and header
    auto headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // resize screen buffer
    m_DeskImage.resize(dataSize + headerSize);
    auto buffer = m_DeskImage.data();

    // update bit map file header
    auto bmfHeader       = reinterpret_cast<BITMAPFILEHEADER*>(buffer);
    bmfHeader->bfOffBits = headerSize;
    bmfHeader->bfSize    = dataSize + headerSize;
    bmfHeader->bfType    = 0x4D42;

    // update bitmap info header
    auto bi             = reinterpret_cast<BITMAPINFOHEADER*>(buffer + sizeof(BITMAPFILEHEADER));
    bi->biSize          = sizeof(BITMAPINFOHEADER);
    bi->biWidth         = bmpScreen.bmWidth;
    bi->biHeight        = bmpScreen.bmHeight;
    bi->biPlanes        = 1;
    bi->biBitCount      = bitCount;
    bi->biCompression   = BI_RGB;
    bi->biSizeImage     = 0;
    bi->biXPelsPerMeter = 0;
    bi->biYPelsPerMeter = 0;
    bi->biClrUsed       = 0;
    bi->biClrImportant  = 0;

    // copy data to buffer
    GetDIBits(hDC, hBitmap, 0, bmpScreen.bmHeight, buffer + headerSize, reinterpret_cast<BITMAPINFO*>(bi), DIB_RGB_COLORS);

    // clean up
    DeleteObject(hBitmap);
    ReleaseDC(nullptr, hDC);
    DeleteDC(hMemoryDC);
}

Vec2 Application::GetCursor() const {
    RECT rc = {};
    GetWindowRect(m_Window, &rc);
    POINT p;
    return GetCursorPos(&p) ? Vec2{
                                  static_cast<double>(p.x - rc.left),
                                  static_cast<double>(p.y - rc.top)}
                            : Vec2{-1, -1};
}

HWND Application::GetDeskWindow() const {
    auto result = FindWindowEx(nullptr, nullptr, L"#32770", nullptr);
    RECT rc;
    GetClientRect(result, &rc);
    return rc.right > 1000 ? result : nullptr;
}

Application& Application::DrawLines(const std::vector<Vec2>& lines) {
    if (lines.size() <= 1) return *this;

    InvalidateRect(m_Window, nullptr, true);

    PAINTSTRUCT ps{};
    HDC         hdc = BeginPaint(m_Window, &ps);

    RECT rc{};
    GetClientRect(m_Window, &rc);

    HPEN    hPen    = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_HIGHLIGHT));
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    for (size_t i = 1; i < lines.size(); i++) {
        MoveToEx(hdc, lines[i - 1].x, lines[i - 1].y, nullptr);
        LineTo(hdc, lines[i].x, lines[i].y);
    }

    if (hOldPen)
        SelectObject(hdc, hOldPen);
    if (hPen)
        DeleteObject(hPen);

    EndPaint(m_Window, &ps);

    return *this;
}
Application& Application::MoveCursor(const Vec2& pos) {
    RECT rc = {};
    GetWindowRect(m_Window, &rc);

    SetForegroundWindow(m_Window);
    SetActiveWindow(m_Window);
    SetFocus(m_Window);
    SetCursorPos(pos.x + rc.left + 0.5, pos.y + rc.top + 0.5);
    return *this;
}

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Application* pThis;
    if (message == WM_NCCREATE) {
        pThis = static_cast<Application*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))) {
            if (GetLastError() != 0) return false;
        }
    } else {
        pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    PAINTSTRUCT ps;
    HDC         hdc;
    RECT        rect;
    switch (message) {
        case WM_HOTKEY: {
            if (pThis->IsBilliardRun()) {
                auto deskWindow = pThis->GetDeskWindow();
                pThis->UpdateCapture(deskWindow);
                RECT rc = {};
                GetWindowRect(deskWindow, &rc);
                SetWindowPos(hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
                if (pThis->m_CallBackFunc.count(wParam) == 1) {
                    pThis->m_CallBackFunc.at(wParam)();
                }
            }
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
            pThis->m_Quit = true;
            return 0;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
