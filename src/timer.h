#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

typedef struct {
    BOOL isActive;
    DWORD endTime;
    UINT_PTR timerId;
    int remainingSeconds;
    HWND hwnd;
} TimerContext;

void start_timer(TimerContext* ctx, HWND window, int minutes);
void stop_timer(TimerContext* ctx, HWND window);
void update_timer_status(TimerContext* ctx);
const char* get_remaining_time_str(TimerContext* ctx);

#endif 