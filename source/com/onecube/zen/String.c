// Saturday, November 04, 2017

#include <com/onecube/zen/CString.h>

uint8_t* zen_String_new(const uint8_t* string) {
    return zen_String_newWithSize(string, zen_String_getLength(string));
}

uint8_t* zen_String_newWithSize(const uint8_t* string, int32_t size) {
    uint8_t* result = zen_Memory_allocate(uint8_t, size + 1);
    int32_t i;
    for (i = 0; i < size; i++) {
        result[i] = string[i];
    }
    result[size] = '\0';
    return result;
}

void zen_String_delete(uint8_t* string) {
    zen_Memory_deallocate(string);
}

bool zen_String_equals(const uint8_t* string1, int32_t size1, const uint8_t* string2, int32_t size2) {
    if (string1 == string2) {
        return 1;
    }
    if ((string1 == NULL) ^ (string2 == NULL)) {
        return false;
    }
    if (size1 == size2) {
        int32_t i;
        for (i = 0; i < size1; i++) {
            if (string1[i] != string2[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

int32_t zen_String_getLength(const uint8_t* string) {
    int32_t i = 0;
    while (string[i++] != '\0');
    return i - 1;
}