#include <Windows.h>
#include <stdio.h>
#include "shell.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine,
                   int nShowCmd) {
  char* res = run("whoami");
  printf("%s", res);
  free(res);
}