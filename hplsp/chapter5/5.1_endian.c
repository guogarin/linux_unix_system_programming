#include <stdio.h>

void byteorder()
{
    union {
        short value;
        char union_bytes[sizeof(short)];
    }test;
    
    test.value = 0x0102;
    
    printf("value: %d\n", test.value);
    printf("union_bytes[0]: %d\n", test.union_bytes[0]);
    printf("union_bytes[1]: %d\n", test.union_bytes[1]);
}

int main()
{
    byteorder();
    return 0;
}
