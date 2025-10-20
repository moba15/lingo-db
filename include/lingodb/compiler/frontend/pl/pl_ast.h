#ifndef LINGODB_COMPILER_FRONTEND_PL_AST_H
#define LINGODB_COMPILER_FRONTEND_PL_AST_H

#include "lingodb/compiler/frontend/ast/AstNode.h"
#include "lingodb/compiler/frontend/ast/query_node.h"
#include <vector>
#include <memory>

// This file defines the Abstract Syntax Tree (AST) nodes for the PL/pgSQL-like language.
// For now, it contains a minimal set of nodes to represent blocks and RETURN statements.
// The main idea is that a RETURN statement can contain a parsed SQL query.

namespace lingodb::ast {
    // We need to extend ExpressionClass enum with PL node types.
    // Since we cannot modify the original file, we assume these are defined elsewhere
    // and use INVALID for now.
    // For example:
    // PL_BLOCK = 100,
    // PL_RETURN = 101,

    namespace pl {

        struct Stmt : public AstNode {
            Stmt(ExpressionClass expressionClass) : AstNode(expressionClass) {}
            virtual ~Stmt() = default;
        };

        struct Block : public Stmt {
            // A block represents a `BEGIN ... END` section, containing a list of statements.
            Block() : Stmt(ExpressionClass::INVALID) /*PL_BLOCK*/ {}
            std::vector<std::shared_ptr<Stmt>> body;
        };

        struct ReturnStmt : public Stmt {
            // A RETURN statement.
            // The key part is `query`, which holds the AST of the SQL statement
            // that was parsed from the text following the RETURN keyword.
            ReturnStmt(std::shared_ptr<AstNode> query)
                : Stmt(ExpressionClass::INVALID) /*PL_RETURN*/, query(query) {}

            std::shared_ptr<AstNode> query;
        };

    } // namespace pl
} // namespace lingodb::ast

#endif // LINGODB_COMPILER_FRONTEND_PL_AST_H
