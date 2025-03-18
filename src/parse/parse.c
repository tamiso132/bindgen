#include <regex.h>
#include <stdio.h>

#include "internal.h"
#include "pub.h"


void
parse_header(const char *file_name)
{
  // Read the file content into a single string
  TRootFolder root_folder    = { 0 };
  root_folder.root_file_name = file_name;
  s_root_folder_search_headers(&root_folder);
  for(u32 i = 0; i < root_folder.includes.len; i++) {
    TInclude  *current_child = &root_folder.includes.data[i];
    HeaderInfo header_info   = { 0 };

    current_child->data = s_read_file_content(current_child->file_name, &current_child->len);
    //s_parse_functions_from_string(current_child->data, &header_info.function_infos);
    s_parse_struct_from_string(current_child->data, &header_info.structs_infos);
  }
}

void
s_root_folder_search_headers(TRootFolder *root_folder)
{
  char *content = s_read_file_content(root_folder->root_file_name, NULL);
  if(!content) {
    return;
  }

  regex_t    regex;
  regmatch_t matches[2];
  VEC_INIT(root_folder->includes, sizeof(TInclude), 10);
  const char *include_pattern = "#\\s*include\\s*\"([^\"]+)\""; // Compile the regex

  if(regcomp(&regex, include_pattern, REG_EXTENDED) != 0) {
    perror("Could not compile regex");
    return;
  } else {
    // Search for function declarations in the content
    const char *cursor = content;
    while(regexec(&regex, cursor, 2, matches, 0) == 0) {
      // Print the matched function declaration
      printf("%.*s\n", matches[0].rm_eo - matches[0].rm_so, cursor + matches[0].rm_so);

      int start = matches[1].rm_so;
      int end   = matches[1].rm_eo;


      printf("File: %.*s\n", end - start, cursor + start);

      TInclude include = { 0 };
      strncpy((char *)include.file_name, (cursor + start), end - start);
      VEC_ADD(root_folder->includes, include);

      // Move the cursor forward in the content to search for the next match
      cursor += matches[0].rm_eo;
    }
  }
  free(content);
  content = NULL;
}
