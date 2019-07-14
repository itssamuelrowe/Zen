// Saturday, March 17, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_BYTE_CODE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_BYTE_CODE_H

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

/**
 * @author Samuel Rowe
 * @since Zen 1.0
 * @class ByteCode
 * @ingroup zen_virtualMachine_feb
 */
enum zen_ByteCode_t {

    /* No Operation */

    ZEN_BYTE_CODE_NOP,

    /* Add */

    /**
     * Add two {@code int} values.
     *
     * Two {@code int} operands are popped off the operand stack. An {@code int}
     * result is evaluated by adding both the operands, shown as {@code value1
     * + value2}.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A {@code int} value. It is popped off the operand stack.
     * value2
     *     A {@code int} value. It is popped off the operand stack.
     * result
     *     A {@code int} value which is computed by addition of the
     *     specified {@code int} values.
     */
    ZEN_BYTE_CODE_ADD_I,

    /**
     * Add two {@code long} values.
     *
     * Two {@code long} operands are popped off the operand stack. An {@code long}
     * result is evaluated by adding both the operands, shown as {@code value1
     * + value2}.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A {@code long} value. It is popped off the operand stack.
     * value2
     *     A {@code long} value. It is popped off the operand stack.
     * result
     *     A {@code long} value which is computed by addition of the
     *     specified {@code long} values.
     */
    ZEN_BYTE_CODE_ADD_L,

    /**
     * Add two `float` values.
     *
     * The following rules, as specified by IEEE 754, should be observed when
     * adding two `float` values.
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
     *       the magnitude is too large to represent as a `float`,
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a `float`, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * add_f
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `float` value. It is popped off the operand stack.
     * value2
     *     A `float` value. It is popped off the operand stack.
     * result
     *     A `float` value which is computed by addition of the
     *     specified `float` values.
     */
    ZEN_BYTE_CODE_ADD_F,

    /**
     * Add two `double` values.
     *
     * The following rules, as specified by IEEE 754, should be observed when
     * adding two `double` values.
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
     *       the magnitude is too large to represent as a `double`,
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a `double`, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * add_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `double` value. It is popped off the operand stack.
     * value2
     *     A `double` value. It is popped off the operand stack.
     * result
     *     A `double` value which is computed by addition of the
     *     specified `double` values.
     */
    ZEN_BYTE_CODE_ADD_D,

    /* Bitwise AND */

    /**
     * Bitwise AND of two 32-bit integer values.
     *
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by performing a  conjunction (bitwise AND)
     * of both the operands, shown as {@code value1 & value2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a conjunction
     *     of both the specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_AND_I,

    /**
     * Bitwise AND of two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped off the operand stack. A 64-bit
     * integer result is evaluated by performing a  conjunction (bitwise AND)
     * of both the operands, shown as {@code value1 & value2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
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
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by performing a  disjunction (bitwise OR)
     * of both the operands, shown as {@code value1 | value2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a disjunction
     *     of both the specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_OR_I,

    /**
     * Bitwise OR of two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped off the operand stack. A 64-bit
     * integer result is evaluated by performing a  disjunction (bitwise OR)
     * of both the operands, shown as {@code value1 | value2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing a disjunction
     *     of both the specified 64-bit integer values.
     */
    ZEN_BYTE_CODE_OR_L,

    /* Bitwise Shift */

    /**
     * Bitwise left shift a 32-bit integer value.
     *
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by performing a left shift on `value1` by
     * the number of positions specifiedy by `value2`, shown as
     * `value1 << value2`. The shift distance should always be in the range 0
     * to 31, inclusive.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * shift_left_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a left shift
     *     on `value1` by the number of positions specifiedy by `value2`.
     */
    ZEN_BYTE_CODE_SHIFT_LEFT_I,

    /**
     * Bitwise left shift a 64-bit integer value.
     *
     * Two 64-bit and 32-bit integer operands are popped off the operand stack.
     * A 64-bit integer result is evaluated by performing a left shift on `value1`
     * by the number of positions specifiedy by `value2`, shown as
     * `value1 << value2`. The shift distance should always be in the range 0
     * to 63, inclusive.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * shift_left_l
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing a left shift
     *     on `value1` by the number of positions specifiedy by `value2`.
     */
    ZEN_BYTE_CODE_SHIFT_LEFT_L,

    /**
     * Bitwise right shift a 32-bit integer value.
     *
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by performing a right shift on `value1` by
     * the number of positions specifiedy by `value2`, shown as
     * `value1 >> value2`. The shift distance should always be in the range 0
     * to 31, inclusive.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * shift_left_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a right shift
     *     on `value1` by the number of positions specifiedy by `value2`.
     */
    ZEN_BYTE_CODE_SHIFT_RIGHT_I,

    /**
     * Bitwise right shift a 64-bit integer value.
     *
     * Two 64-bit and 32-bit integer operands are popped off the operand stack.
     * A 64-bit integer result is evaluated by performing a right shift on `value1`
     * by the number of positions specifiedy by `value2`, shown as
     * `value1 >> value2`. The shift distance should always be in the range 0
     * to 63, inclusive.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * shift_left_l
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing a right shift
     *     on `value1` by the number of positions specifiedy by `value2`.
     */
    ZEN_BYTE_CODE_SHIFT_RIGHT_L,

    /**
     * Bitwise right shift an unsigned 32-bit integer value, with zero extension.
     *
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by performing a right shift on `value1` by
     * the number of positions specifiedy by `value2`, shown as
     * `value1 >>> value2`. The shift distance should always be in the range 0
     * to 31, inclusive.
     *
     * When `value1` is positive and `value2 & 0x1f` is the distance, the
     * result is `value1 >> value2`. However, if `value1` is negative, the
     * result is equal to `(value1 >> (value2 & 0x1f)) + (2 << ~(value2 & 0x1f))`.
     * Adding `(2 << ~(value2 & 0x1f))` cancels the propagated sign bit.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * shift_right_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing a right shift
     *     on `value1` by the number of positions specifiedy by `value2`, with
     *     sign extension.
     */
    ZEN_BYTE_CODE_SHIFT_RIGHT_UI,

    /**
     * Bitwise right shift a 64-bit integer value, with sign extension.
     *
     * Two 64-bit and 32-bit integer operands are popped off the operand stack.
     * A 64-bit integer result is evaluated by performing a right shift on `value1`
     * by the number of positions specifiedy by `value2`, shown as
     * `value1 >>> value2`. The shift distance should always be in the range 0
     * to 63, inclusive.
     *
     * When `value1` is positive and `value2 & 0x3f` is the distance, the
     * result is `value1 >> value2`. However, if `value1` is negative, the
     * result is equal to `(value1 >> (value2 & 0x3f)) + (2 << ~(value2 & 0x3f))`.
     * Adding `(2 << ~(value2 & 0x3f))` cancels the propagated sign bit.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * shift_left_l
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing a right shift
     *     on `value1` by the number of positions specifiedy by `value2`, with
     *     sign extension.
     */
    ZEN_BYTE_CODE_SHIFT_RIGHT_UL,

    /* Bitwise XOR */

    /**
     * Bitwise Exclusive OR (XOR) of two 32-bit integer values.
     *
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by performing an exclusive conjunction
     * (bitwise XOR) of both the operands, shown as {@code value1 ^ value2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by performing an exclusive
     *     conjunction (bitwise XOR) of both the specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_XOR_I,

    /**
     * Bitwise Exclusive OR (XOR) of two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped off the operand stack. A 64-bit
     * integer result is evaluated by performing an exclusive conjunction
     * (bitwise XOR) of both the operands, shown as {@code value1 ^ value2}.
     *
     * The result is pushed onto the operand stack.
     *
     * [Format]
     * add_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by performing an exclusive
     *     conjunction (bitwise XOR) of both the specified 64-bit integer values.
     */
    ZEN_BYTE_CODE_XOR_L,

    /* Cast */

