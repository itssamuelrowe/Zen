// Saturday, March 17, 2018

#ifndef ZEN_VM_BYTE_CODE_H
#define ZEN_VM_BYTE_CODE_H

#include <com/onecube/zen/Configuration.h>

/*
 * For every symbol the definition phase listener encounters, a new entry is
 * add to the constant pool.
 * For every symbol the resolution phase listener encounters, the index of
 * the entry previously added is referenced.
 */

/*******************************************************************************
 * ByteCode                                                                    *
 *******************************************************************************/

/* NOTE: Please update the "Instruction.c" file if enumerations are added or
 * removed from this file.
 */
enum zen_ByteCode_t {

    /* No Operation */

    ZEN_BYTE_CODE_NOP,

    /* Add */

    /**
     * Add two {@code int} values.
     *
     * Two {@code int} operands are popped of the operand stack. An {@code int}
     * result is evaluated by adding both the operads, shown as {@code value_1
     * + value_2}.
     *
     * If an overflow occurs, the bits of the result is truncated to preserve
     * only the 32-bits from the Least Significant Bit (LSB).
     *
     * Further, an overflow never raises an exception.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code int} value. It is popped off the operand stack.
     * value_2
     *     A {@code int} value. It is popped off the operand stack.
     * result
     *     A {@code int} value which is computed by addition of the
     *     specified {@code int} values.
     */
    ZEN_BYTE_CODE_ADD_I,

    /**
     * Add two {@code long} values.
     *
     * Two {@code long} operands are popped of the operand stack. An {@code long}
     * result is evaluated by adding both the operads, shown as {@code value_1
     * + value_2}.
     *
     * If an overflow occurs, the bits of the result is truncated to preserve
     * only the 64-bits from the Least Significant Bit (LSB).
     *
     * Further, an overflow never raises an exception.
     *
     * [Format]
     * add_l
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code long} value. It is popped off the operand stack.
     * value_2
     *     A {@code long} value. It is popped off the operand stack.
     * result
     *     A {@code long} value which is computed by addition of the
     *     specified {@code long} values.
     */
    ZEN_BYTE_CODE_ADD_L,

    /**
     * Add two {@code float} values.
     *
     * The following rules, as specified by IEEE 754, should be observed when
     * adding two {@code float} values.
     *     - If either the first or the second operand is NaN, the result is
     *       always NaN.
     *     - The sum of two infinities of opposite sign is NaN.
     *     - The sum of two infinities of the same sign is the infinity of
     *       the sign in question.
     *     - The sum of an infinity and any finite value is infinity.
     *     - The sum of two zeroes of opposite sign is positive zero.
     *     - The sum of two zeroes of the same sign is the zero of the sign
     *       in question.
     *     - The sum of a zero and a non-zero finite value is the non-zero
     *       value.
     *     - The sum of two non-zero finite values of the same magnitude and
     *       opposite sign is positive zero.
     *     - In the remaining cases, where neither operand is an infinity, a
     *       zero, or NaN and the values have the same sign or have different
     *       magnitudes, the sum is computed and rounded to the nearest
     *       representable value using IEEE 754 round to nearest mode. If
     *       the magnitude is too large to represent as a {@code float},
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a {@code float}, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * add_f
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code float} value. It is popped off the operand stack.
     * value_2
     *     A {@code float} value. It is popped off the operand stack.
     * result
     *     A {@code float} value which is computed by addition of the
     *     specified {@code float} values.
     */
    ZEN_BYTE_CODE_ADD_F,

    /**
     * Add two {@code double} values.
     *
     * The following rules, as specified by IEEE 754, should be observed when
     * adding two {@code double} values.
     *     - If either the first or the second operand is NaN, the result is
     *       always NaN.
     *     - The sum of two infinities of opposite sign is NaN.
     *     - The sum of two infinities of the same sign is the infinity of
     *       the sign in question.
     *     - The sum of an infinity and any finite value is infinity.
     *     - The sum of two zeroes of opposite sign is positive zero.
     *     - The sum of two zeroes of the same sign is the zero of the sign
     *       in question.
     *     - The sum of a zero and a non-zero finite value is the non-zero
     *       value.
     *     - The sum of two non-zero finite values of the same magnitude and
     *       opposite sign is positive zero.
     *     - In the remaining cases, where neither operand is an infinity, a
     *       zero, or NaN and the values have the same sign or have different
     *       magnitudes, the sum is computed and rounded to the nearest
     *       representable value using IEEE 754 round to nearest mode. If
     *       the magnitude is too large to represent as a {@code double},
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a {@code double}, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * add_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code double} value. It is popped off the operand stack.
     * value_2
     *     A {@code double} value. It is popped off the operand stack.
     * result
     *     A {@code double} value which is computed by addition of the
     *     specified {@code double} values.
     */
    ZEN_BYTE_CODE_ADD_D,

    /* Bitwise AND */

    /**
     * Bitwise AND of two 32-bit integer values.
     *
     * Two 32-bit integer operands are popped of the operand stack. A 32-bit
     * integer result is evaluated by performing a  conjunction (bitwise AND)
     * of both the operads, shown as {@code value_1 & value_2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value_2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a conjunction
     *     of both the specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_AND_I,

    /**
     * Bitwise AND of two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped of the operand stack. A 64-bit
     * integer result is evaluated by performing a  conjunction (bitwise AND)
     * of both the operads, shown as {@code value_1 & value_2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value_2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing a conjunction
     *     of both the specified 64-bit integer values.
     */
    ZEN_BYTE_CODE_AND_L,

    /* Bitwise OR */

    /**
     * Bitwise OR of two 32-bit integer values.
     *
     * Two 32-bit integer operands are popped of the operand stack. A 32-bit
     * integer result is evaluated by performing a  disjunction (bitwise OR)
     * of both the operads, shown as {@code value_1 | value_2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value_2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a disjunction
     *     of both the specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_OR_I,

    /**
     * Bitwise OR of two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped of the operand stack. A 64-bit
     * integer result is evaluated by performing a  disjunction (bitwise OR)
     * of both the operads, shown as {@code value_1 | value_2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value_2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing a disjunction
     *     of both the specified 64-bit integer values.
     */
    ZEN_BYTE_CODE_OR_L,

