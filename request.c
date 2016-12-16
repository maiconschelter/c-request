#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "request.h"

#pragma comment(lib, "Ws2_32.lib")

char *do_get(char *address, char *params, uint16_t portno)
{
	WSADATA	wsa;
	SOCKET	sc;
	struct		sockaddr_in server;
	struct		hostent *host;
	char			*message, *response, *buffer;
	int			rsize, rtotal = 0, csize = 11644, osize = csize;

	if(params == NULL || strlen(params) <=0)
	{
		params = "/";
	}

	if(portno == 0)
	{
		portno = 80;
	}

	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Error on startup WinLIB %d\r\n", WSAGetLastError());
		return NULL;
	}

	host = gethostbyname(address);
	if(host == NULL)
	{
		printf("Failed to resolve hostname\r\n");
		WSACleanup();
	}

	if((sc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Failed to create socket\r\n");
		return NULL;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(portno);
	server.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)host->h_addr));

	printf("\r\nRequesting to %s...\r\n\r\n", inet_ntoa(*(struct in_addr *)host->h_addr));

	if(connect(sc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Failed to connect on hostname %d\r\n", WSAGetLastError());
		closesocket(sc);
		WSACleanup();
		return NULL;
	}

	message = (char *)malloc(256);
	sprintf(message, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\nCache-Control: no-cache\r\nAccept: */*\r\n\r\n", params, address);

	if(send(sc, message, strlen(message), 0) < 0)
	{
		printf("Failed to send message %d\r\n", WSAGetLastError());
		closesocket(sc);
		WSACleanup();
		return NULL;
	}

	response = (char *)malloc(11644);

	while((rsize = recv(sc, response + rtotal, 11644, 0)) > 0)
	{
		rtotal += rsize;
		osize = csize;
		csize += 11644;
		buffer = (char *)malloc(csize);
		memcpy(buffer, response, osize);
		free(response);
		response = buffer;
	}

	closesocket(sc);
	WSACleanup();

	return response;
}