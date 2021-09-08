#ifndef _servo_task_h
#define _servo_task_h

#include "stm32f4xx.h"
#include <stdio.h>
#include "usart.h"
#include "freertos.h"
#include "task.h"
#include "common.h"
#include "servo_timer.h"
#include "uart8.h"
#include "remote.h"
#include "buzzer_task.h"
#include "led_task.h"
#include "math2.h"
#define YAW_SERVO_CH TIM_CHANNEL_1
#define PITCH_BOTTOM_SERVO_CH TIM_CHANNEL_4
#define PITCH_MIDDLE_SERVO_CH TIM_CHANNEL_3
#define PITCH_UP_SERVO_CH TIM_CHANNEL_2
#define GRAB_SERVO_CH TIM_CHANNEL_1

#define SET_MOVING_AXIS(MOVING_CHANNEL, servo_compare_value)               \
    {                                                                      \
        __HAL_TIM_SetCompare(&htim4, MOVING_CHANNEL, servo_compare_value); \
        vTaskDelay(50);                                                    \
    }
#define SET_GRAB_AXIS(servo_compare_value)                                \
    {                                                                     \
        __HAL_TIM_SetCompare(&htim5, GRAB_SERVO_CH, servo_compare_value); \
        vTaskDelay(50);                                                   \
    }

enum __SERVO_ID
{
    en_YAW_SERVO_ID = 1,
    en_PITCH_BOTTOM_SERVO_ID,
    en_PITCH_MIDDLE_SERVO_ID,
    en_PITCH_UP_SERVO_ID,
    en_GRAB_SERVO_ID
};

typedef struct
{
    uint8_t servo_id;
    uint16_t compare_value;

} Servo_Move_CMD;

typedef struct Servo_Move_PulseAndSequence
{
    uint16_t all_deinit_pulse[5];
    uint8_t all_deinit_sequence[5];

    uint16_t grab_br_pulse[5];
    uint8_t grab_br_sequence[5];

    uint16_t put_to_left_pulse[5];
    uint8_t put_to_left_sequence[5];
    /* ````` other ````` */
}Servo_Move_PulseAndSequence ;

void Servo_Task(void const *argument);
void Uart8_DMA_GetData(void);
void Servo_All_Deinit(void);
void Parse_Servo_Data(void);
void Servo_Action(uint8_t servo_id, uint16_t servo_compare_value);
void Servo_Coherent_Action(const uint16_t *compare_value, uint8_t *move_seqence,const uint8_t num);
uint16_t Search_Index(uint16_t array[][2], uint8_t *value, uint8_t col);
void Servo_Debug_Action(uint8_t servo_id, uint16_t servo_compare_value);
#endif // _servo_task_h

// const static uint16_t servo_all_de_init[5] = {1754, 1898, 1843, 1853, 1865}; /* servo init compare value array */
// const static int8_t servo_all_de_init_sequence[5] = {1, 2, 3, 4, 5};

// const static uint16_t servo_grab_br_pulse[5] = {1839, 1883, 1826, 1889, 1910};
// const static int8_t servo_grab_br_sequence[5] = {1,4,2,3,5};
