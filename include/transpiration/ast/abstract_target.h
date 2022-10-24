#ifndef AST_ABSTRACT_TARGET_H_
#define AST_ABSTRACT_TARGET_H_

#include "transpiration/ast/abstract_expression.h"

/// This class merely structures the inheritance hierarchy
/// It represents "things that can be assigned to", i.e. lvalues
class AbstractTarget : public AbstractExpression
{
public:
#include "transpiration/ast/utils/warning_hiding_non_virtual_function_prologue.h"

    /// Clones a node recursively, i.e., by including all of its children.
    /// Because return-type covariance does not work with smart pointers,
    /// derived classes are expected to introduce a std::unique_ptr<DerivedNode> clone() method that hides this (for use
    /// with derived class ptrs/refs) \return A clone of the node including clones of all of its children.
    inline std::unique_ptr<AbstractTarget> clone(AbstractNode *parent_) const
    { /* intentionally hiding */
        return std::unique_ptr<AbstractTarget>(clone_impl(parent_));
    }

#include "transpiration/ast/utils/warning_epilogue.h"

private:
    /// Refines return type to AbstractTarget
    AbstractTarget *clone_impl(AbstractNode *parent) const override = 0;
};

#endif // AST_ABSTRACT_TARGET_H_