    /**
     * Convert a 32-bit integer value to a 8-bit integer value.
     *
     * The instruction narrows the 32-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * In other words, the 32-bit integer is truncated to 8-bit integer, then
     * the sign is extended to a 32-bit result.
     *
     * [Format]
     * cast_itb
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     32-bit integer value to 8-bit integer value. The sign is extended to
     *     a 32-bit integer before it is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_CAST_ITB,

    /**
     * Convert a 32-bit integer value to a 16-bit integer value.
     *
     * The instruction narrows the 32-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * In other words, the 32-bit integer is truncated to 16-bit integer, then
     * the sign is extended to a 32-bit result.
     *
     * [Format]
     * cast_itb
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     32-bit integer value to 16-bit integer value. The sign is extended to
     *     a 32-bit integer before it is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_CAST_ITS,

    /**
     * Convert a 32-bit integer value to a 64-bit integer value.
     *
     * The instruction widens the 32-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision is not observed.
     *
     * In other words, the 32-bit integer is expanded to 64-bit integer, then
     * the sign is extended to a 64-bit result.
     *
     * [Format]
     * cast_itl
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit {@code integer} value derived from converting the specified
     *     32-bit integer value to 64-bit integer value. The sign is extended to
     *     a 64-bit integer before it is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_CAST_ITL,

    /**
     * Convert a 32-bit integer value to a 32-bit decimal value.
     *
     * The conversion uses the IEEE 754 round to nearest mode.
     * The instruction widens the 32-bit integer value. However, loss of
     * information in regards to magnitude and precision may be observed.
     * This is because `float` has 24-bit significand bits.
     *
     * [Format]
     * cast_itf
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit `float` value derived from converting the specified
     *     32-bit integer value to 32-bit `float` value.
     */
    ZEN_BYTE_CODE_CAST_ITF,

    /**
     * Convert a 32-bit integer value to a 64-bit decimal value.
     *
     * The conversion uses the IEEE 754 round to nearest mode.
     * The instruction widens the 32-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision is not observed.
     * This is because `double` has 53-bit significand bits.
     *
     * [Format]
     * cast_itd
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit `double` value derived from converting the specified
     *     32-bit integer value to 64-bit `double` value.
     */
    ZEN_BYTE_CODE_CAST_ITD,

    /**
     * Convert a 64-bit integer value to a 8-bit integer value.
     *
     * The instruction narrows the 64-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * In other words, the 64-bit integer is truncated to 8-bit integer, then
     * the sign is extended to a 32-bit result.
     *
     * [Format]
     * cast_itb
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     64-bit integer value to 8-bit integer value. The sign is extended to
     *     a 32-bit integer before it is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_CAST_LTB,

    /**
     * Convert a 64-bit integer value to a 16-bit integer value.
     *
     * The instruction narrows the 64-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * In other words, the 64-bit integer is truncated to 16-bit integer, then
     * the sign is extended to a 32-bit result.
     *
     * [Format]
     * cast_itb
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     64-bit integer value to 16-bit integer value. The sign is extended to
     *     a 32-bit integer before it is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_CAST_LTS,

    /**
     * Convert a 64-bit integer value to a 32-bit integer value.
     *
     * The instruction narrows the 64-bit integer value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * In other words, the 64-bit integer is truncated to 32-bit integer, then
     * the sign is extended to a 32-bit result.
     *
     * [Format]
     * cast_itb
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     64-bit integer value to 32-bit integer value. The sign is extended to
     *     a 32-bit integer before it is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_CAST_LTI,

    /**
     * Convert a 64-bit integer value to a 32-bit decimal value.
     *
     * The conversion uses the IEEE 754 round to nearest mode.
     * The instruction widens the 64-bit integer value. However, loss of
     * information in regards to magnitude and precision may be observed.
     * This is because `float` has 24-bit significand bits.
     *
     * [Format]
     * cast_itf
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit `float` value derived from converting the specified
     *     32-bit integer value to 32-bit `float` value.
     */
    ZEN_BYTE_CODE_CAST_LTF,

    /**
     * Convert a 64-bit integer value to a 64-bit decimal value.
     *
     * The conversion uses the IEEE 754 round to nearest mode.
     * The instruction widens the 64-bit integer value. However, loss of
     * information in regards to magnitude and precision may be observed.
     * This is because `double` has 53-bit significand bits.
     *
     * [Format]
     * cast_itf
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit `double` value derived from converting the specified
     *     64-bit integer value to 64-bit `double` value.
     */
    ZEN_BYTE_CODE_CAST_LTD,

    /**
     * Convert a `float` value to a {@code integer} value.
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
     * The instruction narrows the `float` value. Therefore, loss of
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
     *     A 32-bit `float` value. It is popped off the operand stack.
     * result
     *     A 32-bit {@code integer} value derived from converting the specified
     *     `float` value to {@code integer} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_FTI,

    /**
     * Convert a `float` value to a `double` value.
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
     * The instruction narrows the `double` value. Therefore, loss of
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
     *     A 32-bit `float` value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value derived from converting the specified
     *     `float` value to a {@code long} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_FTL,

    /**
     * Convert a `float` value to a `double` value.
     *
     * The specified `float` value is converted to a `double`
     * value using IEEE 754 round to nearest mode. In other words, the
     * instruction widens the `float` value. Therefore, no loss of
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
     *     A 32-bit `float` value. It is popped off the operand stack.
     * result
     *     A 64-bit `double` value derived from converting the specified
     *     `float` value to `double` value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_FTD,

    /**
     * Convert a `double` value to an {@code integer} value.
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
     * The instruction narrows the `double` value. Therefore, loss of
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
     *     A 64-bit `double` value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value derived from converting the specified
     *     `double` value to {@code integer} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_DTI,

    /**
     * Convert a `double` value to a {@code long} value.
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
     * The instruction narrows the `double` value. Therefore, loss of
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
     *     A 64-bit `double` value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value derived from converting the specified
     *     `double` value to {@code long} value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_DTL,

    /**
     * Convert a `double` value to a `float` value.
     *
     * The specified `double` value is converted to a `float`
     * value using IEEE 754 round to nearest mode. In other words, the
     * instruction narrows the `double` value. Therefore, loss of
     * information in regards to magnitude and precision may be observed.
     *
     * The following cases should be carefully observed:
     *  - Values too small to represent as a `float` value is represented
     *    as zero.
     *  - Values too big to represents as a `float` value is represented
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
     *     A 64-bit `double` value. It is popped off the operand stack.
     * result
     *     A 32-bit float value derived from converting the specified
     *     `double` value to `float` value. It is pushed onto
     *     the operand stack.
     */
    ZEN_BYTE_CODE_CAST_DTF,

    /* TODO: Should I encode characters in UTF-16 or UTF-8. */
    ZEN_BYTE_CODE_CAST_ITC,

    /**
     * Check whether an object is an instance of a type.
     *
     * The operand stack remains unchanged in the following cases:
     *     CASE 1: The specified reference is `null`.
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
     * Compare two `float` values.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `float` value. It is popped off the operand stack.
     * value2
     *     A `float` value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If `value1` is greater than `value2`, then
     *           the result is a positive integer value {@code 1}.
     *         - If `value1` is lesser than `value2`, then
     *           the result is a negative integer value {@code -1}.
     *         - If `value1` is equal to `value2`, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code -1}.
     */
    ZEN_BYTE_CODE_COMPARE_LT_F,

    /**
     * Compare two `float` values.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `float` value. It is popped off the operand stack.
     * value2
     *     A `float` value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If `value1` is greater than `value2`, then
     *           the result is a positive integer value {@code 1}.
     *         - If `value1` is lesser than `value2`, then
     *           the result is a negative integer value {@code -1}.
     *         - If `value1` is equal to `value2`, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code 1}.
     */
    ZEN_BYTE_CODE_COMPARE_GT_F,

    /**
     * Compare two `double` values.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `double` value. It is popped off the operand stack.
     * value2
     *     A `double` value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If `value1` is greater than `value2`, then
     *           the result is a positive integer value {@code 1}.
     *         - If `value1` is lesser than `value2`, then
     *           the result is a negative integer value {@code -1}.
     *         - If `value1` is equal to `value2`, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code -1}.
     */
    ZEN_BYTE_CODE_COMPARE_LT_D,

    /**
     * Compare two `double` values.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `double` value. It is popped off the operand stack.
     * value2
     *     A `double` value. It is popped off the operand stack.
     * result
     *     The result varies contingent on the following cases. It is pushed
     *     onto the stack.
     *         - If `value1` is greater than `value2`, then
     *           the result is a positive integer value {@code 1}.
     *         - If `value1` is lesser than `value2`, then
     *           the result is a negative integer value {@code -1}.
     *         - If `value1` is equal to `value2`, then
     *           the result is a neutral integer {@code 0}.
     *         - For an unordered pair of values, the result is always
     *          {@code 1}.
     */
    ZEN_BYTE_CODE_COMPARE_GT_D,

