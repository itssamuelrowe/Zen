#include <stdint.h>
#include <stdio.h>

struct String {
    uint32_t size;
    uint8_t value[];
};

typedef struct String String;

union Object {
    struct {
        int16_t flags;
        String string;
        int16_t size;
        uint8_t data[];
    } asStructure;
    uint8_t asBytes[21];
};

typedef union Object Object;

int32_t main() {
    Object o;
    o.asStructure.flags = 0;
    
    o.asStructure.string.size = 5;
    o.asStructure.string.value[0] = 'H';
    o.asStructure.string.value[1] = 'e';
    o.asStructure.string.value[2] = 'l';
    o.asStructure.string.value[3] = 'l';
    o.asStructure.string.value[4] = 'o';
    
    o.asStructure.size = 8;
    int i;
    for (i = 0; i < 8; i++) {
        o.asStructure.data[i] = 0xFF;
    }
    
    printf("%d\n", o.asStructure.size);
    
    return 0;
}