    /* Bitwise Shift */

    ZEN_BYTE_CODE_SHIFT_LEFT_I,
    ZEN_BYTE_CODE_SHIFT_LEFT_L,
    ZEN_BYTE_CODE_SHIFT_RIGHT_I,
    ZEN_BYTE_CODE_SHIFT_RIGHT_L,
    ZEN_BYTE_CODE_SHIFT_RIGHT_UI,
    ZEN_BYTE_CODE_SHIFT_RIGHT_UL,

    /* Bitwise XOR */

    ZEN_BYTE_CODE_XOR_I,
    ZEN_BYTE_CODE_XOR_L,

    /* Cast */

    ZEN_BYTE_CODE_CAST_ITL,
    ZEN_BYTE_CODE_CAST_ITF,
    ZEN_BYTE_CODE_CAST_ITD,
    ZEN_BYTE_CODE_CAST_LTI,
    ZEN_BYTE_CODE_CAST_LTF,
    ZEN_BYTE_CODE_CAST_LTD,

    /**
     * Convert a {@code float} value to a {@code integer} value.
     *
     * The following cases should be carefully observed:
     *  - A NaN value is always converted to {@code 0}.
     *  - A finite value is rounded to an integer value using IEEE 754 round
     *    towards zero mode.
     *  - A negative value of very large magnitude or negative infinity is
     *    represented by the smallest integer value that occupies 32-bits.
     *  - A positive value of very large magnitude or positive infinity is
     *    represented by the largest integer value that occupies 32-bits.
     *
     * The instruction narrows the {@code float} value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * [Format]
     * cast_fi
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit {@code float} value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     {@code float} value to {@code integer} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_FTI,

    /**
     * Convert a {@code float} value to a {@code double} value.
     *
     * The following cases should be carefully observed:
     *  - A NaN value is always converted to {@code 0}.
     *  - A finite value is rounded to an integer value using IEEE 754 round
     *    towards zero mode.
     *  - A negative value of very large magnitude or negative infinity is
     *    represented by the smallest signed integer value that occupies
     *    64-bits.
     *  - A positive value of very large magnitude or positive infinity is
     *    represented by the largest signed integer value that occupies
     *    64-bits.
     *
     * The instruction narrows the {@code double} value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * [Format]
     * cast_fl
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit {@code float} value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value derived from converting the specified
     *     {@code float} value to a {@code long} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_FTL,

    /**
     * Convert a {@code float} value to a {@code double} value.
     *
     * The specified {@code float} value is converted to a {@code double}
     * value using IEEE 754 round to nearest mode. In other words, the
     * instruction widens the {@code float} value. Therefore, no loss of
     * information in regards to magnitude and precision may be observed.
     *
     * [Format]
     * cast_fd
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit {@code float} value. It is popped off the operand stack.
     * result
     *     A 64-bit {@code double} value derived from converting the specified
     *     {@code float} value to {@code double} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_FTD,

    /**
     * Convert a {@code double} value to an {@code integer} value.
     *
     * The following cases should be carefully observed:
     *  - A NaN value is always converted to {@code 0}.
     *  - A finite value is rounded to an integer value using IEEE 754 round
     *    towards zero mode.
     *  - A negative value of very large magnitude or negative infinity is
     *    represented by the smallest integer value that occupies 32-bits.
     *  - A positive value of very large magnitude or positive infinity is
     *    represented by the largest integer value that occupies 32-bits.
     *
     * The instruction narrows the {@code double} value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * [Format]
     * cast_di
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit {@code double} value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value derived from converting the specified
     *     {@code double} value to {@code integer} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_DTI,

    /**
     * Convert a {@code double} value to a {@code long} value.
     *
     * The following cases should be carefully observed:
     *  - A NaN value is always converted to {@code 0}.
     *  - A finite value is rounded to an integer value using IEEE 754 round
     *    towards zero mode.
     *  - A negative value of very large magnitude or negative infinity is
     *    represented by the smallest signed integer value that occupies
     *    64-bits.
     *  - A positive value of very large magnitude or positive infinity is
     *    represented by the largest signed integer value that occupies
     *    64-bits.
     *
     * The instruction narrows the {@code double} value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * [Format]
     * cast_dl
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit {@code double} value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value derived from converting the specified
     *     {@code double} value to {@code long} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_DTL,

    /**
     * Convert a {@code double} value to a {@code float} value.
     *
     * The specified {@code double} value is converted to a {@code float}
     * value using IEEE 754 round to nearest mode. In other words, the
     * instruction narrows the {@code double} value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * The following cases should be carefully observed:
     *  - Values too small to represent as a {@code float} value is represented
     *    as zero.
     *  - Values too big to represents as a {@code float} value is represented
     *    as infinity.
     * In both cases, the sign of the value is always retained.
     *
     * [Format]
     * cast_df
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit {@code double} value. It is popped off the operand stack.
     * result
     *     A 32-bit float value derived from converting the specified
     *     {@code double} value to {@code float} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_DTF,

    ZEN_BYTE_CODE_CAST_ITB,

    /* TODO: Should I encode characters in UTF-16 or UTF-8. */
    ZEN_BYTE_CODE_CAST_ITC,

    ZEN_BYTE_CODE_CAST_ITS,

