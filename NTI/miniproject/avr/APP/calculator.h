#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "../LIB/Datatypes.h"

unsigned int add(unsigned int a, unsigned int b);      // Adds two numbers and returns the result.
unsigned int subtract(unsigned int a, unsigned int b); // Subtracts two numbers and returns the result.
unsigned int multiply(unsigned int a, unsigned int b); // Multiplies two numbers and returns the result.
unsigned int divide(unsigned int a, unsigned int b);   // Divides two numbers and returns the result.
unsigned int modulus(unsigned int a, unsigned int b);  // Calculates the modulus of two numbers and returns the result.
uint8 is_digit(uint8 c);                               // Checks if a character is a valid digit (0-9).
uint8 is_operator(uint8 c);                            // Checks if a character is a valid operator (+, -, *, /).
uint8 is_equal_key(uint8 key);                         // Checks if a key is the enter key (e.g. '=')

#endif