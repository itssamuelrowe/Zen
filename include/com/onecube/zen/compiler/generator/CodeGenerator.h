// Friday, March 16, 2018

#ifndef ZEN_CODE_GENERATOR_H
#define ZEN_CODE_GENERATOR_H

/*******************************************************************************
 * CodeGenerator                                                               *
 *******************************************************************************/

struct zen_CodeGenerator_t {
    zen_ASTListener_t* m_astListener;
    zen_SymbolTable_t* m_symbolTable;
    zen_AstAnnotations_t* m_scopes;
};

typedef struct zen_CodeGenerator_t zen_CodeGenerator_t;

zen_CodeGenerator_t* zen_CodeGenerator_new(zen_SymbolTable_t* symbolTable, zen_AstAnnotations_t* scopes);
void zen_CodeGenerator_delete(zen_CodeGenerator_t* generator);
void zen_CodeGenerator_generate();

#endif /* ZEN_CODE_GENERATOR_H */