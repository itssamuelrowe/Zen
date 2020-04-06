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


void zen_VirtualMachineGarbageCollectorListener_onAfterInitialization(
    zen_GarbageCollectorListener_t* listener,
    zen_GarbageCollector_t* collector) {

    zen_Class_t* class0 = zen_Context_findSystemClass(zen_core_OutOfMemoryError);
    if (!zen_Context_isClear(context)) {
        ...
    }
}