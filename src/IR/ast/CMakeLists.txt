add_transpiration_dialect_library(TranspirationASTDialect
        ASTDialect.cpp

        ADDITIONAL_HEADER_DIRS
        ${PROJECT_SOURCE_DIR}/include/transpiration/IR/ast

        DEPENDS
        MLIRASTIncGen

	LINK_LIBS PUBLIC
	MLIRIR
)
