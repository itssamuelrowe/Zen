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

// Monday, June 11, 2018

#include <com/onecube/zen/vm/BinaryEntityVerifier.h>

/*******************************************************************************
 * BinaryEntityVerifier                                                        *
 *******************************************************************************/

void zen_BinaryEntityVerifier_verifyMagicNumber(zen_BinaryEntityVerifier_t* verifier,
    uint32_t magicNumber) {
    jtk_Assert_assertObject(verifier, "The specified binary entity verifier is null.");

    if (magicNumber != ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER) {
        zen_Logger_e("Binary entity file magic number verified: incompatible");
        // throw UnknownEntityMagicNumberException
    }
}

void zen_BinaryEntityVerifier_verifyVersion(zen_BinaryEntityVerifier_t* verifier,
    zen_FebVersion_t* fileVersion) {
    jtk_Assert_assertObject(verifier, "The specified binary entity verifier is null.");

    zen_FebVersion_t* machineVersion = zen_VirtualMachine_getFebVersion(verifier->m_machine);
    if (zen_FebVersionHelper_compare(fileVersion, machineVersion) >= 0) {
        zen_Logger_i("Binary entity file version verified: compatibile");
    }
    else {
        zen_Logger_e("Binary entity file version verified: incompatible");
        // throw UnsupportedEntityVersionException
    }
}