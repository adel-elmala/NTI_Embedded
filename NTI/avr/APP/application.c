#include "application.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"

#include "../MCAL/EXT_INT/External_INT_Interface.h"
#include "../MCAL/EXT_INT/External_INT_Reg.h"

#include "../HAL/KeyPad/Keypad_Interface.h"
#include "../HAL/LCD/LCD_Interface.h"

#include "calculator.h"

#define F_CPU 16000000UL
#include <util/delay.h>

EXT_Int_Conf configuration = {
    ENABLE_INT0,
    DISABLE_INT,
    DISABLE_INT,
    RISING_EDGE_MODE,
    FALLING_EDGE_MODE,
    FALLING_EDGE_MODE

};

uint8 keypadInput = 0;
uint8 keypadInputComplete = 0;

unsigned int lhs = 0;
unsigned int rhs = 0;
unsigned int result = 0;
uint8 operator= 0;

void calc_app(void)
{
    init_project();

    while (1)
    {

        readLHS();

        // read operator
        operator= keypadInput;
        //      update lcd
        lcd_sendData(keypadInput);

        readRHS();

        // wait for '='
        while (keypadInputComplete == 0)
            ;

        calculator();
        //      update lcd
        lcd_clearAndHome();
        printResult();
        keypadInputComplete = 0;
        lhs = 0;
        rhs = 0;
        result = 0;
    }
}
void readLHS(void)
{
    // keypadInput = keypad_GetPress();
    getPressNoINTR();
    lcd_clearAndHome();
    while (is_digit(keypadInput))
    {
        // read lhs
        lhs = lhs * 10 + ascii_to_decimal(keypadInput);
        //      update lcd
        lcd_sendData(keypadInput);
        // keypadInput = keypad_GetPress();
        getPressNoINTR();
    }
}
void getPressNoINTR()
{
    EXT_INT_Disable();
    keypadInput = keypad_GetPress();
    EXT_INT_Enable();
}
void readRHS(void)
{
    while (is_digit((getPressNoINTR(), keypadInput)) && (keypadInputComplete == 0))
    // keypadInput = keypad_GetPress();
    {
        // read lhs
        rhs = rhs * 10 + ascii_to_decimal(keypadInput);
        //      update lcd
        lcd_sendData(keypadInput);
    }
}
unsigned int ascii_to_decimal(uint8 ascii)
{
    return (ascii - (unsigned int)'0');
}

void startCalculation(void)
{
    // keypadInput = keypad_GetPress();
    // lcd_sendData(keypadInput);
    // _delay_ms(1000);

    if (is_equal_key(keypadInput))
    {
        // lcd_displayString("intr");
        keypadInputComplete = 1;
    }
    else
        keypadInputComplete = 0;
}
void init_project(void)
{

    // init keypad
    keypad_init();
    // init lcd
    lcd_init();
    // init ext interrupt
    EXT_INT_SET_CONFIG(configuration);
    init_ext_int(1, 0, 0);
    register_ext_int_callbacks(startCalculation, NULL, NULL);

    // init leds
    DIO_vSetPinDirection(PORTA, PIN5, OUTPUT);
    DIO_vSetPinDirection(PORTA, PIN6, OUTPUT);
    DIO_vSetPinDirection(PORTA, PIN7, OUTPUT);
}
void calculator(void)
{

    switch (operator)
    {
    case '+':
    {
        result = add(lhs, rhs);
        break;
    }

    case '-':
    {
        result = subtract(lhs, rhs);
        break;
    }

    case '*':
    {
        result = multiply(lhs, rhs);
        break;
    }

    case '/':
    {
        // check rhs = 0
        result = divide(lhs, rhs);
        break;
    }

    case '%':
    {
        result = modulus(lhs, rhs);
        break;
    }

    default:
    {
        // lcd_displayString("OOOPPPS");
        // lcd_sendData(operator);
        // _delay_ms(2000);
        break;
    }
    }
}

void printResult()
{
    uint8 digits[16] = {0};
    uint8 digit;
    int i = 0;
    if (result == 0)
    {
        lcd_sendData('0');
        return;
    }
    while (result)
    {
        digit = result % 10;
        result /= 10;
        digits[i] = (uint8)(digit + 48);
        i++;
        // lcd_sendData('9');
    }
    while (i--)

        lcd_sendData(digits[i]);
}

void init_ext_int(uint8 enableINT0, uint8 enableINT1, uint8 enableINT2)
{
    if (enableINT0 != 0)
        DIO_vSetPinDirection(PORTD, PIN2, INPUT);
    if (enableINT1 != 0)
        DIO_vSetPinDirection(PORTD, PIN3, INPUT);
    if (enableINT2 != 0)
        DIO_vSetPinDirection(PORTB, PIN2, INPUT);
}