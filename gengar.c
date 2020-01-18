#define _WIN32_WINNT 0x501

#include <Windows.h>

#include "agent.h"
#include "client.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine,
                   int nShowCmd) {
  start_client();
  start_agent();
}