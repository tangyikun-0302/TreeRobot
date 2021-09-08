#include "buzzer.h"

/**
 * @brief           buzzer PWM start
 * @param           void
 * @retval          none
 */
// TODO operate register
void Buzzer_PWM_Start(void)
{
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
}

/**
 * @brief           when reset and download program, this method will be called
 * @param           void
 * @retval          none
 */
// TODO operate register
void Reset_And_Download(void)
{
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, 950);
        vTaskDelay(80);
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, 0);
    }
}
