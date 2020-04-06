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

// Friday, January 05, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SUPPORT_STACK_TRACE_H
#define COM_ONECUBE_ZEN_COMPILER_SUPPORT_STACK_TRACE_H

/*******************************************************************************
 * StackTrace                                                                  *
 *******************************************************************************/

void zen_StackTrace_enter0(const char* file, const char* name);
void zen_StackTrace_exit0(const char* file, const char* name);

#ifdef ZEN_STACK_TRACE_ENABLE
    #define zen_StackTrace_enter() zen_StackTrace_enter0(__FILE__, __FUNCTION__)
    #define zen_StackTrace_exit() zen_StackTrace_exit0(__FILE__, __FUNCTION__)
#else
    #define zen_StackTrace_enter()
    #define zen_StackTrace_exit()
#endif

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_STACK_TRACE_H */