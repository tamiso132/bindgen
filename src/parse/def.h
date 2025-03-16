#pragma once

#define NDEBUG
#include "common/def.h"

typedef struct {
  const char name[150];
  const char type[150];
} Parameter;
VEC_IMPLEMENT(Parameter);

typedef struct {
  const char *name;
  const char *return_type;

  VecParameter parameters;
} TFunctionInfo;

typedef struct {
  const char *name[50];
  const char *field[50];
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