    /* Divide */

    ZEN_BYTE_CODE_DIVIDE_I,
    ZEN_BYTE_CODE_DIVIDE_L,

    /**
     * Divide two `float` values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when dividing two `float` values.
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
     *       the magnitude is too large to represent as a `float`,
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a `float`, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * divide_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `float` value. It is popped off the operand stack.
     * value2
     *     A `float` value. It is popped off the operand stack.
     * result
     *     A `float` value which is the quotient computed by the
     *     division of `value1` and `value2`, represented as
     *     {@code value1 / value2}.
     */
    ZEN_BYTE_CODE_DIVIDE_F,

    /**
     * Divide two `double` values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when dividing two `double` values.
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
     *       the magnitude is too large to represent as a `double`,
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a `double`, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * divide_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `double` value. It is popped off the operand stack.
     * value2
     *     A `double` value. It is popped off the operand stack.
     * result
     *     A `double` value which is the quotient computed by the
     *     division of `value1` and `value2`, represented as
     *     {@code value1 / value2}.
     */
    ZEN_BYTE_CODE_DIVIDE_D,

    /* Duplicate */

    /**
     * Duplicate the value on the top of the operand stack. The copy is inserted
     * at the top of the operand stack. This instruction always copies a single
     * byte. Remember that the operand stack is unaware of value types.
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
     * result
     *     A duplicate of the value on top of the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE,

    /**
     * Duplicate the value on the top of the operand stack. The copy is inserted
     * below the first two values on top of the operand stack. This instruction
     * always copies a single byte. Remember that the operand stack is unaware
     * of value types.
     *
     * [Format]
     * duplicate_x1
     *
     * [Operand Stack]
     * Before
     *     ..., value2, value1
     * After
     *     ..., value1, value2, value1
     *
     * [Operands]
     * value
     *     The value on top of the operand stack. It remains unchanged.
     * result
     *     A copy of the value is inserted below the fist two values on top
     *     of the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE_X1,

    /**
     * Duplicate the value on the top of the operand stack. The copy is inserted
     * below the first three values on top of the operand stack. This instruction
     * always copies a single byte. Remember that the operand stack is unaware
     * of value types.
     *
     * [Format]
     * duplicate_x2
     *
     * [Operand Stack]
     * Before
     *     ..., value3, value2, value1
     * After
     *     ..., value1, value3, value2, value1
     *
     * [Operands]
     * value
     *     The value on top of the operand stack. It remains unchanged.
     * result
     *     A copy of the value is inserted below the fist three values on top
     *     of the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE_X2,

    /**
     * Duplicate the first two values on the top of the operand stack. The copies
     * are inserted at the top of the operand stack. This instruction always copies
     * two bytes. Remember that the operand stack is unaware of value types.
     *
     * [Format]
     * duplicate2
     *
     * [Operand Stack]
     * Before
     *     ..., value2, value1
     * After
     *     ..., value2, value1, value2, value1
     *
     * [Operands]
     * value1
     *     The value on top of the operand stack. It remains unchanged.
     * value2
     *     The value below the value on top of the operand stack. It remains
     *     unchanged.
     * result
     *     A duplicate of the first two values on top of the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE2,

    /**
     * Duplicate the first two values on the top of the operand stack. The copies
     * are inserted below the first three values on top of the operand stack.
     * This instruction always copies two bytes. Remember that the operand stack
     * is unaware of value types.
     *
     * [Format]
     * duplicate2_x1
     *
     * [Operand Stack]
     * Before
     *     ..., value3, value2, value1
     * After
     *     ..., value2, value1, value3, value2, value1
     *
     * [Operands]
     * value1
     *     The value on top of the operand stack. It remains unchanged.
     * value2
     *     The value below the value on top of the operand stack. It remains
     *     unchanged.
     * result
     *     A duplicate of the first two values on top of the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE2_X1,

    /**
     * Duplicate the first two values on the top of the operand stack. The copies
     * are inserted below the first four values on top of the operand stack.
     * This instruction always copies two bytes. Remember that the operand stack
     * is unaware of value types.
     *
     * [Format]
     * duplicate2_x2
     *
     * [Operand Stack]
     * Before
     *     ..., value4, value3, value2, value1
     * After
     *     ..., value2, value1, value4, value3, value2, value1
     *
     * [Operands]
     * value1
     *     The value on top of the operand stack. It remains unchanged.
     * value2
     *     The value below the value on top of the operand stack. It remains
     *     unchanged.
     * result
     *     A duplicate of the first two values on top of the operand stack.
     */
    ZEN_BYTE_CODE_DUPLICATE2_X2,

    /* Jump */

    /**
     * Jump to the specified offset if `value` is equal to `0`, shown as
     * `value == 0`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_eq0_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_EQ0_I,

    /**
     * Jump to the specified offset if `value` is not equal to `0`, shown as
     * `value != 0`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_ne0_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_NE0_I,

    /**
     * Jump to the specified offset if `value` is lesser than `0`, shown as
     * `value < 0`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_lt0_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_LT0_I,

    /**
     * Jump to the specified offset if `value` is greater than `0`, shown as
     * `value > 0`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_lt0_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_GT0_I,

    /**
     * Jump to the specified offset if `value` is lesser than or equal to `0`,
     * shown as `value <= 0`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_le0_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_LE0_I,

    /**
     * Jump to the specified offset if `value` is greater than or equal to `0`,
     * shown as `value >= 0`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_ge0_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_GE0_I,

    /**
     * Jump to the specified offset if `value1` is equal to `value2`, shown as
     * `value1 == value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_eq_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_EQ_I,

    /**
     * Jump to the specified offset if `value1` is not equal to `value2`, shown
     * as `value1 != value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_ne_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_NE_I,

    /**
     * Jump to the specified offset if `value1` is lesser than `value2`, shown
     * as `value1 < value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_lt_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_LT_I,

    /**
     * Jump to the specified offset if `value1` is greater than `value2`, shown
     * as `value1 > value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_gt_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_GT_I,

    /**
     * Jump to the specified offset if `value1` is lesser than or equal to
     * `value2`, shown as `value1 <= value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_le_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_LE_I,

    /**
     * Jump to the specified offset if `value1` is greater than or equal to
     * `value2`, shown as `value1 <= value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_ge_i offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_GE_I,

    /**
     * Jump to the specified offset if `value1` is equal to `value2`, shown as
     * `value1 == value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_eq_a offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A reference value. The size is contingent on the machine, which is
     *     transparent to the binary entity format. The value is popped off the
     *     operand stack.
     * value2
     *     A reference value. The size is contingent on the machine, which is
     *     transparent to the binary entity format. The value is popped off the
     *     operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_EQ_A,

    /**
     * Jump to the specified offset if `value1` is not equal to `value2`, shown as
     * `value1 != value2`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_ne_a offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     A reference value. The size is contingent on the machine, which is
     *     transparent to the binary entity format. The value is popped off the
     *     operand stack.
     * value2
     *     A reference value. The size is contingent on the machine, which is
     *     transparent to the binary entity format. The value is popped off the
     *     operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_NE_A,

    /**
     * Jump to the specified offset if `value` is equal to `null`, shown as
     * `value1 == null`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_eqn_a offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A reference value. The size is contingent on the machine, which is
     *     transparent to the binary entity format. The value is popped off the
     *     operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_EQN_A, // equal to null

    /**
     * Jump to the specified offset if `value` is not equal to `null`, shown as
     * `value1 != null`.
     *
     * If the comparison succeeds, the `offset0` and `offset1` arguments are
     * used to evaluate a signed 16-bit offset, with the expression
     * `(offset0 << 8) | offset1`. The control is transferred to the instruction
     * at this offset. The offset should point to a valid byte within the byte
     * code range.
     *
     * If the comparison fails, the control transfers to the instruction following
     * this instruction.
     *
     * [Format]
     * jump_nen_a offset0 offset1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     A reference value. The size is contingent on the machine, which is
     *     transparent to the binary entity format. The value is popped off the
     *     operand stack.
     * result
     *     This instruction generates no result.
     */
    ZEN_BYTE_CODE_JUMP_NEN_A, // not equal to null

    /* Increment */

