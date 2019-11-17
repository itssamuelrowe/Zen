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

#include <com/onecube/zen/feb/Instruction.h>
#include <com/onecube/zen/core/Integer.h>
#include <com/onecube/zen/CString.h>

#define ZEN_ZVM_SPECIFICATION_VIEWER_MAJOR_VERSION 1
#define ZEN_ZVM_SPECIFICATION_VIEWER_MINOR_VERSION 0
#define ZEN_ZVM_SPECIFICATION_VIEWER_PATCH_VERSION 0

void zen_AvmSpecificationViewer_printHelp() {
}

int32_t main(int32_t size, char** arguments) {
    if (size == 0) {
        zen_AvmSpecificationViewer_printHelp();
    }
    else {
        int32_t i;
        for (i = 1; i < size; i++) {
            const uint8_t* text = arguments[i];
            int32_t length = jtk_CString_getSize(text);
            if (((length == 2) && jtk_CString_equals(text, length, "-i", 2)) ||
                ((length == 13) && jtk_CString_equals(text, length, "--instruction", 13))) {
                if (i + 1 < size) {
                    const uint8_t* parameter = arguments[i + 1];
                    zen_ByteCode_t byteCode = (zen_ByteCode_t)zen_Integer_parse(parameter, 10);
                    zen_Instruction_t* instruction = zen_Instruction_getInstance(byteCode);
                    const uint8_t* instructionText = zen_Instruction_getText(instruction);
                    fprintf(stdout, "Instruction\t:\t%s\nByte Code\t:\t0x%X (%d)\n", instructionText, byteCode, byteCode);
                    i++;
                }
            }
        }
    }

    fprintf(stdout, "\n----------------------------------------------------------\n");
    fprintf(stdout, "ZVM Specification    :    9\n");
    fprintf(stdout, "Tool                 :    ZVM Specification Viewer\n");
    fprintf(stdout, "Tool Version         :    %d.%d.%d\n", ZEN_ZVM_SPECIFICATION_VIEWER_MAJOR_VERSION, ZEN_ZVM_SPECIFICATION_VIEWER_MINOR_VERSION, ZEN_ZVM_SPECIFICATION_VIEWER_PATCH_VERSION);
    fprintf(stdout, "This document was automatically generated. Please do not edit without appropriate annotations and documentation.\n");

    return 0;
}