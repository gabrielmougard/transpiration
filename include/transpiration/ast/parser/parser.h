#ifndef AST_PARSER_PARSER_H_
#define AST_PARSER_PARSER_H_

#include <memory>

#include "transpiration/ast/abstract_node.h"
#include "transpiration/ast/parser/tokenizer.h"
#include "transpiration/ast/utils/datatype.h"
#include "transpiration/ast/utils/operator.h"

// In order to avoid excessive compilation dependencies,
// we use forward-declarations rather than includes when possible
// However, we must include AbstractNode since we use it with smart ptrs

class AbstractExpression;
class AbstractStatement;
class AbstractTarget;
class BinaryExpression;
class Block;
class ExpressionList;
class Function;
class FunctionParameter;
class For;
class If;
class IndexAccess;
class Operator;
class Return;
class UnaryExpression;
class Assignment;
class VariableDeclaration;
class Variable;

/// Vector to keep track of parsed nodes.
static std::vector<std::reference_wrapper<AbstractNode>> parsedNodes;

/// The parser takes the
class Parser
{
private:
    static AbstractExpression *parseExpression(tokens_iterator &it);

    static AbstractStatement *parseStatement(tokens_iterator &it, bool gobbleTrailingSemicolon = true);

    static AbstractTarget *parseTarget(tokens_iterator &it);

    static Block *parseBlockStatement(tokens_iterator &it);

    static ExpressionList *parseExpressionList(tokens_iterator &it);

    static For *parseForStatement(tokens_iterator &it);

    static Function *parseFunctionStatement(tokens_iterator &it);

    static FunctionParameter *parseFunctionParameter(tokens_iterator &it);

    static If *parseIfStatement(tokens_iterator &it);

    /// Returns a Literal of _some_ type without caring about type
    static AbstractExpression *parseLiteral(tokens_iterator &it);

    static Return *parseReturnStatement(tokens_iterator &it);

    static Variable *parseVariable(tokens_iterator &it);

    static VariableDeclaration *parseVariableDeclarationStatement(tokens_iterator &it);

    static Assignment *parseAssignmentStatement(tokens_iterator &it);

    static void parseTokenValue(tokens_iterator &it, const token_value &value);

    static Datatype parseDatatype(tokens_iterator &it);

    static Operator parseOperator(tokens_iterator &it);

    static std::string parseIdentifier(tokens_iterator &it);

    static Block *parseBlockOrSingleStatement(tokens_iterator &it);

    static AbstractExpression *parseLiteral(tokens_iterator &it, bool isNegative);

public:
    /// Parses a given input program, returns (a unique ptr) to the created root node of the AST.
    /// \param s The program to parse given as string in a C++-like syntax.
    /// \return (A unique pointer) to the root node of the AST.
    static std::unique_ptr<AbstractNode> parse(std::string s);

    /// Parses a given input program, returns (a unique ptr) to the created root node of the AST and stores a reference
    /// to each created node (i.e., statement or expression) into the passed createdNodesList. \param s The program to
    /// parse given as string in a C++-like syntax. \param createdNodesList The list of parsed AbstractNodes. \return (A
    /// unique pointer) to the root node of the AST.
    static std::unique_ptr<AbstractNode> parse(
        std::string s, std::vector<std::reference_wrapper<AbstractNode>> &createdNodesList);

    /// Parses the JSON string representation of an AST and returns (a unique ptr) to the created root node of the AST.
    /// \param s The JSON string to parse
    /// \return (A unique pointer) to the root node of the AST.
    static std::unique_ptr<AbstractNode> parseJson(std::string s);

    /// Parses the JSON representation of an AST and returns (a unique ptr) to the created root node of the AST.
    /// \param j The JSON structure to parse
    /// \throws runtime_error if an unknown type is encountered
    /// \return (A unique pointer) to the root node of the AST.
    static std::unique_ptr<AbstractNode> parseJson(nlohmann::json j);
    /// The following three functions are helpers for parseJson, so that we can get the right abstract node type,
    /// because otherwise we would need to cast down from an AbstractNode.
    static std::unique_ptr<AbstractExpression> parseJsonExpression(nlohmann::json j);
    static std::unique_ptr<AbstractStatement> parseJsonStatement(nlohmann::json j);
    static std::unique_ptr<AbstractTarget> parseJsonTarget(nlohmann::json j);
};
#endif // AST_PARSER_PARSER_H_
