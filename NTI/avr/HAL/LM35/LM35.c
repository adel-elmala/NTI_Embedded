#include "LM35_Interface.h"
#include "../../MCAL/DIO/DIO_Interface.h"
#include "../../MCAL/ADC/ADC_Interface.h"

// typedef struct ADC_Config
// {
//     /* data */
//     uint8 ref_volt;
//     uint8 result_adjust;
//     uint8 analog_channel_and_gain_select_mask;
//     uint8 enable_interrupt;
//     uint8 enable_autoTriggerMode;
//     uint8 autoTriggerSource;
// } ADC_Config_t;
void LM35_vInit()
{
    // init output volt pin to be used as adc input
    // ADC_Config_t adc_conf = {ADC_REFVOLTAGE_AVCC,
    ADC_Config_t adc_conf = {ADC_REFVOLTAGE_INTERNAL,
                             ADC_LEFT_ADJUST,
                             ADC_SINGLE_ENDDED_ADC0,
                             ADC_DISABLE_INTR,
                             ADC_DISABLE_AUTOTRIGGER,
                             ADC_AUTO_TRIGGER_SOURCE_IGNORE};
    ADC_init(adc_conf);
}
unsigned int LM35_u16GetReading()
{
    uint8 adc_low, adc_high;
    unsigned int adc_volt_reading = ADC_PollRead(&adc_low, &adc_high);
    // return ((adc_volt_reading - ((-LM35_MIN_TEMP) * LM35_STEP_mVOLT)) / LM35_STEP_mVOLT);
    return (adc_volt_reading / 4);
}
