#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void print_source(const char *head, CXSourceLocation src)
{
    CXFile file;
    unsigned int line, col, off;
    clang_getSpellingLocation(src, &file, &line, &col, &off);
    printf("%s file: %s, line %u, col %u, off %u\n", head, clang_getCString(clang_getFileName(file)), line, col, off);
}
void print_cursor(CXCursor current_cursor)
    {
        CXSourceLocation start, end;
        CXSourceLocation src = clang_getCursorLocation(current_cursor);
        print_source("location: ", src);
        CXSourceRange range = clang_getCursorExtent(current_cursor);
        src = clang_getRangeStart(range);
        print_source(">>> ", src);
        start = src;
        src = clang_getRangeEnd(range);
        print_source("<<< ", src);
        end = src;
        CXFile file;
        unsigned int line, col, off;
        unsigned int sl, el;
        const char *buf;
        size_t size;
        clang_getSpellingLocation(start, &file, &line, &col, &sl);
        buf = clang_getFileContents(clang_Cursor_getTranslationUnit(current_cursor), file, &size);
        clang_getSpellingLocation(end, &file, &line, &col, &el);
        char *data = (char  *)malloc(256);
        memcpy(data, buf + sl, el - sl + 1);
        data[el - sl + 1] = 0;
        printf("---\n");
        printf("%s\n", data);
        printf("---\n");
        free(data);
    }

int create_src(const char *file)
{
    return open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
}

void write_src(int fd, CXCursor current_cursor)
{
  CXSourceLocation start, end;
  CXSourceRange range = clang_getCursorExtent(current_cursor);
  start = clang_getRangeStart(range);
  end = clang_getRangeEnd(range);
  CXFile file;
  unsigned int line_start, line_end, col, off;
  unsigned int sl, el;
  const char *buf;
  size_t size;
  //clang_getSpellingLocation(start, &file, &line_start, &col, &sl);
  clang_getExpansionLocation(start, &file, &line_start, &col, &sl);
  buf = clang_getFileContents(clang_Cursor_getTranslationUnit(current_cursor), file, &size);
  //clang_getSpellingLocation(end, &file, &line_end, &col, &el);
  clang_getExpansionLocation(end, &file, &line_end, &col, &el);
  const char *src_file = clang_getCString(clang_getFileName(file));
  char *data = (char  *)malloc(1024);
  sprintf(data, "// %s : %u -> %u\n", src_file, line_start, line_end);
  (void)write(fd, data, strlen(data));
  (void)write(fd, buf + sl, el - sl + 1);
  (void)write(fd, "\n", 1);
  free(data);
}

void close_src(int &fd)
{
  (void)close(fd);
  fd = -1;
}
