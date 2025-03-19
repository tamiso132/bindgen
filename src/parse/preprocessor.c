#include <regex.h>
#include <stdio.h>

#include "internal.h"

#include "parse/def.h"
typedef enum {
  DEFINE_MATCH_PARAMETER,
  DEFINE_MATCH_BODY = 1,
} DefineMatch;

void
s_parse_defines_from_string(const char *content, VecTDefineInfo *define_info)
{
  regex_t     regex;
  regmatch_t  matches[10] = { 0 };
  TDefineInfo temp_struct = { 0 };


  const char define_pattern[]
      = "(#else)|(#endif)|(#if\\s+[a-zA-Z0-9_!]*\\s*(\\([^)]*\\)))|(#define\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*(\\([^)]*"
        "\\))?\\s*(\\s*[^\\\\]*\\\\)*\\s*[^\n]+?)";

  const char ifdef_pattern[] = "(#ifdef\\s+[a-zA-Z0-9_!]*)(\\([^)]\\))?|";

  // Pattern for #ifdef
  const char *pattern_if = "#if\\s+[a-zA-Z0-9_!]*\\s*(\\([^)]*\\))";
  // Pattern for #endif
  const char *pattern_endif = "#endif\\s*";


  if(regcomp(&regex, define_pattern, REG_EXTENDED | REG_NEWLINE) != 0) {
    perror("Could not compile regex");
    return;
  } else {
    // Search for function declarations in the content
    const char *cursor = content;
    while(regexec(&regex, cursor, 10, matches, 0) == 0) {
      printf("%.*s \n\n", matches[0].rm_eo - matches[0].rm_so, &cursor[matches[0].rm_so]);
      cursor += matches[0].rm_eo;
    }
  }
  // Clean up
  regfree(&regex);
}
