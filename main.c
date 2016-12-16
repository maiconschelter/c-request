#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "util.h"
#include "request.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
	char					hostname[256], protocol[256], aux[256];
	char					*sprotocol, *address, *params, *response;
	unsigned char	*addrutil, *sutil;
	uint16_t			portno;

	if(argv[1])
		strcpy(hostname, argv[1]);
	else
		strcpy(hostname, "localhost");

	if(argv[2])
		portno = strtol(argv[2], NULL, 0);
	else
		portno = 80;

	if((strstr(hostname, "http")) == NULL)
	{
		strcpy(aux, hostname);
		strcpy(hostname, "http://");
		strcat(hostname, aux);
	}
	strcpy(protocol, hostname);
	sprotocol = strtok((char *)&protocol, ":");
	if(sprotocol == NULL)
	{
		sprotocol = "http";
	}

	if(!argv[2] && (strstr(sprotocol, "https")) != NULL)
	{
		portno = 443;
	}

	sprintf(sprotocol, "%s://", sprotocol);
	addrutil = strtok((char *)&hostname, "/");
	if(addrutil != NULL)
	{
		addrutil = strtok(NULL, "/");
		if(addrutil != NULL)
		{
			address = (char *)malloc(256);
			*address++ = '\0';
			strcpy(address, addrutil);
		}
	}
	if(strlen(address) > 0)
	{
		params = (char *)malloc(256);
		*params++ = '\0';
		while(sutil != NULL)
		{
			sutil = strtok(NULL, "/");
			if(sutil != NULL)
			{
				strcat(params, "/");
				strcat(params, sutil);
			}
		}
	}

	response = do_get(address, params, portno);
	logger(response);

	fprintf(stdout, "%s", response);
	fflush(stdout);
	fflush(stdout);

	free(response);

	return 0;
}