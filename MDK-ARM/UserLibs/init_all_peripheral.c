#include "init_all_peripheral.h"
#include "remote.h"
#include "common.h"
#include "usart6.h"
#include "shell.h"
#include "uart8.h"
#include "servo_timer.h"
#include "chassis_motor.h"
#include "led.h"

/**
 * @brief           init all peripherals
 * @note            call in the frertos.c
 * @author          weihao<wiehaoysgs@gmail.com>
 * @date            2021-8-28
 * @param           void
 * @retval          none
 */
void Init_All_Peripherals(void)
{
    dbus_uart_init();      // open dbus uart receive it
    can_filter_init();     // can filter init
    Shell_Uart6_Init();    // init shell usart
    Servo_Usart8_Init();   // init servo communicate uart
    Print_Logo_2_Com();    // print logo
    Shell_Init();          // init shell
    Servo_All_PWM_Start(); // PWM start
}

