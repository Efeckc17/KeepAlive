#ifndef TRAY_H
#define TRAY_H

#include <windows.h>
#include <shellapi.h>

#define WM_TRAYICON (WM_USER + 1)

#define ID_START 1
#define ID_30MIN 2
#define ID_1HOUR 3
#define ID_2HOURS 4
#define ID_4HOURS 5
#define ID_8HOURS 6
#define ID_STOP 7
#define ID_EXIT 8

#define TRAY_ICON_TITLE "KeepAlive"

typedef struct {
    NOTIFYICONDATA nid;
    HMENU hMenu;
    HWND hwnd;
    BOOL isActive;
} TrayContext;

void init_tray(TrayContext* ctx, HINSTANCE hInst);
void show_tray_menu(TrayContext* ctx);
void update_tray_tip(TrayContext* ctx, const char* tip);
void cleanup_tray(TrayContext* ctx);
void update_menu_state(TrayContext* ctx, BOOL isActive);
void update_status_text(TrayContext* ctx, const char* status);

#endif 