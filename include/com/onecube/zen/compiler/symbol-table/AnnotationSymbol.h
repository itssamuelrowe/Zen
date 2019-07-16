// Sunday, January 07, 2018

#ifndef ZEN_ST_ANNOTATION_SYMBOL_H
#define ZEN_ST_ANNOTATION_SYMBOL_H

#include <com/onecube/zen/collection/ArrayList.h>
#include <com/onecube/zen/st/Symbol.h>

/*******************************************************************************
 * AnnotationSymbol                                                            *
 *******************************************************************************/

struct zen_AnnotationSymbol_t {
    zen_Symbol_t* m_symbol;
    jtk_ArrayList_t* m_superAnnotations; /* <zen_AnnotationSymbol_t*> */
};

typedef struct zen_AnnotationSymbol_t zen_AnnotationSymbol_t;

zen_AnnotationSymbol_t* zen_AnnotationSymbol_new(zen_ASTNode_t* identifier,
    jtk_ArrayList_t* superAnnotations);
void zen_AnnotationSymbol_delete(zen_AnnotationSymbol_t* symbol);

#endif /* ZEN_ST_ANNOTATION_SYMBOL_H */