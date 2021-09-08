#include "buzzer_task.h"

static QueueHandle_t buzzer_queue;		/* buzzer process data queue */

void Buzzer_Task(void const *argument)
{
    uint8_t i;
    // 开启 PWM
    Buzzer_PWM_Start();
    Reset_And_Download();
    // 消息队列初始化
    buzzer_queue = xQueueCreate(3,sizeof( Beep_Play_t));
    // 蜂鸣器播放数据
    Beep_Play_t beep_msg;

    vTaskDelay(200);

    while (1)
    {
        if (xQueueReceive(buzzer_queue,&beep_msg,portMAX_DELAY) == pdPASS)
        {
            for(i=0;i<beep_msg.beep_times;i++)
            {
                BUZZER_ON(beep_msg.pwm);
                vTaskDelay(beep_msg.delay_time);
                BUZZER_OFF();
                vTaskDelay(beep_msg.delay_time);
            }
        }
    }
}

// 设置蜂鸣器
BaseType_t Set_Beep_Time(uint8_t times,uint16_t pwm,uint16_t delay)
{
    Beep_Play_t beep_send_msg;
    beep_send_msg.beep_times = times;
    beep_send_msg.delay_time = delay;
    beep_send_msg.pwm = pwm;
    return xQueueSend(buzzer_queue,&beep_send_msg,0);
}
