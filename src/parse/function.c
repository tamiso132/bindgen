#include <assert.h>
#include <ctype.h>
#include <regex.h>
#include <stdio.h>

#include "pub.h"

enum {
  FUNCTION_TYPE_RETURN_TYPE = 1,
  FUNCTION_TYPE_NAME,
  FUNCTION_TYPE_PARAM,
} FunctionType;


void
s_parse_parameter(const char *cursor, regmatch_t matches[], TFunctionInfo *func_info)
{

  u32       param_len = matches[FUNCTION_TYPE_PARAM].rm_eo - matches[FUNCTION_TYPE_PARAM].rm_so;
  Parameter param     = { 0 };
  char      buffer[param_len + 1];
  strncpy(buffer, &cursor[matches[FUNCTION_TYPE_PARAM].rm_so], param_len);
  buffer[param_len] = '\0';

  char *save_ptr_top = NULL;
  char *token        = __strtok_r(buffer, ",", &save_ptr_top);

  while(token != NULL) {
    char *save_ptr_inner = NULL;

    char *sub_token = __strtok_r(token, " ", &save_ptr_inner);

    u32 last_word_len = 0;
    u32 current_len   = 0;
    while(sub_token != NULL) {
      printf("SubToken: %s\n", sub_token);
      sub_token     = __strtok_r(NULL, " ", &save_ptr_inner);
      u32 token_len = strlen(sub_token);
      strncpy((char *)&param.type[current_len], sub_token, token_len);
      current_len += token_len;
      last_word_len = token_len;
    }

    memset(param.type, 0, last_word_len);
    token = __strtok_r(NULL, ",", &save_ptr_top);
  }
}

void
s_parse_functions_from_string(const char *content, VecTFunctionInfo *function_infos)
{
  regex_t    regex;
  regmatch_t matches[10] = { 0 };

  VEC_INIT((*function_infos), sizeof(TFunctionInfo), 30);
  const char *func_pattern = "([a-zA-Z_][a-zA-Z0-9_]*)(\\s+[a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(([^)]*)\\)\\s*;";

  //\\s+ checks for whitespace 1 or more
  //\\s* checks for whitespace 0 or more
  //(w+)?  optionally matches something


  if(regcomp(&regex, func_pattern, REG_EXTENDED) != 0) {
    perror("Could not compile regex");
    return;
  } else {
    // Search for function declarations in the content
    const char *cursor = content;
    while(regexec(&regex, cursor, 10, matches, 0) == 0) {
      // Print the matched function declaration

      u32 name_byte_len   = matches[FUNCTION_TYPE_NAME].rm_eo - matches[FUNCTION_TYPE_NAME].rm_so;
      u32 return_byte_len = matches[FUNCTION_TYPE_RETURN_TYPE].rm_eo - matches[FUNCTION_TYPE_RETURN_TYPE].rm_so;

      if(matches[FUNCTION_TYPE_NAME].rm_eo == matches[FUNCTION_TYPE_NAME].rm_so) {
        // should be an error
        continue;
      }

      TFunctionInfo func_info = { 0 };
      func_info.name          = calloc(1, name_byte_len + 1);
      func_info.return_type   = calloc(1, return_byte_len + 1);
      VEC_INIT(func_info.parameters, sizeof(Parameter), 10);

      strncpy((char *)func_info.name, &cursor[matches[FUNCTION_TYPE_NAME].rm_so], name_byte_len);
      strncpy((char *)func_info.return_type, &cursor[matches[FUNCTION_TYPE_RETURN_TYPE].rm_so], return_byte_len);

      s_parse_parameter(cursor, matches, &func_info);


      printf("\n");
      // Move the cursor forward in the content to search for the next match
      cursor += matches[0].rm_eo;
    }
  }
  // Clean up
  regfree(&regex);
}