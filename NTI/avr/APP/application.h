#ifndef APPLICATION_H
#define APPLICATION_H
#include "../LIB/Datatypes.h"

void app_start();
void led_blink(void);
void init_ext_int(uint8 enableINT0, uint8 enableINT1, uint8 enableINT2);

void testKeypad();
void testLCD();
#endif