    /**
     * Increment a local integer variable.
     *
     * The `increment_i` instruction can be used in conjunction with the `wide`
     * mode, in which case the index and constant arguments occupy two bytes each.
     *
     * [Format]
     * increment_i index constant
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a 32-bit integer.
     * constant
     *    A signed byte which is added to the current value of the local
     *    variable.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ...
     *
     * [Operands]
     *     This instruction does not require any operands.
     */
    ZEN_BYTE_CODE_INCREMENT_I,

    /* Invoke */

    /**
     * Invoke a special function such as constructor.
     *
     * A new frame is created on the virtual machine stack for the function
     * being invoked. The instance reference and arguments are consecutively
     * stored in the local variable array of the new frame. The instance reference
     * stored at index `0`, `argument1` at `1`, `argument2` at `2` and so on.
     *
     * It should be noted that in the case of values such `long` and `double`,
     * two indexes are occupied. Because the reference type depends on the platform,
     * it may occupy one or two indexes. This is transparent to the binary entity.
     *
     * After the new frame is initialized, it is promoted as the current frame.
     * The instruction pointer (IP) of the current frame is initialized to the
     * first instruction of the invoked function.
     *
     *
     * [Format]
     * invoke_special index0 index1
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1` forms an index into the constant
     *     pool. The entry at the specified index should be a `ConstantPoolFunction`
     *     instance.
     * index1
     *    Please refer the documentation of `index0` for more details.
     *
     * [Operand Stack]
     * Before
     *     ..., instance_reference, [argument1, [argument2, ...]]
     * After
     *     ...
     *
     * [Operands]
     * instance_reference
     *     A reference to an instance whose special function is to be invoked.
     *     It is popped off the operand stack. The size is contingent on the
     *     machine, which is transparent to the binary entity format. The value
     *     is popped off the operand stack.
     * argument1, argument2, ...
     *     The arguments passed to the special function. The signature of the
     *     function determines the number of operands popped off the operand
     *     stack as arguments.
     * result
     *     This instruction does not generate any result.
     *
     * [Exceptions]
     * NullPointerException
     *     If the instance reference is `null`, this instruction causes the
     *     virtual machine to throw a `zen.core.NullPointerException`.
     */
    ZEN_BYTE_CODE_INVOKE_SPECIAL,

    /**
     * Invoke a function against an instance.
     *
     * [Format]
     * invoke_virtual index0 index1
     *
     * [Operand Stack]
     * Before
     *     ..., object_reference, [argument_1, [argument_2, [argument_3, ...]]]
     * After
     *     ...,
     *
     * [Operands]
     * index_byte_1
     *     The `index0` and `index1` bytes together indicate an unsigned integer
     *     index into the runtime constant pool of the current binary entity.
     *     The item at this index represents a function descriptor.
     * index_byte_2
     *     Please refer the documentation for `index0`.
     *
     */
    ZEN_BYTE_CODE_INVOKE_VIRTUAL,
    ZEN_BYTE_CODE_INVOKE_DYNAMIC,

    /**
     * Invoke a static function.
     *
     * If the function is not native, a new frame is created on the virtual
     * machine stack for the function being invoked. The arguments are consecutively
     * stored in the local variable array of the new frame. `argument1` at `1`,
     * `argument2` at `2` and so on.
     *
     * After the new frame is initialized, it is promoted as the current frame.
     * The instruction pointer (IP) of the current frame is initialized to the
     * first instruction of the invoked function.
     *
     * If the function returns a value, the value is pushed onto the operand
     * stack of the previous stack frame.
     *
     * If the function is native, a new frame is created on the virtual
     * machine stack for the function being invoked. The arguments are consecutively
     * stored in the local variable array of the new frame. `argument1` at `1`,
     * `argument2` at `2` and so on.
     *
     * If the native function returns a value, the platform dependent return
     * value is converted to an implementation dependent value. The new value is
     * then pushed onto the operand stack of the previous stack frame.
     *
     * It should be noted that in the case of values such `long` and `double`,
     * two indexes are occupied. Because the reference type depends on the platform,
     * it may occupy one or two indexes. This is transparent to the binary entity.
     *
     * [Format]
     * invoke_static index0 index1
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1` forms an index into the constant
     *     pool. The entry at the specified index should be a `ConstantPoolFunction`
     *     instance.
     * index1
     *    Please refer the documentation of `index0` for more details.
     *
     * [Operand Stack]
     * Before
     *     ..., [argument1, [argument2, ...]]
     * After
     *     ...
     *
     * [Operands]
     * argument1, argument2, ...
     *     The arguments passed to the static function. The signature of the
     *     function determines the number of operands popped off the operand
     *     stack as arguments.
     * result
     *     This instruction does not generate any result.
     *
     * [Exceptions]
     * UndefinedNativeFunctionError
     *     If a native function is invoked and the virtual machine is not able
     *     to find its definition, this instruction causes the virtual machine
     *     to throw a `zen.core.UndefinedNativeFunctionError`.
     */
    ZEN_BYTE_CODE_INVOKE_STATIC,

    /* Jump */

    /**
     * Jump to the specified index.
     *
     * The instruction pointer (IP) of the current frame is updated to the
     * instruction at the specified index. The control transfers to that
     * instruction.
     *
     * [Format]
     * jump index0 index1
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1` forms an index into the constant
     *     pool. The instruction at the specified index should be a valid instruction.
     * index1
     *    Please refer the documentation of `index0` for more details.
     */
    ZEN_BYTE_CODE_JUMP,

    /* Load */

    /**
     * Load a 32-bit integer value from a local variable.
     *
     * The `load_i` instruction can be used in conjunction with the `wide`
     * mode, in which case the index occupies two bytes.

     *
     * [Format]
     * load_i index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a 32-bit integer value. This value is pushed onto the operand
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
     *     The 32-bit integer value stored at the specified index. It is
     *     pushed onto the stack.
     */
    ZEN_BYTE_CODE_LOAD_I,

    /**
     * Load a 64-bit integer value from a local variable.
     *
     * The `load_l` instruction can be used in conjunction with the `wide`
     * mode, in which case the index occupies two bytes.
     *
     * [Format]
     * load_l index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The values at `index` and `index + 1`
     *     are combined to construct a 64-bit integer value. This value is
     *     pushed onto the operand stack.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 64-bit integer value constructed with the values stored at
     *     `index` and `index + 1`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_LOAD_L,

    /**
     * Load a `float` value from a local variable.
     *
     * The `load_f` instruction can be used in conjunction with the `wide`
     * mode, in which case the index occupies two bytes.
     *
     * [Format]
     * load_f index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a `float`. This value is pushed onto the operand
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
     *     The `float` value stored at the specified index. It is
     *     pushed onto the stack.
     */
    ZEN_BYTE_CODE_LOAD_F,

    /**
     * Load a `double` value from a local variable.
     *
     * The `load_d` instruction can be used in conjunction with the `wide`
     * mode, in which case the index occupies two bytes.
     *
     * [Format]
     * load_d index
     *
     * [Arguments]
     * index
     *     An unsigned byte which is a valid index into the local variable
     *     array of the current frame. The local variable at the index should
     *     be a `double`. This reference is pushed onto the operand
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
     *     The `double` value stored at the specified index. It is
     *     pushed onto the stack.
     */
    ZEN_BYTE_CODE_LOAD_D,

