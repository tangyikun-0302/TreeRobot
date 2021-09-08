#include "shell_task.h"
#include "usart6.h"
#include "shell.h"

const static uint8_t* u6_shell_rx_len;
static uint8_t* u6_shell_rx_buffer;
static SemaphoreHandle_t shell_get_data_semaphore;  

static void Shell_Str_Process(void);

void Shell_Task(void const *argument)
{
	shell_get_data_semaphore = xSemaphoreCreateBinary();
	if(shell_get_data_semaphore == NULL)
    {
        INFO_LOG("ERR >> Create shell get data semaphore error!\r\n");
    }
    else
    {
		u6_shell_rx_buffer = Get_Uart6_RxDataBuffer();
        while(1)
        {
            if( xSemaphoreTake(shell_get_data_semaphore, portMAX_DELAY) == pdTRUE )
            {
                Shell_Str_Process();
				Shell_Command_Parse(u6_shell_rx_buffer);
            }
        }
    }
}

/**
 * @brief           give binary signal value, let the shell task get the mutex
 * @param           void
 * @retval          none
 */
void Shell_Get_Data(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
	/* 释放二值信号量 */
	xSemaphoreGiveFromISR(shell_get_data_semaphore, &pxHigherPriorityTaskWoken);
    /* 中断服务程序中使用portYIELD_FROM_ISR()强制任务切换 */
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

/**
 * @brief           preprocess receive data
 * @param           void
 * @retval          none
 */
static void Shell_Str_Process(void)
{
    u6_shell_rx_len = Get_Uart6_ShellRxLength();
	if(u6_shell_rx_buffer[*u6_shell_rx_len-2]=='\r' && u6_shell_rx_buffer[*u6_shell_rx_len-1]=='\n')
	{
		u6_shell_rx_buffer[*u6_shell_rx_len-2] = '\0';
	}
	else
	{
		u6_shell_rx_buffer[*u6_shell_rx_len] = '\0';
	}
}




