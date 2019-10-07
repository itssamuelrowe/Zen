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

// Monday, October 07, 2019

#include <com/onecube/zen/compiler/generator/ConstantPoolBuilder.h>


/*******************************************************************************
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

// Constructor

zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new() {
    zen_ConstantPoolBuilder_t* builder = zen_Memory_allocate(zen_ConstantPoolBuilder_t, 1);
    builder->m_entries = jtk_ArrayList_new();

    return builder;
}

// Destructor

void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    int32_t size = jtk_ArrayList_getSize(builder->m_entries);
    int32_t i;
    for (i = 0; i < size; i++) {
        /* Retrieve the constant pool entry to destroy during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* If the entry is a constant pool UTF8 entry, then it needs to be destroyed
         * specially.
         */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_UTF8) {
            /* Convert the entry to zen_ConstantPoolUtf8_t, to destroy the bytes. */
            zen_ConstantPoolUtf8_t* entry0 = (zen_ConstantPoolUtf8_t*)entry;
            /* Destroy the memory used by the entry. */
            zen_Memory_deallocate(entry0->m_bytes);
        }
        
        /* Destroy the entry. */
        zen_Memory_deallocate(entry);
    }

    jtk_ArrayList_delete(builder->m_entries);
    zen_Memory_deallocate(builder);
}

// Entries

int32_t zen_ConstantPoolBuilder_countEntries(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return jtk_ArrayList_getSize(builder->m_entries);
}

zen_ConstantPoolEntry_t* zen_ConstantPoolBuilder_getEntry(zen_ConstantPoolBuilder_t* builder,
    int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return jtk_ArrayList_getValue(builder->m_entries, index);
}

// UTF-8 Entry

zen_ConstantPoolUtf8_t* zen_ConstantPoolBuilder_getUtf8Entry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");
    
    return (zen_ConstantPoolUtf8_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getUtf8EntryIndex(
    zen_ConstantPoolBuilder_t* builder, jtk_String_t* string) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    int32_t size = jtk_ArrayList_getSize(builder->m_entries);
    /* Apply linear search to find the UTF8 entry.
     * In the future, please find a better data structure (probably a dual hash map)
     * to store the entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 0; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_UTF8. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_UTF8) {
            /* Convert the entry to zen_ConstantPoolUtf8_t, to extract the bytes. */
            zen_ConstantPoolUtf8_t* entry0 = (zen_ConstantPoolUtf8_t*)entry;
            /* Compare the entry bytes to the bytes of the given string. */
            if (jtk_CString_equals(entry0->m_bytes, entry0->m_length,
                string->m_value, string->m_size)) {
                /* Looks like we found a match! Terminate the loop and return the
                 * current index.
                 */
                result = i;
                break;
            }
        }
    }

    /* If the result is still negative, the entry was not found. In which case,
     * the entry should be appended to the end of the list.
     */
    if (result < 0) {
        zen_ConstantPoolUtf8_t* entry0 = zen_Memory_allocate(zen_ConstantPoolUtf8_t, 1);
        entry0->m_length = string->m_size;
        entry0->m_bytes = jtk_Arrays_clone_b(string->m_value, string->m_size);

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;

        /* Add the new constant pool UTF8 entry to the list. */
        jtk_ArrayList_add(builder->m_entries, entry0);
    }

    return result;
}