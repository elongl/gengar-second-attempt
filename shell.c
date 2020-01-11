#include <Windows.h>
#include <stdio.h>

#define CMD_PATH "C:\\Windows\\System32\\cmd.exe"
#define BUFFSIZE 256

HANDLE out_rd = NULL;
HANDLE out_wr = NULL;

void readpipe() {
  int bufsize = 4096;
  char* buf = malloc(bufsize);
  DWORD dwRead;
  BOOL bSuccess = FALSE;

  bSuccess = ReadFile(out_rd, buf, bufsize, &dwRead, NULL);
  printf("Buffer: %s\n", buf);
  CloseHandle(out_rd);
}

char* run(char* cmd) {
  int res;
  char cli_cmd[BUFFSIZE] = "/c ";
  strncat(cli_cmd, cmd, BUFFSIZE);
  SECURITY_ATTRIBUTES secattr = {.bInheritHandle = TRUE};
  res = CreatePipe(&out_rd, &out_wr, &secattr, 0);
  if (res == 0) {
    printf("CreatePipe failed: %d\n", GetLastError());
    exit(1);
  }

  PROCESS_INFORMATION procinfo;
  ZeroMemory(&procinfo, sizeof(PROCESS_INFORMATION));
  STARTUPINFO startupinfo = {.hStdOutput = out_wr,
                             .hStdError = out_wr,
                             .dwFlags = STARTF_USESTDHANDLES};
  res = CreateProcessA(CMD_PATH, cli_cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW,
                       NULL, "C:\\", &startupinfo, &procinfo);
  if (res == 0) {
    printf("CreateProcess failed: %d\n", GetLastError());
    exit(1);
  } else {
    CloseHandle(procinfo.hProcess);
    CloseHandle(procinfo.hThread);
    CloseHandle(out_wr);
    readpipe();
  }
}
