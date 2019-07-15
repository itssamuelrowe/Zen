// Monday, July 15, 2019

#include <jtk/core/CString.h>
#include <com/onecube/zen/virtual-machine/feb/EntityFile.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/Attribute.h>
#include <com/onecube/zen/virtual-machine/feb/attribute/PredefinedAttribute.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>
#include <com/onecube/zen/virtual-machine/object/Class.h>
#include <com/onecube/zen/virtual-machine/processor/StackFrame.h>

/*******************************************************************************
 * StackFrame                                                                  *
 *******************************************************************************/

// Constructor

zen_StackFrame_t* zen_StackFrame_new(zen_Function_t* function) {
    jtk_Assert_assertObject(function, "The specified function is null.");

    zen_Class_t* class0 = function->m_class;
    zen_EntityFile_t* entityFile = class0->m_entityFile;
    zen_FunctionEntity_t* functionEntity = function->m_functionEntity;
    zen_ConstantPool_t* constantPool = classEntity->m_constantPool;

    int32_t maxStackSize = 0;
    int32_t localVariableCount = 0;
    
    // Check if the function is native or not.

    /* Search in a linear fashion for the instruction attribute in the specified
     * function. It is recommended that the compilers generate instruction attribute
     * as the first attribute for functions in general.
     */
    zen_InstructionAttribute_t* instructionAttribute = NULL;
    int32_t limit = functionEntity->m_attributeTable.size;
    int32_t i;
    for (i = 0; i < limit; i++) {
        zen_Attribute_t* attribute = function->m_attributeTable.m_attributes[i];
        zen_ConstantPoolUtf8_t* nameEntry =
            (zen_ConstantPoolUtf8_t*)constantPool->m_entries[attribute->m_nameIndex];

        if (jtk_CString_equalsEx(nameEntry->m_bytes, nameEntry->m_length,
            ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION, ZEN_PREDEFINED_ATTRIBUTE_INSTRUCTION_SIZE)) {
            instructionAttribute = (zen_InstructionAttribute_t*)attribute;
            maxStackSize = instructionAttribute->m_maxStackSize;
            localVariableCount = instructionAttribute->m_localVariableCount
    
            break;
        }
    }

    /* At this point, the instructionAttribute variable should hold a reference
     * to an instance of zen_InstructionAttribute_t.
     */

    zen_StackFrame_t* stackFrame = jtk_Memory_allocate(zen_StackFrame_t, 1);
    stackFrame->m_operandStack = zen_OperandStack_new(maxStackSize);
    stackFrame->m_localVariableArray = zen_LocalVariableArray_new(localVariableCount);
    stackFrame->m_class = class0;
    stackFrame->m_function = function;
    stackFrame->m_instructionAttribute = instructionAttribute;
    stackFrame->m_ip = 0;

    return stackFrame;
}

// Destructor

void zen_StackFrame_delete(zen_StackFrame_t* stackFrame) {
    jtk_Assert_assertObject(stackFrame, "The specified stack frame is null.");
    
    zen_OperandStack_delete(stackFrame->m_operandStack);
    zen_LocalVariableArray_delete(stackFrame->m_localVariableArray);
    jtk_Memory_deallocate(stackFrame);
}

/* Class */

zen_Class_t* zen_StackFrame_getClass(zen_StackFrame_t* stackFrame) {
    jtk_Assert_assertObject(stackFrame, "The specified stack frame is null.");
    
    return stackFrame->m_class;
}

/* Function */

zen_Function_t* zen_StackFrame_getFunction(zen_StackFrame_t* stackFrame) {
    jtk_Assert_assertObject(stackFrame, "The specified stack frame is null.");
    
    return stackFrame->m_function;
}

/* Local Variable Array */

zen_LocalVariableArray_t* zen_StackFrame_getLocalVariableArray(zen_StackFrame_t* stackFrame) {
    jtk_Assert_assertObject(stackFrame, "The specified stack frame is null.");
    
    return stackFrame->m_localVariableArray;
}

/* Operand Stack */

zen_OperandStack_t* zen_StackFrame_getOperandStack(zen_StackFrame_t* stackFrame) {
    jtk_Assert_assertObject(stackFrame, "The specified stack frame is null.");
    
    return stackFrame->m_operandStack;
}