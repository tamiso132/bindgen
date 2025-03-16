#include <stdlib.h>
#include <stdio.h>

#include "pub.h"

char *
s_read_file_content(const char *filename, u32 *len)
{
  char full_path[256] = { 0 };
  sprintf(full_path, "example/%s", filename);
  FILE *file = fopen(full_path, "r");
  if(!file) {
    perror("Could not open file");
    return NULL;
  }

  // Seek to the end of the file to get the size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);

  if(len != NULL) {
    *len = file_size;
  }
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the file content
  char *content = (char *)malloc(file_size + 1);
  if(!content) {
    perror("Could not allocate memory for file content");
    fclose(file);
    return NULL;
  }

  // Read the file content into the buffer
  fread(content, 1, file_size, file);
  content[file_size] = '\0'; // Null-terminate the string

  fclose(file);
  return content;
}