    /**
     * Load a reference from a local variable.
     *
     * The `load_a` instruction can be used in conjunction with the `wide`
     * mode, in which case the index occupies two bytes.
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

    /**
     * Load a 32-bit integer value from a local variable.
     *
     * Here, `0` should be a valid index into the local variable array. The local
     * variable at this index should be a 32-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_i0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 32-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_I0,

    /**
     * Load a 32-bit integer value from a local variable.
     *
     * Here, `1` should be a valid index into the local variable array. The local
     * variable at this index should be a 32-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_i1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 32-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_I1,

    /**
     * Load a 32-bit integer value from a local variable.
     *
     * Here, `2` should be a valid index into the local variable array. The local
     * variable at this index should be a 32-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_i2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 32-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_I2,

    /**
     * Load a 32-bit integer value from a local variable.
     *
     * Here, `3` should be a valid index into the local variable array. The local
     * variable at this index should be a 32-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_i3
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 32-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_I3,

    /**
     * Load a 64-bit integer value from a local variable.
     *
     * Here, `0` should be a valid index into the local variable array. The local
     * variable at this index should be a 64-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_l0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 64-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_L0,


    /**
     * Load a 64-bit integer value from a local variable.
     *
     * Here, `1` should be a valid index into the local variable array. The local
     * variable at this index should be a 64-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_l1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 64-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_L1,

    /**
     * Load a 64-bit integer value from a local variable.
     *
     * Here, `2` should be a valid index into the local variable array. The local
     * variable at this index should be a 64-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_l2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 64-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_L2,

    /**
     * Load a 64-bit integer value from a local variable.
     *
     * Here, `3` should be a valid index into the local variable array. The local
     * variable at this index should be a 64-bit integer value. This value
     * is pushed onto the operand stack.
     *
     * [Format]
     * load_l3
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     The 64-bit integer value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_L3,

    /**
     * Load a `float` value from a local variable.
     *
     * Here, 0 should be a valid index into the local variable array. The local
     * variable at this index should be a `float` value. This reference
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
     *     The `float` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F0,

    /**
     * Load a `float` value from a local variable.
     *
     * Here, 1 should be a valid index into the local variable array. The local
     * variable at this index should be a `float` value. This reference
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
     *     The `float` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F1,

    /**
     * Load a `float` value from a local variable.
     *
     * Here, 2 should be a valid index into the local variable array. The local
     * variable at this index should be a `float` value. This reference
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
     *     The `float` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F2,

    /**
     * Load a `float` value from a local variable.
     *
     * Here, 3 should be a valid index into the local variable array. The local
     * variable at this index should be a `float` value. This reference
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
     *     The `float` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_F3,

    /**
     * Load a `double` value from a local variable.
     *
     * Here, 0 should be a valid index into the local variable array. The local
     * variable at this index should be a `double` value. This reference
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
     *     The `double` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D0,

    /**
     * Load a `double` value from a local variable.
     *
     * Here, 1 should be a valid index into the local variable array. The local
     * variable at this index should be a `double` value. This reference
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
     *     The `double` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D1,

    /**
     * Load a `double` value from a local variable.
     *
     * Here, 2 should be a valid index into the local variable array. The local
     * variable at this index should be a `double` value. This reference
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
     *     The `double` value at the specified index in the local
     *     variable array. It is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_D2,

    /**
     * Load a `double` value from a local variable.
     *
     * Here, 3 should be a valid index into the local variable array. The local
     * variable at this index should be a `double` value. This reference
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
     *     The `double` value at the specified index in the local
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
     * value
     *     A signed 32-bit integer derived by extending the sign bit of the
     *     8-bit integer value at the specified index.
     *
     * [Exceptions]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.core.InvalidArrayIndexException
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
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AC,

    // TODO: Characters are stored in 16-bit integers or 8-bit integers?!

    /**
     * Load a 16-bit integer value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * [Format]
     * load_as
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code short}.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *     A signed 32-bit integer derived by extending the sign bit of the
     *     16-bit integer value at the specified index.
     *
     * [Exception]
     *
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.core.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AS,

    /**
     * Load a 32-bit integer value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * [Format]
     * load_ai
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code integer}.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *     The signed 32-bit integer value at the specified index.
     *
     * [Exception]
     *
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.core.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AI,

    /**
     * Load a 64-bit integer value from an array.
     *
     * The value at the specified index is pushed onto the operand stack.
     *
     * [Format]
     * load_ai
     *
     * [Operand Stack]
     * Before
     *     ..., array, index
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is {@code integer}.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *     The signed 64-bit integer value at the specified index.
     *
     * [Exception]
     *
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.core.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AL,

    /**
     * Load a `float` value from an array.
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
     *     A reference to an array whose component type is `float`.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *    A 32-bit `float` value that is stored at the specified index
     *    in the specified `float` array. It is pushed onto the stack.
     *
     * [Exception]
     *
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_LOAD_AF,

    /**
     * Load a `double` value from an array.
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
     *     A reference to an array whose component type is `double`.
     *     It is popped off the stack.
     * index
     *     A signed 32-bit integer which indicates the index of the
     *     component to retrieve. It is popped off the stack.
     * value
     *    A 64-bit `double` value that is stored at the specified index
     *    in the specified `double` array. It is pushed onto the stack.
     *
     * [Exception]
     *
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
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
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid.
     */
    ZEN_BYTE_CODE_LOAD_AA,

    /**
     * Load the value stored in an instance field.
     *
     * Pops the argument from the stack. The value stored in the instance field
     * is retrieved and pushed onto the operand stack.
     *
     * [Format]
     * load_instance_field index0 index1
     *
     * [Operand Stack]
     * Before
     *     ..., instance_reference
     * After
     *     ..., value
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1`, forms a valid index into the
     *     constant pool where the entry is a `ConstantPoolField` instance.
     * index1
     *     Please refer the documentation for `index0` for more details.
     *
     * [Operands]
     * instance_reference
     *     A reference to an instance whose field is to be loaded.
     *     It is popped off the operand stack. The size is contingent on the
     *     machine, which is transparent to the binary entity format. The value
     *     is popped off the operand stack.
     * value
     *     The value stored in the instance field.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified instance reference is `null`.
     */
    ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD,

    /**
     * Load the value stored in a static field.
     *
     * Pops the argument from the stack. The value stored in the static field
     * is retrieved and pushed onto the operand stack.
     *
     * [Format]
     * load_static_field index0 index1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1`, forms a valid index into the
     *     constant pool where the entry is a `ConstantPoolField` instance.
     * index1
     *     Please refer the documentation for `index0` for more details.
     *
     * [Operands]
     * value
     *     The value stored in the static field.
     */
    ZEN_BYTE_CODE_LOAD_STATIC_FIELD,

    /**
     * Load an item from the constant pool.
     *
     * The `load_cpr` instruction can be used in conjunction with the `wide`
     * mode, in which case the index occupies two bytes.
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
     *     The value of the item is pushed onto the operand stack.
     */
    ZEN_BYTE_CODE_LOAD_CPR, // Constant Pool Reference

    /**
     * Load the size of an array.
     *
     * Pops the argument from the stack. The size of the array is retrieved
     * and pushed onto the operand stack.
     *
     * [Format]
     * load_array_length
     *
     * [Operand Stack]
     * Before
     *     ..., array
     * After
     *     ..., value
     *
     * [Operands]
     * array
     *     A reference to an array whose size is to be loaded.
     *     It is popped off the operand stack. The size is contingent on
     *     the machine, which is transparent to the binary entity format.
     *     The value is popped off the operand stack.
     * value
     *     The size of the array.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     */
    ZEN_BYTE_CODE_LOAD_ARRAY_SIZE,

    /* Modulo */

    /**
     * Divide two 32-bit integer values to compute the remainder.
     *
     * [Format]
     * modulo_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is the remainder of dividing
     *     `value1` and `value2`. It is pushed onto the stack.
     *
     * [Exception]
     * zen.core.InvalidDivisorException
     *     An instance of the {@code InvalidDivisorException} class is thrown
     *     if the specified divisor is `0`.
     */
    ZEN_BYTE_CODE_MODULO_I,

    /**
     * Divide two 64-bit integer values to compute the remainder.
     *
     * [Format]
     * modulo_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is the remainder of dividing
     *     `value1` and `value2`. It is pushed onto the stack.
     *
     * [Exception]
     * zen.core.InvalidDivisorException
     *     An instance of the {@code InvalidDivisorException} class is thrown
     *     if the specified divisor is `0`.
     */
    ZEN_BYTE_CODE_MODULO_L,

    /**
     * Divide two `float` values to compute the remainder.
     *
     * It should be noted that the remainder operation is not in
     * accordance with the IEEE 754. Otherwise, the {@code remainder_f}
     * instruction would not behave analogous to the {@code remainder_i}
     * and {@code remainder_l} instructions. Therefore, the rules for
     * the {@code remainder_f} is derived from the {@code fmod} function
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
     *       expression {@code value1 - (value2 * x)}, where x is a
     *       positive or negative integer with the sign equivalent to the
     *       sign of {@code value1 / value2}.
     *
     * [Format]
     * modulo_f
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `float` value. It is popped off the operand stack.
     * value2
     *     A `float` value. It is popped off the operand stack.
     * result
     *     A `float` value which is the remainder of dividing
     *     `value1` and `value2`. It is pushed onto the
     *     stack.
     */
    ZEN_BYTE_CODE_MODULO_F,

