#include "application.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"

#include "../MCAL/EXT_INT/External_INT_Interface.h"
#include "../MCAL/EXT_INT/External_INT_Reg.h"

#include "../HAL/KeyPad/Keypad_Interface.h"
#include "../HAL/LCD/LCD_Interface.h"

// #define F_CPU 16000000UL
// #include <util/delay.h>

EXT_Int_Conf configuration =
    {
        DISABLE_INT,
        DISABLE_INT,
        ENABLE_INT2,
        RISING_EDGE_MODE,
        RISING_EDGE_MODE,
        RISING_EDGE_MODE};

void app_start()
{
    DIO_vSetPortDirection(PORTA, OUTPUT);
    EXT_INT_SET_CONFIG(configuration);
    init_ext_int(0, 0, 1);
    register_ext_int_callbacks(led_blink, led_blink, led_blink);
    while (1)
    {
        volatile unsigned long cnt = 0;
        DIO_vTogglePin(PORTA, PIN1);
        while (cnt < 25000)
        {
            cnt++;
        }
    }
}
void led_blink(void)
{
    DIO_vTogglePin(PORTA, PIN5);
    // DIO_vTogglePort(PORTA);
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

void testKeypad()
{
    DIO_vSetPinDirection(PORTA, PIN0, OUTPUT);
    DIO_vSetPinDirection(PORTA, PIN1, OUTPUT);
    DIO_vSetPinDirection(PORTA, PIN2, OUTPUT);
    keypad_init();
    volatile uint8 result = keypad_GetPress();

    if (result == 5)
        // DIO_vWritePin(PORTA, PIN0, HIGH);
        DIO_vTogglePin(PORTA, PIN0);

    else if (result == 9)
        // DIO_vWritePin(PORTA, PIN1, HIGH);
        DIO_vTogglePin(PORTA, PIN1);

    else if (result == 13)
        // DIO_vWritePin(PORTA, PIN2, HIGH);
        DIO_vTogglePin(PORTA, PIN2);
}

void testLCD()
{

    lcd_init();
    lcd_displayString("Hello AVR!");
    // lcd_sendData('%');
}