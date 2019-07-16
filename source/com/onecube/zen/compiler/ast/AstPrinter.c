// Sunday, March 11, 2018

#include <com/onecube/zen/compiler/ast/AstPrinter.h>
#include <com/onecube/zen/Parser.h>

zen_AstPrinter_t* zen_AstPrinter_new() {
    zen_AstPrinter_t* listener = zen_Memory_allocate(zen_AstPrinter_t, 1);
    listener->m_astListener = zen_ASTListener_newWithContext(listener);
    listener->m_depth = 0;

    zen_ASTListener_t* astListener = listener->m_astListener;
    astListener->m_onEnterEveryRule = zen_AstPrinter_onEnterEveryRule;
    astListener->m_onExitEveryRule = zen_AstPrinter_onExitEveryRule;
    astListener->m_onVisitTerminal = zen_AstPrinter_onVisitTerminal;

    // TODO: Initialize astListener

    return listener;
}

void zen_AstPrinter_delete(zen_AstPrinter_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");

    zen_ASTListener_delete(listener->m_astListener);
    zen_Memory_deallocate(listener);
}

zen_ASTListener_t* zen_AstPrinter_getAstListener(zen_AstPrinter_t* listener) {
    jtk_Assert_assertObject(listener, "The specified listener is null.");
    return listener->m_astListener;
}

void zen_AstPrinter_indent(int32_t depth) {
    int32_t i;
    for (i = 0; i < depth; i++) {
        printf("    ");
    }
}

void zen_AstPrinter_onVisitTerminal(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_AstPrinter_t* listener = (zen_AstPrinter_t*)astListener->m_context;
    
    listener->m_depth++;
    zen_AstPrinter_indent(listener->m_depth);
    printf("<%s>\n", zen_Token_getText((zen_Token_t*)node->m_context));
    listener->m_depth--;
}

void zen_AstPrinter_onEnterEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_AstPrinter_t* listener = (zen_AstPrinter_t*)astListener->m_context;
    
    listener->m_depth++;
    zen_AstPrinter_indent(listener->m_depth);
    printf("%s\n", zen_Parser_getRuleName(node->m_type));
}

void zen_AstPrinter_onExitEveryRule(zen_ASTListener_t* astListener, zen_ASTNode_t* node) {
    zen_AstPrinter_t* listener = (zen_AstPrinter_t*)astListener->m_context;
    listener->m_depth--;
}
