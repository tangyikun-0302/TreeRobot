#ifndef _common_h_
#define _common_h_

#include <stdio.h>
#include "usart.h"
#include "stm32f4xx.h"
#include "init_task.h"
#include "shell_task.h"
#include "chassis_task.h"

#define TreeRobotVersion "1.0"

#define INFO_LOG(format, arg...) printf("<INFO> " format "\r\n", ##arg)
#define INFO_PRINT(format, ...) printf(format, ##__VA_ARGS__)

void Print_Logo_2_Com(void);
#endif // _common_h_

