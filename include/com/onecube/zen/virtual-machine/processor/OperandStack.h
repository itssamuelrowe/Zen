// Sunday, September 06, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_OPERAND_STACK_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_OPERAND_STACK_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/memory/MemoryManager.h>

/*******************************************************************************
 * OperandStack                                                                *
 *******************************************************************************/

/**
 * @class OperandStack
 * @ingroup zen_vm_processor
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_OperandStack_t {
    int32_t* m_values;
    int32_t m_size;
    int32_t m_capacity;
};

/**
 * @memberof OperandStack
 */
typedef struct zen_OperandStack_t zen_OperandStack_t;

/* Constructor */

/**
 * @memberof OperandStack
 */
zen_OperandStack_t* zen_OperandStack_new(int32_t capacity);

/* Destructor */

/**
 * @memberof OperandStack
 */
void zen_OperandStack_delete(zen_OperandStack_t* operandStack);

/* Available */

/**
 * @memberof OperandStack
 */
bool zen_OperandStack_isOperandAvailable(zen_OperandStack_t* operandStack, int32_t count);

/**
 * @memberof OperandStack
 */
bool zen_OperandStack_isSpaceAvailable(zen_OperandStack_t* operandStack, int32_t count);

/* Double */

/**
 * @memberof OperandStack
 */
double zen_OperandStack_peekDouble(zen_OperandStack_t* operandStack);

/**
 * @memberof OperandStack
 */
void zen_OperandStack_pushDouble(zen_OperandStack_t* operandStack, double value);

/**
 * @memberof OperandStack
 */
double zen_OperandStack_popDouble(zen_OperandStack_t* operandStack);

/* Float */

/**
 * @memberof OperandStack
 */
float zen_OperandStack_peekFloat(zen_OperandStack_t* operandStack);
 
/**
 * @memberof OperandStack
 */
void zen_OperandStack_pushFloat(zen_OperandStack_t* operandStack, float value);

/**
 * @memberof OperandStack
 */
float zen_OperandStack_popFloat(zen_OperandStack_t* operandStack);

/* Integer */

/**
 * @memberof OperandStack
 */
int32_t zen_OperandStack_peekInteger(zen_OperandStack_t* operandStack);

/**
 * @memberof OperandStack
 */
void zen_OperandStack_pushInteger(zen_OperandStack_t* operandStack, int32_t value);

/**
 * @memberof OperandStack
 */
int32_t zen_OperandStack_popInteger(zen_OperandStack_t* operandStack);

/* Long */

/**
 * @memberof OperandStack
 */
int64_t zen_OperandStack_peekLong(zen_OperandStack_t* operandStack);
 
/**
 * @memberof OperandStack
 */
void zen_OperandStack_pushLong(zen_OperandStack_t* operandStack, int64_t value);

/**
 * @memberof OperandStack
 */
int64_t zen_OperandStack_popLong(zen_OperandStack_t* operandStack);

/* Reference */

/**
 * @memberof OperandStack
 */
uintptr_t zen_OperandStack_peekReference(zen_OperandStack_t* operandStack);
 
/**
 * @memberof OperandStack
 */
void zen_OperandStack_pushReference(zen_OperandStack_t* operandStack, uintptr_t value);

/**
 * @memberof OperandStack
 */
uintptr_t zen_OperandStack_popReference(zen_OperandStack_t* operandStack);


/* Swap */

/**
 * @memberof OperandStack
 */
void zen_OperandStack_swap(zen_OperandStack_t* operandStack);

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_PROCESSOR_OPERAND_STACK_H */