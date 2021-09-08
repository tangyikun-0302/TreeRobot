#ifndef _led_task_h_
#define _led_task_h_
#include "common.h"
#include "led.h"

typedef struct 
{
    uint8_t led_location;
    uint16_t continue_time;
}LED_Flash_t;

void LED_Task(void const * argument);
BaseType_t Set_LED_Flash(uint8_t location,uint16_t continue_time);

#endif // _led_task_h_
