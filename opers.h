#pragma once


using oper_func = std::function<void(CXCursor)>;
struct Operator
{
    const char *option;
    oper_func pfun;
};

#define OPER(x) Operator{#x, x}

void get_struct_size(CXCursor root_cursor);
void get_function_declare(CXCursor root_cursor);
void get_function_template_args(CXCursor root_cursor);
void split_file_to_host_device(CXCursor root_cursor);

Operator g_opers[] = {OPER(get_struct_size),
                      OPER(get_function_declare),
                      OPER(get_function_template_args),
                      OPER(split_file_to_host_device)};

void print_options(void)
{
    printf("  opers are:\n");
    for (int i = 0; i < sizeof(g_opers) / sizeof(Operator); i++)
    {
        printf("             %s\n", g_opers[i].option);
    }
}

int execute(const char *option, CXCursor cursor)
{
    for (int i = 0; i < sizeof(g_opers) / sizeof(Operator); i++)
    {
        if (strcmp(g_opers[i].option, option) == 0)
        {
            g_opers[i].pfun(cursor);
            return 0;
        }
    }
    return 1;
}

void get_struct_size(CXCursor root_cursor)
{
  clang_visitChildren(root_cursor,
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
}

void get_function_declare(CXCursor root_cursor)
{
  clang_visitChildren(root_cursor,
    [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
    CXCursorKind kind = clang_getCursorKind(current_cursor);
    /*
        if ((kind == CXCursorKind::CXCursor_FunctionDecl || kind == CXCursorKind::CXCursor_CXXMethod || kind == CXCursorKind::CXCursor_FunctionTemplate || \
             kind == CXCursorKind::CXCursor_Constructor))
    */
        if (kind == CXCursorKind::CXCursor_FunctionDecl)
        {
            printf("===================================================================\n");
            print_function_prototype(current_cursor, kind);
            printf("===================================================================\n");
        }
        return CXChildVisit_Recurse;
    }, nullptr);
}

void get_function_template_args(CXCursor root_cursor)
{
  clang_visitChildren(root_cursor,
    [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
    CXCursorKind kind = clang_getCursorKind(current_cursor);
    CXType cursor_type = clang_getCursorType(current_cursor);
    CXString kstr = clang_getTypeKindSpelling(cursor_type.kind);
    printf("kind %s\n", clang_getCString(kstr));
        if (kind == CXCursorKind::CXCursor_FunctionTemplate)
        {
            printf("===================================================================\n");
            print_function_template(current_cursor, kind);
            printf("===================================================================\n");
        }
        return CXChildVisit_Recurse;
    }, nullptr);
}

void split_file_to_host_device(CXCursor root_cursor)
{
}
