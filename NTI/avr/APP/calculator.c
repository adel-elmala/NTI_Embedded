#include "calculator.h"

uint8 add(uint8 a, uint8 b)
{
    return a + b;
}
uint8 subtract(uint8 a, uint8 b)
{
    return a - b;
}
uint8 multiply(uint8 a, uint8 b)
{
    return a * b;
}
uint8 divide(uint8 a, uint8 b)
{
    return a / b;
}
uint8 modulus(uint8 a, uint8 b)
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