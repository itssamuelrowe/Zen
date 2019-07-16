// Sunday, April 28, 2018

#ifndef COM_ONECUBE_COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_COMPILER_AST_AST_ANNOTATION_H
#define COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_ANNOTATION_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotationType.h>

/*******************************************************************************
 * ASTAnnotation                                                               *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since  zen 1.0
 */
struct zen_ASTAnnotation_t {
    zen_ASTAnnotationType_t m_type;
    void* m_bundle;
};

typedef struct zen_ASTAnnotation_t zen_ASTAnnotation_t;

zen_ASTAnnotation_t* zen_ASTAnnotation_new(zen_ASTAnnotationType_t type, void* bundle);
void zen_ASTAnnotation_delete(zen_ASTAnnotation_t* annotation);

#endif /* COM_ONECUBE_COM_ONECUBE_ZEN_COMPILER_COMPILER_AST_AST_ANNOTATION_H */