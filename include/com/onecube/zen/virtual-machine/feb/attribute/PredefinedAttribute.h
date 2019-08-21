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

// Saturday, April 28, 2018

#ifndef ZEN_FEB_PREDEFINED_ATTIRIBUTE_H
#define ZEN_FEB_PREDEFINED_ATTIRIBUTE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Predefined Attribute                                                        *
 *******************************************************************************/

/* Primary */

#define ZEN_PREDEFINED_ATTRIBUTE_CONSTANT_VALUE "vm/primary/ConstantValue"

#define ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION "vm/primary/Instruction"
#define ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE 22

#define ZEN_PREDEFINED_ATTRIBUTE_STACK_MAP_TABLE "vm/primary/StackMapTable"
#define ZEN_PREDEFINED_ATTRIBUTE_EXCEPTION_LIST "vm/primary/ExceptionList"

/* Level 1 */

#define ZEN_PREDEFINED_ATTRIBUTE_INNER_CLASSES "vm/InnerClasses"
#define ZEN_PREDEFINED_ATTRIBUTE_INNER_ENUMERATIONS "vm/InnerEnumerations"
#define ZEN_PREDEFINED_ATTRIBUTE_ENCLOSING_FUNCTION "vm/InnerEnumerations"
#define ZEN_PREDEFINED_ATTRIBUTE_SYNTHETIC "vm/Synthetic"
#define ZEN_PREDEFINED_ATTRIBUTE_SIGNATURE "vm/Signature"
#define ZEN_PREDEFINED_ATTRIBUTE_SOURCE_FILE "vm/SourceFile"
#define ZEN_PREDEFINED_ATTRIBUTE_LINE_NUMBER_TABLE "vm/LineNumberTable"
#define ZEN_PREDEFINED_ATTRIBUTE_LOCAL_VARIABLE_TABLE "vm/LocalVariableTable"

/* Level 2 */

#define ZEN_PREDEFINED_ATTRIBUTE_DEPRECATED "vm/Deprecated"
#define ZEN_PREDEFINED_ATTRIBUTE_FUNCTION_PARAMETERS "vm/FunctionParameters"
#define ZEN_PREDEFINED_ATTRIBUTE_SOURCE_LANGUAGE "vm/SourceLanguage"
#define ZEN_PREDEFINED_ATTRIBUTE_TYPE_SYSTEM "vm/TypeSystem"
#define ZEN_PREDEFINED_ATTRIBUTE_DEVELOPEMENT_ENVIRONMENT "vm/DevelopementEnvironment"

#endif /* ZEN_FEB_PREDEFINED_ATTIRIBUTE_H */