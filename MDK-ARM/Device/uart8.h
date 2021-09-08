#ifndef _uart8_h_
#define _uart8_h_

#include "stm32f4xx.h"
#include <stdio.h>
#include "usart.h"
#include "common.h"
#define SERVO_USART huart8





void Servo_Usart8_Init(void);
void Usart8_Receive_Handle(UART_HandleTypeDef *huart);
uint16_t uart8_dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);
uint8_t * Get_Uart8_RxDataBuffer(void);
uint8_t * Get_Uart8_RxLength(void);
#endif // _uart8_h_
