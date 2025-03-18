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
} Parameter;
VEC_IMPLEMENT(Parameter);

typedef struct {
  const char *name;
  const char *return_type;

  VecParameter parameters;
} TFunctionInfo;

typedef struct {
  const char *name[MAX_NAME_LEN];
  const char *field[MAX_TYPE_LEN];
} TStructField;
VEC_IMPLEMENT(TStructField);

typedef struct {
  VecTStructField fields;
  char           *name;
} TStruct;

VEC_IMPLEMENT(TStruct);

typedef struct {
  const char file_name[256];
  char      *data;
  u32        len;
} TInclude;

VEC_IMPLEMENT(TInclude);
VEC_IMPLEMENT(TFunctionInfo);

typedef struct {
  VecTFunctionInfo function_infos;
  VecTStruct       structs_infos;
} HeaderInfo;

VEC_IMPLEMENT(HeaderInfo);

typedef struct {
  VecHeaderInfo headers;
  VecTInclude   includes;
  const char   *root_file_name;
} TRootFolder;