    /**
     * Divide two `double` values to compute the remainder.
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
     *       expression {@code value1 - (value2 * x)}, where x is a
     *       positive or negative integer with the sign equivalent to the
     *       sign of {@code value1 / value2}.
     *
     * [Format]
     * modulo_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `double` value. It is popped off the operand stack.
     * value2
     *     A `double` value. It is popped off the operand stack.
     * result
     *     A `double` value which is the remainder of dividing
     *     `value1` and `value2`. It is pushed onto the
     *     stack.
     */
    ZEN_BYTE_CODE_MODULO_D,

    /* Multiply */

    /**
     * Multiply two 32-bit integer values.
     *
     * [Format]
     * multiply_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is the quotient computed by the
     *     multiplication of `value1` and `value2`, represented
     *     as `value1 * value2`.
     */
    ZEN_BYTE_CODE_MULTIPLY_I,

    /**
     * Multiply two 64-bit integer values.
     *
     * [Format]
     * multiply_i
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is the quotient computed by the
     *     multiplication of `value1` and `value2`, represented
     *     as `value1 * value2`.
     */
    ZEN_BYTE_CODE_MULTIPLY_L,

    /**
     * Multiply two `float` values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when two `float` values are multiplied.
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
     *       If the magnitude is too large to represent as a `float`,
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a `float`, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * multiply_f
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `float` value. It is popped off the operand stack.
     * value2
     *     A `float` value. It is popped off the operand stack.
     * result
     *     A `float` value which is the quotient computed by the
     *     multiplication of `value1` and `value2`, represented
     *     as `value1 * value2`.
     */
    ZEN_BYTE_CODE_MULTIPLY_F,

    /**
     * Multiply two `double` values.
     *
     * The following rules, in accordance with IEEE 754, should be observed
     * when two `float` values are multiplied.
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
     *       If the magnitude is too large to represent as a `double`,
     *       the result is an infinity of an appropriate sign. If the magnitude
     *       is too small to represent as a `double`, the result is a
     *       zero of an appropriate sign.
     *
     * [Format]
     * multiply_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A `double` value. It is popped off the operand stack.
     * value2
     *     A `double` value. It is popped off the operand stack.
     * result
     *     A `double` value which is the quotient computed by the
     *     multiplication of `value1` and `value2`, represented
     *     as `value1 * value2`.
     */
    ZEN_BYTE_CODE_MULTIPLY_D,

    /* Negate */

    /**
     * Negate a 32-bit integer value.
     *
     * The 32-bit integer operand is popped off the operand stack. A 32-bit
     * integer result is evaluated by negating the operand, shown as `-value`.
     *
     * [Format]
     * negate_i
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by negating the
     *     specified 32-bit integer value.
     */
    ZEN_BYTE_CODE_NEGATE_I,

    /**
     * Negate a 64-bit integer value.
     *
     * The 64-bit integer operand is popped off the operand stack. A 64-bit
     * integer result is evaluated by negating the operand, shown as `-value`.
     *
     * [Format]
     * negate_l
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by negating the
     *     specified 64-bit integer value.
     */
    ZEN_BYTE_CODE_NEGATE_L,

    /**
     * Negate a 32-bit decimal value.
     *
     * The 32-bit decimal operand is popped off the operand stack. A 32-bit
     * decimal result is evaluated by negating the operand, shown as `-value`.
     *
     * It should be noted that the following cases are treated specially.
     *     - If the operand is NaN, the result is NaN.
     *     - If the operand is infinity, the result is the infinity of opposite
     *       sign.
     *     - If the operand is a zero, the result is the zero of opposite sign.
     *
     * [Format]
     * negate_f
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 32-bit decimal value. It is popped off the operand stack.
     * result
     *     A 32-bit decimal value which is computed by negating the
     *     specified 32-bit decimal value.
     */
    ZEN_BYTE_CODE_NEGATE_F,

    /**
     * Negate a 64-bit decimal value.
     *
     * The 64-bit decimal operand is popped off the operand stack. A 64-bit
     * decimal result is evaluated by negating the operand, shown as `-value`.
     *
     * It should be noted that the following cases are treated specially.
     *     - If the operand is NaN, the result is NaN.
     *     - If the operand is infinity, the result is the infinity of opposite
     *       sign.
     *     - If the operand is a zero, the result is the zero of opposite sign.
     *
     * [Format]
     * negate_f
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ..., result
     *
     * [Operands]
     * value
     *     A 64-bit decimal value. It is popped off the operand stack.
     * result
     *     A 64-bit decimal value which is computed by negating the
     *     specified 64-bit decimal value.
     */
    ZEN_BYTE_CODE_NEGATE_D,

    /* New */

    /**
     * Create a new object.
     *
     * [Format]
     * new index0 index1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., result
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1` forms an index into the constant
     *     pool. The entry at the specified index should be a `ConstantPoolClass`
     *     instance.
     * index1
     *    Please refer the documentation of `index0` for more details.
     *
     * [Operands]
     * result
     *     A reference to the new instance of the specified class.
     */
    ZEN_BYTE_CODE_NEW,

    /**
     * Create a new single dimension primitive array.
     *
     * A new array with the specified number of components is created. All
     * components of the newly created array are initialized to `0`.
     *
     * [Format]
     * new_array type
     *
     * [Arguments]
     * type
     *     The type represents the component type of the array. Please refer
     *     the documentation of `zen_PrimitiveType_t` for more information.
     *
     * [Operand Stack]
     * Before
     *     ..., size
     * After
     *     ..., primitive_array
     *
     * [Operands]
     * size
     *     An unsigned 32-bit integer which indicates the size of the array.
     *     It is popped off the operand stack.
     * primitive_array
     *     A reference to the new array is pushed onto the stack.
     *
     * [Exceptions]
     * zen.base.InvalidArraySizeException
     *     An instance of the `zen.core.InvalidArraySizeException` class is thrown
     *     if the specified array size is invalid. An array size is invalid
     *     if it is less than zero.
     */
    ZEN_BYTE_CODE_NEW_ARRAY,

    /**
     * Create a new single dimension reference array.
     *
     * A new array with the specified number of components is created. All
     * components of the newly created array are initialized to `null`.
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
     * reference_array
     *     A reference to the new array is pushed onto the stack.
     *
     * [Exception]
     * During resolution of the class, enumeration, or array types, any of the
     * documented exceptions for type resolution may be thrown.
     *
     * zen.base.InvalidArraySizeException
     *     An instance of the `zen.core.InvalidArraySizeException` class is thrown
     *     if the specified array size is invalid. An array size is invalid
     *     if it is less than zero.
     */
    ZEN_BYTE_CODE_NEW_ARRAY_A,

    /**
     * Create a new multi-dimension reference array.
     *
     * A new array with the specified number of dimensions is created. All
     * components of the newly created array are initialized to `null`.
     *
     * [Format]
     * new_array_an index_byte_1 index_byte_2 dimensions
     *
     * [Arguments]
     * index_byte_1
     *     The {@code index_byte_1} and {@code index_byte_2} together indicate
     *     an unsigned integer index into the run-time constant pool of the
     *     current binary entity. The item at this index represents a reference
     *     to a class, an enumeration, or an array type.
     * index_byte_2
     *     Please refer the documentation for {@code index_byte_1}.
     * dimensions
     *     An unsigned byte that represents the number of dimensions. It should
     *     be greater than or equal to 1.
     *
     * [Operand Stack]
     * Before
     *     ..., size1, [size2, [size3, ...]]
     * After
     *     ..., reference_array
     *
     * [Operands]
     * size1
     *     An signed 32-bit integer which indicates the size of the first
     *     dimension. It is popped off the operand stack.
     * [size2, [size3, ...]]
     *     Signed 32-bit integers which indicate the sizes of the dimensions
     *     of the array. They are all popped off the operand stack.
     * reference_array
     *     A reference to the new array is pushed onto the stack.
     *
     * [Exception]
     * During resolution of the class, enumeration, or array types, any of the
     * documented exceptions for type resolution may be thrown.
     *
     * zen.base.InvalidArraySizeException
     *     An instance of the `zen.core.InvalidArraySizeException` class is thrown
     *     if the specified array size is invalid. An array size is invalid
     *     if it is less than zero.
     */
    ZEN_BYTE_CODE_NEW_ARRAY_AN,

    /* Pop */

    /**
     * Pops the value on top of the operand stack.
     *
     * [Format]
     * pop
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit value on top of the operand stack that is popped off.
     */
    ZEN_BYTE_CODE_POP,

