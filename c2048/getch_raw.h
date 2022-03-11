#pragma once
#ifndef GETCH_RAW_H
#define GETCH_RAW_H

#ifdef _WIN32
#include <conio.h>

#define KEY_ARROW_UP 72
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_RIGHT 77
#else
#error "Linux support TODO!"
#endif

char getch_raw();

#endif // GETCH_RAW_H