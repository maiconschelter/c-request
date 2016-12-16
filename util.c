#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <string.h>

#include "util.h"

#pragma comment(lib, "Ws2_32.lib")

void logger(char *text)
{
	FILE	*fileini;

	fileini = fopen("request.log", "a+");
	fwrite(text, strlen(text), 1, fileini);
	fputc(10, fileini);
	fwrite("-------------", strlen("-------------"), 1, fileini);
	fputc(10, fileini);
	fputc(10, fileini);
	fputc(10, fileini);
	fclose(fileini);
}