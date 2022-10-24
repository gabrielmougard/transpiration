#ifndef AST_ABSTRACT_STATEMENT_H_
#define AST_ABSTRACT_STATEMENT_H_

#include "transpiration/ast/abstract_node.h"

/// This class merely structures the inheritance hierarchy
class AbstractStatement : public AbstractNode
{
public:
    /// Clones a node recursively, i.e., by including all of its children.
    /// Because return-type covariance does not work with smart pointers,
    /// derived classes are expected to introduce a std::unique_ptr<DerivedNode> clone() method that hides this (for use
    /// with derived class ptrs/refs) \return A clone of the node including clones of all of its children.
    inline std::unique_ptr<AbstractStatement> clone(AbstractNode *parent_) const
    {
        return std::unique_ptr<AbstractStatement>(clone_impl(parent_));
    }

private:
    /// Refines return type to AbstractStatement
    AbstractStatement *clone_impl(AbstractNode *parent_) const override = 0;
};

#endif // AST_ABSTRACT_STATEMENT_H_