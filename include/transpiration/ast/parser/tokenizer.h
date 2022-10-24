#ifndef AST_PARSER_TOKENIZER_H_
#define AST_PARSER_TOKENIZER_H_

#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>

#include "tokens.h"

using get_character = std::function<char()>;

inline get_character getCharacterFunc(std::string &inputString)
{
    return [&inputString]() {
        if (inputString.empty())
        {
            return (char)EOF;
        }
        else
        {
            char c = inputString.at(0);
            inputString.erase(0, 1);
            return c;
        }
    };
}

class PushBackStream;

class tokens_iterator
{
private:
    std::function<token()> _get_next_token;
    token _current;

public:
    explicit tokens_iterator(PushBackStream &stream);

    explicit tokens_iterator(std::deque<token> &tokens);

    tokens_iterator(const tokens_iterator &) = delete;

    void operator=(const tokens_iterator &) = delete;

    const token &operator*() const;

    const token *operator->() const;

    tokens_iterator &operator++();

    explicit operator bool() const;
};


#endif // AST_PARSER_TOKENIZER_H_