    /**
     * Check whether an object is an instance of a type.
     *
     * The operand stack remains unchanged in the following cases:
     *     CASE 1: The specified reference is {@code null}.
     *     CASE 2: The object can be cast to the specified class, enumeration,
     *             or array type.
     * Otherwise, an instance of the {@code ClassCastException} class is
     * thrown.
     *
     * [Format]
     * check_cast index_byte_1 index_byte_2
     *
     * [Arguments]
     * index_byte_1
     *     The {@code index_byte_1} and {@code index_byte_2} together indicate
     *     an unsigned integer index into the run-time constant pool of the
     *     current binary entity. The item at this index represents a reference
     *     to a class, an enumeration, or an array type.
     * index_byte_2
     *     Please refer the documentation for {@code index_byte_1}.
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     ..., object_reference
     *
     * [Operands]
     * object_reference
     *     A reference to the object to cast.
     *
     * [Exception]
     * Resolution Exceptions
     *     During resolution of the class, enumeration, or array types, any of
     *     the documented exceptions for type resolution may be thrown.
     * zen.base.ClassCastException
     *     An instance of the {@code ClassCastException} class is thrown
     *     if the specified object cannot be cast to the specified type.
     */
    ZEN_BYTE_CODE_CHECK_CAST,

    /* Compare */

    ZEN_BYTE_CODE_COMPARE_L,

    /**
     * Compare two {@code float} values.
     *
     * If at least one of the values is NaN, the pair of values is unordered.
     * Therefore, in order to prevent failure in comparison of such a pair and
     * also compute consistent results the {@code compare_lt_f} instruction
     * works relatively to the {@code compare_gt_f} instruction.
     *
     * [Format]
     * compare_lt_f
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code float} value. It is popped off the operand stack.
     * value_2
     *     A {@code float} value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If {@code value_1} is greater than {@code value_2}, then
     *           the result is a positive integer value {@code 1}.
     *         - If {@code value_1} is lesser than {@code value_2}, then
     *           the result is a negative integer value {@code -1}.
     *         - If {@code value_1} is equal to {@code value_2}, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code -1}.
     */
    ZEN_BYTE_CODE_COMPARE_LT_F,

    /**
     * Compare two {@code float} values.
     *
     * If at least one of the values is NaN, the pair of values is unordered.
     * Therefore, in order to prevent failure in comparison of such a pair and
     * also compute consistent results the {@code compare_gt_d} instruction
     * works relatively to the {@code compare_lt_d} instruction.
     *
     * [Format]
     * compare_gt_f
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code float} value. It is popped off the operand stack.
     * value_2
     *     A {@code float} value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If {@code value_1} is greater than {@code value_2}, then
     *           the result is a positive integer value {@code 1}.
     *         - If {@code value_1} is lesser than {@code value_2}, then
     *           the result is a negative integer value {@code -1}.
     *         - If {@code value_1} is equal to {@code value_2}, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code 1}.
     */
    ZEN_BYTE_CODE_COMPARE_GT_F,

    /**
     * Compare two {@code double} values.
     *
     * If at least one of the values is NaN, the pair of values is unordered.
     * Therefore, in order to prevent failure in comparison of such a pair and
     * also compute consistent results the {@code compare_lt_d} instruction
     * works relatively to the {@code compare_gt_d} instruction.
     *
     * [Format]
     * compare_lt_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code double} value. It is popped off the operand stack.
     * value_2
     *     A {@code double} value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If {@code value_1} is greater than {@code value_2}, then
     *           the result is a positive integer value {@code 1}.
     *         - If {@code value_1} is lesser than {@code value_2}, then
     *           the result is a negative integer value {@code -1}.
     *         - If {@code value_1} is equal to {@code value_2}, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code -1}.
     */
    ZEN_BYTE_CODE_COMPARE_LT_D,

    /**
     * Compare two {@code double} values.
     *
     * If at least one of the values is NaN, the pair of values is unordered.
     * Therefore, in order to prevent failure in comparison of such a pair and
     * also compute consistent results the {@code compare_gt_d} instruction
     * works relatively to the {@code compare_lt_d} instruction.
     *
     * [Format]
     * compare_gt_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code double} value. It is popped off the operand stack.
     * value_2
     *     A {@code double} value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If {@code value_1} is greater than {@code value_2}, then
     *           the result is a positive integer value {@code 1}.
     *         - If {@code value_1} is lesser than {@code value_2}, then
     *           the result is a negative integer value {@code -1}.
     *         - If {@code value_1} is equal to {@code value_2}, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code 1}.
     */
    ZEN_BYTE_CODE_COMPARE_GT_D,

    /* Divide */

    ZEN_BYTE_CODE_DIVIDE_I,
    ZEN_BYTE_CODE_DIVIDE_L,

    /**
     * Divide two {@code float} values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when dividing two {@code float} values.
     *     - If either the first or the second operand is NaN, the result is
     *       always NaN.
     *     - The result is positive if both the operands have the same sign.
     *     - The result is negative if both the operands have different signs.
     *     - The result is NaN if both the operands are infinities, regardless
     *       of their signs.
     *     - The result is positive infinity if an infinity is divided by a
     *       finite value with the same sign as the infinity.
     *     - The result is negative infinity if an infinity is divided by a
     *       finite value with the sign opposite to the sign of the infinity.
     *     - The result is positive zero if a finite value is divied by an
     *       infinity with the same sign as the finite value.
     *     - The result is negative zero if a finite value is divied by an
     *       infinity with the sign opposite to the finite values sign.
     *     - The result is NaN if a zero is divided by a zero, regardless of
     *       the signs.
     *     - The result is positive zero if a zero is divided by a finite
     *       value with the same sign as the zero.
     *     - The result is negative zero if a zero is divided by a finite
     *       value with the sign opposite to the sign of zero.
     *     - The result is positive infinity if a finite value (other than
     *       zero) is divided by zero with the same sign as the finite value.
     *     - The result is negative infinity if a finite value (other than
     *       zero) is divided by zero with the sign opposite to the sign of
     *       the finite value.
     *     - In all other cases, where neither operand is an infinity, a
     *       zero, or NaN, the quotient is computed and rounded to the nearest
     *       representable value using IEEE 754 round to nearest mode. If
     *       the magnitude is too large to represent as a {@code float},
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a {@code float}, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * divide_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code float} value. It is popped off the operand stack.
     * value_2
     *     A {@code float} value. It is popped off the operand stack.
     * result
     *     A {@code float} value which is the quotient computed by the
     *     division of {@code value1} and {@code value2}, represented as
     *     {@code value1 / value2}.
     */
    ZEN_BYTE_CODE_DIVIDE_F,

