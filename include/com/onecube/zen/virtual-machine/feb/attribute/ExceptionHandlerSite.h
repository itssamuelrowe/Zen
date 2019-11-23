/*
 * Copyright 2018-2019 OneCube
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

    /**
     * A valid index into the constant pool. The entry at this index should
     * be an instance of the zen_ConstantPoolClass_t structure.
     *
     * A value of zero is considered a null value. Such a value indicates
     * that the handler site is capable of handling any type of exception.
     * The reason why constant pool indexes start from 1 is apparent here.
     *
     * An alternative design to represent a generic exception handler site
     * would use the constant pool index of a class entry with the value
     * "zen.core.Throwable".
     */
    uint16_t m_exceptionClassIndex;
};

/**
 * @memberof ExceptionHandlerSite
 */
typedef struct zen_ExceptionHandlerSite_t zen_ExceptionHandlerSite_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_EXCEPTION_HANDLER_SITE_H */