/*---------------------------------------------------------------------------------|
| --------------------------- TIM4_CH_1 -> PD12 : H -------------------------------|	 
| --------------------------- TIM4_CH_2 -> PD13 : G -------------------------------|   
| --------------------------- TIM4_CH_3 -> PD14 : F -------------------------------|	 
| --------------------------- TIM4_CH_4 -> PD15 : E -------------------------------|   
| --------------------------- TIM5_CH_1 -> PH10 : D -------------------------------|   
----------------------------------------------------------------------------------*/
#include "servo_task.h"

static SemaphoreHandle_t servo_get_data_semaphore;    /* servo data semaphore */
static uint8_t *servo_uart8_dma_receive_data;         /* uart8 dma receive data */
static uint8_t *servo_usart8_dma_have_receive_length; /* uart8 dma receive data length */
static Servo_Move_CMD servo_move_msg;                 /* servo cmd struct */

struct Servo_Move_PulseAndSequence servo_pulse_sequence = {
    .all_deinit_pulse = {1754, 1898, 1843, 1853, 1865},
    .all_deinit_sequence = {1, 2, 3, 4, 5},
    .grab_br_pulse = {1842, 1894, 1829, 1877, 1913},
    .grab_br_sequence = {1, 4, 3, 2, 5},
    .put_to_left_pulse = {1875, 1880, 1843, 1877, 1865},
    .put_to_left_sequence = {2, 3, 1, 4, 5}
};

/**
 * @brief           Servo Task
 * @param           void
 * @retval          none
 */
void Servo_Task(void const *argument)
{
    // init all servo
    Servo_Coherent_Action(servo_pulse_sequence.all_deinit_pulse, servo_pulse_sequence.all_deinit_sequence, 5);

    vTaskDelay(1000);

    // TODO 将一系列动作和数据进行再次封装
    Servo_Coherent_Action(servo_pulse_sequence.grab_br_pulse, servo_pulse_sequence.grab_br_sequence, 5);

    vTaskDelay(1000);

    Servo_Coherent_Action(servo_pulse_sequence.put_to_left_pulse, servo_pulse_sequence.put_to_left_sequence, 5);

    vTaskDelay(1000);

    servo_uart8_dma_receive_data = Get_Uart8_RxDataBuffer();

    /* create binary semaphore signal */
    servo_get_data_semaphore = xSemaphoreCreateBinary();

    while (1)
    {
        if (xSemaphoreTake(servo_get_data_semaphore, portMAX_DELAY) == pdTRUE)
        {
            Parse_Servo_Data();
            Servo_Debug_Action(servo_move_msg.servo_id, servo_move_msg.compare_value);
        }
    }
}

/**
 * @brief           give the binary signal, let the servo task to process data
 * @param           void
 * @retval          none
 */
void Uart8_DMA_GetData(void)
{
    BaseType_t pxHigherPriorityTaskWoken;
    /* 释放二值信号量 */
    xSemaphoreGiveFromISR(servo_get_data_semaphore, &pxHigherPriorityTaskWoken);
    /* 中断服务程序中使用portYIELD_FROM_ISR()强制任务切换 */
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

/**
 * @brief           parse the uart8 dma receive data, servo_id and compare_value
 * @param           void
 * @retval          none
 */
void Parse_Servo_Data(void)
{
    servo_usart8_dma_have_receive_length = Get_Uart8_RxLength();
    if (servo_uart8_dma_receive_data[*servo_usart8_dma_have_receive_length - 2] == 0x0D &&
        servo_uart8_dma_receive_data[*servo_usart8_dma_have_receive_length - 1] == 0x0A &&
        *servo_usart8_dma_have_receive_length == 5)
    {
        servo_move_msg.servo_id = servo_uart8_dma_receive_data[0];
        servo_move_msg.compare_value = servo_uart8_dma_receive_data[1] << 8 | servo_uart8_dma_receive_data[2];
        Set_Beep_Time(1, 800, 50);
        Set_LED_Flash(servo_move_msg.servo_id, 50);
    }
}

/**
 * @brief           deinit all servo
 * @param           void
 * @retval          none
 */
const static uint16_t servo_all_de_init[5] = {1754, 1898, 1843, 1853, 1865};
void Servo_All_Deinit(void)
{
    __HAL_TIM_SetCompare(&htim4, GRAB_SERVO_CH, servo_all_de_init[en_GRAB_SERVO_ID - 1]);
    vTaskDelay(500);
    __HAL_TIM_SetCompare(&htim4, PITCH_UP_SERVO_CH, servo_all_de_init[en_PITCH_UP_SERVO_ID - 1]);
    vTaskDelay(500);
    __HAL_TIM_SetCompare(&htim4, PITCH_MIDDLE_SERVO_CH, servo_all_de_init[en_PITCH_MIDDLE_SERVO_ID - 1]);
    vTaskDelay(500);
    __HAL_TIM_SetCompare(&htim4, PITCH_BOTTOM_SERVO_CH, servo_all_de_init[en_PITCH_BOTTOM_SERVO_ID - 1]);
    vTaskDelay(500);
    __HAL_TIM_SetCompare(&htim4, YAW_SERVO_CH, servo_all_de_init[en_YAW_SERVO_ID - 1]);
}

/**
 * @brief           according to the dma receive data to drive the servo
 * @param[in]       servo_id: servo id, correponding different servo
 * @param[in]       compare_value: duty ratio
 * @retval          none                
 */
// TODO 对机械臂的角度进行限幅
void Servo_Action(uint8_t servo_id, uint16_t servo_compare_value)
{
    switch (servo_id)
    {
    case en_YAW_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, YAW_SERVO_CH, servo_compare_value);
        vTaskDelay(800);
        break;
    case en_PITCH_BOTTOM_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, PITCH_BOTTOM_SERVO_CH, servo_compare_value);
        vTaskDelay(800);
        break;
    case en_PITCH_MIDDLE_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, PITCH_MIDDLE_SERVO_CH, servo_compare_value);
        vTaskDelay(800);
        break;
    case en_PITCH_UP_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, PITCH_UP_SERVO_CH, servo_compare_value);
        vTaskDelay(800);
        break;
    case en_GRAB_SERVO_ID:
        __HAL_TIM_SetCompare(&htim5, GRAB_SERVO_CH, servo_compare_value);
        vTaskDelay(800);
        break;
    default:
        break;
    }
}

