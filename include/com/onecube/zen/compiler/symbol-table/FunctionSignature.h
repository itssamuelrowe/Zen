// Monday, January 08, 2018

#ifndef ZEN_ST_FUNCTION_SIGNATURE_H
#define ZEN_ST_FUNCTION_SIGNATURE_H

#include <com/onecube/zen/collection/ArrayList.h>
#include <com/onecube/zen/compiler/ast/Node.h>

/*******************************************************************************
 * FunctionSignature                                                           *
 *******************************************************************************/

struct zen_FunctionSignature_t {
    jtk_ArrayList_t* m_fixedParameters;
    zen_ASTNode_t* m_variableParameter;
};

typedef struct zen_FunctionSignature_t zen_FunctionSignature_t;

zen_FunctionSignature_t* zen_FunctionSignature_new(jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter);
void zen_FunctionSignature_delete(zen_FunctionSignature_t* signature);

#endif /* ZEN_ST_FUNCTION_SIGNATURE_H */