    /**
     * Divide two {@code double} values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when dividing two {@code double} values.
     *     - If either the first or the second operand is NaN, the result is
     *       always NaN.
     *     - The result is positive if both the operands have the same sign.
     *     - The result is negative if both the operands have different signs.
     *     - The result is NaN if both the operands are infinities, regardless
     *       of their signs.
     *     - The result is positive infinity if an infinity is divided by a
     *       finite value with the same sign as the infinity.
     *     - The result is negative infinity if an infinity is divided by a
     *       finite value with the sign opposite to the sign of the infinity.
     *     - The result is positive zero if a finite value is divied by an
     *       infinity with the same sign as the finite value.
     *     - The result is negative zero if a finite value is divied by an
     *       infinity with the sign opposite to the finite values sign.
     *     - The result is NaN if a zero is divided by a zero, regardless of
     *       the signs.
     *     - The result is positive zero if a zero is divided by a finite
     *       value with the same sign as the zero.
     *     - The result is negative zero if a zero is divided by a finite
     *       value with the sign opposite to the sign of zero.
     *     - The result is positive infinity if a finite value (other than
     *       zero) is divided by zero with the same sign as the finite value.
     *     - The result is negative infinity if a finite value (other than
     *       zero) is divided by zero with the sign opposite to the sign of
     *       the finite value.
     *     - In all other cases, where neither operand is an infinity, a
     *       zero, or NaN, the quotient is computed and rounded to the nearest
     *       representable value using IEEE 754 round to nearest mode. If
     *       the magnitude is too large to represent as a {@code double},
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a {@code double}, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * divide_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code double} value. It is popped off the operand stack.
     * value_2
     *     A {@code double} value. It is popped off the operand stack.
     * result
     *     A {@code double} value which is the quotient computed by the
     *     division of {@code value1} and {@code value2}, represented as
     *     {@code value1 / value2}.
     */
    ZEN_BYTE_CODE_DIVIDE_D,

    /* Duplicate */

    /**
     * Duplicate the value on the top of the operand stack.
     *
     * [Format]
     * duplicate
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., value, value
     *
     * [Operands]
     * value
     *     The value on top of the operand stack. It remains unchanged.
     *     In fact, a copy of this value is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE,
    ZEN_BYTE_CODE_DUPLICATE_X1,
    ZEN_BYTE_CODE_DUPLICATE_X2,
    ZEN_BYTE_CODE_DUPLICATE2,
    ZEN_BYTE_CODE_DUPLICATE2_X1,
    ZEN_BYTE_CODE_DUPLICATE2_X2,

    /* Jump */

    ZEN_BYTE_CODE_JUMP_EQ0_I,
    ZEN_BYTE_CODE_JUMP_NE0_I,
    ZEN_BYTE_CODE_JUMP_LT0_I,
    ZEN_BYTE_CODE_JUMP_GT0_I,
    ZEN_BYTE_CODE_JUMP_LE0_I,
    ZEN_BYTE_CODE_JUMP_GE0_I,

    ZEN_BYTE_CODE_JUMP_EQ_I,
    ZEN_BYTE_CODE_JUMP_NE_I,
    ZEN_BYTE_CODE_JUMP_LT_I,
    ZEN_BYTE_CODE_JUMP_GT_I,
    ZEN_BYTE_CODE_JUMP_LE_I,
    ZEN_BYTE_CODE_JUMP_GE_I,
    ZEN_BYTE_CODE_JUMP_EQ_A,
    ZEN_BYTE_CODE_JUMP_NE_A,
    ZEN_BYTE_CODE_JUMP_EQN_A,
    ZEN_BYTE_CODE_JUMP_NEN_A, // not equal to null

    /* Increment */

    ZEN_BYTE_CODE_INCREMENT_I,

    /* Invoke */

    ZEN_BYTE_CODE_INVOKE_SPECIAL,

    /**
     * Invoke a function against an instance.
     *
     * TODO!!
     *
     * [Format]
     * invoke_virtual index_byte_1 index_byte_2
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference, [argument_1, [argument_2, [argument_3, ...]]]
     * After
     *     ...,
     *
     * [Operands]
     * index_byte_1
     *     The {@code index_byte_1} and {@code index_byte_2} together indicate
     *     an unsigned integer index into the run-time constant pool of the
     *     current binary entity. The item at this index represents a function
     *     descriptor.
     * index_byte_2
     *     Please refer the documentation for {@code index_byte_1}.
     *
     */
    ZEN_BYTE_CODE_INVOKE_VIRTUAL,
    ZEN_BYTE_CODE_INVOKE_DYNAMIC,
    ZEN_BYTE_CODE_INVOKE_STATIC,
    ZEN_BYTE_CODE_INVOKE_FRAGMENT,

    /* Jump */

    ZEN_BYTE_CODE_JUMP,

    /* Load */

    ZEN_BYTE_CODE_LOAD_I,
    ZEN_BYTE_CODE_LOAD_L,

    /**
     * Load a {@code float} value from a local variable.
     *
     * [Format]
     * load_f index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a {@code float}. This reference is pushed onto the operand
     *     stack.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code float} value stored at the specified index. It is
     *     pushed onto the stack.
     */
    ZEN_BYTE_CODE_LOAD_F,

    /**
     * Load a {@code double} value from a local variable.
     *
     * [Format]
     * load_d index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a {@code double}. This reference is pushed onto the operand
     *     stack.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code double} value stored at the specified index. It is
     *     pushed onto the stack.
     */
    ZEN_BYTE_CODE_LOAD_D,

