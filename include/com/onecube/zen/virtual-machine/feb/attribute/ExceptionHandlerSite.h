// Saturday, September 22, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_HANDLER_SITE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_HANDLER_SITE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * ExceptionHandlerSite                                                        *
 *******************************************************************************/

/**
 * An exception table attribute appears for a function entity only when
 * the function handles one or more exceptions. Which means this attribute
 * may not appear if the function handles no exceptions.
 *
 * @class ExceptionHandlerSite
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExceptionHandlerSite_t {
    /**
     * A valid index into the instructions array, which indicates the starting
     * index of the range where the exception handler is active.
     * It is inclusive.
     *
     * The instructions array is located in the code attribute.
     */
    uint16_t m_startIndex;

    /**
     * A valid index into the instructions array, which indicates the stopping
     * index of the range where the exception handler is active.
     * It is inclusive.
     *
     * The instructions array is located in the code attribute.
     */
    uint16_t m_stopIndex;

    /**
     * A valid index into the instructions array. It indicates the starting
     * index of the implementation of the exception handler.
     */
    uint16_t m_handlerIndex;

    uint16_t m_exceptionClassIndex;
};

/**
 * @memberof ExceptionHandlerSite
 */
typedef struct zen_ExceptionHandlerSite_t zen_ExceptionHandlerSite_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_HANDLER_SITE_H */