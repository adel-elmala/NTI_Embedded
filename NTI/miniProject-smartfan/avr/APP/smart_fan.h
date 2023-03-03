#ifndef SMART_FAN_H
#define SMART_FAN_H

void sf_init_project();

void sf_app();
unsigned int regulate_motor_speed(unsigned int current_temp);
void update_lcd(unsigned int temp, unsigned int duty_cycle);

#endif