uint16_t Search_Index(uint16_t array[][2], uint8_t *value, uint8_t col)
{
    uint8_t i;
    for (i = 0; i < col; i++)
    {
        if (array[i][0] == *value)
        {
            return array[i][1];
        }
    }
    return 0;
}
/**
 * @brief               根据 pulse 和移动顺序执行相应的连贯动作
 * @param[in]           pulse_value: 占空比
 * @param[in]           move_sequence: 移动顺序
 * @param[in]           num: 舵机数量
 * @retval              none
 */
void Servo_Coherent_Action(const uint16_t *pulse_value, uint8_t *move_seqence, const uint8_t num)
{
    uint8_t i;
    uint16_t _index;
    static uint16_t array_index[5][2];
    for (i = 0; i < num; i++)
    {
        // 存储原先的顺序以及对应的下标
        // 1、    4、    2、    3、    5
        // 0、    1、    2、    3、    4
        // 1754, 1898,  1843,  1853,  1865
        array_index[i][0] = move_seqence[i];
        array_index[i][1] = i;
    }
    Bubble_Sort(move_seqence, num);

    for (i = 0; i < num; i++)
    {
        uint8_t *search_value = &(move_seqence[i]);
        _index = Search_Index(array_index, search_value, 5);
        Servo_Action(_index + 1, pulse_value[_index]);
    }

    /*  SET_MOVING_AXIS(YAW_SERVO_CH, pulse_value[en_YAW_SERVO_ID - 1]);
    SET_MOVING_AXIS(PITCH_BOTTOM_SERVO_CH, pulse_value[en_PITCH_BOTTOM_SERVO_ID - 1]);
    SET_MOVING_AXIS(PITCH_MIDDLE_SERVO_CH, pulse_value[en_PITCH_MIDDLE_SERVO_ID - 1]);
    SET_MOVING_AXIS(PITCH_UP_SERVO_CH, pulse_value[en_PITCH_UP_SERVO_ID - 1]);
    SET_GRAB_AXIS(pulse_value[en_GRAB_SERVO_ID - 1]); */
}

/**
 * @brief           according to the dma receive data to drive the servo
 * @param[in]       servo_id: servo id, correponding different servo
 * @param[in]       compare_value: duty ratio
 * @retval          none                
 */
void Servo_Debug_Action(uint8_t servo_id, uint16_t servo_compare_value)
{
    switch (servo_id)
    {
    case en_YAW_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, YAW_SERVO_CH, servo_compare_value);
        vTaskDelay(10);
        break;
    case en_PITCH_BOTTOM_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, PITCH_BOTTOM_SERVO_CH, servo_compare_value);
        vTaskDelay(10);
        break;
    case en_PITCH_MIDDLE_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, PITCH_MIDDLE_SERVO_CH, servo_compare_value);
        vTaskDelay(10);
        break;
    case en_PITCH_UP_SERVO_ID:
        __HAL_TIM_SetCompare(&htim4, PITCH_UP_SERVO_CH, servo_compare_value);
        vTaskDelay(10);
        break;
    case en_GRAB_SERVO_ID:
        __HAL_TIM_SetCompare(&htim5, GRAB_SERVO_CH, servo_compare_value);
        vTaskDelay(10);
        break;
    default:
        break;
    }
}
