// Saturday, March 10, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_PRINTER_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_PRINTER_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTListener.h>

/*******************************************************************************
 * ASTPrinter                                                                  *
 *******************************************************************************/

/**
 * @class ASTPrinter
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ASTPrinter_t {
    zen_ASTListener_t* m_astListener;
    int32_t m_depth;
};

/**
 * @memberof ASTPrinter
 */
typedef struct zen_ASTPrinter_t zen_ASTPrinter_t;

/**
 * @memberof ASTPrinter
 */
zen_ASTPrinter_t* zen_ASTPrinter_new();

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_delete(zen_ASTPrinter_t* listener);

/**
 * @memberof ASTPrinter
 */
zen_ASTListener_t* zen_ASTPrinter_getASTListener(zen_ASTPrinter_t* listener);

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);

/**
 * @memberof ASTPrinter
 */
void zen_ASTPrinter_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_PRINTER_H */