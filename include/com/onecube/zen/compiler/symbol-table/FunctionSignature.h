// Monday, January 08, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SIGNATURE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SIGNATURE_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

/*******************************************************************************
 * FunctionSignature                                                           *
 *******************************************************************************/

/**
 * @class FunctionSignature
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionSignature_t {
    jtk_ArrayList_t* m_fixedParameters;
    zen_ASTNode_t* m_variableParameter;
};

/**
 * @memberof FunctionSignature
 */
typedef struct zen_FunctionSignature_t zen_FunctionSignature_t;

// Constructor

/**
 * @memberof FunctionSignature
 */
zen_FunctionSignature_t* zen_FunctionSignature_new(jtk_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter);

// Destructor

/**
 * @memberof FunctionSignature
 */
void zen_FunctionSignature_delete(zen_FunctionSignature_t* signature);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_FUNCTION_SIGNATURE_H */