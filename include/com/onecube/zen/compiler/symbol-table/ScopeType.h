// Sunday, January 07, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SCOPE_TYPE_H
#define COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SCOPE_TYPE_H

/*******************************************************************************
 * ScopeType                                                                   *
 *******************************************************************************/

/**
 * @class ScopeType
 * @ingroup zen_compiler_symbolTable
 * @author Samuel Rowe
 * @since zen 1.0
 */
enum zen_ScopeType_t {
    ZEN_SCOPE_COMPILATION_UNIT,
    ZEN_SCOPE_ANNOTATION,
    ZEN_SCOPE_CLASS,
    ZEN_SCOPE_FUNCTION,
    ZEN_SCOPE_ENUMERATION,
    ZEN_SCOPE_LOCAL
};

/**
 * @memberof ScopeType
 */
typedef enum zen_ScopeType_t zen_ScopeType_t;
 
#endif /* COM_ONECUBE_ZEN_COMPILER_SYMBOL_TABLE_SCOPE_TYPE_H */