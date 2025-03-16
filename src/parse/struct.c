#include <regex.h>
#include <stdio.h>

#include "internal.h"

void
s_parse_struct_from_string(const char *content, VecTStruct *structs)
{
  regex_t    regex;
  regmatch_t matches[10] = { 0 };

  VEC_INIT((*structs), sizeof(TStruct), 30);

  //\\s+ checks for whitespace 1 or more
  //\\s* checks for whitespace 0 or more
  //(w+)?  optionally matches something
  const char *struct_pattern = "struct\\s*(\\w*)\\s*\\{([^}]*)\\}\\s*(\\w*)\\s*;";

  if(regcomp(&regex, struct_pattern, REG_EXTENDED) != 0) {
    perror("Could not compile regex");
    return;
  } else {
    // Search for function declarations in the content
    const char *cursor = content;
    while(regexec(&regex, cursor, 10, matches, 0) == 0) {
      // Print the matched function declaration

      for(u32 i = 1; i < 10; i++) {
        if(matches[i].rm_eo == matches[i].rm_so) {
          continue;
        }
        printf("i=%d %.*s\n", i, matches[i].rm_eo - matches[i].rm_so, cursor + matches[i].rm_so);
      }
      printf("\n");
      // Move the cursor forward in the content to search for the next match
      cursor += matches[0].rm_eo;
    }
  }
  // Clean up
  regfree(&regex);
}