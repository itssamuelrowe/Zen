/*
 * Copyright 2017-2020 Samuel Rowe
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

#include <com/onecube/zen/compiler/Compiler.h>

int32_t main(int32_t length, char** arguments) {
    // jtk_Assert_assertTrue(zen_Instruction_verify(), "The instruction set is invalid.");

    jtk_System_initialize();

    zen_Compiler_t* compiler = zen_Compiler_new();
    bool result = zen_Compiler_compileEx(compiler, arguments, length);
    zen_Compiler_delete(compiler);

    return result? 0 : 1;
}