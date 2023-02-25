#include "test.h"
#include "../MCAL/TIMER/timer.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"
void test_timer0()
{
    TIMER0_SetConfig();
    DIO_vSetPortDirection(PORTB, OUTPUT);

    while (1)
    {
        DIO_vWritePin(PORTB, PIN7, HIGH);
        TIMER0_Delay_Milli_Seconds_with_Blocking(2000);
        // TIMER0_Delay_MicroSeconds_with_Blocking(1);

        DIO_vWritePin(PORTB, PIN7, LOW);
        TIMER0_Delay_Milli_Seconds_with_Blocking(2000);
        // TIMER0_Delay_MicroSeconds_with_Blocking(1);
    }
}