    /**
     * Load a reference from a local variable.
     *
     * [Format]
     * load_a index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a reference. This reference is pushed onto the operand stack.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., object_reference
     *
     * [Operands]
     * object_reference
     *     The {@code reference} to an object stored at the specified index.
     *     It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_A,

    ZEN_BYTE_CODE_LOAD_I0,
    ZEN_BYTE_CODE_LOAD_I1,
    ZEN_BYTE_CODE_LOAD_I2,
    ZEN_BYTE_CODE_LOAD_I3,

    ZEN_BYTE_CODE_LOAD_L0,
    ZEN_BYTE_CODE_LOAD_L1,
    ZEN_BYTE_CODE_LOAD_L2,
    ZEN_BYTE_CODE_LOAD_L3,

    /**
     * Load a {@code float} value from a local variable.
     *
     * Here, 0 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code float} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_f0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code float} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F0,

    /**
     * Load a {@code float} value from a local variable.
     *
     * Here, 1 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code float} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_f1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code float} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F1,

    /**
     * Load a {@code float} value from a local variable.
     *
     * Here, 2 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code float} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_f2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code float} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F2,

    /**
     * Load a {@code float} value from a local variable.
     *
     * Here, 3 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code float} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_f3
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code float} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F3,

    /**
     * Load a {@code double} value from a local variable.
     *
     * Here, 0 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code double} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_d0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code double} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D0,

    /**
     * Load a {@code double} value from a local variable.
     *
     * Here, 1 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code double} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_d1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code double} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D1,

    /**
     * Load a {@code double} value from a local variable.
     *
     * Here, 2 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code double} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_d2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code double} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D2,

    /**
     * Load a {@code double} value from a local variable.
     *
     * Here, 3 should be a valid index into the local variable array. The local
     * variable at this index should be a {@code double} value. This reference
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_d3
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The {@code double} value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D3,

    /**
     * Load a reference from a local variable. Here, 0 should be a valid index
     * into the local variable array. The local variable at this index should
     * be a reference. This reference is pushed onto the operand stack.
     *
     * [Format]
     * load_a0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., object_reference
     */
    ZEN_BYTE_CODE_LOAD_A0,

    /**
     * Load a reference from a local variable. Here, 1 should be a valid index
     * into the local variable array. The local variable at this index should
     * be a reference. This reference is pushed onto the operand stack.
     *
     * [Format]
     * load_a1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., object_reference
     */
    ZEN_BYTE_CODE_LOAD_A1,

    /**
     * Load a reference from a local variable. Here, 2 should be a valid index
     * into the local variable array. The local variable at this index should
     * be a reference. This reference is pushed onto the operand stack.
     *
     * [Format]
     * load_a0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., object_reference
     */
    ZEN_BYTE_CODE_LOAD_A2,

    /**
     * Load a reference from a local variable. Here, 3 should be a valid index
     * into the local variable array. The local variable at this index should
     * be a reference. This reference is pushed onto the operand stack.
     *
     * [Format]
     * load_a0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., object_reference
     */
    ZEN_BYTE_CODE_LOAD_A3,

    /**
     * Load a {@code byte} or a {@code boolean} value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * This instruction is common to both byte and boolean arrays.
     * The implementation of the virtual machine is free to choose the
     * internal representation of the arrays.
     *
     * [Format]
     * load_ab
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code byte} or
     *     {@code boolean}. It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     *
     * [Exception]
     *
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AB,

    /**
     * Load a {@code char} value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * [Format]
     * load_ac
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code char}.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *    A 16-bit {@code char} value that is stored at the specified index
     *    in the specified {@code char} array. It is pushed onto the stack.
     *
     * [Exception]
     *
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AC,
    // TODO: Characters are stored in 16-bit integers or 8-bit integers?!

    ZEN_BYTE_CODE_LOAD_AS,
    ZEN_BYTE_CODE_LOAD_AI,
    ZEN_BYTE_CODE_LOAD_AL,

    /**
     * Load a {@code float} value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * [Format]
     * load_af
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code float}.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *    A 32-bit {@code float} value that is stored at the specified index
     *    in the specified {@code float} array. It is pushed onto the stack.
     *
     * [Exception]
     *
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AF,

    /**
     * Load a {@code double} value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * [Format]
     * load_ad
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code double}.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *    A 64-bit {@code double} value that is stored at the specified index
     *    in the specified {@code double} array. It is pushed onto the stack.
     *
     * [Exception]
     *
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AD,

    /**
     * Load a reference from a reference array.
     *
     * Pops both the arguments from the stack. The element at the specified
     * index is retrieved and pushed onto the operand stack.
     *
     * [Format]
     * load_aa
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Arguments]
     * array
     *     A reference to the array whose component type is {@code reference}.
     * index
     *     The index of the element to retrieve.
     *
     * [Exception]
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid.
     */
    ZEN_BYTE_CODE_LOAD_AA,

    ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD,
    ZEN_BYTE_CODE_LOAD_STATIC_FIELD,


    /**
     * Load an item from the constant pool.
     *
     * [Format]
     * load_cpr index
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Arguments]
     * index
     *     An unsigned 8-bit integer which indicates the index of the
     *     item in the constant pool.
     *
     * [Operands]
     * value
     * The value of the item is pushed onto the operand stack.
     * TODO: The types and values pushed onto the operand stack.
     *
     * [Exception]
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid.
     */
    ZEN_BYTE_CODE_LOAD_CPR, // Constant Pool Reference

    ZEN_BYTE_CODE_LOAD_ARRAY_SIZE,

    /* Modulo */

    ZEN_BYTE_CODE_MODULO_I,
    ZEN_BYTE_CODE_MODULO_L,
    ZEN_BYTE_CODE_MODULO_F,

    /**
     * Divide two {@code double} values to compute the remainder.
     *
     * It should be noted that the remainder operation is not in
     * accordance with the IEEE 754. Otherwise, the {@code remainder_d}
     * instruction would not behave analogous to the {@code remainder_i}
     * and {@code remainder_l} instructions. Therefore, the rules for
     * the {@code remainder_d} is derived from the {@code fmod} function
     * specified in the C specification.
     * The following rules should be observed when computing the remainder
     * of the division of the specified values.
     *     - The result is NaN if any of the operands is NaN.
     *     - The result has the same sign as the dividend, given the value
     *       pair is ordered.
     *     - The result is NaN if the dividend is infinity, regardless of
     *       its sign.
     *     - The result is NaN if the divisor is zero, regardless of
     *       its sign.
     *     - The result is the dividend itself if the dividend is a finite
     *       value and the divisor is infinity, regardless of its sign.
     *     - The result is the dividend itself if the dividend is a zero
     *       and the divisor is a finite value.
     *     - In all other cases, where neither operand is an infinity, a
     *       zero, or NaN, the remainder is computed using the following
     *       expression {@code value_1 - (value_2 * x)}, where x is a
     *       positive or negative integer with the sign equivalent to the
     *       sign of {@code value_1 / value_2}.
     *
     * [Format]
     * modulo_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code double} value. It is popped off the operand stack.
     * value_2
     *     A {@code double} value. It is popped off the operand stack.
     * result
     *     A {@code double} value which is the remainder of dividing
     *     {@code value_1} and {@code value_2}. It is pushed onto the
     *     stack.
     */
    ZEN_BYTE_CODE_MODULO_D,

