#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "../LIB/Datatypes.h"

uint8 add(uint8 a, uint8 b);      // Adds two numbers and returns the result.
uint8 subtract(uint8 a, uint8 b); // Subtracts two numbers and returns the result.
uint8 multiply(uint8 a, uint8 b); // Multiplies two numbers and returns the result.
uint8 divide(uint8 a, uint8 b);   // Divides two numbers and returns the result.
uint8 modulus(uint8 a, uint8 b);  // Calculates the modulus of two numbers and returns the result.
uint8 is_digit(uint8 c);          // Checks if a character is a valid digit (0-9).
uint8 is_operator(uint8 c);       // Checks if a character is a valid operator (+, -, *, /).
uint8 is_equal_key(uint8 key);    // Checks if a key is the enter key (e.g. '=')

#endif