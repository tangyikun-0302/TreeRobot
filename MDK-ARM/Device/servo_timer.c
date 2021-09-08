#include "servo_timer.h"

void Servo_All_PWM_Start(void)
{
    /*----------------------|
    | TIM4_CH_1 -> PD12 : H |	 
    | TIM4_CH_2 -> PD13 : G |   
    | TIM4_CH_3 -> PD14 : F |	 
    | TIM4_CH_4 -> PD15 : E |   
    | TIM5_CH_1 -> PH10 : D |   
    -----------------------*/
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
}