    /**
     * Pops the first two values on top of the operand stack.
     *
     * [Format]
     * pop2
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ...
     *
     * [Operands]
     * value1
     *     The 32-bit value below `value2` that is popped off.
     * value2
     *     The 32-bit value on top of the operand stack that is popped off.
     */
    ZEN_BYTE_CODE_POP2,

    /* Push */

    /**
     * Push `null` onto the operand stack.
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
     * The {@code push_in1} and {@code push_b -1} instructions both push a value
     * of `-1` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas, the latter instruction occupies 2 bytes.
     * The value pushed by both the instruction always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     */
    ZEN_BYTE_CODE_PUSH_IN1,

    /**
     * Push a 32-bit integer value of `0` onto the operand stack.
     *
     * The {@code push_i0} and {@code push_b 0} instructions both push a value
     * of `0` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas the latter instruction occupies 2 bytes.
     * The value pushed by either of the instructions always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     *
     * [Format]
     * push_i0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value of `0`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_I0,

    /**
     * Push a 32-bit integer value of `1` onto the operand stack.
     *
     * The {@code push_i1} and {@code push_b 1} instructions both push a value
     * of `1` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas the latter instruction occupies 2 bytes.
     * The value pushed by either of the instructions always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     *
     * [Format]
     * push_i1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value of `1`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_I1,

    /**
     * Push a 32-bit integer value of `2` onto the operand stack.
     *
     * The {@code push_i2} and {@code push_b 2} instructions both push a value
     * of `2` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas the latter instruction occupies 2 bytes.
     * The value pushed by either of the instructions always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     *
     * [Format]
     * push_i2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value of `2`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_I2,

    /**
     * Push a 32-bit integer value of `3` onto the operand stack.
     *
     * The {@code push_i3} and {@code push_b 3} instructions both push a value
     * of `3` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas the latter instruction occupies 2 bytes.
     * The value pushed by either of the instructions always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     *
     * [Format]
     * push_i3
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value of `3`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_I3,

    /**
     * Push a 32-bit integer value of `4` onto the operand stack.
     *
     * The {@code push_i4} and {@code push_b 4} instructions both push a value
     * of `4` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas the latter instruction occupies 2 bytes.
     * The value pushed by either of the instructions always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     *
     * [Format]
     * push_i4
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value of `4`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_I4,

    /**
     * Push a 32-bit integer value of `5` onto the operand stack.
     *
     * The {@code push_i5} and {@code push_b 5} instructions both push a value
     * of `5` onto the operand stack. The former instruction occupies 1 byte
     * in the byte steam. Whereas the latter instruction occupies 2 bytes.
     * The value pushed by either of the instructions always occupies four bytes.
     *
     * The values -1, 0, 1, 2, 3, 4, and 5 are pushed with push_i<n> instructions.
     * The values -128 .. -2 and 6 .. 127 are pushed with push_b instruction.
     * The values -32768 .. -128 and 128 .. 32,767 are pushed with push_s
     * instruction. Other values are pushed with `load_cpr` instruction.
     *
     * [Format]
     * push_i5
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value of `5`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_I5,

    /**
     * Push a 64-bit integer value of `0` onto the operand stack.
     *
     * [Format]
     * push_l0
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 64-bit integer value of `0`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_L0,

    /**
     * Push a 64-bit integer value of `1` onto the operand stack.
     *
     * [Format]
     * push_l1
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 64-bit integer value of `1`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_L1,

    /**
     * Push a 64-bit integer value of `2` onto the operand stack.
     *
     * [Format]
     * push_l2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 64-bit integer value of `2`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_L2,

    /**
     * Push a `float` value of `0.0` onto the operand stack.
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
     *     A `float` value of `0.0`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_F0,

    /**
     * Push a `float` value of {@code 1.0} onto the operand stack.
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
     *     A `float` value of {@code 1.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_F1,

    /**
     * Push a `float` value of {@code 2.0} onto the operand stack.
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
     *     A `float` value of {@code 2.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_F2,

    /**
     * Push a `double` value of `0.0` onto the operand stack.
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
     *     A `double` value of `0.0`. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_D0,

    /**
     * Push a `double` value of {@code 1.0} onto the operand stack.
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
     *     A `double` value of {@code 1.0}. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_D1,

    /**
     * Push a `double` value of {@code 2.0} onto the operand stack.
     *
     * [Format]
     * push_d2
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A `double` value of {@code 2.0}. It is pushed onto the stack.
     */
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
     *
     * [Operands]
     * value
     *     A 32-bit integer value derived from by sign extending the 8-bit
     *     integer argument. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_B,

    /**
     * Push a {@code byte} onto the operand stack.
     *
     * [Format]
     * push_b value0 value1
     *
     * [Arguments]
     * value0
     *     A unsigned byte, along with `value1` forms a signed 16-bit integer.
     * value1
     *     Please refer the documentation of `value0` for more information.
     *
     * [Operand Stack]
     * Before
     *     ...
     * After
     *     ..., value
     *
     * [Operands]
     * value
     *     A 32-bit integer value derived from by sign extending the 16-bit
     *     integer argument. It is pushed onto the stack.
     */
    ZEN_BYTE_CODE_PUSH_S,

    /* Return */

    /**
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
     * return
     */
    ZEN_BYTE_CODE_RETURN,

    /**
     * Return an `integer` value.
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
     * return_i
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     [invalid]
     *
     * [Operands]
     * value
     *     An `integer` value. This value is popped off the operand stack
     *     of the current frame and pushed onto the operand stack of the
     *     invoker frame.
     */
    ZEN_BYTE_CODE_RETURN_I,

    /**
     * Return a `long` value.
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
     * return_l
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     [invalid]
     *
     * [Operands]
     * value
     *     An `long` value. This value is popped off the operand stack
     *     of the current frame and pushed onto the operand stack of the
     *     invoker frame.
     */
    ZEN_BYTE_CODE_RETURN_L,

    /**
     * Return a `float` value.
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
     * return_f
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     [invalid]
     *
     * [Operands]
     * value
     *     An `float` value. This value is popped off the operand stack
     *     of the current frame and pushed onto the operand stack of the
     *     invoker frame.
     */
    ZEN_BYTE_CODE_RETURN_F,

    /**
     * Return a `double` value.
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
     *     A `double` value. This value is popped off the operand stack
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

    /* RTTI */

    ZEN_BYTE_CODE_RTTI,

    /* Store */

    /**
     * Store a 32-bit integer value in a local variable.
     *
     * Further, a local variable can be accessed using a two-byte unsigned
     * index using the wide mode of this instruction.
     *
     * [Format]
     * store_i index
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
     *     The 32-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_I,

    /**
     * Store a 32-bit integer value in a local variable.
     *
     * Here, `0` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_i0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_I0,

    /**
     * Store a 32-bit integer value in a local variable.
     *
     * Here, `1` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_i0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_I1,

    /**
     * Store a  32-bit integer value in a local variable.
     *
     * Here, `2` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_i0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_I2,

    /**
     * Store a 32-bit integer value in a local variable.
     *
     * Here, `3` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_i0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_I3,

    /**
     * Store a 64-bit integer value in a local variable.
     *
     * Further, a local variable can be accessed using a two-byte unsigned
     * index using the wide mode of this instruction.
     *
     * [Format]
     * store_l index
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
     *     The 64-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_L,

    /**
     * Store a 64-bit integer value in a local variable.
     *
     * Here, `0` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_l0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 64-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_L0,

    /**
     * Store a 64-bit integer value in a local variable.
     *
     * Here, `1` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_l1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 64-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_L1,

    /**
     * Store a 64-bit integer value in a local variable.
     *
     * Here, `2` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_l2
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 64-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_L2,

    /**
     * Store a 64-bit integer value in a local variable.
     *
     * Here, `3` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_l3
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 64-bit integer value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_L3,

    /**
     * Store a 32-bit decimal value in a local variable.
     *
     * Further, a local variable can be accessed using a two-byte unsigned
     * index using the wide mode of this instruction.
     *
     * [Format]
     * store_f index
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
     *     The 32-bit decimal value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_F,

    /**
     * Store a 32-bit decimal value in a local variable.
     *
     * Here, `0` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_f0
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit decimal value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_F0,

    /**
     * Store a 32-bit decimal value in a local variable.
     *
     * Here, `1` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_f1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit decimal value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_F1,

    /**
     * Store a 32-bit decimal value in a local variable.
     *
     * Here, `2` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_f2
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit decimal value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_F2,

    /**
     * Store a 32-bit decimal value in a local variable.
     *
     * Here, `3` should be valid index into the local variable array of the
     * current frame.
     *
     * [Format]
     * store_f3
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Operands]
     * value
     *     The 32-bit decimal value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_F3,

    /**
     * Store a `double` value into local variable.
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
     *     The `double` value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D,

    /**
     * Store a `double` value into local variable.
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
     *     The `double` value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D0,

    /**
     * Store a `double` value into local variable.
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
     *     The `double` value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D1,

    /**
     * Store a `double` value into local variable.
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
     *     The `double` value which is assigned to the local variable.
     *     It is popped off the operand stack.
     */
    ZEN_BYTE_CODE_STORE_D2,