    /* Multiply */

    ZEN_BYTE_CODE_MULTIPLY_I,
    ZEN_BYTE_CODE_MULTIPLY_L,
    ZEN_BYTE_CODE_MULTIPLY_F,

    /**
     * Multiply two {@code double} values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when multiply two {@code double} values.
     *     - If either the first or the second operand is NaN, the result is
     *       always NaN.
     *     - The result is positive if both the operands have the same sign.
     *     - The result is negative if both the operands have different signs.
     *     - The result is NaN if the an infinity is multiplied by a zero,
     *       regardless of signs.
     *     - The result is positive infinity if an infinity is multiplied by a
     *       finite value with the same sign as the infinity.
     *     - The result is negative infinity if an infinity is multiplied by a
     *       finite value with the sign opposite to the sign of the infinity.
     *     - In all other cases, where neither operand is an infinity nor NaN
     *       is involved the multiplication is computed and rounded to the
     *       nearest representable value using IEEE 754 round to nearest mode.
     *       If the magnitude is too large to represent as a {@code double},
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a {@code double}, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * multiply_d
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A {@code double} value. It is popped off the operand stack.
     * value_2
     *     A {@code double} value. It is popped off the operand stack.
     * result
     *     A {@code double} value which is the quotient computed by the
     *     multiplication of {@code value1} and {@code value2}, represented
     *     as {@code value1 * value2}.
     */
    ZEN_BYTE_CODE_MULTIPLY_D,

    /* Negate */

    ZEN_BYTE_CODE_NEGATE_I,
    ZEN_BYTE_CODE_NEGATE_L,
    ZEN_BYTE_CODE_NEGATE_F,
    ZEN_BYTE_CODE_NEGATE_D,

    /* New */

    ZEN_BYTE_CODE_NEW,
    ZEN_BYTE_CODE_NEW_ARRAY,
    ZEN_BYTE_CODE_NEW_ARRAY_N,

    /**
     * Create a new single dimension reference array.
     *
     * A new array with the specified number of components is created. All
     * components of the newly created array are initialized to {@code null}.
     *
     * [Format]
     * new_array_a index_byte_1 index_byte_2
     *
     * [Arguments]
     * index_byte_1
     *     The {@code index_byte_1} and {@code index_byte_2} together indicate
     *     an unsigned integer index into the run-time constant pool of the
     *     current binary entity. The item at this index represents a reference
     *     to a class, an enumeration, or an array type.
     * index_byte_2
     *     Please refer the documentation for {@code index_byte_1}.
     *
     * [Operand Stack]
     * Before
     *     ..., size
     * After
     *     ..., reference_array
     *
     * [Operands]
     * size
     *     An unsigned 32-bit integer which indicates the size of the array.
     *     It is popped off the operand stack.
     *
     * [Exception]
     * During resolution of the class, enumeration, or array types, any of the
     * documented exceptions for type resolution may be thrown.
     *
     * zen.base.InvalidArraySizeException
     *     An instance of the {@code InvalidArraySizeException} class is thrown
     *     if the specified array is size is invalid. An array size is invalid
     *     if it is less than zero.
     */
    ZEN_BYTE_CODE_NEW_ARRAY_A,

    /* Pop */

    ZEN_BYTE_CODE_POP,
    ZEN_BYTE_CODE_POP2,

    /* Push */

    /**
     * Push {@code null} onto the operand stack.
     *
     * [Format]
     * push_null
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., null
     */
    ZEN_BYTE_CODE_PUSH_NULL,

    /**
     * The {@code push_i0} and {@code push_b 0} instructions both push a value
     * of zero onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas, the latter instruction occupies 2 bytes.
     * The value pushed by both the instruction always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The value -...  -2 and 6 .. 255 are pushed with push_b instruction.
     * The value 256 through 64... are pushed with push_s instruction.
     * Other values are pushed with load_cpr instruction.
     */
    ZEN_BYTE_CODE_PUSH_IN1,
    ZEN_BYTE_CODE_PUSH_I0,
    ZEN_BYTE_CODE_PUSH_I1,
    ZEN_BYTE_CODE_PUSH_I2,
    ZEN_BYTE_CODE_PUSH_I3,
    ZEN_BYTE_CODE_PUSH_I4,
    ZEN_BYTE_CODE_PUSH_I5,
    ZEN_BYTE_CODE_PUSH_L0,
    ZEN_BYTE_CODE_PUSH_L1,
    ZEN_BYTE_CODE_PUSH_L2,

    /**
     * Push a {@code float} value of {@code 0.0} onto the operand stack.
     *
     * [Format]
     * push_f0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A {@code float} value of {@code 0.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_F0,

    /**
     * Push a {@code float} value of {@code 1.0} onto the operand stack.
     *
     * [Format]
     * push_f1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A {@code float} value of {@code 1.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_F1,

    /**
     * Push a {@code float} value of {@code 2.0} onto the operand stack.
     *
     * [Format]
     * push_f2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A {@code float} value of {@code 2.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_F2,

    /**
     * Push a {@code double} value of {@code 0.0} onto the operand stack.
     *
     * [Format]
     * push_d0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A {@code double} value of {@code 0.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_D0,

    /**
     * Push a {@code double} value of {@code 1.0} onto the operand stack.
     *
     * [Format]
     * push_d1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A {@code double} value of {@code 1.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_D1,
    ZEN_BYTE_CODE_PUSH_D2,

    /**
     * Push a {@code byte} onto the operand stack.
     *
     * [Format]
     * push_b value
     *
     * [Arguments]
     * value
     *     An signed 8-bit integer.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     */
    ZEN_BYTE_CODE_PUSH_B,

