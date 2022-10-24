#include "transpiration/ast/utils/scoped_visitor.h"
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

void ScopedVisitor::visit(BinaryExpression &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(Block &elem)
{
    enterScope(elem);
    visitChildren(elem);
    exitScope();
}

void ScopedVisitor::visit(Call &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(ExpressionList &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(For &elem)
{
    enterScope(elem);
    visitChildren(elem);
    exitScope();
}

void ScopedVisitor::visit(Function &elem)
{
    enterScope(elem);
    visitChildren(elem);
    exitScope();
}

void ScopedVisitor::visit(FunctionParameter &elem)
{
    getCurrentScope().addIdentifier(elem.getIdentifier());
    visitChildren(elem);
}

void ScopedVisitor::visit(If &elem)
{
    enterScope(elem);
    visitChildren(elem);
    exitScope();
}

void ScopedVisitor::visit(IndexAccess &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(LiteralBool &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(LiteralChar &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(LiteralInt &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(LiteralFloat &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(LiteralDouble &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(LiteralString &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(OperatorExpression &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(Return &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(TernaryOperator &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(UnaryExpression &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(Assignment &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visit(VariableDeclaration &elem)
{
    getCurrentScope().addIdentifier(elem.getTarget().getIdentifier());
    visitChildren(elem);
}

void ScopedVisitor::visit(Variable &elem)
{
    visitChildren(elem);
}

void ScopedVisitor::visitChildren(AbstractNode &elem)
{
    // special treatment for For loops: we need to visit the children of the initializer/update blocks separately as we
    // do not want to create a new scope when visiting them (otherwise variables declared in initializer will not be
    // accessible in condition and update)
    if (auto forStatement = dynamic_cast<For *>(&elem))
    {
        // call visitChildren directly on the initializer block, otherwise this would create a new scope but that's
        // wrong!
        visitChildren(forStatement->getInitializer());

        forStatement->getCondition().accept(*this);

        // call visitChildren directly on the update block, otherwise this would create a new scope but that's wrong!
        visitChildren(forStatement->getUpdate());

        forStatement->getBody().accept(*this);
    }
    else
    {
        for (auto &c : elem)
        {
            c.accept(*this);
        }
    }
}

Scope &ScopedVisitor::getCurrentScope()
{
    if (currentScope == nullptr)
    {
        throw std::runtime_error("Cannot return non-existent scope!");
    }
    return *currentScope;
}

const Scope &ScopedVisitor::getCurrentScope() const
{
    if (currentScope == nullptr)
    {
        throw std::runtime_error("Cannot return non-existent scope!");
    }
    return *currentScope;
}

Scope &ScopedVisitor::getRootScope()
{
    if (rootScope == nullptr)
    {
        throw std::runtime_error("Cannot return non-existent root scope!");
    }
    return *rootScope;
}

std::unique_ptr<Scope> ScopedVisitor::takeRootScope()
{
    return std::move(rootScope);
}

const Scope &ScopedVisitor::getRootScope() const
{
    if (rootScope == nullptr)
    {
        throw std::runtime_error("Cannot return non-existent root scope!");
    }
    return *rootScope;
}

void ScopedVisitor::setRootScope(std::unique_ptr<Scope> &&scope)
{
    rootScope = std::move(scope);
}

void ScopedVisitor::overrideCurrentScope(Scope *scope)
{
    currentScope = scope;
}

void ScopedVisitor::enterScope(AbstractNode &node)
{
    if (rootScope == nullptr)
    {
        // no scope created yet: create root scope and also set it as current scope
        rootScope = std::make_unique<Scope>(node);
        currentScope = rootScope.get();
        return;
    }
    else if (currentScope == nullptr)
    {
        // Root scope exists but no current one: set current scope to rootScope
        currentScope = rootScope.get();
    }
    // create nested scope with current scope as parent
    currentScope = Scope::createNestedScope(getCurrentScope(), node);
}

void ScopedVisitor::exitScope()
{
    if (currentScope)
    {
        currentScope = &currentScope->getParentScope();
    }
    else
    {
        throw std::runtime_error("Cannot leave non-existent scope. "
                                 "Did you forget to call enterScope(...)?");
    }
}
