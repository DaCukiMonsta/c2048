#pragma once
#ifndef GETCH_RAW_H
#define GETCH_RAW_H

#ifdef _WIN32
#include <conio.h>

#define KEY_ARROW_ESCAPE -32
#define KEY_ARROW_ESCAPE_NUMPAD 0

#define KEY_ARROW_UP 72
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_RIGHT 77

#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

#include <termios.h>

#define KEY_ARROW_ESCAPE 27
#define KEY_ARROW_TWO_ESCAPES

#define KEY_ARROW_UP 65
#define KEY_ARROW_LEFT 68
#define KEY_ARROW_DOWN 66
#define KEY_ARROW_RIGHT 67

#else

#error "System is not supported."

#endif

char getch_raw(void);
void init_getch_raw(void);

#endif // GETCH_RAW_H