    ZEN_BYTE_CODE_PUSH_S,

    /* Return */

    ZEN_BYTE_CODE_RETURN,
    ZEN_BYTE_CODE_RETURN_I,
    ZEN_BYTE_CODE_RETURN_L,
    ZEN_BYTE_CODE_RETURN_F,

    /**
     * Return a {@code double} value.
     *
     * Execution of this instruction results in termination of the currently
     * executing function. The control is returned to the invoker function.
     *
     * The instruction is completely unaware of locks. In other words, any
     * locks acquired within the function should be released before the
     * execution of this instruction.
     *
     * All values, other than the top most value, on the operand stack are
     * discarded. Whereas, the top most value is popped off the operand stack
     * of the current frame and pushed onto the operand stack of the invoker
     * frame.
     *
     * [Format]
     * return_d
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     [invalid]
     *
     * [Operands]
     * value
     *     A {@code double} value. This value is popped off the operand stack
     *     of the current frame and pushed onto the operand stack of the
     *     invoker frame.
     */
    ZEN_BYTE_CODE_RETURN_D,

    /**
     * Return a reference.
     *
     * Execution of this instruction results in termination of the currently
     * executing function. The control is returned to the invoker function.
     *
     * The instruction is completely unaware of locks. In other words, any
     * locks acquired within the function should be released before execution
     * of this instruction.
     *
     * All values, other than the top most value, on the operand stack are
     * discarded. Whereas, the top most value is popped off the operand stack
     * of the current frame and pushed onto the operand stack of the invoker
     * frame.
     *
     * [Format]
     * return_a
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     [invalid]
     *
     * [Operands]
     * object_reference
     *     A reference to an object. This reference is popped off the operand
     *     stack of the current frame and pushed onto the operand stack of the
     *     invoker frame.
     */
    ZEN_BYTE_CODE_RETURN_A,

    ZEN_BYTE_CODE_RETURN_FRAGMENT,

    /* RTTI */

    ZEN_BYTE_CODE_RTTI,

    /* Store */

    ZEN_BYTE_CODE_STORE_I,
    ZEN_BYTE_CODE_STORE_I0,
    ZEN_BYTE_CODE_STORE_I1,
    ZEN_BYTE_CODE_STORE_I2,
    ZEN_BYTE_CODE_STORE_I3,

    ZEN_BYTE_CODE_STORE_L,
    ZEN_BYTE_CODE_STORE_L0,
    ZEN_BYTE_CODE_STORE_L1,
    ZEN_BYTE_CODE_STORE_L2,
    ZEN_BYTE_CODE_STORE_L3,

    ZEN_BYTE_CODE_STORE_F,
    ZEN_BYTE_CODE_STORE_F0,
    ZEN_BYTE_CODE_STORE_F1,
    ZEN_BYTE_CODE_STORE_F2,
    ZEN_BYTE_CODE_STORE_F3,

    /**
     * Store a {@code double} value into local variable.
     *
     * Further, a local variable can be accessed using a two-byte unsigned
     * index using the wide version of this instruction, {@code store_wd}.
     *
     * [Format]
     * store_d index
     *
     * [Arguments]
     * index
     *     An unsigned byte which should be valid index into the local
     *     variable array of the current frame.
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The {@code double} value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D,

    /**
     * Store a {@code double} value into local variable.
     *
     * Here, 0 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_d0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The {@code double} value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D0,

    /**
     * Store a {@code double} value into local variable.
     *
     * Here, 1 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_d1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The {@code double} value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D1,

    /**
     * Store a {@code double} value into local variable.
     *
     * Here, 2 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_d2
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The {@code double} value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D2,

    /**
     * Store a {@code double} value into local variable.
     *
     * Here, 3 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_d3
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The {@code double} value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D3,

    /**
     * Store a reference into local variable.
     *
     * Further, a local variable can be accessed using a two-byte unsigned
     * index using the wide version of this instruction, {@code store_wa}.
     *
     * [Format]
     * store_a index
     *
     * [Arguments]
     * index
     *     An unsigned byte which should be valid index into the local
     *     variable array of the current frame.
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     ...
     *
     * [Operands]
     * object_reference
     *     The object reference which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_A,

    /**
     * Store a reference into local variable.
     *
     * Here, 0 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_a0
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     ...
     *
     * [Operands]
     * object_reference
     *     The object reference which is assigned to the local variable.
     */
    ZEN_BYTE_CODE_STORE_A0,

    /**
     * Store a reference into local variable.
     *
     * Here, 1 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_a0
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     ...
     *
     * [Operands]
     * object_reference
     *     The object reference which is assigned to the local variable.
     */
    ZEN_BYTE_CODE_STORE_A1,

    /**
     * Store a reference into local variable.
     *
     * Here, 2 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_a0
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     ...
     *
     * [Operands]
     * object_reference
     *     The object reference which is assigned to the local variable.
     */
    ZEN_BYTE_CODE_STORE_A2,

    /**
     * Store a reference into local variable.
     *
     * Here, 3 should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_a0
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     ...
     *
     * [Operands]
     * object_reference
     *     The object reference which is assigned to the local variable.
     */
    ZEN_BYTE_CODE_STORE_A3,

    ZEN_BYTE_CODE_STORE_AB,

