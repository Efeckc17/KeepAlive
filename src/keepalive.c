#include <windows.h>
#include <commctrl.h>
#include "tray.h"
#include "timer.h"
#include "power.h"

#define WM_TRAYICON (WM_USER + 1)
#define APP_MUTEX_NAME "KeepAliveSingleInstance"
#define FULLSCREEN_CHECK_TIMER 2
#define FULLSCREEN_CHECK_INTERVAL 1000

HINSTANCE hInst;
HWND hwnd;
TrayContext trayCtx;
TimerContext timerCtx;
HANDLE hMutex;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    
    hMutex = CreateMutex(NULL, TRUE, APP_MUTEX_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(NULL, "KeepAlive is already running!", "Warning", MB_ICONWARNING);
        return 1;
    }
    
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "KeepAliveClass";
    RegisterClassEx(&wc);
    
    hwnd = CreateWindowEx(0, "KeepAliveClass", "KeepAlive",
                         WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                         0, 0, NULL, NULL, hInstance, NULL);
    
    trayCtx.hwnd = hwnd;
    init_tray(&trayCtx, hInstance);
    
    SetTimer(hwnd, FULLSCREEN_CHECK_TIMER, FULLSCREEN_CHECK_INTERVAL, NULL);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    cleanup_tray(&trayCtx);
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP) {
                show_tray_menu(&trayCtx);
            }
            break;
            
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_START:
                    start_timer(&timerCtx, hwnd, 30);
                    update_menu_state(&trayCtx, TRUE);
                    break;
                case ID_30MIN:
                    start_timer(&timerCtx, hwnd, 30);
                    update_menu_state(&trayCtx, TRUE);
                    break;
                case ID_1HOUR:
                    start_timer(&timerCtx, hwnd, 60);
                    update_menu_state(&trayCtx, TRUE);
                    break;
                case ID_2HOURS:
                    start_timer(&timerCtx, hwnd, 120);
                    update_menu_state(&trayCtx, TRUE);
                    break;
                case ID_4HOURS:
                    start_timer(&timerCtx, hwnd, 240);
                    update_menu_state(&trayCtx, TRUE);
                    break;
                case ID_8HOURS:
                    start_timer(&timerCtx, hwnd, 480);
                    update_menu_state(&trayCtx, TRUE);
                    break;
                case ID_STOP:
                    stop_timer(&timerCtx, hwnd);
                    update_menu_state(&trayCtx, FALSE);
                    break;
                case ID_EXIT:
                    PostQuitMessage(0);
                    break;
            }
            update_tray_tip(&trayCtx, get_remaining_time_str(&timerCtx));
            update_status_text(&trayCtx, get_remaining_time_str(&timerCtx));
            break;
            
        case WM_TIMER:
            if (wParam == FULLSCREEN_CHECK_TIMER) {
                check_fullscreen_state();
            } else {
                update_timer_status(&timerCtx);
                if (!timerCtx.isActive) {
                    update_menu_state(&trayCtx, FALSE);
                }
                update_tray_tip(&trayCtx, get_remaining_time_str(&timerCtx));
                update_status_text(&trayCtx, get_remaining_time_str(&timerCtx));
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
} 