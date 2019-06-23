// Sunday, January 07, 2018

#ifndef ZEN_ST_SCOPE_TYPE_H
#define ZEN_ST_SCOPE_TYPE_H

/*******************************************************************************
 * ScopeType                                                                   *
 *******************************************************************************/

enum zen_ScopeType_t {
    ZEN_SCOPE_COMPILATION_UNIT,
    ZEN_SCOPE_ANNOTATION,
    ZEN_SCOPE_CLASS,
    ZEN_SCOPE_FUNCTION,
    ZEN_SCOPE_ENUMERATION,
    ZEN_SCOPE_LOCAL
};

typedef enum zen_ScopeType_t zen_ScopeType_t;
 
#endif /* ZEN_ST_SCOPE_TYPE_H */