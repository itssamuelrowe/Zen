// Tuesday, October 02, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_SOURCE_FILE_ATTRIBUTE_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_SOURCE_FILE_ATTRIBUTE_H

/*******************************************************************************
 * SourceFileAttribute                                                         *
 *******************************************************************************/

/**
 * The source file attribute is a secondary attribute, with a fixed length.
 * It represents the file from which a primary entity was compiled. Therefore,
 * this attribute can only be associated with a primary entities such as classes,
 * enumerations and annotations.
 *
 * @class SourceFileAttribute
 * @ingroup zen_vm_feb
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SourceFileAttribute_t {
    
    /**
     * A valid index into the constant pool. The entry at the specified index
     * should be a `ConstantPoolUtf8` instance, which contains the path of
     * the source file. Further, the value does not hold any special meaning
     * to the virtual machine.
     */
    uint16_t m_pathIndex;
};

/**
 * @memberof SourceFileAttribute
 */
typedef struct zen_SourceFileAttribute_t zen_SourceFileAttribute_t;

#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_FEB_ATTRIBUTE_SOURCE_FILE_ATTRIBUTE_H */