#ifndef AST_UTILS_ABC_AST_TO_MLIR_VISITOR_H_
#define AST_UTILS_ABC_AST_TO_MLIR_VISITOR_H_

#include <mlir/IR/Builders.h>
#include <mlir/IR/MLIRContext.h>

#include "transpiration/IR/ast/ast_dialect.h"
#include "transpiration/ast/utils/plain_visitor.h"
#include "transpiration/ast/utils/visitor.h"

/// Forward declaration of the class that will actually implement the AbcAstToMlirVisitor's logic
class SpecialAbcAstToMlirVisitor;

/// AbcAstToMlirVisitor uses the Visitor<T> template to allow specifying default behaviour
typedef Visitor<SpecialAbcAstToMlirVisitor, PlainVisitor> AbcAstToMlirVisitor;

class SpecialAbcAstToMlirVisitor : public PlainVisitor
{
private:
    mlir::OpBuilder builder;
    mlir::Block *block;

    void add_op(mlir::Operation *op);
    void add_recursive_result_to_region(AbstractNode &node, mlir::Region &region);
    mlir::Type translate_type(Datatype abc_type);
    void recursive_visit(AbstractNode &node, mlir::Block *childBlock);

public:
    explicit SpecialAbcAstToMlirVisitor(mlir::MLIRContext &ctx);

#include "transpiration/ast/utils/warning_suggest_override_prologue.h"

    mlir::Block *getBlockPtr();

    void visit(AbstractExpression &expr);

    void visit(Assignment &elem);

    void visit(BinaryExpression &elem);

    void visit(Block &elem);

    void visit(Call &elem);

    // TODO: What is the ABC MLIR equivalent of the expression list? There is no ExpressionListOp
    //  void visit(ExpressionList &elem);

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

    void visit(AbstractStatement &stmt);

    // TODO (Miro): There is no ternary operator in the ABC MLIR (yet?)
    // void visit(TernaryOperator &elem);

    void visit(UnaryExpression &elem);

    void visit(VariableDeclaration &elem);

    void visit(Variable &elem);

#include "transpiration/ast/utils/warning_epilogue.h"
};

#endif // AST_UTILS_ABC_AST_TO_MLIR_VISITOR_H_
