#ifndef AST_PARSER_HELPERS_H_
#define AST_PARSER_HELPERS_H_

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif // AST_PARSER_HELPERS_H_
