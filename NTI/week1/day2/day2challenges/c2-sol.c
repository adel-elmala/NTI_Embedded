#include "stdio.h"

int main(int argc, char const *argv[])
{
    /* code */

    int x = 0xaabbccdd;
    unsigned char *c_ptr = (unsigned char *)&x;
    if (*c_ptr == 0xdd)
        printf("Little endian\n");
    else
        printf("Big endian\n");
    return 0;
}
