// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_EXCEPTION_LIST_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_EXCEPTION_LIST_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ExceptionListAttribute                                                      *
 *******************************************************************************/

/**
 * The exception list attribute is a primary attribute, with variable length.
 * It can be associated with functions and constructors. It represents the list
 * of exceptions a function may directly throw.
 *
 * @class ExceptionListAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExceptionListAttribute_t {
    /**
     * The number of exception types the associated function or constructor
     * throws.
     */
    uint16_t m_numberOfExceptions;
    
    /**
     * An array of indexes into the constant pool. Each index must be a valid
     * index within the boundaries of the constant pool. Further, the entry at
     * the specified index should be a `ConstantPoolClass` instance.
     */
    uint16_t* m_classIndexes;
};

/**
 * @memberof ExceptionListAttribute
 */
typedef struct zen_ExceptionListAttribute_t zen_ExceptionListAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_EXCEPTION_LIST_ATTRIBUTE_H */