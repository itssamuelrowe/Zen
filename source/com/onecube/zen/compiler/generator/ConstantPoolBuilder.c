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

// Monday, October 07, 2019

#include <jtk/core/CString.h>
#include <jtk/collection/array/Arrays.h>

#include <com/onecube/zen/compiler/generator/ConstantPoolBuilder.h>


/*******************************************************************************
 * ConstantPoolBuilder                                                         *
 *******************************************************************************/

// Constructor

zen_ConstantPoolBuilder_t* zen_ConstantPoolBuilder_new() {
    zen_ConstantPoolBuilder_t* builder = zen_Memory_allocate(zen_ConstantPoolBuilder_t, 1);
    builder->m_entries = jtk_ArrayList_new();
    /* The first slot of the constant pool is reserved. */
    jtk_ArrayList_add(builder->m_entries, NULL);

    return builder;
}

// Destructor

void zen_ConstantPoolBuilder_delete(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    int32_t size = jtk_ArrayList_getSize(builder->m_entries);
    int32_t i;
    for (i = 1; i < size; i++) {
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

// Clear

void zen_ConstantPoolBuilder_clear(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    int32_t size = jtk_ArrayList_getSize(builder->m_entries);
    int32_t i;
    for (i = 1; i < size; i++) {
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

    jtk_ArrayList_clear(builder->m_entries);
}

// Reset

void zen_ConstantPoolBuilder_reset(zen_ConstantPoolBuilder_t* builder) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    zen_ConstantPoolBuilder_clear(builder);
    /* The first slot of the constant pool is reserved. */
    jtk_ArrayList_add(builder->m_entries, NULL);
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

// Class Entry

zen_ConstantPoolClass_t* zen_ConstantPoolBuilder_getClassEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return (zen_ConstantPoolClass_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getClassEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    /* 1. Find the UTF-8 entry with the given sequence of bytes.
     * 2. If step 1 found no UTF-8 entry, insert an UTF-8 entry with the given
     *    sequence of bytes.
     * 3. Find the class entry with the index of the UTF-8 entry.
     * 4. If step 3 found no class entry, insert a class entry with the index
     *    pointing to the UTF-8 entry.
     */

    /* Make sure that the UTF-8 entry exists with the given sequence of bytes.
     * Rretrieve the index of such an entry.
     */
    int32_t nameIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(builder, bytes,
        bytesSize);

    /* Retrieve the current size of the entry list. */
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);

    /* Apply linear search to find the class entry.
     * In the future, please find a better data structure (probably a dual hash map)
     * to store the entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 1; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_FUNCTION. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_CLASS) {
            /* Convert the entry to zen_ConstantPoolClass_t, to extract the index
             * of the UTF-8 entry.
             */
            zen_ConstantPoolClass_t* constantPoolClass = (zen_ConstantPoolClass_t*)entry;

            /* Compare the entry bytes to the bytes of the given string. */
            if (constantPoolClass->m_nameIndex == nameIndex) {
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
        /* Create the constant pool class entry. */
        zen_ConstantPoolClass_t* constantPoolClass = zen_Memory_allocate(zen_ConstantPoolClass_t, 1);
        /* Mark the constant pool entry with ZEN_CONSTANT_POOL_TAG_CLASS. */
        constantPoolClass->m_tag = ZEN_CONSTANT_POOL_TAG_CLASS;
        /* Update the class index of the class entry to point to the UTF-8
         * entry which holds its content.
         */
        constantPoolClass->m_nameIndex = nameIndex;

        /* Add the new constant pool class entry to the list. */
        jtk_ArrayList_add(builder->m_entries, constantPoolClass);

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;
    }

    return result;
}

// Field Entry

zen_ConstantPoolField_t* zen_ConstantPoolBuilder_getFieldEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return (zen_ConstantPoolField_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getFieldEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, const uint8_t* class0,
        int32_t classSize, const uint8_t* descriptor, int32_t descriptorSize,
        const uint8_t* name, int32_t nameSize) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    /* 1. Find the class entry with the given sequence of bytes for class name.
     * 2. If step 1 found no class entry, insert an UTF-8 entry with the given
     *    sequence of bytes.
     * 3. Find the UTF-8 entry with the given sequence of bytes for function
     *    descriptor.
     * 4. If step 1 found no UTF-8 entry, insert an UTF-8 entry with the given
     *    sequence of bytes for function descriptor.
     * 5. Find the UTF-8 entry with the given sequence of bytes for function
     *    name.
     * 6. If step 1 found no UTF-8 entry, insert an UTF-8 entry with the given
     *    sequence of bytes for function name.
     * 7. Find the field entry with the index of the UTF-8 entries found in
     *    the previous.
     * 8. If step 7 found no field entry, insert a field entry with the index
     *    pointing to the UTF-8 entries.
     */

    /* Make sure that the class entry exists with the given sequence of bytes for
     * class name.
     * Retrieve the index of such an entry.
     */
    int32_t classIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(builder,
        class0, classSize);

    /* Make sure that the UTF-8 entry exists with the given sequence of bytes for
     * function descriptor.
     * Retrieve the index of such an entry.
     */
    int32_t descriptorIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(builder,
        descriptor, descriptorSize);

    /* Make sure that the UTF-8 entry exists with the given sequence of bytes for
     * function name.
     * Retrieve the index of such an entry.
     */
    int32_t nameIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
        builder, name, nameSize);

    /* Retrieve the current size of the entry list. */
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);

    /* Apply linear search to find the field entry.
     * In the future, please find a better data structure (probably a dual hash
     * map) to store the entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 1; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_FIELD. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_FIELD) {
            /* Convert the entry to zen_ConstantPoolField_t, to extract the index
             * of the UTF-8 entries.
             */
            zen_ConstantPoolField_t* constantPoolField = (zen_ConstantPoolField_t*)entry;

            /* Compare the UTF-8 indexes of the entry to the UTF-8 indexes equivalent to
             * the given strings.
             */
            if ((constantPoolField->m_classIndex == classIndex) &&
                (constantPoolField->m_descriptorIndex == descriptorIndex) &&
                (constantPoolField->m_nameIndex == nameIndex)) {
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
        /* Create the constant pool field entry. */
        zen_ConstantPoolField_t* constantPoolField = zen_Memory_allocate(zen_ConstantPoolField_t, 1);
        /* Mark the constant pool entry with ZEN_CONSTANT_POOL_TAG_FIELD. */
        constantPoolField->m_tag = ZEN_CONSTANT_POOL_TAG_FIELD;
        /* Update the class index of the field entry to point to the class
         * entry which holds the class details.
         */
        constantPoolField->m_classIndex = classIndex;
        /* Update the descriptor index of the field entry to point to the
         * UTF-8 entry which holds the descriptor details.
         */
        constantPoolField->m_descriptorIndex = descriptorIndex;
        /* Update the name index of the function entyr to point to the
         * UTF-8 entry which holds the name details.
         */
        constantPoolField->m_nameIndex = nameIndex;

        /* Add the new constant pool class entry to the list. */
        jtk_ArrayList_add(builder->m_entries, constantPoolField);

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;
    }

    return result;
}

