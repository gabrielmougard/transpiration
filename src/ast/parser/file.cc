#include "transpiration/ast/parser/file.h"

File::~File()
{
    if (_fp)
    {
        fclose(_fp);
    }
}

File::File(const char *path) : _fp(fopen(path, "rt"))
{
    if (!_fp)
    {
        throw FileNotFound(std::string("'") + path + "' not found");
    }
}

int File::operator()()
{
    return fgetc(_fp);
}
