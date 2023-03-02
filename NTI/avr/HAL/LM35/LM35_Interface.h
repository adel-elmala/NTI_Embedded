#ifndef LM35_INTERFACE_H
#define LM35_INTERFACE_H

#define LM35_MAX_TEMP 125
#define LM35_MIN_TEMP -55
#define LM35_STEP_mVOLT 10 // mv/c

#define LM35_OUTPUT_PORT PORTA
#define LM35_OUTPUT_PIN PIN0 // adc0

#include "../../LIB/Datatypes.h"
void LM35_vInit();
unsigned int LM35_u16GetReading();

#endif