    /**
     * Store a {@code char} value in a {@code char} array.
     *
     * [Format]
     * store_ac
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code char}.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 16-bit unsigned integer which represents the {@code char} value
     *     to store.
     *
     * [Exception]
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AC,
    // TODO: 16-bit or 8-bit characters?

    ZEN_BYTE_CODE_STORE_AS,
    ZEN_BYTE_CODE_STORE_AI,
    ZEN_BYTE_CODE_STORE_AL,

    /**
     * Store a {@code float} value in a {@code float} array.
     *
     * [Format]
     * store_af
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code float}.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 32-bit {@code float} value which represents the {@code float}
     *     value to store.
     *
     * [Exception]
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AF,

    /**
     * Store a {@code double} value in a {@code double} array.
     *
     * [Format]
     * store_ad
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code double}.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 64-bit {@code double} value which represents the {@code double}
     *     value to store.
     *
     * [Exception]
     * zen.base.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is {@code null}.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AD,

    /**
     * Store a reference in a reference array.
     *
     * [Format]
     * store_aa
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Arguments]
     * array
     *     A reference to the array whose component type is {@code reference}.
     * index
     *     A index of the element to store.
     * value
     *     The reference value to store. The instruction first determines
     *     whether the reference value of the type, say VT, is compatible
     *     with the  reference array with the component type, say CT.
     *     CASE 1: CT is a class type
     *         - VT is same as CT
     *         - VT is a subclass of CT
     *     CASE 2: CT is an array type
     *         -
     *
     * [Exception]
     */
    ZEN_BYTE_CODE_STORE_AA,

    ZEN_BYTE_CODE_STORE_INSTANCE_FIELD,
    
    ZEN_BYTE_CODE_STORE_STATIC_FIELD,

    /* Subtract */

    /**
     * Subtracts two 32-bit integer values.
     *
     * Two 32-bit integer operands are popped of the operand stack. A 32-bit
     * integer result is evaluated by subtracting the second operand from the
     * first operand, shown as {@code value_1 - value_2}.
     *
     * If an overflow occurs, the bits of the result is truncated to preserve
     * only the first 32-bits from the Least Significant Bit (LSB).
     *
     * Further, an overflow never raises an exception.
     *
     * [Format]
     * subtract_i
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value_2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by addition of the
     *     specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_SUBTRACT_I,

    /**
     * Subtracts two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped of the operand stack. A 64-bit
     * integer result is evaluated by subtracting the second operand from the
     * first operand, shown as {@code value_1 - value_2}.
     *
     * If an overflow occurs, the bits of the result is truncated to preserve
     * only the first 64-bits from the Least Significant Bit (LSB).
     *
     * Further, an overflow never raises an exception.
     *
     * [Format]
     * subtract_l
     *
     * [Operand Stack]
     * Before
     *     ..., value_1, value_2
     * After
     *     ..., result
     *
     * [Operands]
     * value_1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value_2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by addition of the
     *     specified 64-bit integer values.
     */
    ZEN_BYTE_CODE_SUBTRACT_L,
    ZEN_BYTE_CODE_SUBTRACT_F,
    ZEN_BYTE_CODE_SUBTRACT_D,

    /* Swap */

    ZEN_BYTE_CODE_SWAP,

    /* Switch */

    ZEN_BYTE_CODE_SWITCH_TABLE,
    ZEN_BYTE_CODE_SWITCH_SEARCH,

    /* Throw */

    /**
     * Throw an exception or an error.
     *
     * The specified exception is received by the first suitable exception
     * handler along the stack trace. All the frames above this function is
     * cleared. The execution control is passed to the function containing the
     * exception handler. The execution resumes at the offset indicated by the
     * exception table. The program counter is reset to this location.
     * The reference to the exception object is pushed onto the operand stack.
     *
     * The instruction is completely unaware of locks. In other words, any
     * locks acquired within the function and all the functions above the
     * function containing the exception handler should manually release the
     * locks before or after execution of this instruction.
     *
     * [Format]
     * throw
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference
     * After
     *     [invalid]
     *
     * [Operands]
     * object_reference
     *     A reference to an instance of the {@code Throwable} class, or its
     *     subclass. This object represents the exception. It is popped off the
     *     operand stack of the current frame. It is received by the first
     *     suitable exception handler along the stack trace, if any.
     *
     * [Exception]
     * zen.base.NullPointerException
     *     An instance of the {@code InvalidArraySizeException} class is thrown
     *     if the specified exception is {@code null}.
     */
    ZEN_BYTE_CODE_THROW,

    /* Wide */

    ZEN_BYTE_CODE_WIDE,
};

typedef enum zen_ByteCode_t zen_ByteCode_t;

#endif /* ZEN_VM_BYTE_CODE_H */

/*
 * [Instruction (or Mnemonic) Nomenclature]
 *
 * Format: name_[_modifier_][wide][array][sign][primitive_type][index]
 *
 * [modifier]
 * A modifier is local to the instruction. It indicates the modified primary
 * operation the instruction performs. This part is rarely seen in the ZVM
 * instruction set.
 * [Wide]
 * An instruction may have an extended version where the size of one or more
 * input data is wider than the normal version. Such instructions are marked
 * with 'w'.
 *
 * [Array]
 * An instruction which operates on one or more array references is marked
 * with 'a'.
 *
 * [Sign]
 * Albeit the ZVM does not support unsigned integers in general, certain
 * instructions interpret signed and unsigned properties of an integer value.
 * Such instructions are marked with one of the following characters:
 * 's' - signed (This marker is usually omitted.)
 * 'u' - unsigned
 *
 * [Primitive Types]
 * Certain instructions have multiple version which operate exclusively on
 * a primitive type. Such instructions are marked with one of the following
 * characters:
 * 'b' - byte
 * 's' - short
 * 'i' - integer
 * 'l' - long
 * 'f' - float
 * 'd' - double
 * 'a' - reference
 *
 * [Index]
 * Certain instructions operate on data at a specified index. The meaning of
 * the index is local to the instruction in question. In order to enhance the
 * performance of the virtual machine, these instructions usually have hybrid
 * versions which operate on predefined indexes. Such instructions are marked
 * with {@code 0} or any of the positive integers.
 *
 * [Examples]
 *
 * NOTE: These instructions are completely imaginary and have no special
 *       meaning to the virtual machine. They are here merely to help the
 *       reader understand the nomenclature.
 *
 * name_wad0
 * name_0
 * name_as
 * name_i0
 */