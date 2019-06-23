// Sunday, January 07, 2018

#ifndef ZEN_ST_MEMBER_FUNCTION_SYMBOL_H
#define ZEN_ST_MEMBER_FUNCTION_SYMBOL_H

#include <zen/st/Symbol.h>
#include <zen/st/FunctionSignature.h>

/*******************************************************************************
 * FunctionSymbol                                                              *
 *******************************************************************************/

typedef struct zen_FunctionSymbol_t zen_FunctionSymbol_t;
 
struct zen_FunctionSymbol_t {
    zen_Symbol_t* m_symbol;
    /* Signatures are destroyed by the destructor. */
    zen_ArrayList_t* m_signatures; /* <zen_FunctionSignature_t*> */
    zen_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
    int32_t m_parameterThreshold;
};

zen_FunctionSymbol_t* zen_FunctionSymbol_new(zen_ASTNode_t* identifier, zen_Scope_t* enclosingScope);
void zen_FunctionSymbol_delete(zen_FunctionSymbol_t* symbol);
zen_ArrayList_t* zen_FunctionSymbol_getSignatures(zen_FunctionSymbol_t* symbol);
void zen_FunctionSymbol_addSignature(zen_FunctionSymbol_t* symbol, zen_FunctionSignature_t* signature);
int32_t zen_FunctionSymbol_getParameterThreshold(zen_FunctionSymbol_t* symbol);
void zen_FunctionSymbol_setParameterThreshold(zen_FunctionSymbol_t* symbol, int32_t parameterThreshold);
zen_Symbol_t* zen_FunctionSymbol_getSymbol(zen_FunctionSymbol_t* symbol);

#endif /* ZEN_ST_MEMBER_FUNCTION_SYMBOL_H */