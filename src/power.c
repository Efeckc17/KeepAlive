#include "power.h"

static BOOL was_fullscreen = FALSE;

void prevent_sleep() {
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
}

void allow_sleep() {
    SetThreadExecutionState(ES_CONTINUOUS);
}

BOOL is_fullscreen_window() {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return FALSE;

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    return (windowWidth >= screenWidth && windowHeight >= screenHeight) || IsZoomed(hwnd);
}

void check_fullscreen_state() {
    BOOL is_fullscreen = is_fullscreen_window();
    
    if (is_fullscreen && !was_fullscreen) {
        prevent_sleep();
    } else if (!is_fullscreen && was_fullscreen) {
        allow_sleep();
    }
    
    was_fullscreen = is_fullscreen;
} 