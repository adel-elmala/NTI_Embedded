#include "test.h"
#include "../MCAL/TIMER/timer.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"
#include "../MCAL/PWM/PWM_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"

#include "../HAL/L298_H_Bridge/L298_H_Bridge_Interface.h"
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

ADC_Config_t conf = {
    // ADC_REFVOLTAGE_INTERNAL,
    ADC_REFVOLTAGE_AVCC,
    // ADC_REFVOLTAGE_VREF,
    ADC_RIGHT_ADJUST,
    ADC_SINGLE_ENDDED_ADC0,
    ADC_DISABLE_INTR,
    ADC_DISABLE_AUTOTRIGGER,
    ADC_AUTO_TRIGGER_SOURCE_IGNORE};

void test_adc()
{
    ADC_init(conf);
    DIO_vSetPortDirection(PORTC, OUTPUT);
    DIO_vSetPortDirection(PORTD, OUTPUT);

    while (1)
    {
        uint8 low, high;
        uint16 result = ADC_PollRead(&low, &high);

        DIO_vWritePort(PORTC, (result & 0x00ff));
        DIO_vWritePort(PORTD, ((result & 0xff00) >> 8));
        // DIO_vWritePort(PORTC, low);
        // DIO_vWritePort(PORTD, high);
    }
}

L298_H_Bridge_Config_t dc_motor_configuration = {
    ENABLE_MOTOR_A,
    DISABLE_MOTOR_B,
    MOTOR_A_DIRECTION_INPUT_PORT,
    MOTOR_A_DIRECTION_INPUT_1_PIN,
    MOTOR_A_DIRECTION_INPUT_2_PIN,
    MOTOR_B_DIRECTION_INPUT_3_PIN,
    MOTOR_B_DIRECTION_INPUT_4_PIN
    //     // MOTOR_A_PWM_PORT,
    //     // MOTOR_A_PWM_PIN,
    //     // MOTOR_B_PWM_PORT,
    //     // MOTOR_B_PWM_PIN
};

void test_motor()
{
    L298_H_Bridge_init(dc_motor_configuration);
    L298_H_Bridge_Motor_A_Direction_Control(MOTOR_DIRECTION_FORWARD);
    while (1)
    {
        L298_H_Bridge_Motor_A_Speed_Control(128, 20000);
    }
}