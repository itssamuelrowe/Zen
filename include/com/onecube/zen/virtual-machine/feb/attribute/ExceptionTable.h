// Saturday, September 22, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_TABLE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_TABLE_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/ExceptionHandlerSite.h>

/*******************************************************************************
 * ExceptionTable                                                              *
 *******************************************************************************/

/**
 * @class ExceptionTable
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExceptionTable_t {
    uint16_t m_size;
    zen_ExceptionHandlerSite_t** m_exceptionHandlerSites;
};

/**
 * @memberof ExceptionTable
 */
typedef struct zen_ExceptionTable_t zen_ExceptionTable_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_TABLE_H */