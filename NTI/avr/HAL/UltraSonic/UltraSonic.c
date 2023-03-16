#include "UltraSonic_Interface.h"
#include "UltraSonic_Private.h"

#include "../../MCAL/DIO/DIO_Interface.h"
#include "../../MCAL/EXT_INT/External_INT_Interface.h"

EXT_Int_Conf ultrasonic_rising_intr_conf = {
    ENABLE_INT0,      // uint8 Enable_INT0_Interrupt;
    DISABLE_INT,      // uint8 Enable_INT1_Interrupt;
    DISABLE_INT,      // uint8 Enable_INT2_Interrupt;
    RISING_EDGE_MODE, // uint8 INT0_Triggering_Mode;
    RISING_EDGE_MODE, // uint8 INT1_Triggering_Mode;
    RISING_EDGE_MODE  // uint8 INT2_Triggering_Mode;
};

EXT_Int_Conf ultrasonic_failing_intr_conf = {
    ENABLE_INT0,       // uint8 Enable_INT0_Interrupt;
    DISABLE_INT,       // uint8 Enable_INT1_Interrupt;
    DISABLE_INT,       // uint8 Enable_INT2_Interrupt;
    FALLING_EDGE_MODE, // uint8 INT0_Triggering_Mode;
    FALLING_EDGE_MODE, // uint8 INT1_Triggering_Mode;
    FALLING_EDGE_MODE  // uint8 INT2_Triggering_Mode;
};
bool g_rising_edge_detected = false;
timer2_clk_t current_clk = {0};
void Ultrsonic_callback(void)
{
    if (g_rising_edge_detected)
    {
        g_rising_edge_detected = false;
        EXT_INT_SET_CONFIG(ultrasonic_rising_intr_conf);
        timer2_end();
        // update time
    }
    else
    {
        g_rising_edge_detected = true;
        EXT_INT_SET_CONFIG(ultrasonic_failing_intr_conf);
        timer2_start(&current_clk);
    }
}
void Ultrasonic_Init()
{
    DIO_vSetPinDirection(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, OUTPUT);
    DIO_vSetPinDirection(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN, INPUT);
    EXT_INT_SET_CONFIG(ultrasonic_rising_intr_conf);
}
uint16 Ultrasonic_Read_Distance(void)
{
    //  distance = (velocity * time) / 2;
}
