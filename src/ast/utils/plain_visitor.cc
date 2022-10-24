#include "transpiration/ast/utils/plain_visitor.h"

#include "transpiration/ast/abstract_expression.h"
#include "transpiration/ast/abstract_node.h"
#include "transpiration/ast/abstract_statement.h"
#include "transpiration/ast/abstract_target.h"
#include "transpiration/ast/assignment.h"
#include "transpiration/ast/binary_expression.h"
#include "transpiration/ast/block.h"
#include "transpiration/ast/call.h"
#include "transpiration/ast/expression_list.h"
#include "transpiration/ast/for.h"
#include "transpiration/ast/function.h"
#include "transpiration/ast/function_parameter.h"
#include "transpiration/ast/if.h"
#include "transpiration/ast/index_access.h"
#include "transpiration/ast/literal.h"
#include "transpiration/ast/operator_expression.h"
#include "transpiration/ast/return.h"
#include "transpiration/ast/ternary_operator.h"
#include "transpiration/ast/unary_expression.h"
#include "transpiration/ast/variable.h"
#include "transpiration/ast/variable_declaration.h"

void PlainVisitor::visit(BinaryExpression &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(Block &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(Call &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(ExpressionList &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(For &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(Function &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(FunctionParameter &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(If &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(IndexAccess &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(LiteralBool &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(LiteralChar &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(LiteralInt &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(LiteralFloat &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(LiteralDouble &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(LiteralString &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(OperatorExpression &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(Return &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(TernaryOperator &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(UnaryExpression &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(Assignment &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(VariableDeclaration &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visit(Variable &elem)
{
    visitChildren(elem);
}

void PlainVisitor::visitChildren(AbstractNode &elem)
{
    for (auto &c : elem)
    {
        c.accept(*this);
    }
}
