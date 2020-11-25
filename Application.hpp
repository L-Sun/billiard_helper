#pragma once

#include "Vector.hpp"

#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <future>

class Application {
public:
    int         Initialize();
    void        Tick();
    void        Finalize();
    inline bool Quit() const { return m_Quit; }

    Application& DrawLines(const std::vector<Vec2>& lines);
    Application& DrawLine(const Vec2& from, const Vec2& to) {
        return DrawLines({from, to});
    }
    Application& MoveCursor(const Vec2& pos);

    Vec2 GetCursor() const;

    bool IsBilliardRun() const {
        return GetDeskWindow();
    }

    const std::vector<uint8_t>& GetDeskImage() {
        return m_DeskImage;
    }

    template <typename Func, typename... Args>
    Application& AddCallback(int hotKeyId, Func&& func) {
        m_CallBackFunc.emplace(hotKeyId, std::forward<Func>(func));
        return *this;
    }

private:
    void UpdateCapture(HWND hwnd);
    HWND GetDeskWindow() const;

    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

    bool m_Init    = false;
    bool m_Quit    = false;
    int  m_HotKeyD = 1;
    int  m_HotKeyF = 2;
    HWND m_Window  = 0;

    std::unordered_map<int, std::function<void()>> m_CallBackFunc;

    std::vector<uint8_t> m_DeskImage;
};
