#include "chassis_task.h"

/* remote data */
const static Rc_ctrl_t *remoter_control;

/* chassis motor of M3508 feedback data */
const static motor_measure_t *chassis_motor_measure_data;

/* Set this value to change the chassis motor speed */
const uint8_t MOTOR_SPEED = 5;

/**
 * 1 -> 前左
 * 2 -> 后左
 * 3 -> 后右
 * 4 -> 前右
 * + front 
 * - back
 */
/**
 * @brief			Chassis Task. Ctrol omnidirectional wheel motion
 * @param			void
 * @retval			none
 */
void Chassis_Task(void const *argument)
{
	remoter_control = Get_Remote_DataPointer();
	chassis_motor_measure_data = Get_Chassis_MotorData_Pointer();
	while (1)
	{
		Set_Chassis_Motors_Speed((remoter_control->rc.ch2 + remoter_control->rc.ch0 - remoter_control->rc.ch1) * MOTOR_SPEED,
								 (remoter_control->rc.ch2 - remoter_control->rc.ch0 - remoter_control->rc.ch1) * MOTOR_SPEED,
								 (remoter_control->rc.ch2 + remoter_control->rc.ch0 + remoter_control->rc.ch1) * MOTOR_SPEED,
								 (remoter_control->rc.ch2 - remoter_control->rc.ch0 + remoter_control->rc.ch1) * MOTOR_SPEED); 

		/* back and front */
		/* Set_Chassis_Motors_Speed((-remoter_control->rc.ch1) * MOTOR_SPEED,
								 (remoter_control->rc.ch0) * MOTOR_SPEED,
								 (remoter_control->rc.ch1) * MOTOR_SPEED,
								 (-remoter_control->rc.ch0) * MOTOR_SPEED); */

		vTaskDelay(1);
	}
}
