#pragma once

#define NDEBUG
#include "common/def.h"

#define TYPE_PARAMETER_SPECIFIER_MAX 3
#define MAX_NAME_LEN 50
#define MAX_TYPE_LEN 50

typedef struct {
  const char name[MAX_NAME_LEN];
  const char type[TYPE_PARAMETER_SPECIFIER_MAX][MAX_TYPE_LEN];
  u32        type_len;
} Field;

typedef struct {
  const char name[MAX_NAME_LEN];
  u32        instance_count;
} TDefineField;


VEC_IMPLEMENT(Field);
VEC_IMPLEMENT(TDefineField);

typedef struct {
  const char *name;
  const char *return_type;
  VecField    parameters;
} TFunctionInfo;

typedef struct {
  VecField fields;
  char     name[MAX_NAME_LEN];
} TStructInfo;

typedef struct {
  char         name[MAX_NAME_LEN];
  TDefineField fields;
  const char  *body;
  u32          body_len;
} TDefineInfo;

typedef struct {
  const char file_name[256];
  char      *data;
  u32        len;
} TIncludeInfo;

VEC_IMPLEMENT(TStructInfo);
VEC_IMPLEMENT(TFunctionInfo);
VEC_IMPLEMENT(TDefineInfo);
VEC_IMPLEMENT(TIncludeInfo);


typedef struct {
  VecTFunctionInfo function_infos;
  VecTStructInfo   structs_infos;
} HeaderInfo;

VEC_IMPLEMENT(HeaderInfo);

typedef struct {
  VecHeaderInfo   headers;
  VecTIncludeInfo includes;
  VecTDefineInfo  all_defines; // TODO, have mutex associated with it
  const char     *root_file_name;
} TRootFolder;
