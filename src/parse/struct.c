#include <regex.h>
#include <stdio.h>

#include "internal.h"
#include "parse/def.h"

typedef enum {
  STRUCT_MATCH_NAME_1 = 1,
  STRUCT_MATCH_FIELDS,
  STRUCT_MATCH_NAME_2,
  STRUCT_MATCH_MAX,
} StructMatch;


void
s_print_function_struct(TStruct *struct_info)
{
  VecField *params = &struct_info->fields;
  printf("FunctionName: %s\n", struct_info->name);

  if(params->len > 0) {
    for(u32 i = 0; i < params->len; i++) {
      printf("Parameter %d: ", i);
      for(u32 t = 0; t < params->data[i].type_len; t++) {
        printf("%.*s ", MAX_TYPE_LEN, params->data[i].type[t * MAX_TYPE_LEN]);
      }
      printf("%s\n", params->data[i].name);
    }
  }
  printf("\n\n");
}
void
s_parse_struct_from_string(const char *content, VecTStruct *structs)
{
  regex_t    regex;
  regmatch_t matches[10] = { 0 };
  TStruct    temp_struct = { 0 };

  VEC_INIT((*structs), sizeof(TStruct), 10);


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
      VEC_ADD((*structs), temp_struct);
      TStruct *struct_info = &structs->data[structs->len - 1];

      if(matches[STRUCT_MATCH_NAME_1].rm_eo != matches[STRUCT_MATCH_NAME_1].rm_so) {
        u32 name_len = matches[STRUCT_MATCH_NAME_1].rm_eo - matches[STRUCT_MATCH_NAME_1].rm_so;
        strncpy(struct_info->name, &cursor[matches[STRUCT_MATCH_NAME_1].rm_so], name_len);
      } else if(matches[STRUCT_MATCH_NAME_2].rm_eo != matches[STRUCT_MATCH_NAME_2].rm_so) {
        u32 name_len = matches[STRUCT_MATCH_NAME_2].rm_eo - matches[STRUCT_MATCH_NAME_2].rm_so;
        strncpy(struct_info->name, &cursor[matches[STRUCT_MATCH_NAME_2].rm_so], name_len);
      }
      u32 param_len = matches[STRUCT_MATCH_FIELDS].rm_eo - matches[STRUCT_MATCH_FIELDS].rm_so;
      if(param_len > 0) {
        Field field = { 0 };
        char  buffer[param_len];
        memset(buffer, 0, param_len);

        char *save_ptr_top = NULL;
        char *token        = __strtok_r(buffer, ";", &save_ptr_top);

        while(token != NULL) {
          char *save_ptr_inner = NULL;
          char *sub_token      = __strtok_r(token, " ", &save_ptr_inner);
          u32   type_index     = 0;
          char  sub_words[TYPE_PARAMETER_SPECIFIER_MAX * MAX_TYPE_LEN + MAX_NAME_LEN] = { 0 };

          while(sub_token != NULL) {
            u32 token_len = strlen(sub_token);
            if(token_len == 1 && sub_token[0] == '\n') {
              sub_token = __strtok_r(NULL, " ", &save_ptr_inner);
              continue;
            }

            strncpy((char *)&sub_words[type_index * MAX_TYPE_LEN], sub_token, token_len);
            sub_token = __strtok_r(NULL, " ", &save_ptr_inner);
            type_index++;
          }
          type_index--;
          field.type_len = type_index;
          u32 type_len   = MAX_TYPE_LEN * field.type_len;
          strncpy((char *)field.type, (char *)sub_words, type_len);
          strncpy((char *)field.name, (char *)&sub_words[type_len], MAX_NAME_LEN);

          token = __strtok_r(NULL, ",", &save_ptr_top);

          VEC_ADD(struct_info->fields, field);
        }
      }

      cursor += matches[0].rm_eo;
      s_print_function_struct(struct_info);
    }
  }
  // Clean up
  regfree(&regex);
}
