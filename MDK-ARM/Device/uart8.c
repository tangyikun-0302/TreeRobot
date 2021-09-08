#include "uart8.h"
#include "servo_task.h"
/* uart8 rx max size */
const uint8_t uart8_rx_max_size = 50;
/* uart8 rx buffer size */
uint8_t uart8_rx_buffer[uart8_rx_max_size];
/* uart8 rx size (static variable) */
static uint8_t uart8_rx_size;

/**
 * @brief           Init uart8
 * @param           void
 * @retval          none
 */
void Servo_Usart8_Init(void)
{
    /* open uart idle it */
    __HAL_UART_CLEAR_IDLEFLAG(&SERVO_USART);
    __HAL_UART_ENABLE_IT(&SERVO_USART, UART_IT_IDLE);

    HAL_UART_Receive_DMA(&SERVO_USART, uart8_rx_buffer, uart8_rx_max_size);
}

/**
 * @brief           Get the uart of dma rx size
 * @param[in]       dma_stream: dma stream
 * @retval          empty size of dma buffer 
 */
uint16_t uart8_dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
    return ((uint16_t)(dma_stream->NDTR));
}

/**
 * @brief           uart8 rx idle it callback, static method, process rx data
 * @param           huart: UART_Handle_TypeDef
 * @retval          none
 */
static void uart8_rx_idle_callback(UART_HandleTypeDef *huart)
{
    /* clear idle it flage avoid idle interrupt all the time */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* handle received data in idle interrupt */
    if (huart == &SERVO_USART)
    {
        /* clear DMA transfer complete flag */
        __HAL_DMA_DISABLE(huart->hdmarx);

        /* handle servo uart8 data, uart8_rx_buffer from DMA */
        uart8_rx_size = uart8_rx_max_size - uart8_dma_current_data_counter(huart->hdmarx->Instance);
        // TODO 将接收到的数据进行解析，或者直接通知 servo_task 任务进行解析调用
        // HAL_UART_Transmit(huart, uart8_rx_buffer, uart8_rx_size, 0xfff);
        Uart8_DMA_GetData();
        
        /* restart DMA transmission */
        __HAL_DMA_SET_COUNTER(huart->hdmarx, uart8_rx_max_size);
        __HAL_DMA_ENABLE(huart->hdmarx);
    }
}

/**
 * @brief           Uart8 rx it handle, this function must call in the stm32f4xx_it.c
 *                  when uart8 it generate, call this method
 * @param           huart: UART_Handle_TypeDef
 * @retval          none
 */
void Usart8_Receive_Handle(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
        __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    {
        uart8_rx_idle_callback(huart);
    }
}

/**
 * @brief           get uart8 dma rx data, may call in the servo_task.c
 * @param           void
 * @retval          uart8_rx_buffer
 */
uint8_t *Get_Uart8_RxDataBuffer(void)
{
    return uart8_rx_buffer;
}

/**
 * @brief           get uart8 dma rx size
 * @param           void
 * @retval          none
 */
uint8_t *Get_Uart8_RxLength(void)
{
    return &uart8_rx_size;
}
