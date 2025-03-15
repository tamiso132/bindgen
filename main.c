#include "def.h"
#include <clang-c/Index.h>
#include <stdio.h>



typedef struct{
    const char name;
    const char type;
}Parameter;
VEC_IMPLEMENT(Parameter);


typedef struct{
const char name[100];
const char return_type[50];

VecParameter parameters;
} TFunctionInfo;

VEC_IMPLEMENT(TFunctionInfo);

typedef  struct{
VecTFunctionInfo function_infos;
}HeaderInfo;





void printFunctionInfo(CXCursor cursor) {
    CXString funcName = clang_getCursorSpelling(cursor);
    CXType funcType = clang_getCursorType(cursor);
    CXType returnType = clang_getResultType(funcType);

    printf("Function: %s\n", clang_getCString(funcName));
    printf("  Return Type: %s\n", clang_getCString(clang_getTypeSpelling(returnType)));

    int numArgs = clang_Cursor_getNumArguments(cursor);
    for (int i = 0; i < numArgs; i++) {
        CXCursor argCursor = clang_Cursor_getArgument(cursor, i);
        CXString argName = clang_getCursorSpelling(argCursor);
        CXType argType = clang_getCursorType(argCursor);

        printf("  Param %d: %s (%s)\n", i + 1, clang_getCString(argName),
               clang_getCString(clang_getTypeSpelling(argType)));

        clang_disposeString(argName);
    }

    clang_disposeString(funcName);
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
      HeaderInfo *info = client_data;
    if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
        TFunctionInfo func_info = {0};
        sprintf((char*)func_info.return_type, "void");
        printFunctionInfo(cursor);
    }
    return CXChildVisit_Recurse;
}

void parseHeader(const char *filename) {
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit tu = clang_parseTranslationUnit(index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);
    HeaderInfo info = {0};
    VEC_INIT(info.function_infos, sizeof(TFunctionInfo), 30);
    if (!tu) {
        printf("Failed to parse translation unit\n");
        return;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(cursor, visitor, &info);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
}

int main() {
    parseHeader("test.h");
    return 0;
}