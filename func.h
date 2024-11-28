#pragma once

#include <stdio.h>


std::string Convert(const CXString& s)
{
    std::string result = clang_getCString(s);
    clang_disposeString(s);
    return result;
}
void print_function_prototype(CXCursor cursor, CXCursorKind kind)
{
    // TODO : Print data! 
    auto type = clang_getCursorType(cursor);
    char *fun = (char *)malloc(1024);

    int tmp_num = clang_Cursor_getNumTemplateArguments(cursor);
    printf("template arg count %d\n", tmp_num);

    auto function_name = Convert(clang_getCursorSpelling(cursor));
    auto return_type   = Convert(clang_getTypeSpelling(clang_getResultType(type)));
    int num_args = clang_Cursor_getNumArguments(cursor);
    printf("function name: %s\n", function_name.c_str());
    printf("return type: %s\n", return_type.c_str());
    printf("count of args: %u\n", num_args);
    int offset = sprintf(fun, "%s %s (", return_type.c_str(), function_name.c_str());
    for (int i = 0; i < num_args; ++i)
    {
        auto arg_cursor = clang_Cursor_getArgument(cursor, i);
        auto arg_name = Convert(clang_getCursorSpelling(arg_cursor));
        if (arg_name.empty())
        {
            arg_name = "no name!";
        }
        auto arg_data_type = Convert(clang_getTypeSpelling(clang_getArgType(type, i)));
        offset += sprintf(fun + offset, "%s %s", arg_data_type.c_str(), arg_name.c_str());
        printf("  arg %u: type %s name %s\n", i + 1, arg_data_type.c_str(), arg_name.c_str());
        if (i != num_args - 1)
        {
            offset += sprintf(fun + offset, ", ");
        }
    }
    offset += sprintf(fun + offset, ");\n");
    printf("the complete prototype is:\n%s\n", fun);
    free(fun);
}
void print_function_template(CXCursor cursor, CXCursorKind kind)
{
    // TODO : Print data! 
    auto type = clang_getCursorType(cursor);
    char *fun = (char *)malloc(1024);

    int tmp_num = clang_Cursor_getNumTemplateArguments(cursor);
    printf("template arg count %d\n", tmp_num);
    for (int i = 0; i < tmp_num; i++)
    {
        CXType t = clang_Cursor_getTemplateArgumentType(cursor, i);
        auto t_type = Convert(clang_getTypeSpelling(clang_getResultType(t)));
        printf("template arg %u: type %s\n", i + 1, t_type.c_str());
    }

    free(fun);
}
