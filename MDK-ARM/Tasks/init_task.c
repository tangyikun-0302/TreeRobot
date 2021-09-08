#include "init_task.h"
#include "chassis_task.h"
#include "shell_task.h"
#include "servo_task.h"
#include "buzzer_task.h"
#include "led_task.h"

/* chassis task thread handle, in a word, which is a thread ID */
osThreadId chassis_task_handle;

/* shell task thread handle, in a word, which is a thread ID */
osThreadId shell_task_handle;

/* servo task thread handle, */
osThreadId servo_task_handle;

/* buzzer task thread handle */
osThreadId buzzer_task_handle;

/* led task thread handle */
osThreadId led_thread_handle;

/**
 * @brief           init all task
 * @note            call in the frertos.c
 * @author          weihao<wiehaoysgs@gmail.com>
 * @date            2021-8-28
 * @param           void
 * @retval          none
 */
void Init_All_Tasks(void)
{
    /* chassis task */
    osThreadDef(chassis_task, Chassis_Task, osPriorityNormal, 1, 128);
    chassis_task_handle = osThreadCreate(osThread(chassis_task), NULL);

    /* shell task */
    osThreadDef(shell_task, Shell_Task, osPriorityNormal, 2, 128);
    shell_task_handle = osThreadCreate(osThread(shell_task), NULL);

    /* servo task */
    osThreadDef(servo_task, Servo_Task, osPriorityNormal, 3, 128);
    servo_task_handle = osThreadCreate(osThread(servo_task), NULL);

    /* buzzer_task */
    osThreadDef(buzzer_task, Buzzer_Task, osPriorityNormal, 4, 128);
    buzzer_task_handle = osThreadCreate(osThread(buzzer_task), NULL);

    /* led_task */
    osThreadDef(led_task, LED_Task, osPriorityNormal, 5, 128);
    led_thread_handle = osThreadCreate(osThread(led_task), NULL);

    /*
	`````` other task ``````
	*/
}
