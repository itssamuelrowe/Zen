/*
 * Copyright 2018-2020 Samuel Rowe
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

// Monday, June 11, 2018

#ifndef ZEN_FEB_FEB_VERSION_H
#define ZEN_FEB_FEB_VERSION_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * FebVersion                                                                  *
 *******************************************************************************/

/**
 * Represents a version of the Binary Entity Format (FEB).
 *
 * @author Samuel Rowe
 * @since  FEB 1.0
 */
struct zen_FebVersion_t {
    /**
     * The major version of the Binary Entity Format (FEB) in which the entity
     * is encoded. Symbolically, 'M' represents the major version and 'm' represents
     * the minor version. Thus, 'M.m' represents the FEB version which may be
     * subjected to lexicographic comparision.
     */
    uint16_t m_majorVersion;

    /**
     * The minor version of the Binary Entity Format (FEB) in which the entity
     * is encoded. Symbolically, 'M' represents the major version and 'm' represents
     * the minor version. Thus, 'M.m' represents the FEB version which may be
     * subjected to lexicographic comparision.
     */
    uint16_t m_minorVersion;
};

typedef struct zen_FebVersion_t zen_FebVersion_t;

#endif /* ZEN_FEB_FEB_VERSION_H */