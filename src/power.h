#ifndef POWER_H
#define POWER_H

#include <windows.h>

void prevent_sleep();
void allow_sleep();
BOOL is_fullscreen_window();
void check_fullscreen_state();

#endif 