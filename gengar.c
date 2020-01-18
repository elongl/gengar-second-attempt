#include <Windows.h>

#include "agent.h"
#include "client.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine,
                   int nShowCmd) {
  startclient();
  startagent();
}