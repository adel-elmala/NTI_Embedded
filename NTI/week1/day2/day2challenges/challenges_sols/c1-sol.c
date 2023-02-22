#include "stdio.h"
#include "stdlib.h"
typedef unsigned char uint8;
#define MAX_VALUE 255
#define MIN_VALUE 0

int Addition(uint8 n1, uint8 n2);
int Subtraction(uint8 n1, uint8 n2);

int main(int argc, char const *argv[])
{
    /* code */
    unsigned int x, y;
    char op;
    scanf("%u", &x);
    getchar();

    scanf("%u", &y);
    getchar();

    scanf("%c", &op);
    getchar();

    uint8 result;
    if ((x > MAX_VALUE) || (y > MAX_VALUE))
    {
        printf("OverFlow\n");
        exit(1);
    }

    if (op == '+')
        result = Addition((uint8)x, (uint8)y);
    else if (op == '-')
    {
        result = Subtraction((uint8)x, (uint8)y);
    }
    else
        printf("Wrong Operation\n");

    printf("%u\n", result);
    return 0;
}
int Addition(uint8 n1, uint8 n2)
{
    if (((uint8)(n1 + n2) < n1) || ((uint8)(n1 + n2) < n2))
    {
        printf("OverFlow\n");
        exit(1);
    }

    return n1 + n2;
}
int Subtraction(uint8 n1, uint8 n2)
{

    if (((uint8)(n1 - n2) > n1) || ((uint8)(n1 - n2) > n2))
    {
        printf("UnderFlow\n");
        exit(2);
    }

    return n1 - n2;
}