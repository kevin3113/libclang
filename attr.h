#pragma once

#include <unordered_map>
#include <vector>
#include "func.h"

static std::vector<CXCursor> g_tiling_cursors;
void AddTilingAttr(CXCursor cursor, CXCursor parent)
{
    if (clang_isAttribute(clang_getCursorKind(cursor)))
    {
        g_tiling_cursors.emplace_back(parent);
    }
}

const std::vector<CXCursor> &GetTilingCX()
{
    return g_tiling_cursors;
}

void GetFileds(CXCursor cursor)
{
  std::vector<CXCursor> fields;
  clang_visitChildren(cursor,
    [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
        if (clang_getCursorKind(current_cursor) == CXCursorKind::CXCursor_FieldDecl)
        {
            auto pf = (std::vector<CXCursor> *)client_data;
            pf->emplace_back(current_cursor);
        }
        return CXChildVisit_Recurse;
    }, &fields);
  for (auto cur : fields)
  {
    printf("Field Name %s type %s\n", Convert(clang_getCursorSpelling(cur)).c_str(), Convert(clang_getTypeSpelling(clang_getCursorType(cur))).c_str());
  }
}
