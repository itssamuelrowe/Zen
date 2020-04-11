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

// Sunday, ‎April ‎29, ‎2018

#include <com/onecube/zen/virtual-machine/feb/Instruction.h>

/*******************************************************************************
 * Instruction                                                                 *
 *******************************************************************************/

zen_Instruction_t zen_Instruction_instructions[] = {

    /* No Operation */

    { "nop", ZEN_BYTE_CODE_NOP, 0, 0, 0 },

    /* Add */

    { "add_i", ZEN_BYTE_CODE_ADD_I, 0, 2, 1 },
    { "add_l", ZEN_BYTE_CODE_ADD_L, 0, 2, 1 },
    { "add_f", ZEN_BYTE_CODE_ADD_F, 0, 2, 1 },
    { "add_d", ZEN_BYTE_CODE_ADD_D, 0, 2, 1 },

    /* Bitwise AND */

    { "and_i", ZEN_BYTE_CODE_AND_I, 0, 2, 1 },
    { "and_l", ZEN_BYTE_CODE_AND_L, 0, 2, 1 },

    /* Bitwise OR */

    { "or_i", ZEN_BYTE_CODE_OR_I, 0, 2, 1 },
    { "or_l", ZEN_BYTE_CODE_OR_L, 0, 2, 1 },

    /* Bitwise Shift */

    { "shift_left_i", ZEN_BYTE_CODE_SHIFT_LEFT_I, 0, 2, 1 },
    { "shift_left_l", ZEN_BYTE_CODE_SHIFT_LEFT_L, 0, 2, 1 },
    { "shift_right_i", ZEN_BYTE_CODE_SHIFT_RIGHT_I, 0, 2, 1 },
    { "shift_right_l", ZEN_BYTE_CODE_SHIFT_RIGHT_L, 0, 2, 1 },
    { "shift_right_ui", ZEN_BYTE_CODE_SHIFT_RIGHT_UI, 0, 2, 1 },
    { "shift_right_ul", ZEN_BYTE_CODE_SHIFT_RIGHT_UL, 0, 2, 1 },

    /* Bitwise XOR */

    { "xor_i", ZEN_BYTE_CODE_XOR_I, 0, 2, 1 },
    { "xor_l", ZEN_BYTE_CODE_XOR_L, 0, 2, 1 },

    /* Cast */

    { "cast_itb", ZEN_BYTE_CODE_CAST_ITL, 0, 1, 1 },
    { "cast_its", ZEN_BYTE_CODE_CAST_ITL, 0, 1, 1 },
    { "cast_itl", ZEN_BYTE_CODE_CAST_ITL, 0, 1, 1 },
    { "cast_itf", ZEN_BYTE_CODE_CAST_ITF, 0, 1, 1 },
    { "cast_itd", ZEN_BYTE_CODE_CAST_ITD, 0, 1, 1 },
    { "cast_ltb", ZEN_BYTE_CODE_CAST_LTI, 0, 1, 1 },
    { "cast_lts", ZEN_BYTE_CODE_CAST_LTI, 0, 1, 1 },
    { "cast_lti", ZEN_BYTE_CODE_CAST_LTI, 0, 1, 1 },
    { "cast_ltf", ZEN_BYTE_CODE_CAST_LTF, 0, 1, 1 },
    { "cast_ltd", ZEN_BYTE_CODE_CAST_LTD, 0, 1, 1 },
    { "cast_fti", ZEN_BYTE_CODE_CAST_FTI, 0, 1, 1 },
    { "cast_ftl", ZEN_BYTE_CODE_CAST_FTL, 0, 1, 1 },
    { "cast_ftd", ZEN_BYTE_CODE_CAST_FTD, 0, 1, 1 },
    { "cast_dti", ZEN_BYTE_CODE_CAST_DTI, 0, 1, 1 },
    { "cast_dtl", ZEN_BYTE_CODE_CAST_DTL, 0, 1, 1 },
    { "cast_dtf", ZEN_BYTE_CODE_CAST_DTF, 0, 1, 1 },
    { "cast_itc", ZEN_BYTE_CODE_CAST_ITC, 0, 1, 1 },

    /* Check Cast */

    { "check_cast", ZEN_BYTE_CODE_CHECK_CAST, 2, 1, 1 },

    /* Compare */

    { "compare_l", ZEN_BYTE_CODE_COMPARE_L, 0, 2, 1 },
    { "compare_lt_f", ZEN_BYTE_CODE_COMPARE_LT_F, 0, 2, 1 },
    { "compare_gt_f", ZEN_BYTE_CODE_COMPARE_GT_F, 0, 2, 1 },
    { "compare_lt_d", ZEN_BYTE_CODE_COMPARE_LT_D, 0, 2, 1 },
    { "compare_gt_d", ZEN_BYTE_CODE_COMPARE_GT_D, 0, 2, 1 },

    /* Divide */

    { "divide_i", ZEN_BYTE_CODE_DIVIDE_I, 0, 2, 1 },
    { "divide_l", ZEN_BYTE_CODE_DIVIDE_L, 0, 2, 1 },
    { "divide_f", ZEN_BYTE_CODE_DIVIDE_F, 0, 2, 1 },
    { "divide_d", ZEN_BYTE_CODE_DIVIDE_D, 0, 2, 1 },

    /* Duplicate */

    { "duplicate", ZEN_BYTE_CODE_DUPLICATE, 0, 1, 2 },
    { "duplicate_x1", ZEN_BYTE_CODE_DUPLICATE_X1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "duplicate_x2", ZEN_BYTE_CODE_DUPLICATE_X2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "duplicate2", ZEN_BYTE_CODE_DUPLICATE2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "duplicate2_x1", ZEN_BYTE_CODE_DUPLICATE2_X1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "duplicate2_x2", ZEN_BYTE_CODE_DUPLICATE2_X2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Jump */

    { "jump_eq0_i", ZEN_BYTE_CODE_JUMP_EQ0_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_ne0_i", ZEN_BYTE_CODE_JUMP_NE0_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_lt0_i", ZEN_BYTE_CODE_JUMP_LT0_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_gt0_i", ZEN_BYTE_CODE_JUMP_GT0_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_ge0_i", ZEN_BYTE_CODE_JUMP_GE0_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_le0_i", ZEN_BYTE_CODE_JUMP_LE0_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_eq_i", ZEN_BYTE_CODE_JUMP_EQ_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_ne_i", ZEN_BYTE_CODE_JUMP_NE_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_lt_i", ZEN_BYTE_CODE_JUMP_LT_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_gt_i", ZEN_BYTE_CODE_JUMP_GT_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_ge_i", ZEN_BYTE_CODE_JUMP_GE_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_le_i", ZEN_BYTE_CODE_JUMP_LE_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_eq_a", ZEN_BYTE_CODE_JUMP_EQ_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_ne_a", ZEN_BYTE_CODE_JUMP_NE_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_eqn_a", ZEN_BYTE_CODE_JUMP_EQN_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "jump_nen_a", ZEN_BYTE_CODE_JUMP_NEN_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Increment */

    { "increment_i", ZEN_BYTE_CODE_INCREMENT_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Invoke */

    { "invoke_special", ZEN_BYTE_CODE_INVOKE_SPECIAL, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "invoke_virtual", ZEN_BYTE_CODE_INVOKE_VIRTUAL, 1, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "invoke_dynamic", ZEN_BYTE_CODE_INVOKE_DYNAMIC, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "invoke_static", ZEN_BYTE_CODE_INVOKE_STATIC, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Jump */

    { "jump", ZEN_BYTE_CODE_JUMP, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Load */

    { "load_i", ZEN_BYTE_CODE_LOAD_I, 1, 0, 1 },
    { "load_l", ZEN_BYTE_CODE_LOAD_L, 1, 0, 1 },
    { "load_f", ZEN_BYTE_CODE_LOAD_F, 1, 0, 1 },
    { "load_d", ZEN_BYTE_CODE_LOAD_D, 1, 0, 1 },
    { "load_a", ZEN_BYTE_CODE_LOAD_A, 1, 0, 1 },
    { "load_i0", ZEN_BYTE_CODE_LOAD_I0, 1, 0, 1 },
    { "load_i1", ZEN_BYTE_CODE_LOAD_I1, 1, 0, 1 },
    { "load_i2", ZEN_BYTE_CODE_LOAD_I2, 1, 0, 1 },
    { "load_i3", ZEN_BYTE_CODE_LOAD_I3, 1, 0, 1 },
    { "load_l0", ZEN_BYTE_CODE_LOAD_L0, 1, 0, 1 },
    { "load_l1", ZEN_BYTE_CODE_LOAD_L1, 1, 0, 1 },
    { "load_l2", ZEN_BYTE_CODE_LOAD_L2, 1, 0, 1 },
    { "load_l3", ZEN_BYTE_CODE_LOAD_L3, 1, 0, 1 },
    { "load_f0", ZEN_BYTE_CODE_LOAD_F0, 1, 0, 1 },
    { "load_f1", ZEN_BYTE_CODE_LOAD_F1, 1, 0, 1 },
    { "load_f2", ZEN_BYTE_CODE_LOAD_F2, 1, 0, 1 },
    { "load_f3", ZEN_BYTE_CODE_LOAD_F3, 1, 0, 1 },
    { "load_d0", ZEN_BYTE_CODE_LOAD_D0, 1, 0, 1 },
    { "load_d1", ZEN_BYTE_CODE_LOAD_D1, 1, 0, 1 },
    { "load_d2", ZEN_BYTE_CODE_LOAD_D2, 1, 0, 1 },
    { "load_d3", ZEN_BYTE_CODE_LOAD_D3, 1, 0, 1 },
    { "load_a0", ZEN_BYTE_CODE_LOAD_A0, 1, 0, 1 },
    { "load_a1", ZEN_BYTE_CODE_LOAD_A1, 1, 0, 1 },
    { "load_a2", ZEN_BYTE_CODE_LOAD_A2, 1, 0, 1 },
    { "load_a3", ZEN_BYTE_CODE_LOAD_A3, 1, 0, 1 },
    { "load_ab", ZEN_BYTE_CODE_LOAD_AB, 0, 2, 1 },
    { "load_ac", ZEN_BYTE_CODE_LOAD_AC, 0, 2, 1 },
    { "load_as", ZEN_BYTE_CODE_LOAD_AS, 0, 2, 1 },
    { "load_ai", ZEN_BYTE_CODE_LOAD_AI, 0, 2, 1 },
    { "load_al", ZEN_BYTE_CODE_LOAD_AL, 0, 2, 1 },
    { "load_af", ZEN_BYTE_CODE_LOAD_AF, 0, 2, 1 },
    { "load_ad", ZEN_BYTE_CODE_LOAD_AD, 0, 2, 1 },
    { "load_aa", ZEN_BYTE_CODE_LOAD_AA, 0, 2, 1 },
    { "load_instance_field", ZEN_BYTE_CODE_LOAD_INSTANCE_FIELD, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "load_static_field", ZEN_BYTE_CODE_LOAD_STATIC_FIELD, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "load_cpr", ZEN_BYTE_CODE_LOAD_CPR, 1, 0, 1 },
    { "load_array_size", ZEN_BYTE_CODE_LOAD_ARRAY_SIZE, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Modulo */

    { "modulo_i", ZEN_BYTE_CODE_MODULO_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "modulo_l", ZEN_BYTE_CODE_MODULO_L, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "modulo_f", ZEN_BYTE_CODE_MODULO_F, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "modulo_d", ZEN_BYTE_CODE_MODULO_D, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Multiply */

    { "multiply_i", ZEN_BYTE_CODE_MULTIPLY_I, 0, 2, 1 },
    { "multiply_l", ZEN_BYTE_CODE_MULTIPLY_L, 0, 2, 1 },
    { "multiply_f", ZEN_BYTE_CODE_MULTIPLY_F, 0, 2, 1 },
    { "multiply_d", ZEN_BYTE_CODE_MULTIPLY_D, 0, 2, 1 },

    /* Negate */

    { "negate_i", ZEN_BYTE_CODE_NEGATE_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "negate_l", ZEN_BYTE_CODE_NEGATE_L, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "negate_f", ZEN_BYTE_CODE_NEGATE_F, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "negate_d", ZEN_BYTE_CODE_NEGATE_D, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* New */

    { "new", ZEN_BYTE_CODE_NEW, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "new_array", ZEN_BYTE_CODE_NEW_ARRAY, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "new_array_a", ZEN_BYTE_CODE_NEW_ARRAY_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "new_array_an", ZEN_BYTE_CODE_NEW_ARRAY_AN, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Pop */

    { "pop", ZEN_BYTE_CODE_POP, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "pop2", ZEN_BYTE_CODE_POP2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Push */

    { "push_null", ZEN_BYTE_CODE_PUSH_NULL, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_in1", ZEN_BYTE_CODE_PUSH_IN1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_i0", ZEN_BYTE_CODE_PUSH_I0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_i1", ZEN_BYTE_CODE_PUSH_I1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_i2", ZEN_BYTE_CODE_PUSH_I2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_i3", ZEN_BYTE_CODE_PUSH_I3, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_i4", ZEN_BYTE_CODE_PUSH_I4, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_i5", ZEN_BYTE_CODE_PUSH_I5, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_l0", ZEN_BYTE_CODE_PUSH_L0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_l1", ZEN_BYTE_CODE_PUSH_L1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_f0", ZEN_BYTE_CODE_PUSH_F0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_f1", ZEN_BYTE_CODE_PUSH_F1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_f2", ZEN_BYTE_CODE_PUSH_F2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_d0", ZEN_BYTE_CODE_PUSH_D0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_d1", ZEN_BYTE_CODE_PUSH_D1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_b", ZEN_BYTE_CODE_PUSH_B, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "push_s", ZEN_BYTE_CODE_PUSH_S, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Return */

    { "return", ZEN_BYTE_CODE_RETURN, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "return_i", ZEN_BYTE_CODE_RETURN_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "return_l", ZEN_BYTE_CODE_RETURN_L, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "return_f", ZEN_BYTE_CODE_RETURN_F, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "return_d", ZEN_BYTE_CODE_RETURN_D, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "return_a", ZEN_BYTE_CODE_RETURN_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* RTTI */

    { "rtti", ZEN_BYTE_CODE_RTTI, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Store */

    { "store_i", ZEN_BYTE_CODE_STORE_I, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_i0", ZEN_BYTE_CODE_STORE_I0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_i1", ZEN_BYTE_CODE_STORE_I1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_i2", ZEN_BYTE_CODE_STORE_I2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_i3", ZEN_BYTE_CODE_STORE_I3, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_l", ZEN_BYTE_CODE_STORE_L, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_l0", ZEN_BYTE_CODE_STORE_L0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_l1", ZEN_BYTE_CODE_STORE_L1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_l2", ZEN_BYTE_CODE_STORE_L2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_l3", ZEN_BYTE_CODE_STORE_L3, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_f", ZEN_BYTE_CODE_STORE_F, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_f0", ZEN_BYTE_CODE_STORE_F0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_f1", ZEN_BYTE_CODE_STORE_F1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_f2", ZEN_BYTE_CODE_STORE_F2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_f3", ZEN_BYTE_CODE_STORE_F3, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_d", ZEN_BYTE_CODE_STORE_D, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_d0", ZEN_BYTE_CODE_STORE_D0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_d1", ZEN_BYTE_CODE_STORE_D1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_d2", ZEN_BYTE_CODE_STORE_D2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_d3", ZEN_BYTE_CODE_STORE_D3, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_a", ZEN_BYTE_CODE_STORE_A, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_a0", ZEN_BYTE_CODE_STORE_A0, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_a1", ZEN_BYTE_CODE_STORE_A1, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_a2", ZEN_BYTE_CODE_STORE_A2, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_a3", ZEN_BYTE_CODE_STORE_A3, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_ab", ZEN_BYTE_CODE_STORE_AB, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_ac", ZEN_BYTE_CODE_STORE_AC, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_as", ZEN_BYTE_CODE_STORE_AS, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_ai", ZEN_BYTE_CODE_STORE_AI, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_al", ZEN_BYTE_CODE_STORE_AL, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_af", ZEN_BYTE_CODE_STORE_AF, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_ad", ZEN_BYTE_CODE_STORE_AD, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_aa", ZEN_BYTE_CODE_STORE_AA, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_static_field", ZEN_BYTE_CODE_STORE_STATIC_FIELD, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "store_instance_field", ZEN_BYTE_CODE_STORE_INSTANCE_FIELD, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Swap */

    { "swap", ZEN_BYTE_CODE_SWAP, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Switch */

    { "switch_table", ZEN_BYTE_CODE_SWITCH_TABLE, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
    { "switch_search", ZEN_BYTE_CODE_SWITCH_SEARCH, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Throw */

    { "throw", ZEN_BYTE_CODE_THROW, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },

    /* Wide */

    { "wide", ZEN_BYTE_CODE_WIDE, 0x0 /* Unknown */, 0x0 /* Unknown */, 0x0 /* Unknown */ },
};

bool zen_Instruction_verify() {
    int32_t i;
    bool result = true;
    for (i = 0; i < (int32_t)ZEN_BYTE_CODE_WIDE; i++) {
        zen_Instruction_t* instruction = &zen_Instruction_instructions[i];
        if (instruction->m_byteCode != i) {
            fprintf(stderr, "[internal error] The instruction index and byte pair is invalid. (index=%d, ibc=%d, abc=%d, text=%s)\n", i, instruction->m_byteCode, i, instruction->m_text);
            fflush(stdout);
            result = false;
        }
    }

    if (result) {
        fprintf(stderr, "[success] The instruction set was successfully verified.\n");
        fflush(stdout);
    }

    return result;
}

zen_Instruction_t* zen_Instruction_getInstance(zen_ByteCode_t byteCode) {
    // jtk_Assert_assertTrue(..., "The specified byte code is invalid.");
    return &zen_Instruction_instructions[(uint8_t)byteCode];
}

const uint8_t* zen_Instruction_getText(zen_Instruction_t* instruction) {
    return instruction->m_text;
}

uint8_t zen_Instruction_getByteCode(zen_Instruction_t* instruction) {
    return instruction->m_byteCode;
}

uint8_t zen_Instruction_getArgumentCount(zen_Instruction_t* instruction) {
    return instruction->m_argumentCount;
}

uint8_t zen_Instruction_getOperandCount(zen_Instruction_t* instruction) {
    return instruction->m_operandCount;
}

uint8_t zen_Instruction_getResultCount(zen_Instruction_t* instruction) {
    return instruction->m_resultCount;
}