    /**
     * Store a `double` value into local variable.
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
     *     The `double` value which is assigned to the local variable.
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

    /* Store Array */

    /**
     * Store a 8-bit integer value in a `byte` array.
     *
     * [Format]
     * store_ab
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is `byte`.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 32-bit integer value which represents the 8-bit value to store.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
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
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AC,

    // TODO: 16-bit or 8-bit characters?

    /**
     * Store a 16-bit integer value in a `short` array.
     *
     * [Format]
     * store_as
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is `short`.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 32-bit integer value which represents the 16-bit value to store.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AS,

    /**
     * Store a 32-bit integer value in a `integer` array.
     *
     * [Format]
     * store_ai
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is `integer`.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 32-bit integer value which represents the value to store.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AI,

    /**
     * Store a 64-bit integer value in a `long` array.
     *
     * [Format]
     * store_al
     *
     * [Operand Stack]
     * Before
     *     ..., array, index, value
     * After
     *     ...
     *
     * [Operands]
     * array
     *     A reference to an array whose component type is `long`.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 64-bit integer value which represents the value to store.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AL,

    /**
     * Store a `float` value in a `float` array.
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
     *     A reference to an array whose component type is `float`.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 32-bit `float` value which represents the `float`
     *     value to store.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AF,

    /**
     * Store a `double` value in a `double` array.
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
     *     A reference to an array whose component type is `double`.
     *     It is popped off the operand stack.
     * index
     *     A 32-bit signed integer index into the specified array. The value is
     *     stored here. It is popped off the operand stack.
     * value
     *     A 64-bit `double` value which represents the `double`
     *     value to store.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
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
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified array is `null`.
     * zen.base.InvalidArrayIndexException
     *     An instance of the {@code InvalidArrayIndexException} class is
     *     thrown if the specified index is invalid. An array index is invalid
     *     if one of the following is true:
     *         - Index is negative
     *         - Index is greater than or equal to the array size
     */
    ZEN_BYTE_CODE_STORE_AA,

    /* Store Field */

    /**
     * Store a value in an instance field.
     *
     * Pops the arguments from the operand stack. The value is stored in the
     * instance field.
     *
     * [Format]
     * store_instance_field index0 index1
     *
     * [Operand Stack]
     * Before
     *     ..., instance_reference, value
     * After
     *     ...
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1`, forms a valid index into the
     *     constant pool where the entry is a `ConstantPoolField` instance.
     * index1
     *     Please refer the documentation for `index0` for more details.
     *
     * [Operands]
     * instance_reference
     *     A reference to an instance whose field is to be modified.
     *     It is popped off the operand stack. The size is contingent on the
     *     machine, which is transparent to the binary entity format. The
     *     reference is popped off the operand stack.
     * value
     *     The value stored in the instance field.
     *
     * [Exception]
     * zen.core.NullPointerException
     *     An instance of the {@code NullPointerException} class is thrown
     *     if the specified instance reference is `null`.
     */
    ZEN_BYTE_CODE_STORE_INSTANCE_FIELD,

    /**
     * Store a value in a static field.
     *
     * Pops the argument from the stack. The value is stored in the static
     * field.
     *
     * [Format]
     * store_static_field index0 index1
     *
     * [Operand Stack]
     * Before
     *     ..., value
     * After
     *     ...
     *
     * [Arguments]
     * index0
     *     An unsigned byte, along with `index1`, forms a valid index into the
     *     constant pool where the entry is a `ConstantPoolField` instance.
     * index1
     *     Please refer the documentation for `index0` for more details.
     *
     * [Operands]
     * value
     *     The value stored in the static field.
     */
    ZEN_BYTE_CODE_STORE_STATIC_FIELD,

    /* Subtract */

    /**
     * Subtracts two 32-bit integer values.
     *
     * Two 32-bit integer operands are popped off the operand stack. A 32-bit
     * integer result is evaluated by subtracting the second operand from the
     * first operand, shown as {@code value1 - value2}.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit integer value. It is popped off the operand stack.
     * value2
     *     A 32-bit integer value. It is popped off the operand stack.
     * result
     *     A 32-bit integer value which is computed by subtraction of the
     *     specified 32-bit integer values.
     */
    ZEN_BYTE_CODE_SUBTRACT_I,

    /**
     * Subtracts two 64-bit integer values.
     *
     * Two 64-bit integer operands are popped off the operand stack. A 64-bit
     * integer result is evaluated by subtracting the second operand from the
     * first operand, shown as {@code value1 - value2}.
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
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit integer value. It is popped off the operand stack.
     * value2
     *     A 64-bit integer value. It is popped off the operand stack.
     * result
     *     A 64-bit integer value which is computed by subtraction of the
     *     specified 64-bit integer values.
     */
    ZEN_BYTE_CODE_SUBTRACT_L,

    /**
     * Subtracts two 32-bit decimal values.
     *
     * Two 32-bit decimal operands are popped off the operand stack. A 32-bit
     * decimal result is evaluated by subtracting the second operand from the
     * first operand, shown as {@code value1 - value2}.
     *
     * If an overflow occurs, the bits of the result is truncated to preserve
     * only the first 32-bits from the Least Significant Bit (LSB).
     *
     * Further, an overflow never raises an exception.
     *
     * [Format]
     * subtract_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 32-bit decimal value. It is popped off the operand stack.
     * value2
     *     A 32-bit decimal value. It is popped off the operand stack.
     * result
     *     A 32-bit decimal value which is computed by subtraction of the
     *     specified 32-bit decimal values.
     */
    ZEN_BYTE_CODE_SUBTRACT_F,

    /**
     * Subtracts two 64-bit decimal values.
     *
     * Two 64-bit decimal operands are popped off the operand stack. A 64-bit
     * decimal result is evaluated by subtracting the second operand from the
     * first operand, shown as {@code value1 - value2}.
     *
     * If an overflow occurs, the bits of the result is truncated to preserve
     * only the first 64-bits from the Least Significant Bit (LSB).
     *
     * Further, an overflow never raises an exception.
     *
     * [Format]
     * subtract_d
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., result
     *
     * [Operands]
     * value1
     *     A 64-bit decimal value. It is popped off the operand stack.
     * value2
     *     A 64-bit decimal value. It is popped off the operand stack.
     * result
     *     A 64-bit decimal value which is computed by subtraction of the
     *     specified 64-bit decimal values.
     */
    ZEN_BYTE_CODE_SUBTRACT_D,

    /* Swap */

    /**
     * Swaps the first two values on top of the operand stack.
     *
     * [Format]
     * swap
     *
     * [Operand Stack]
     * Before
     *     ..., value1, value2
     * After
     *     ..., value2, value1
     *
     * [Operands]
     * value1
     *     A 32-bit value. It is swapped with `value2`.
     * value2
     *     A 32-bit value. It is swapped with `value1`.
     * result
     *     There is no result. However, the state of the operand stack is
     *     modified.
     */
    ZEN_BYTE_CODE_SWAP,

    /* Switch */

    // TODO: These instructions are reserved as of version 1.0. Their behavior
    // will be updated in the later versions of this specification.

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
     * zen.core.NullPointerException
     *     An instance of the `zen.core.InvalidArraySizeException` class is thrown
     *     if the specified exception is `null`.
     */
    ZEN_BYTE_CODE_THROW,

    /* Wide */

    /**
     * Extend the argument length of an instruction.
     */
    ZEN_BYTE_CODE_WIDE,
};

typedef enum zen_ByteCode_t zen_ByteCode_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_BYTE_CODE_H */

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