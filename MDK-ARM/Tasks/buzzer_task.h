#ifndef _buzzer_task_h_
#define _buzzer_task_h_
#include "common.h"
#include "buzzer.h"


typedef struct 
{
    uint8_t beep_times;
    uint16_t pwm;
    uint16_t delay_time;
}Beep_Play_t;

#define BUZZER_ON(pwm) __HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,pwm)
#define BUZZER_OFF()  __HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,0)

void Buzzer_Task(void const *argument);
BaseType_t Set_Beep_Time(uint8_t times,uint16_t pwm,uint16_t delay);

#endif // _buzzer_task_h_

