#include "getch_raw.h"
#include <stdio.h>

struct termios old_settings, new_settings;

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
	tcgetattr(0, &old_settings); /* grab old terminal i/o settings */
	new_settings = old_settings; /* copy the old settings into a new one */
	new_settings.c_lflag &= ~ICANON; /* disable buffered i/o */
	new_settings.c_lflag &= ~ECHO; /* set no echo mode */
	tcsetattr(0, TCSANOW, &new_settings); /* use these new terminal i/o settings now */
#endif
}

void end_getch_raw(void){
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)) /* only these systems need quit */
	tcsetattr(0, TCSANOW, &old_settings); /* restore the old terminal settings */
#endif
}