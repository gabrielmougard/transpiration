#ifndef AST_UTILS_PARENT_SETTING_VISITOR_H
#define AST_UTILS_PARENT_SETTING_VISITOR_H

#include <stack>

#include "transpiration/ast/utils/plain_visitor.h"
#include "transpiration/ast/utils/visitor.h"

class SpecialParentSettingVisitor;

typedef Visitor<SpecialParentSettingVisitor, PlainVisitor> ParentSettingVisitor;

/// This is an ugly hack since the parser currently does not set parents!
/// TODO: Set parents in parser properly, then remove this visitor!
class SpecialParentSettingVisitor : public PlainVisitor
{
private:
    std::stack<AbstractNode *> stack;

public:
    void visit(AbstractNode &elem);
};

#endif // AST_UTILS_PARENT_SETTING_VISITOR_H