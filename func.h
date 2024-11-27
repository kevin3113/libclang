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

    int tmp_num = clang_Cursor_getNumTemplateArguments(cursor);
    printf("template arg count %d\n", tmp_num);

    auto function_name = Convert(clang_getCursorSpelling(cursor));
    auto return_type   = Convert(clang_getTypeSpelling(clang_getResultType(type)));
    int num_args = clang_Cursor_getNumArguments(cursor);
    printf("num args %u\n", num_args);
    printf("%s %s (", return_type.c_str(), function_name.c_str());
    for (int i = 0; i < num_args; ++i)
    {
        auto arg_cursor = clang_Cursor_getArgument(cursor, i);
        auto arg_name = Convert(clang_getCursorSpelling(arg_cursor));
        if (arg_name.empty())
        {
            arg_name = "no name!";
        }
        auto arg_data_type = Convert(clang_getTypeSpelling(clang_getArgType(type, i)));
        printf("%s %s", arg_data_type.c_str(), arg_name.c_str());
        if (i != num_args - 1)
        {
            printf(", ");
        }
    }
    printf(");\n");
}
