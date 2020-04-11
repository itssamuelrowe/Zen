// Saturday, January 04, 2019

#ifndef COM_ONECUBE_ZEN_COMPILER_AST_EXPRESSION_ANNOTATION_H
#define COM_ONECUBE_ZEN_COMPILER_AST_EXPRESSION_ANNOTATION_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ExpressionAnnotation                                                        *
 *******************************************************************************/

/**
 * @ingroup 
 * @author Samuel Rowe
 * @since Zen 1.0
 */
enum zen_ExpressionAnnotation_t {
    ZEN_EXPRESSION_ANNOTATION_UNKNOWN,
    ZEN_EXPRESSION_ANNOTATION_PLACEHOLDER,
    ZEN_EXPRESSION_ANNOTATION_VALUE
};

typedef enum zen_ExpressionAnnotation_t zen_ExpressionAnnotation_t;

#endif /* COM_ONECUBE_ZEN_COMPILER_AST_EXPRESSION_ANNOTATION_H */