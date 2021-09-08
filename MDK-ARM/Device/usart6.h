#ifndef _usart6_h_
#define _usart6_h_
#include "usart.h"
#include "stm32f427xx.h"
#include <stdio.h>

#define SHELL_USART huart6

void Usart6_Receive_Handle(UART_HandleTypeDef *huart);
void Shell_Uart6_Init(void);
uint8_t * Get_Uart6_ShellRxLength(void);
uint8_t * Get_Uart6_RxDataBuffer(void);
uint16_t uasrt6_dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);
#endif //_usart6_h_
