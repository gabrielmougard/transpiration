#ifndef AST_PARSER_PUSH_BACK_STREAM_H_
#define AST_PARSER_PUSH_BACK_STREAM_H_

#include <functional>
#include <stack>

using get_character = std::function<char()>;

class PushBackStream
{
private:
    const get_character &_input;
    std::stack<char> _stack;
    size_t _line_number;
    size_t _char_index;

public:
    explicit PushBackStream(const get_character *input);

    char operator()();

    void pushBack(char c);

    [[nodiscard]] size_t getLineNumber() const;

    [[nodiscard]] size_t getCharIndex() const;
};

#endif // AST_PARSER_PUSH_BACK_STREAM_H_