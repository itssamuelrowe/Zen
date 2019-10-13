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

// Integer Entry

zen_ConstantPoolInteger_t* zen_ConstantPoolBuilder_getIntegerEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return (zen_ConstantPoolInteger_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getIntegerEntryIndex(
    zen_ConstantPoolBuilder_t* builder, int32_t value) {
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);

    /* Apply linear search to find the integer entry. In the future, please
     * find a better data structure (probably a dual hash map) to store the
     * entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 0; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_INTEGER. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_INTEGER) {
            /* Convert the entry to zen_ConstantPoolInteger_t, to extract the bytes. */
            zen_ConstantPoolInteger_t* constantPoolInteger = (zen_ConstantPoolInteger_t*)entry;
            /* Compare the entry bytes to the bytes of the given integer value. */
            if (constantPoolInteger->m_bytes == value) {
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
        zen_ConstantPoolInteger_t* constantPoolInteger = zen_Memory_allocate(
            zen_ConstantPoolInteger_t, 1);
        constantPoolInteger->m_tag = ZEN_CONSTANT_POOL_TAG_INTEGER;
        
        // TODO: Am I doing it right here?!!!
        union {
            uint32_t x;
            int32_t y;
        } converter;
        converter.y = value;
        constantPoolInteger->m_bytes = converter.x;

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;

        /* Add the new constant pool integer entry to the list. */
        jtk_ArrayList_add(builder->m_entries, constantPoolInteger);
    }

    return result;
}

// Long Entry

zen_ConstantPoolLong_t* zen_ConstantPoolBuilder_getLongEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return (zen_ConstantPoolLong_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getLongEntryIndex(
    zen_ConstantPoolBuilder_t* builder, int64_t value) {
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);
    
    // TODO: Am I doing it right here?!!!
    union {
        uint64_t x;
        int64_t y;
    } converter;
    converter.y = value;

    /* Apply linear search to find the long entry. In the future, please
     * find a better data structure (probably a dual hash map) to store the
     * entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 0; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_LONG. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_LONG) {
            /* Convert the entry to zen_ConstantPoolLong_t, to extract the bytes. */
            zen_ConstantPoolLong_t* constantPoolLong = (zen_ConstantPoolLong_t*)entry;
            /* Compare the entry bytes to the bytes of the given integer value. */
            if ((constantPoolLong->m_highBytes == ((converter.y & 0xFFFFFFFF00000000ULL) >> 32)) &&
                (constantPoolLong->m_lowBytes == (converter.y & 0x00000000FFFFFFFFULL))) {
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
        zen_ConstantPoolLong_t* constantPoolLong = zen_Memory_allocate(
            zen_ConstantPoolLong_t, 1);
        constantPoolLong->m_tag = ZEN_CONSTANT_POOL_TAG_LONG;
        
        constantPoolLong->m_highBytes = (converter.x & 0xFFFFFFFF00000000ULL) >> 32;
        constantPoolLong->m_lowBytes = converter.x & 0x00000000FFFFFFFFULL;

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;

        /* Add the new constant pool integer entry to the list. */
        jtk_ArrayList_add(builder->m_entries, constantPoolLong);
    }

    return result;
}

// String Entry

zen_ConstantPoolString_t* zen_ConstantPoolBuilder_getStringEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return (zen_ConstantPoolString_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getStringEntryIndex(
    zen_ConstantPoolBuilder_t* builder, jtk_String_t* string) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return zen_ConstantPoolBuilder_getStringEntryIndexEx(builder, string->m_value,
        string->m_size);
}

int32_t zen_ConstantPoolBuilder_getStringEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    /* 1. Find the UTF-8 entry with the given sequence of bytes.
     * 2. If step 1 found no UTF-8 entry, insert an UTF-8 entry with the given
     *    sequence of bytes.
     * 3. Find the string entry with the index of the UTF-8 entry.
     * 4. If step 3 found no string entry, insert a string entry with the index
     *    pointing to the UTF-8 entry.
     */

    /* Make sure that the UTF-8 entry exists with the given sequence of bytes.
     * Rretrieve the index of such an entry.
     */
    int32_t stringIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(builder, bytes,
        bytesSize);

    /* Retrieve the current size of the entry list. */
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);

    /* Apply linear search to find the string entry.
     * In the future, please find a better data structure (probably a dual hash map)
     * to store the entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 0; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_STRING. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_STRING) {
            /* Convert the entry to zen_ConstantPoolString_t, to extract the index
             * of the UTF-8 entry.
             */
            zen_ConstantPoolString_t* constantPoolString = (zen_ConstantPoolString_t*)entry;

            /* Compare the entry bytes to the bytes of the given string. */
            if (constantPoolString->m_stringIndex == stringIndex) {
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
        /* Create the constant pool string entry. */
        zen_ConstantPoolString_t* constantPoolString = zen_Memory_allocate(zen_ConstantPoolString_t, 1);
        /* Mark the constant pool entry with ZEN_CONSTANT_POOL_TAG_STRING. */
        constantPoolString->m_tag = ZEN_CONSTANT_POOL_TAG_STRING;
        /* Update the string index of the string entry to point to the UTF-8
         * entry which holds its content.
         */
        constantPoolString->m_stringIndex = stringIndex;

        /* Add the new constant pool string entry to the list. */
        jtk_ArrayList_add(builder->m_entries, constantPoolString);

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;
    }

    return result;
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

    return zen_ConstantPoolBuilder_getUtf8EntryIndexEx(builder, string->m_value,
        string->m_size);
}

int32_t zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize) {
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
                bytes, bytesSize)) {
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
        entry0->m_tag = ZEN_CONSTANT_POOL_TAG_UTF8;
        entry0->m_length = bytesSize;
        entry0->m_bytes = jtk_Arrays_clone_b(bytes, bytesSize);

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;

        /* Add the new constant pool UTF8 entry to the list. */
        jtk_ArrayList_add(builder->m_entries, entry0);
    }

    return result;
}