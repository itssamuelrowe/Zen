// Saturday, March 10, 2018

#ifndef ZEN_AST_AST_PRINTER_H
#define ZEN_AST_AST_PRINTER_H

#include <zen/ASTListener.h>

/*******************************************************************************
 * AstPrinter                                                                  *
 *******************************************************************************/

struct zen_AstPrinter_t {
    zen_ASTListener_t* m_astListener;
    int32_t m_depth;
};

typedef struct zen_AstPrinter_t zen_AstPrinter_t;

zen_AstPrinter_t* zen_AstPrinter_new();
void zen_AstPrinter_delete(zen_AstPrinter_t* listener);
zen_ASTListener_t* zen_AstPrinter_getAstListener(zen_AstPrinter_t* listener);

void zen_AstPrinter_onEnterEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_AstPrinter_onExitEveryRule(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_AstPrinter_onVisitTerminal(zen_ASTListener_t* listener, zen_ASTNode_t* node);

#endif /* ZEN_AST_AST_PRINTER_H */