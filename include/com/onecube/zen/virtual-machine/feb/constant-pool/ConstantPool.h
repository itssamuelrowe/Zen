// Saturday, April 28, 2018

#ifndef ZEN_FEB_CONSTANT_POOL_CONSTANT_POOL_H
#define ZEN_FEB_CONSTANT_POOL_CONSTANT_POOL_H

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolEntry.h>
#include <com/onecube/zen/virtual-machine/feb/constant-pool/ConstantPoolUtf8.h>

/*******************************************************************************
 * ConstantPool                                                                *
 *******************************************************************************/

/**
 * The constant pool is a table where references to various constants,
 * identifiers, and other such entries are stored.
 *
 * Each entry begins with a tag byte. Contingent on the tag, the length of
 * the entry may vary. Please refer the documentation of the entries for
 * further information.
 *
 * @author Samuel Rowe
 * @since  FEB 1.0
 */
struct zen_ConstantPool_t {

    /*
     * The constant pool entries are represented via structures. From the
     * implementers perspective, this design is convenient. On the downside,
     * the construction of a constant pool at runtime equals the product of
     * the number of entries and the allocation time for each entry.
     *
     * An economic design would represent the entries as an array of bytes.
     * In the former design, the allocation time can be reduced greatly if all
     * the constant pool entries are allocated at once as a byte array.
     * Later, appropriate chunks of memory can be divided amongst the entries
     * via casting as pointers to structures.
     *
     * The first four bytes in a constant pool determine the total number of
     * bytes occupied by the entries.
     *
     * TODO: As of now, the implementation follows only the former design.
     */
    // uint32_t m_length;

    /**
     * The number of entries in the constant pool table.
     */
    uint16_t m_size;

    /**
     * The tags of the constant pool entries.
     */
    uint8_t* m_tags;

    /**
     * The various entries stored in the constant pool table.
     */
    zen_ConstantPoolEntry_t** m_entries;
};

typedef struct zen_ConstantPool_t zen_ConstantPool_t;

zen_ConstantPoolUtf8_t* zen_ConstantPool_resolveUtf8(zen_ConstantPool_t* constantPool, uint16_t nameIndex);

#endif /* ZEN_FEB_CONSTANT_POOL_CONSTANT_POOL_H */