#include "timer.h"
#include <windows.h>
#include <stdio.h>
#include "power.h"

#define ID_TIMER 1

static char timeStr[128];

void start_timer(TimerContext* ctx, HWND window, int minutes) {
    if (ctx->timerId) {
        KillTimer(window, ctx->timerId);
    }
    
    ctx->isActive = TRUE;
    ctx->remainingSeconds = minutes * 60;
    ctx->hwnd = window;
    ctx->endTime = GetTickCount() + (minutes * 60 * 1000);
    ctx->timerId = SetTimer(window, ID_TIMER, 1000, NULL);
    prevent_sleep();
}

void stop_timer(TimerContext* ctx, HWND window) {
    if (ctx->timerId) {
        KillTimer(window, ctx->timerId);
        ctx->timerId = 0;
    }
    ctx->isActive = FALSE;
    ctx->remainingSeconds = 0;
    allow_sleep();
}

void update_timer_status(TimerContext* ctx) {
    if (ctx->isActive && GetTickCount() >= ctx->endTime) {
        ctx->isActive = FALSE;
        allow_sleep();
    }
}

const char* get_remaining_time_str(TimerContext* ctx) {
    if (!ctx->isActive) {
        return "KeepAlive - Inactive";
    }

    DWORD remaining = (ctx->endTime - GetTickCount()) / 1000;
    int hours = remaining / 3600;
    int minutes = (remaining % 3600) / 60;
    int seconds = remaining % 60;
    if (hours > 0) {
        if (minutes > 0) {
            sprintf_s(timeStr, sizeof(timeStr), "KeepAlive - %d hours %d minutes remaining", hours, minutes);
        } else {
            sprintf_s(timeStr, sizeof(timeStr), "KeepAlive - %d hours remaining", hours);
        }
    } else {
        sprintf_s(timeStr, sizeof(timeStr), "KeepAlive - %02d:%02d remaining", minutes, seconds);
    }
    return timeStr;
} 