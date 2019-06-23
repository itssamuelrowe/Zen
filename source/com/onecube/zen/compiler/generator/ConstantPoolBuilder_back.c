

int32_t zen_ConstantPoolBuilder_addConstant(zen_ConstantPoolBuilder_t* builder, uint8_t tag, ...) {
    jtk_Assert_assertObject(builder, "The specified builder is null.");

    /* Currently, the algorithm uses a linear search to determine the existence of a previous
     * copy of the given constant. This has a huge impact on the performance on the binary entity
     * generator. In the future, an ordered map should be employed for performance gain.
     */
    int32_t size = zen_ArrayList_getSize(builder->m_constants);
    int32_t i;
    int32_t result = -1;
    for (i = 0; i < size; i++) {
        zen_ConstantPoolEntry_t* entry = (zen_ConstantPoolEntry_t*)zen_ArrayList_getValue(builder->m_constants, i);
        if (entry->m_tag == tag) {
            /*
            zen_ConstantPoolEntry_t* temporaryConstant = zen_ConstantPoolEntry_new(tag, body);
            void* indexAsPointer = zen_OrderedHashMap_getValue(builder->m_constants, temporaryConstant);

            if (((indexAsPointer == NULL) && (zen_OrderedHashMap_containsKey(temporaryConstant))) ||
                (indexAsPointer != NULL)) {
                result = (uint16_t)indexAsPointer;
            }
            else {
                result = -1;
            }
            zen_ConstantPoolEntry_delete(temporaryConstant);
            return result;
            */

            bool found = false;
            switch (tag) {
                case ZEN_CONSTANT_POOL_TAG_UTF8: {
                    zen_ConstantPoolUtf8_t* previousUtf8 = (zen_ConstantPoolUtf8_t*)entry->m_body;
                    zen_ConstantPoolUtf8_t* newUtf8 = (zen_ConstantPoolUtf8_t*)body;
                    found = zen_ConstantPoolUtf8_equals(previousUtf8, newUtf8);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_INTEGER: {
                    zen_ConstantPoolInteger_t* previousInteger = (zen_ConstantPoolInteger_t*)entry->m_body;
                    zen_ConstantPoolInteger_t* newInteger = (zen_ConstantPoolInteger_t*)body;
                    found = zen_ConstantPoolInteger_equals(previousInteger, newInteger);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_LONG: {
                    zen_ConstantPoolLong_t* previousLong = (zen_ConstantPoolLong_t*)entry->m_body;
                    zen_ConstantPoolLong_t* newLong = (zen_ConstantPoolLong_t*)body;
                    found = zen_ConstantPoolLong_equals(previousLong, newLong);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_FLOAT: {
                    zen_ConstantPoolFloat_t* previousFloat = (zen_ConstantPoolFloat_t*)entry->m_body;
                    zen_ConstantPoolFloat_t* newFloat = (zen_ConstantPoolFloat_t*)body;
                    found = zen_ConstantPoolFloat_equals(previousFloat, newFloat);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_DOUBLE: {
                    zen_ConstantPoolDouble_t* previousDouble = (zen_ConstantPoolDouble_t*)entry->m_body;
                    zen_ConstantPoolDouble_t* newDouble = (zen_ConstantPoolDouble_t*)body;
                    found = zen_ConstantPoolDouble_equals(previousDouble, newDouble);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_STRING: {
                    zen_ConstantPoolString_t* previousString = (zen_ConstantPoolString_t*)entry->m_body;
                    zen_ConstantPoolString_t* newString = (zen_ConstantPoolString_t*)body;
                    found = zen_ConstantPoolString_equals(previousString, newString);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_CLASS: {
                    zen_ConstantPoolClass_t* previousClass = (zen_ConstantPoolClass_t*)entry->m_body;
                    zen_ConstantPoolClass_t* newClass = (zen_ConstantPoolClass_t*)body;
                    found = zen_ConstantPoolClass_equals(previousClass, newClass);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_FUNCTION: {
                    zen_ConstantPoolFunction_t* previousFunction = (zen_ConstantPoolFunction_t*)entry->m_body;
                    zen_ConstantPoolFunction_t* newFunction = (zen_ConstantPoolFunction_t*)body;
                    found = zen_ConstantPoolFunction_equals(previousFunction, newFunction);
                    break;
                }

                case ZEN_CONSTANT_POOL_TAG_FIELD: {
                    zen_ConstantPoolField_t* previousField = (zen_ConstantPoolField_t*)entry->m_body;
                    zen_ConstantPoolField_t* newField = (zen_ConstantPoolField_t*)body;
                    found = zen_ConstantPoolField_equals(previousField, newField);
                    break;
                }

                default: {
                    fprintf(stderr, "[internal error] Unknown constant pool tag\n");
                    break;
                }
            }

            if (found) {
                result = i;
                break;
            }
        }
    }

    if (result < 0) {
        zen_ConstantPoolEntry_t* newEntry = zen_ConstantPoolEntry_new(tag, body);
        result = zen_ArrayList_getSize(builder->m_constants);
        zen_ArrayList_add(builder->m_constants, newEntry);
    }
    
    return result;
}
