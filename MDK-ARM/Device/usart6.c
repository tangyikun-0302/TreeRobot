#include "usart6.h"
#include "shell_task.h"
/* shell rx max buffer size */
const uint16_t shell_rx_max_size = 50;
/* shell rx buffer */
uint8_t shell_rx_buffer[shell_rx_max_size];
/* static variable, shell rx data length */
static uint8_t shell_rx_size = 0;

/**
 * @brief			get the dma stream now empty buffer size
 * @param			dma_stream : DMA_Stream_TypeDef
 * @retval			dma stream empty size
 */
uint16_t uasrt6_dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
	/* Return the number of remaining data units for DMAy Streamx */
	return ((uint16_t)(dma_stream->NDTR));
}

/**
 * @brief			uart6 rx idle callback, process the uart6 DMA receive data
 * @param			huart UART_HandleTypeDef
 * @retval 			none 
 */
static void uart6_rx_idle_callback(UART_HandleTypeDef *huart)
{
	/* clear idle it flag avoid idle interrupt all the time */
	__HAL_UART_CLEAR_IDLEFLAG(huart);

	/* handle received data in idle interrupt */
	if (huart == &SHELL_USART)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);

		/* handle shell usart6 data shell_rx_buf from DMA */
		shell_rx_size = shell_rx_max_size - uasrt6_dma_current_data_counter(huart->hdmarx->Instance);

		Shell_Get_Data();
		/* restart dma transmission */
		__HAL_DMA_SET_COUNTER(huart->hdmarx, shell_rx_max_size);
		__HAL_DMA_ENABLE(huart->hdmarx);
	}
}

/**
 * @brief			this method must be called in the stm32f4xx_it.c
 * 					when uart6 it generate, call this function
 * @param			huart UART_HandleTypeDef
 * @retval			none
 */
void Usart6_Receive_Handle(UART_HandleTypeDef *huart)
{
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
		__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
	{
		uart6_rx_idle_callback(huart);
	}
}

/**
 * @brief			this method must be call in the main init function
 * 					clear usart6 idle flag and start DMA transfer
 * @param			void
 * @retval 			none
 */
void Shell_Uart6_Init(void)
{
	/* open uart idle it */
	__HAL_UART_CLEAR_IDLEFLAG(&SHELL_USART);
	__HAL_UART_ENABLE_IT(&SHELL_USART, UART_IT_IDLE);

	HAL_UART_Receive_DMA(&SHELL_USART, shell_rx_buffer, shell_rx_max_size);
}

/**
 * @brief			get usart6 rx data length
 * 					may call in the shell_task.c
 * @param			void
 * @retval			shell rx size
 */
uint8_t *Get_Uart6_ShellRxLength(void)
{
	return &shell_rx_size;
}

/**
 * @brief			get the usart6 DMA rx buffer
 * 					may call in the shell_task.c
 * @param			void
 * @retval 			shell_rx_buffer
 */
uint8_t *Get_Uart6_RxDataBuffer(void)
{
	return shell_rx_buffer;
}
