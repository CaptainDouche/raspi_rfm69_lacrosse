#ifndef _COLORPRINT_H_
#define _COLORPRINT_H_

#include <errno.h>
#include <string.h>

#define CONSOLE_COLOR_BRIGHTGREEN		"\x1b[32m"
#define CONSOLE_COLOR_BRIGHTRED			"\x1b[31m"
#define CONSOLE_COLOR_BRIGHTBLUE		"\x1b[34m"
#define CONSOLE_COLOR_YELLOW			"\x1b[33m"
#define CONSOLE_COLOR_NORMAL			"\x1b[0m"

#define printf_red(format, ...)			printf(CONSOLE_COLOR_BRIGHTRED 		format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#define printf_grn(format, ...)			printf(CONSOLE_COLOR_BRIGHTGREEN 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#define printf_green(format, ...)		printf(CONSOLE_COLOR_BRIGHTGREEN 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#define printf_blu(format, ...)			printf(CONSOLE_COLOR_BRIGHTBLUE 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#define printf_blue(format, ...)		printf(CONSOLE_COLOR_BRIGHTBLUE 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#define printf_yel(format, ...)			printf(CONSOLE_COLOR_YELLOW 		format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#define printf_yellow(format, ...)		printf(CONSOLE_COLOR_YELLOW 		format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)

void printf_init(void);
#define printf_deinit()
#define GetLastErrorText()				strerror(errno)
	
// http://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
//#define eprintf(...) 					fprintf(stderr, __VA_ARGS__)
//#define eprintf(format, ...)			fprintf (stderr, format, __VA_ARGS__)
//#define eprintf(format, ...)			fprintf (stderr, format, ##__VA_ARGS__)

#define eprintf(format, ...)			fprintf (stderr, CONSOLE_COLOR_BRIGHTRED format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	
#endif
