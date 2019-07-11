// Thursday, July 11, 2019

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_H

/*******************************************************************************
 * LocalVariable                                                               *
 *******************************************************************************/

/**
 * @class LocalVariable
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LocalVariable_t {
    /**
     * The index at which the local variable is first visible. In reality,
     * the binary entity format does not acknowledge the concept of local varaibles.
     * Instead the compilers should be designed to handle the scopes of local
     * variables.
     *
     * Further, the specified index should be within the boundaries of the
     * instructions.
     */
    uint16_t m_startIndex;
    
    /**
     * The number of instructions that can "access" the local variable. In reality,
     * the binary entity format does not acknowledge the concept of local varaibles.
     * Instead the compilers should be designed to handle the scopes of local
     * variables.
     *
     * Further, the sum of the specified start index and count should be within
     * the boundaries of the instructions. In other words, the following condition
     * should be true.
     * `m_startIndex + m_count < instructionAttribute.m_numberOfInstructions`
     */
    uint16_t m_count;
    
    /**
     * A valid index into the constant pool. The entry at the specified index
     * should be a `ConstantPoolUtf8` instance. The descriptor represents the
     * type of the local variable.
     */
    uint16_t m_descriptorIndex;
    
    /**
     * A valid index into the constant pool. The entry at the specified index
     * should be a `ConstantPoolUtf8` instance. The name represents the identifier
     * of the local variable as it appears in the source code.
     */
    uint16_t m_nameIndex;
    
    /**
     * A valid index into the local variable array in the current frame. The entry
     * at the specified index contains the value of the variable. The entry may
     * occupy more than one spot, provided that the variable is long or double.
     */
    uint16_t m_index;
};

/**
 * @memberof LocalVariable
 */
typedef struct zen_LocalVariable_t zen_LocalVariable_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_LOCAL_VARIABLE_H */