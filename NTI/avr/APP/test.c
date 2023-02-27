#include "test.h"
#include "../MCAL/TIMER/timer.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"
#include "../MCAL/PWM/PWM_Interface.h"
void test_timer0()
{

    TIMER0_SetConfig();
    DIO_vSetPortDirection(PORTB, OUTPUT);

    while (1)
    {

        DIO_vWritePin(PORTB, PIN7, HIGH);
        // TIMER0_Delay_ms_with_Blocking(1);
        TIMER0_Delay_us_with_Blocking(100);

        // TIMER0_Delay_MicroSeconds_with_Blocking(1);

        DIO_vWritePin(PORTB, PIN7, LOW);
        // TIMER0_Delay_ms_with_Blocking(1);
        TIMER0_Delay_us_with_Blocking(100);

        // TIMER0_Delay_MicroSeconds_with_Blocking(1);
    }
}

void test_pwm()
{
    PWM_Init();
    PWM_Generate_CHANNELA(128, 20000);
    PWM_Generate_CHANNELB(0, 20000);
}