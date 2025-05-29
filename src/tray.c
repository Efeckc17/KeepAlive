#include "tray.h"
#include "timer.h"
#include <commctrl.h>

#define ID_START 1
#define ID_30MIN 2
#define ID_1HOUR 3
#define ID_2HOURS 4
#define ID_4HOURS 5
#define ID_8HOURS 6
#define ID_STOP 7
#define ID_EXIT 8

void init_tray(TrayContext* ctx, HINSTANCE hInstance) {
    ctx->nid.cbSize = sizeof(NOTIFYICONDATA);
    ctx->nid.hWnd = ctx->hwnd;
    ctx->nid.uID = 1;
    ctx->nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    ctx->nid.uCallbackMessage = WM_TRAYICON;
    ctx->nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(400));
    if (!ctx->nid.hIcon) {
        ctx->nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }
    strncpy_s(ctx->nid.szTip, sizeof(ctx->nid.szTip), TRAY_ICON_TITLE, _TRUNCATE);
    Shell_NotifyIcon(NIM_ADD, &ctx->nid);

    ctx->hMenu = CreatePopupMenu();
    AppendMenu(ctx->hMenu, MF_STRING | MF_GRAYED, 0, "Status: Inactive");
    AppendMenu(ctx->hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(ctx->hMenu, MF_STRING, ID_START, "Start");
    AppendMenu(ctx->hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(ctx->hMenu, MF_STRING, ID_30MIN, "30 minutes");
    AppendMenu(ctx->hMenu, MF_STRING, ID_1HOUR, "1 hour");
    AppendMenu(ctx->hMenu, MF_STRING, ID_2HOURS, "2 hours");
    AppendMenu(ctx->hMenu, MF_STRING, ID_4HOURS, "4 hours");
    AppendMenu(ctx->hMenu, MF_STRING, ID_8HOURS, "8 hours");
    AppendMenu(ctx->hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(ctx->hMenu, MF_STRING | MF_GRAYED, ID_STOP, "Stop");
    AppendMenu(ctx->hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(ctx->hMenu, MF_STRING, ID_EXIT, "Exit");
    
    ctx->isActive = FALSE;
}

void update_menu_state(TrayContext* ctx, BOOL isActive) {
    ctx->isActive = isActive;
    
    EnableMenuItem(ctx->hMenu, ID_START, isActive ? MF_GRAYED : MF_ENABLED);
    EnableMenuItem(ctx->hMenu, ID_30MIN, isActive ? MF_GRAYED : MF_ENABLED);
    EnableMenuItem(ctx->hMenu, ID_1HOUR, isActive ? MF_GRAYED : MF_ENABLED);
    EnableMenuItem(ctx->hMenu, ID_2HOURS, isActive ? MF_GRAYED : MF_ENABLED);
    EnableMenuItem(ctx->hMenu, ID_4HOURS, isActive ? MF_GRAYED : MF_ENABLED);
    EnableMenuItem(ctx->hMenu, ID_8HOURS, isActive ? MF_GRAYED : MF_ENABLED);
    EnableMenuItem(ctx->hMenu, ID_STOP, isActive ? MF_ENABLED : MF_GRAYED);
}

void update_status_text(TrayContext* ctx, const char* status) {
    MENUITEMINFO mii = {0};
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;
    mii.dwTypeData = (LPSTR)status;
    SetMenuItemInfo(ctx->hMenu, 0, TRUE, &mii);
}

void show_tray_menu(TrayContext* ctx) {
    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(ctx->hwnd);
    TrackPopupMenu(ctx->hMenu, TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, ctx->hwnd, NULL);
}

void update_tray_tip(TrayContext* ctx, const char* tip) {
    strcpy_s(ctx->nid.szTip, sizeof(ctx->nid.szTip), tip);
    Shell_NotifyIcon(NIM_MODIFY, &ctx->nid);
}

void cleanup_tray(TrayContext* ctx) {
    Shell_NotifyIcon(NIM_DELETE, &ctx->nid);
    DestroyMenu(ctx->hMenu);
}