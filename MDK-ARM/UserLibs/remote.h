
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "usart.h"

#define UART_RX_DMA_SIZE (1024)
#define DBUS_MAX_LEN (50)
#define DBUS_BUFLEN (18)
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define DBUS_HUART huart1 /* for dji remote controler reciever */

#define ROCKER_DATA_CHECK(data) ((data > 660) || (data < -660))
#define SWITCH_DATA_CHECK(data) (!((data == 1) || (data == 2) || (data == 3)))
#define RC_KEY_PRESSED(rc_key, key) (rc_key & key)

/* define key value */
#define KEY_W 0x01
#define KEY_A 0x04
#define KEY_S 0x02
#define KEY_D 0x08

#define KEY_CTRL 0x20
#define KEY_SHIFT 0x10

#define KEY_Z 0x800
#define KEY_X 0x1000
#define KEY_C 0x2000
#define KEY_V 0x4000
#define KEY_B 0x8000
#define KEY_F 0x200
#define KEY_G 0x400
#define KEY_Q 0x40
#define KEY_E 0x80
#define KEY_R 0x100

/** 
  * @brief  remote control information
  */
typedef __packed struct
{
        /* rocker channel information */
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;
        int16_t ch4;

        /* left and right lever information */
        uint8_t sw1;
        uint8_t sw2;
} rc_info_t;

typedef __packed struct
{
        __packed struct
        {
                int16_t ch0;
                int16_t ch1;
                int16_t ch2;
                int16_t ch3;
                int16_t ch4;
                uint8_t s1;
                uint8_t s2;
        } rc;

        __packed struct
        {
                int16_t x;
                int16_t y;
                int16_t z;
                uint8_t press_l;
                uint8_t press_r;
        } mouse;

        __packed struct
        {
                uint16_t value;
        } key;

        __packed struct
        {
                int16_t ch2; //right and left
                int16_t ch3; //back and front

        } virtual_rocker;

} Rc_ctrl_t;

void uart_receive_handler(UART_HandleTypeDef *huart);
void dbus_uart_init(void);
void USART_SendData(USART_TypeDef *USARTx, uint16_t Data);
void Wasd_Key_To_Virtual_Rocker(Rc_ctrl_t *rc);
const Rc_ctrl_t* Get_Remote_DataPointer(void);
uint8_t Remoter_Data_Check(Rc_ctrl_t *remote_controller);
#endif
