#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

void init_winsock()
{
    int status_code;
    WSADATA wsaData;
    status_code = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (status_code != 0)
    {
        printf("WSAStartup failed: %d\n", status_code);
        exit(1);
    }
}