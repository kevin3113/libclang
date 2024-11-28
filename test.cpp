#include <clang-c/Index.h>
#include <iostream>
#include <functional>
#include <string.h>
#include "func.h"
#include "attr.h"
#include "src.h"
#include "opers.h"

void help(const char *app)
{
    printf("%s <src_file> <oper> [options]\n", app);
    print_options();
}

int main(int argc, char *argv[]) {
  if (argc < 3)
  {
    help(argv[0]);
    return 1;
  }
  int option_cnt = argc - 3 + 2;
  const char **opts = (const char **)malloc(option_cnt * sizeof(const char *));
  opts[0] = "-include";
  opts[1] = "pre_def.h";
  memcpy(opts + 2, &argv[3], (argc - 3) * sizeof(const char *));
  CXIndex index = clang_createIndex(0, 0); //Create index
  CXTranslationUnit unit = clang_parseTranslationUnit(
    index,
    argv[1], opts, option_cnt,
    nullptr, 0,
    CXTranslationUnit_None); //Parse "structs.cpp"

  if (unit == nullptr){
    std::cerr << "Unable to parse translation unit. Quitting.\n";
    return 0;
  }
  CXCursor cursor = clang_getTranslationUnitCursor(unit); //Obtain a cursor at the root of the translation unit
  int ret = execute(argv[2], cursor);
  if (ret)
  {
    printf("oper not correct!\n");
    print_options();
    return 1;
  }
  return 0;
  clang_visitChildren(cursor,
    [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
        AddTilingAttr(current_cursor, parent);
        return CXChildVisit_Recurse;
    }, nullptr);

  auto tilings = GetTilingCX();
  for (auto cursor : tilings)
  {
    print_cursor(cursor); 
    printf("size of = %llu\n", clang_Type_getSizeOf(clang_getCursorType(cursor)));
    GetFileds(cursor);
  }

  int fd = create_src("out.cpp");
  clang_visitChildren(
  cursor,
  [](CXCursor current_cursor, CXCursor parent, CXClientData client_data){
    print_cursor(current_cursor);
    //write_src(*(int *)client_data, current_cursor);
    write_src_without_system_header(*(int *)client_data, current_cursor);
    CXType cursor_type = clang_getCursorType(current_cursor);

    CXString type_kind_spelling = clang_getTypeKindSpelling(cursor_type.kind);
    std::cout << "Type Kind: " << clang_getCString(type_kind_spelling);
    clang_disposeString(type_kind_spelling);

    if(cursor_type.kind == CXType_Pointer ||                     // If cursor_type is a pointer
      cursor_type.kind == CXType_LValueReference ||              // or an LValue Reference (&)
      cursor_type.kind == CXType_RValueReference){               // or an RValue Reference (&&),
      CXType pointed_to_type = clang_getPointeeType(cursor_type);// retrieve the pointed-to type

      CXString pointed_to_type_spelling = clang_getTypeSpelling(pointed_to_type);     // Spell out the entire
      std::cout << "pointing to type: " << clang_getCString(pointed_to_type_spelling);// pointed-to type
      clang_disposeString(pointed_to_type_spelling);
    }
    else if(cursor_type.kind == CXType_Record){
      CXString type_spelling = clang_getTypeSpelling(cursor_type);
      std::cout <<  ", namely " << clang_getCString(type_spelling);
      clang_disposeString(type_spelling);
    }
    std::cout << "\n";
    CXCursorKind kind = clang_getCursorKind(current_cursor);
    if ((kind == CXCursorKind::CXCursor_FunctionDecl || kind == CXCursorKind::CXCursor_CXXMethod || kind == CXCursorKind::CXCursor_FunctionTemplate || \
         kind == CXCursorKind::CXCursor_Constructor))
    {
        printf("===================================================================\n");
        print_function_prototype(current_cursor, kind);
        printf("===================================================================\n");
    }
    //print_cursor(current_cursor);
    //return CXChildVisit_Recurse;
    return CXChildVisit_Continue;
  },
  &fd
  );
  close_src(fd);
  return 0;
}
