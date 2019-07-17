// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ANNOTATION_SYMBOL_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ANNOTATION_SYMBOL_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/compiler/symbol-table/Symbol.h>

/*******************************************************************************
 * AnnotationSymbol                                                            *
 *******************************************************************************/

/**
 * @class AnnotationSymbol
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_superAnnotations; /* <zen_AnnotationSymbol_t*> */
};

/**
 * @memberof AnnotationSymbol
 */
typedef struct zen_AnnotationSymbol_t zen_AnnotationSymbol_t;

// Constructor

/**
 * @memberof AnnotationSymbol
 */
zen_AnnotationSymbol_t* zen_AnnotationSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* superAnnotations);

// Destructor

/**
 * @memberof AnnotationSymbol
 */
void zen_AnnotationSymbol_delete(zen_AnnotationSymbol_t* symbol);

#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_ANNOTATION_SYMBOL_H */