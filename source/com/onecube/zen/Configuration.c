#include <com/onecube/zen/Configuration.h>

static int32_t s_footprint = 0;

void* zen_Memory_allocate0(int32_t size) {
    s_footprint += size;

    void* object = malloc(size);
    return object;
}

int32_t zen_Memory_getFootprint() {
    return s_footprint;
}