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

// Friday, April 17, 2019

#include <jtk/core/CString.h>
#include <jtk/collection/array/Arrays.h>
#include <com/onecube/zen/disassembler/BinaryEntityDisassembler.h>

int32_t main(int32_t length, char** arguments) {
    zen_Instruction_verify();

    zen_BinaryEntityDisassembler_t* disassembler = zen_BinaryEntityDisassembler_new(NULL);
    zen_BinaryEntityDisassembler_addDirectory(disassembler, ".", 1);

    int32_t descriptorSize = -1;
    uint8_t* descriptor = jtk_CString_make(arguments[1], &descriptorSize);
    jtk_Arrays_replace_b(descriptor, descriptorSize, '.', '/');
    zen_BinaryEntityDisassembler_disassembleClass(disassembler, descriptor,
        descriptorSize);
    jtk_CString_delete(descriptor);

    zen_BinaryEntityDisassembler_delete(disassembler);

    return 0;
}