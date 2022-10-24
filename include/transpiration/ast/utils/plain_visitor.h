#ifndef AST_UTILS_PLAIN_VISITOR_H_
#define AST_UTILS_PLAIN_VISITOR_H_

#include "transpiration/ast/utils/ivisitor.h"
#include "transpiration/ast/utils/scope.h"

/// This class implements the "default" behaviour of a visitor
/// simply visiting a node's children
class PlainVisitor : public IVisitor
{
public:
    ~PlainVisitor() override = default;

    void visit(BinaryExpression &elem) override;

    void visit(Block &elem) override;

    void visit(Call &elem) override;

    void visit(ExpressionList &elem) override;

    void visit(For &elem) override;

    void visit(Function &elem) override;

    void visit(FunctionParameter &elem) override;

    void visit(If &elem) override;

    void visit(IndexAccess &elem) override;

    void visit(LiteralBool &elem) override;

    void visit(LiteralChar &elem) override;

    void visit(LiteralInt &elem) override;

    void visit(LiteralFloat &elem) override;

    void visit(LiteralDouble &elem) override;

    void visit(LiteralString &elem) override;

    void visit(OperatorExpression &elem) override;

    void visit(Return &elem) override;

    void visit(TernaryOperator &elem) override;

    void visit(UnaryExpression &elem) override;

    void visit(Assignment &elem) override;

    void visit(VariableDeclaration &elem) override;

    void visit(Variable &elem) override;

    void visitChildren(AbstractNode &elem);
};

#endif // AST_UTILS_PLAIN_VISITOR_H_
