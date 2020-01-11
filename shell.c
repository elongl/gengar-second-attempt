#include <Windows.h>
#include <stdio.h>

HANDLE out_rd = NULL;
HANDLE out_wr = NULL;

char* ReadFromPipe() {
  int bufsize = 4096;
  char buf[bufsize];
  DWORD dwRead;
  BOOL bSuccess = FALSE;

  while (1) {
    bSuccess = ReadFile(out_rd, buf, bufsize, &dwRead, NULL);
    if (!bSuccess || dwRead == 0) break;
  }
  return buf;
}

char* run(char* cmd) {
  int res;
  char* cli_cmd = "/c ";
  PROCESS_INFORMATION procinfo;
  SECURITY_ATTRIBUTES secattr;

  CreatePipe(&out_rd, &out_wr, &secattr, 0);
  STARTUPINFO startupinfo = {.hStdOutput = out_wr, .hStdError = out_wr};

  strcat(cli_cmd, cmd);
  res = CreateProcessA("cmd.exe", cli_cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW,
                       NULL, "C:\\", &startupinfo, &procinfo);
  if (res == 0) {
    printf("CreateProcess failed: %d\n", GetLastError());
    exit(1);
  } else {
    CloseHandle(procinfo.hProcess);
    CloseHandle(procinfo.hThread);
    return ReadFromPipe();
  }
}
