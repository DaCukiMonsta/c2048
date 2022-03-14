#include "getch_raw.h"
#include <stdio.h>

char getch_raw(){
#ifdef _WIN32
	return _getch();
#else
	return getchar();
#endif
}

void init_getch_raw(void){
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)) /* only these systems need init */
	/* based on niko's answer on https://stackoverflow.com/a/7469410 */
	struct termios settings;
	tcgetattr(0, &settings); /* grab old terminal i/o settings */
	settings.c_lflag &= ~ICANON; /* disable buffered i/o */
	settings.c_lflag &= ~ECHO; /* set no echo mode */
	tcsetattr(0, TCSANOW, &settings); /* use these new terminal i/o settings now */
#endif
}