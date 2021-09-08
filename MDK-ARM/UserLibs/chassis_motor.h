/**
 ***************************************(C) COPYRIGHT 2021 CSS***************************************
 * @file       motor.h
 * @brief      this file contains the common defines and functions prototypes for 
 *             the motor.c driver
 * @note         
 * @Version    V1.0.0
 * @Date       Feb-15-2021      
 ***************************************(C) COPYRIGHT 2021 CSS***************************************
 */

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "can.h"

typedef enum
{
	CAN_CHASSIS_ALL_ID = 0x200,
	CAN_AUXILIARY_ALL_ID = 0x1FF,
	motor1 = 0x201,
	motor2 = 0x202,
	motor3 = 0x203,
	motor4 = 0x204,
	motor6 = 0x205,
} can_msg_id;

typedef struct
{
	uint16_t ecd;
	int16_t speed_rpm;
	int16_t given_current;
	uint8_t temperate;
	int16_t last_ecd;
} motor_measure_t;

void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void CAN_cmd_auxiliary(int16_t M5, int16_t M6);
void CAN_cmd_chassis(int16_t M3508_1, int16_t M3508_2, int16_t M3508_3, int16_t M3508_4);
const motor_measure_t *get_chassis_motor_data(void);
void Set_Chassis_Motors_Speed(float speed_fl, float speed_fr, float speed_bl, float speed_br);
const motor_measure_t *Get_Chassis_MotorData_Pointer(void);
#endif
