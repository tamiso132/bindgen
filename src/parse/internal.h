#pragma once
#include "def.h"

void  s_root_folder_search_headers(TRootFolder *root_folder);
char *s_read_file_content(const char *filename, u32 *len);

void s_parse_functions_from_string(const char *content, VecTFunctionInfo *function_infos);
void s_parse_struct_from_string(const char *content, VecTStruct *structs);
