#include "led_task.h"

static QueueHandle_t led_flash_queue;	/* led process data queue */

void LED_Task(void const * argument)
{
    LED_Flash_t led_flash_receive_msg;

    Waterfall_Light();

    led_flash_queue = xQueueCreate(3,sizeof(LED_Flash_t));

    while (1)
    {
        if (xQueueReceive(led_flash_queue,&led_flash_receive_msg,portMAX_DELAY) == pdPASS)
        {
            LED_Flash_OneTime(led_flash_receive_msg.led_location,led_flash_receive_msg.continue_time);
        }
    }
}

/**
 * @brief           send data to the queue back
 * @note            LED_Task will use the data 
 * @param[in]       location: led location,range from 1~8
 * @param[in]       continue_time: led flash contine time, advised is 50 (ms)
 */
BaseType_t Set_LED_Flash(uint8_t location,uint16_t continue_time)
{
    LED_Flash_t led_send_msg;
    led_send_msg.led_location = location;
    led_send_msg.continue_time = continue_time;
    return xQueueSend(led_flash_queue,&led_send_msg,0);
}
