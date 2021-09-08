#ifndef _led_h_
#define _led_h_
#include "common.h"
#include "main.h"

void LED_All_Off(void);
void Waterfall_Light(void);
void LED_All_ON(void);
void LED_Flash_OneTime(uint8_t location, uint16_t continue_time);
#endif // _led_h_
