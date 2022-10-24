#ifndef AST_UTILS_PROGRAM_PRINT_VISITOR_H_
#define AST_UTILS_PROGRAM_PRINT_VISITOR_H_

#include "transpiration/ast/utils/plain_visitor.h"
#include "transpiration/ast/utils/visitor.h"

class SpecialProgramPrintVisitor;

typedef Visitor<SpecialProgramPrintVisitor, PlainVisitor> ProgramPrintVisitor;

class SpecialProgramPrintVisitor : public PlainVisitor
{
private:
    /// Reference to the stream to which we write the output
    std::ostream &os;

    /// Current indentation level
    int indentation_level = 0;

    /// Compute the current required indentation string
    /// from the current indentation_level
    [[nodiscard]] std::string getIndentation() const;

public:
    explicit SpecialProgramPrintVisitor(std::ostream &os);

#include "transpiration/ast/utils/warning_suggest_override_prologue.h"

    void visit(BinaryExpression &elem);

    void visit(Block &elem);

    void visit(Call &elem);

    void visit(ExpressionList &elem);

    void visit(For &elem);

    void visit(Function &elem);

    void visit(FunctionParameter &elem);

    void visit(If &elem);

    void visit(IndexAccess &elem);

    void visit(LiteralBool &elem);

    void visit(LiteralChar &elem);

    void visit(LiteralInt &elem);

    void visit(LiteralFloat &elem);

    void visit(LiteralDouble &elem);

    void visit(LiteralString &elem);

    void visit(OperatorExpression &elem);

    void visit(Return &elem);

    void visit(TernaryOperator &elem);

    void visit(UnaryExpression &elem);

    void visit(Assignment &elem);

    void visit(VariableDeclaration &elem);

    void visit(Variable &elem);

#include "transpiration/ast/utils/warning_epilogue.h"
};

#endif // AST_UTILS_PROGRAM_PRINT_VISITOR_H_
