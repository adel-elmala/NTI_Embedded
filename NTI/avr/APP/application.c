#include "application.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"

#include "../MCAL/EXT_INT/External_INT_Interface.h"
#include "../MCAL/EXT_INT/External_INT_Reg.h"

#include "../HAL/KeyPad/Keypad_Interface.h"
#include "../HAL/LCD/LCD_Interface.h"

#include "calculator.h"

// #define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>

EXT_Int_Conf configuration = {
    DISABLE_INT,
    ENABLE_INT1,
    DISABLE_INT,
    RISING_EDGE_MODE,
    RISING_EDGE_MODE,
    RISING_EDGE_MODE

};

uint8 keypadInputComplete = 0;
uint8 error = 0;
uint8 restart = 0;

uint8 keypadInput = 0;
unsigned int lhs = 0;
unsigned int rhs = 0;
int result = 0;
uint8 operator= 0;

void calc_app(void)
{
    init_project();

    while (1)
    {
        readLHS();
        if (error || restart)
            continue;
        // read operator

        operator= keypadInput;
        // update lcd
        lcd_sendData(keypadInput);

        readRHS();
        if (error || restart)
            continue;

        calculator();
        if (error)
            continue;
        //  update lcd
        lcd_clearAndHome();
        printResult();
        validState();

        keypadInputComplete = 0;
        lhs = 0;
        rhs = 0;
        result = 0;
    }
}
void readLHS(void)
{
    getPressNoINTR();
    if (!is_digit(keypadInput))
    {
        ErrorState();
        return;
    }
    else if ((keypadInput == 'c'))
    {
        clearState();
        return;
    }

    lcd_clearAndHome();
    waitingState();
    while (is_digit(keypadInput))
    {
        // read lhs
        lhs = lhs * 10 + ascii_to_decimal(keypadInput);
        //  update lcd
        lcd_sendData(keypadInput);
        getPressNoINTR();
        if ((keypadInput == 'c'))
        {
            clearState();
            break;
        }
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
    getPressNoINTR();
    if (!is_digit(keypadInput) || (keypadInput == 'c'))
    {
        ErrorState();
        return;
    }

    waitingState();
    while (is_digit(keypadInput) && (keypadInputComplete == 0))
    {
        // read lhs
        rhs = rhs * 10 + ascii_to_decimal(keypadInput);
        // update lcd
        lcd_sendData(keypadInput);
        getPressNoINTR();
        if ((keypadInput == 'c'))
        {
            // restart = 1;
            clearState();
            break;
        }
    }
}
unsigned int ascii_to_decimal(uint8 ascii)
{
    return (ascii - (unsigned int)'0');
}

void startCalculation(void)
{
    if (is_equal_key(keypadInput))
    {
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
    init_ext_int(0, 1, 0);
    register_ext_int_callbacks(startCalculation, startCalculation, startCalculation);

    // init leds
    DIO_vSetPinDirection(LED_GREEN_PORT, LED_GREEN_PIN, OUTPUT);
    DIO_vSetPinDirection(LED_YELLOW_PORT, LED_YELLOW_PIN, OUTPUT);
    DIO_vSetPinDirection(LED_RED_PORT, LED_RED_PIN, OUTPUT);
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
        if (rhs == 0)
        {
            ErrorState();
            return;
        }
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
        break;
    }
    }
}

void printResult()
{
    // uint8 digits[16] = {0};
    char str[16] = {0};

    // uint8 digit;
    // int i = 0;
    // int tmp = result;

    if (result == 0)
    {
        lcd_sendData('0');
        return;
    }
    // while (result)
    // {
    //     digit = result % 10;
    //     result /= 10;
    //     digits[i] = (uint8)(digit + 48);
    //     i++;
    //     // lcd_sendData('9');
    // }

    // // if (tmp < 0)
    // //     digits[i] = (uint8)'-';
    // while (i--)
    //     lcd_sendData(digits[i]);

    sprintf(str, "%d", result);
    lcd_displayString(str);
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

void validState(void)
{
    DIO_vWritePin(LED_GREEN_PORT, LED_GREEN_PIN, HIGH);
    DIO_vWritePin(LED_RED_PORT, LED_RED_PIN, LOW);
    DIO_vWritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, LOW);
    error = 0;
    restart = 0;
}
void waitingState(void)
{
    DIO_vWritePin(LED_GREEN_PORT, LED_GREEN_PIN, LOW);
    DIO_vWritePin(LED_RED_PORT, LED_RED_PIN, LOW);
    DIO_vWritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, HIGH);
    error = 0;
    restart = 0;
}
void ErrorState(void)
{
    DIO_vWritePin(LED_GREEN_PORT, LED_GREEN_PIN, LOW);
    DIO_vWritePin(LED_RED_PORT, LED_RED_PIN, HIGH);
    DIO_vWritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, LOW);
    lcd_clearAndHome();
    lcd_displayString("Error!");
    error = 1;

    reset();
}

void clearState(void)
{
    DIO_vWritePin(LED_GREEN_PORT, LED_GREEN_PIN, HIGH);
    DIO_vWritePin(LED_RED_PORT, LED_RED_PIN, LOW);
    DIO_vWritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, LOW);
    lcd_clearAndHome();
    // lcd_displayString("Error!");
    restart = 1;

    reset();
}
void reset(void)
{
    lhs = 0;
    rhs = 0;
    result = 0;
    keypadInputComplete = 0;
    // lcd_clearAndHome();
}