#include "getch_raw.h"

char getch_raw(){
#ifdef _WIN32
	return _getch();
#else
#error "Linux support TODO!"
#endif
}