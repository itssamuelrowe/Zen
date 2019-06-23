// Monday, June 11, 2018

#include <zen/vm/BinaryEntityVerifier.h>

/*******************************************************************************
 * BinaryEntityVerifier                                                        *
 *******************************************************************************/

void zen_BinaryEntityVerifier_verifyMagicNumber(zen_BinaryEntityVerifier_t* verifier,
    uint32_t magicNumber) {
    jtk_Assert_assertObject(verifier, "The specified binary entity verifier is null.");

    if (magicNumber != ZEN_BINARY_ENTITY_FORMAT_MAGIC_NUMBER) {
        zen_Logger_e("Binary entity file magic number verified: incompatible");
        // throw UnknownEntityMagicNumberException
    }
}

void zen_BinaryEntityVerifier_verifyVersion(zen_BinaryEntityVerifier_t* verifier,
    zen_FebVersion_t* fileVersion) {
    jtk_Assert_assertObject(verifier, "The specified binary entity verifier is null.");

    zen_FebVersion_t* machineVersion = zen_VirtualMachine_getFebVersion(verifier->m_machine);
    if (zen_FebVersionHelper_compare(fileVersion, machineVersion) >= 0) {
        zen_Logger_i("Binary entity file version verified: compatibile");
    }
    else {
        zen_Logger_e("Binary entity file version verified: incompatible");
        // throw UnsupportedEntityVersionException
    }
}