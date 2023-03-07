#include "test.h"
#include "../MCAL/TIMER/timer.h"
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/DIO/DIO_REG.h"
#include "../MCAL/PWM/PWM_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"
#include "../MCAL/UART/UART_Interface.h"
#include "../MCAL/SPI/SPI_Interface.h"

#include "../HAL/L298_H_Bridge/L298_H_Bridge_Interface.h"
#include "../HAL/LCD/LCD_Interface.h"

#include "../LIB/Queue/Queue.h"
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

UART_Config_t uart_conf = {9600,
                           UART_DISABLE_DOUBLE_TRANSMISSION_SPEED,
                           UART_DISABLE_MULTIPROCESSOR_COMMUNICATION_MODE,
                           UART_ENABLE_RECEIVE_COMPLETE_INTERRUPT,
                           UART_ENABLE_TRANSMIT_COMPLETE_INTERRUPT,
                           //    UART_DISABLE_TRANSMIT_COMPLETE_INTERRUPT,
                           UART_DISABLE_DATA_REG_EMPTY_COMPLETE_INTERRUPT,
                           UART_ENABLE_RECEIVER,
                           UART_ENABLE_TRANSMITER,
                           UART_8BIT_CHAR_SIZE,
                           //    UART_EVEN_PARITY,
                           UART_DISABLE_PARITY,
                           UART_1_BIT_STOP};
void test_uart()
{
    UART_vInit(uart_conf);
    // UART_vTransmitString("HI from uart");

    lcd_init();

    while (1)
    {

        // uint8 d = UART_u16Receive_poll();
        // UART_vTransmit_poll(d);
        uint8 d = UART_u16Receive_no_poll();
        if (d == QUEUE_ERROR_EMPTY)
            continue;
        else
        {

            UART_vTransmit_no_poll(d);

            lcd_sendData(d);
        }
        // UART_vTransmit_poll(d);
    }
}

SPI_Config_t master_spi_conf = {
    true,            // bool enable_interrupt;
    true,            // bool enable_spi;
    true,            // bool is_master;
    false,           // bool double_speed;
    CLK_LOW_IDLE,    // uint8 clk_polarity;
    SAMPLE_ON_FIRST, // uint8 clk_phase;
    CLK_DIV_16,      // uint8 clk_divisor;
    DORD_LSB_FIRST   // uint8 data_order;
};
SPI_Config_t slave_spi_conf = {
    false,           // bool enable_interrupt;
    true,            // bool enable_spi;
    false,           // bool is_master;
    false,           // bool double_speed;
    CLK_LOW_IDLE,    // uint8 clk_polarity;
    SAMPLE_ON_FIRST, // uint8 clk_phase;
    CLK_DIV_16,      // uint8 clk_divisor;
    DORD_LSB_FIRST   // uint8 data_order;
};
void test_spi_master()
{
    lcd_init();
    SPI_Init(&master_spi_conf);
    // char str[] = "adel,using spi as a master!";
    // uint8 counter = 0;
    uint8 data;
    // TIMER0_Delay_ms_with_Blocking(100);

    // SPI_Transmit_Async('a');
    // SPI_Transmit_Async('a');
    // SPI_Transmit_Async('d');
    // SPI_Transmit_Async('e');
    // SPI_Transmit_Async('l');
    // SPI_Transmit_Async('!');
    // data = SPI_Receive_Async();
    // lcd_sendData(data);
    // data = SPI_Receive_Async();
    // lcd_sendData(data);
    // data = SPI_Receive_Async();
    // lcd_sendData(data);
    // data = SPI_Receive_Async();
    // lcd_sendData(data);

    while (1)
    {
        // SPI_Master_send_Sync('b');
        // data = SPI_Master_receive_Sync();
        SPI_Transmit_Async('a');
        SPI_Transmit_Async('b');
        data = SPI_Receive_Async();
        lcd_sendData(data);

        // uint8 rec = SPI_Transieve_Sync(str[counter]);
        // if ((++counter) > 28)
        //     counter = 0;
        // lcd_sendData(rec);
    }
}

void test_spi_slave()
{
    SPI_Init(&slave_spi_conf);
    // char str[] = "adel,using spi as a master!";
    // uint8 counter = 0;
    while (1)
    {
        // SPI_slave_send_Sync('a');

        // uint8 rec = SPI_Transieve_Sync(str[counter]);
        // if ((++counter) > 28)
        //     counter = 0;
        // lcd_sendData(rec);
        /* code */
    }
}