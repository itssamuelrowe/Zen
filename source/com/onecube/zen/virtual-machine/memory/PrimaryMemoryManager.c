// Sunday, January 27, 2019

#include <com/onecube/zen/virtual-machine/memory/PrimaryMemoryManager.h>

/*******************************************************************************
 * PrimaryMemoryManager                                                        *
 *******************************************************************************/

/* Allocate */

uint8_t* zen_PrimaryMemoryManager_allocateBooleanArray(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_BOOLEAN);
}

uint8_t* zen_PrimaryMemoryManager_allocateInteger8Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_INTEGER_8);
}

uint8_t* zen_PrimaryMemoryManager_allocateInteger16Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_INTEGER_16);
}

uint8_t* zen_PrimaryMemoryManager_allocateInteger32Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_INTEGER_32);
}

uint8_t* zen_PrimaryMemoryManager_allocateInteger64Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_INTEGER_64);
}

uint8_t* zen_PrimaryMemoryManager_allocateDecimal32Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_DECIMAL_32);
}

uint8_t* zen_PrimaryMemoryManager_allocateDecimal64Array(
    zen_PrimaryMemoryManager_t* primaryMemoryManager, int32_t length) {
    jtk_Assert_assertObject(primaryMemoryManager, "The specified primary memory manager is null.");
    jtk_Assert_assertTrue(length >= 0, "The specified array length is invalid.");

    return zen_PrimaryMemoryManager_allocatePrimitiveArray(primaryMemoryManager,
        length, ZEN_PRIMITIVE_TYPE_DECIMAL_64);
}