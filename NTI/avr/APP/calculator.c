#include "calculator.h"

unsigned int add(unsigned int a, unsigned int b)
{
    return a + b;
}
int subtract(unsigned int a, unsigned int b)
{

    return a - b;
}
unsigned int multiply(unsigned int a, unsigned int b)
{
    return a * b;
}
unsigned int divide(unsigned int a, unsigned int b)
{
    return a / b;
}
unsigned int modulus(unsigned int a, unsigned int b)
{
    return a % b;
}
uint8 is_digit(uint8 c)
{
    if ((c >= 48) && (c <= 57))
        return 1;
    else
        return 0;
}
uint8 is_operator(uint8 c)
{
    if ((c == '+') ||
        (c == '-') ||
        (c == '*') ||
        (c == '/') ||
        (c == '%'))
        return 1;
    else
        return 0;
}
uint8 is_equal_key(uint8 key)
{
    if (key == '=')
        return 1;
    else
        return 0;
}