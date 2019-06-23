// Monday, June 11, 2018

#ifndef COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_VERIFIER_H
#define COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_VERIFIER_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * BinaryEntityVerifier                                                        *
 *******************************************************************************/

struct zen_BinaryEntityVerifier_t {
};

typedef struct zen_BinaryEntityVerifier_t zen_BinaryEntityVerifier_t;

/* Constructor */

zen_BinaryEntityVerifier_t* zen_BinaryEntityVerifier_new();

/* Destructor */

void zen_BinaryEntityVerifier_delete(zen_BinaryEntityVerifier_t* verifier);
 
#endif /* COM_ONECUBE_ZEN_VIRTUAL_MACHINE_LOADER_BINARY_ENTITY_VERIFIER_H */