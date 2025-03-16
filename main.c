#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <stdio.h>
#include <string.h>

#include <regex.h>
#include <stdlib.h>

#define NDEBUG
#include "def.h"
typedef struct {
  const char *name;
  const char type[50];
} Parameter;
VEC_IMPLEMENT(Parameter);

typedef struct {
  const char *name;
  const char *return_type;

  VecParameter parameters;
} TFunctionInfo;

VEC_IMPLEMENT(TFunctionInfo);

typedef struct {
  VecTFunctionInfo function_infos;
} HeaderInfo;

CXCursor root_cursor = {0};
const char *FILE_NAME = "example/bindgentest.h";

void header_print(HeaderInfo *header) {
  for (u32 i = 0; i < header->function_infos.len; i++) {
    TFunctionInfo *func_info = &header->function_infos.data[i];
    // returntype name(
    printf("%s %s(", func_info->return_type, func_info->name);

    for (u32 p = 0; p < func_info->parameters.len; p++) {
      Parameter *param = &func_info->parameters.data[p];
      printf("%s %s", param->type, param->name);
    }
    printf(")\n\n");
  }
}

void parse_function_info(CXCursor cursor, TFunctionInfo *out_func) {
  CXString func_name = clang_getCursorSpelling(cursor);
  CXType funcType = clang_getCursorType(cursor);
  CXType returnType = clang_getResultType(funcType);

  int numArgs = clang_Cursor_getNumArguments(cursor);
  VEC_INIT(out_func->parameters, sizeof(Parameter), numArgs + 1);

  for (int i = 0; i < numArgs; i++) {
    CXCursor argCursor = clang_Cursor_getArgument(cursor, i);
    CXString argName = clang_getCursorSpelling(argCursor);
    CXType argType = clang_getCursorType(argCursor);

    Parameter param = {0};
    VEC_ADD(out_func->parameters, param);

    Parameter *parameter = &out_func->parameters.data[i];

    parameter->name = clang_getCString(argName);
    strcpy((char *)parameter->type,
           clang_getCString(clang_getTypeSpelling(argType)));
  }
  out_func->name = clang_getCString(func_name);
  out_func->return_type = clang_getCString(clang_getTypeSpelling(funcType));
}

int include_count = 0;

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data) {

  HeaderInfo *info = client_data;

  enum CXCursorKind kind = clang_getCursorKind(cursor);

  switch (kind) {

  case CXCursor_FunctionDecl:
    TFunctionInfo func_info = {0};
    VEC_ADD(info->function_infos, func_info);
    TFunctionInfo *out_func_info =
        &info->function_infos.data[info->function_infos.len - 1];

    parse_function_info(cursor, out_func_info);
    break;

  case CXCursor_TypedefDecl:
    CXString typedef_name = clang_getCursorSpelling(cursor);

    // Get the underlying type of the typedef
    CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);

    // Get the spelling of the underlying type (its name)
    CXString type_spelling = clang_getTypeSpelling(underlying_type);

    printf("typedef %s %s\n", clang_getCString(type_spelling),
           clang_getCString(typedef_name));

    clang_disposeString(typedef_name);
    clang_disposeString(type_spelling);
  }
  return CXChildVisit_Continue;
}

void parse_header(const char *filename) {

  const char *args[] = {"-I", "example"};

  CXIndex index = clang_createIndex(1, 1);
  CXTranslationUnit tu = {0};
  clang_defaultEditingTranslationUnitOptions();
  enum CXErrorCode error = clang_parseTranslationUnit2(
      index, filename, args, sizeof(args) / sizeof(args[0]), NULL, 0,
      CXTranslationUnit_None, &tu);

  if (error != CXError_Success || tu == NULL) {
    fprintf(stderr, "Error parsing translation unit\n");
    clang_disposeIndex(index);
  }

  HeaderInfo info = {0};
  VEC_INIT(info.function_infos, sizeof(TFunctionInfo), 30);
  if (!tu) {
    printf("Failed to parse translation unit\n");
    return;
  }

  root_cursor = clang_getTranslationUnitCursor(tu);
  clang_visitChildren(root_cursor, visitor, &info);
  header_print(&info);
  clang_disposeTranslationUnit(tu);
  clang_disposeIndex(index);
}

#define BUFFER_SIZE 1024

// Function to read the entire content of a file into a single string
char *read_file_content(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Could not open file");
    return NULL;
  }

  // Seek to the end of the file to get the size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the file content
  char *content = (char *)malloc(file_size + 1);
  if (!content) {
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

// Function to get function declarations from the file content
void get_functions_from_content(const char *content) {
  regex_t regex;
  regmatch_t matches[1];

  // Regular expression to match a simple function declaration (multi-line
  // aware) Matches function declarations like: return_type
  // function_name(params);
  const char *pattern =
      "([a-zA-Z_][a-zA-Z0-9_]*\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*\\([^)]*\\))";

  // Compile the regex
  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    perror("Could not compile regex");
    return;
  }

  // Search for function declarations in the content
  const char *cursor = content;
  while (regexec(&regex, cursor, 1, matches, 0) == 0) {
    // Print the matched function declaration
    printf("Found function: ");
    printf("%.*s\n", matches[0].rm_eo - matches[0].rm_so,
           cursor + matches[0].rm_so);

    // Move the cursor forward in the content to search for the next match
    cursor += matches[0].rm_eo;
  }

  // Clean up
  regfree(&regex);
}

int main() {
  const char *filename = "example/resource.h"; // Change this to your file path

  // Read the file content into a single string
  char *file_content = read_file_content(filename);
  if (!file_content) {
    return -1;
  }

  // Get functions from the file content
  get_functions_from_content(file_content);

  // Free the file content memory
  free(file_content);

  return 0;
}