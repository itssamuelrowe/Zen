// Saturday, February 24, 2018

#include <zen/st/FunctionSignature.h>

zen_FunctionSignature_t* zen_FunctionSignature_new(zen_ArrayList_t* fixedParameters,
    zen_ASTNode_t* variableParameter) {
    zen_FunctionSignature_t* signature = zen_Memory_allocate(zen_FunctionSignature_t, 1);
    signature->m_fixedParameters = fixedParameters;
    signature->m_variableParameter = variableParameter;
    
    return signature;
}

void zen_FunctionSignature_delete(zen_FunctionSignature_t* signature) {
    jtk_Assert_assertObject(signature, "The specified signature is null.");
    zen_Memory_deallocate(signature);
}