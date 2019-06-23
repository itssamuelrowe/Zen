// Monday, November 27, 2017

#ifndef ZEN_PAIR_H
#define ZEN_PAIR_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * Pair                                                                        *
 *******************************************************************************/

struct zen_Pair_t {
    void* m_element1;
    void* m_element2;
};

typedef struct zen_Pair_t zen_Pair_t;

zen_Pair_t* zen_Pair_new();
void zen_Pair_delete(zen_Pair_t* pair);

#endif /* ZEN_PAIR_H */