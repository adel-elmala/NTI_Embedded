#include "smart_fan.h"
#include "../HAL/LCD/LCD_Interface.h"
#include "../HAL/LM35/LM35_Interface.h"
#include "../HAL/L298_H_Bridge/L298_H_Bridge_Interface.h"
#include "../MCAL/TIMER/timer.h"
#include <stdio.h>
// #include <util/delay.h>

static L298_H_Bridge_Config_t dc_motor_configuration = {
    ENABLE_MOTOR_A,
    DISABLE_MOTOR_B,
    MOTOR_A_DIRECTION_INPUT_PORT,
    MOTOR_A_DIRECTION_INPUT_1_PIN,
    MOTOR_A_DIRECTION_INPUT_2_PIN,
    MOTOR_B_DIRECTION_INPUT_3_PIN,
    MOTOR_B_DIRECTION_INPUT_4_PIN};
void sf_init_project()
{
    TIMER0_SetConfig();
    lcd_init();
    LM35_vInit();
    L298_H_Bridge_init(dc_motor_configuration);
    L298_H_Bridge_Motor_A_Direction_Control(MOTOR_DIRECTION_FORWARD);
}

void sf_app()
{
    sf_init_project();
    // read tmp
    unsigned int last_temp = 0xffff;

    while (1)
    {

        // read temp sensor
        unsigned int temp = LM35_u16GetReading();
        // unsigned int temp = LM35_u16GetReading_NotBLocking();

        if (temp != last_temp)
        {
            unsigned int fSpeed = regulate_motor_speed(temp);
            update_lcd(temp, fSpeed);
        }
        last_temp = temp;
    }
}

unsigned int regulate_motor_speed(unsigned int current_temp)
{
    unsigned int fan_speed;
    if (current_temp < 25)
    {
        L298_H_Bridge_Motor_A_Speed_Control(0, 500);
        fan_speed = 0;
    }
    else if ((25 <= current_temp) && (current_temp < 30))
    {
        // L298_H_Bridge_Motor_A_Speed_Control(0.3 * 255, 5000);
        L298_H_Bridge_Motor_A_Speed_Control(76, 500);
        fan_speed = 30;
    }

    else if ((30 <= current_temp) && (current_temp < 35))
    {

        // L298_H_Bridge_Motor_A_Speed_Control(0.5 * 255, 10000);
        L298_H_Bridge_Motor_A_Speed_Control(128, 500);
        fan_speed = 50;
    }
    else
    {
        // L298_H_Bridge_Motor_A_Speed_Control(0.8 * 255, 20000);
        L298_H_Bridge_Motor_A_Speed_Control(205, 500);
        fan_speed = 80;
    }
    return fan_speed;
}

void update_lcd(unsigned int temp, unsigned int duty_cycle)
{
    // temprature string
    char line1[16] = "Temprature: ";
    char line2[16] = "Fan Speed: ";
    char temp_str[16] = {0};
    char speed_str[16] = {0};
    sprintf(temp_str, "%u", temp);
    sprintf(speed_str, "%u", duty_cycle);

    lcd_clearAndHome();

    lcd_displayString(line1);
    lcd_displayString(temp_str);
    lcd_displayString(" Celesius ");

    lcd_goto_line2();
    lcd_displayString(line2);
    lcd_displayString(speed_str);
    lcd_displayString(" %% ");
    // _delay_ms(300);
    TIMER0_Delay_ms_with_Blocking(300);
}