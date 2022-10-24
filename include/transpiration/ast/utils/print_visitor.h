#ifndef AST_UTILS_PRINT_VISITOR_H_
#define AST_UTILS_PRINT_VISITOR_H_

#include <list>
#include <sstream>
#include <string>
#include <utility>

#include "transpiration/ast/abstract_node.h"
#include "transpiration/ast/utils/plain_visitor.h"
#include "transpiration/ast/utils/visitor.h"

/// Forward declaration of the class that will actually implement the PrintVisitor's logic
class SpecialPrintVisitor;

/// PrintVisitor uses the Visitor<T> template to allow specifying default behaviour
typedef Visitor<SpecialPrintVisitor, PlainVisitor> PrintVisitor;

class SpecialPrintVisitor : public PlainVisitor
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
    explicit SpecialPrintVisitor(std::ostream &os);

#include "transpiration/ast/utils/warning_suggest_override_prologue.h"

    void visit(AbstractNode &);

    void visit(LiteralBool &);

#include "transpiration/ast/utils/warning_epilogue.h"
};

#endif // AST_UTILS_PRINT_VISITOR_H_