// Function Entry

zen_ConstantPoolFunction_t* zen_ConstantPoolBuilder_getFunctionEntry(
    zen_ConstantPoolBuilder_t* builder, int32_t index) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    return (zen_ConstantPoolFunction_t*)jtk_ArrayList_getValue(builder->m_entries, index);
}

int32_t zen_ConstantPoolBuilder_getFunctionEntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, const uint8_t* class0,
    int32_t classSize, const uint8_t* descriptor, int32_t descriptorSize,
    const uint8_t* name, int32_t nameSize, uint16_t tableIndex) {
    jtk_Assert_assertObject(builder, "The specified constant pool builder is null.");

    /* 1. Find the class entry with the given sequence of bytes for class name.
     * 2. If step 1 found no class entry, insert an UTF-8 entry with the given
     *    sequence of bytes.
     * 3. Find the UTF-8 entry with the given sequence of bytes for function
     *    descriptor.
     * 4. If step 1 found no UTF-8 entry, insert an UTF-8 entry with the given
     *    sequence of bytes for function descriptor.
     * 5. Find the UTF-8 entry with the given sequence of bytes for function
     *    name.
     * 6. If step 1 found no UTF-8 entry, insert an UTF-8 entry with the given
     *    sequence of bytes for function name.
     * 7. Find the function entry with the index of the UTF-8 entries found in
     *    the previous.
     * 8. If step 7 found no function entry, insert a function entry with the index
     *    pointing to the UTF-8 entries.
     */

    /* Make sure that the class entry exists with the given sequence of bytes for
     * class name.
     * Retrieve the index of such an entry.
     */
    int32_t classIndex = zen_ConstantPoolBuilder_getClassEntryIndexEx(builder,
        class0, classSize);

    /* Make sure that the UTF-8 entry exists with the given sequence of bytes for
     * function descriptor.
     * Retrieve the index of such an entry.
     */
    int32_t descriptorIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(builder,
        descriptor, descriptorSize);

    /* Make sure that the UTF-8 entry exists with the given sequence of bytes for
     * function name.
     * Retrieve the index of such an entry.
     */
    int32_t nameIndex = zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
        builder, name, nameSize);

    /* Retrieve the current size of the entry list. */
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);

    /* Apply linear search to find the function entry.
     * In the future, please find a better data structure (probably a dual hash map)
     * to store the entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 1; i < size; i++) {
        /* Retrieve the constant pool entry to test during this iteration. */
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)jtk_ArrayList_getValue(
            builder->m_entries, i);
        /* Test the entry only if it is tagged with ZEN_CONSTANT_POOL_TAG_STRING. */
        if (entry->m_tag == ZEN_CONSTANT_POOL_TAG_FUNCTION) {
            /* Convert the entry to zen_ConstantPoolFunction_t, to extract the index
             * of the UTF-8 entries.
             */
            zen_ConstantPoolFunction_t* constantPoolFunction = (zen_ConstantPoolFunction_t*)entry;

            /* Compare the UTF-8 indexes of the entry to the UTF-8 indexes equivalent to
             * the given strings.
             */
            if ((constantPoolFunction->m_classIndex == classIndex) &&
                (constantPoolFunction->m_descriptorIndex == descriptorIndex) &&
                (constantPoolFunction->m_nameIndex == nameIndex)) {
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
        /* Create the constant pool function entry. */
        zen_ConstantPoolFunction_t* constantPoolFunction = zen_Memory_allocate(zen_ConstantPoolFunction_t, 1);
        /* Mark the constant pool entry with ZEN_CONSTANT_POOL_TAG_FUNCTION. */
        constantPoolFunction->m_tag = ZEN_CONSTANT_POOL_TAG_FUNCTION;
        /* Update the class index of the function entry to point to the class
         * entry which holds the class details.
         */
        constantPoolFunction->m_classIndex = classIndex;
        /* Update the descriptor index of the function entry to point to the
         * UTF-8 entry which holds the descriptor details.
         */
        constantPoolFunction->m_descriptorIndex = descriptorIndex;
        /* Update the name index of the function entry to point to the
         * UTF-8 entry which holds the name details.
         */
        constantPoolFunction->m_nameIndex = nameIndex;
        /* Update the index at which this function exists within the function table. */
        constantPoolFunction->m_tableIndex = tableIndex;

        /* Add the new constant pool class entry to the list. */
        jtk_ArrayList_add(builder->m_entries, constantPoolFunction);

        /* The index of the newly inserted entry is equal to the current size of
         * the entry list.
         */
        result = size;
    }

    return result;
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
    for (i = 1; i < size; i++) {
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
    for (i = 1; i < size; i++) {
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
    for (i = 1; i < size; i++) {
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

int32_t zen_ConstantPoolBuilder_getUtf8EntryIndexEx(
    zen_ConstantPoolBuilder_t* builder, uint8_t* bytes, int32_t bytesSize) {
    int32_t size = jtk_ArrayList_getSize(builder->m_entries);

    /* Apply linear search to find the UTF8 entry.
     * In the future, please find a better data structure (probably a dual hash map)
     * to store the entries.
     */
    int32_t i;
    int32_t result = -1;
    for (i = 1; i < size; i++) {
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