#ifndef AST_PARSER_FILE_H_
#define AST_PARSER_FILE_H_

#include <string>
#include "transpiration/ast/parser/errors.h"

class File
{
private:
    FILE *_fp;

public:
    explicit File(const char *path);

    ~File();

    int operator()();

    File(const File &) = delete;

    void operator=(const File &) = delete;
};

#endif // AST_PARSER_FILE_H_