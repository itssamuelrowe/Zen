// Saturday, September 22, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_EXCEPTION_TABLE_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_EXCEPTION_TABLE_ATTRIBUTE_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/ExceptionHandlerSite.h>

/*******************************************************************************
 * ExceptionTableAttribute                                                              *
 *******************************************************************************/

/**
 * @class ExceptionTableAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExceptionTableAttribute_t {
    uint16_t m_size;
    zen_ExceptionHandlerSite_t** m_exceptionHandlerSites;
};

/**
 * @memberof ExceptionTableAttribute
 */
typedef struct zen_ExceptionTableAttribute_t zen_ExceptionTableAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_EXCEPTION_TABLE_ATTRIBUTE_H */