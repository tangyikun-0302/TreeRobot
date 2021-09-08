
#include "string.h"
#include "stdlib.h"
#include "remote.h"
#include "usart.h"
#include "main.h"
#include "math2.h"
uint8_t dbus_buf[DBUS_BUFLEN];

Rc_ctrl_t rc;
#define RC_KEY rc->key.value
#define KEY_PRESSED(key) (RC_KEY & key)
#define VIRTUAL_ROCKER_STEP1 20
#define VIRTUAL_ROCKER_STEP2 10

/**
  * @brief      		enable global uart it and do not use DMA transfer done it
  * @param[in]  		huart: uart IRQHandler id
  * @param[in]  		pData: receive buff 
  * @param[in]  		Size:  buff size
  * @retval     		set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
{
	uint32_t tmp1 = 0;

	tmp1 = huart->RxState;

	if (tmp1 == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0))
		{
			return HAL_ERROR;
		}

		huart->pRxBuffPtr = pData;
		huart->RxXferSize = Size;
		huart->ErrorCode = HAL_UART_ERROR_NONE;

		/* Enable the DMA Stream */
		HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

		/* 
		 * Enable the DMA transfer for the receiver request by setting the DMAR bit
		 * in the UART CR3 register 
		 */
		SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

		return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
}

/**
  * @brief      		returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param[in]  		dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
  *             		to 7 to select the DMA Stream.
  * @retval     		The number of remaining data units in the current DMAy Streamx transfer.
  */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
	/* Return the number of remaining data units for DMAy Streamx */
	return ((uint16_t)(dma_stream->NDTR));
}

/**
  * @brief       		handle received rc data
  * @param[out]  		rc:   structure to save handled rc data
  * @param[in]   		buff: the buff which saved raw rc data
  * @retval 			none
  */
void rc_callback_handler(volatile const uint8_t *sbus_buf, Rc_ctrl_t *rc_ctrl)
{
	rc_ctrl->rc.ch0 = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;		  //!< Channel 0
	rc_ctrl->rc.ch1 = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
	rc_ctrl->rc.ch2 = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |		  //!< Channel 2
					   (sbus_buf[4] << 10)) &
					  0x07ff;
	rc_ctrl->rc.ch3 = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
	rc_ctrl->rc.s1 = ((sbus_buf[5] >> 4) & 0x0003);						  //!< Switch left
	rc_ctrl->rc.s2 = ((sbus_buf[5] >> 4) & 0x000C) >> 2;				  //!< Switch right
	rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);				  //!< Mouse X axis
	rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);				  //!< Mouse Y axis
	rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);				  //!< Mouse Z axis
	rc_ctrl->mouse.press_l = sbus_buf[12];								  //!< Mouse Left Is Press ?
	rc_ctrl->mouse.press_r = sbus_buf[13];								  //!< Mouse Right Is Press ?
	rc_ctrl->key.value = sbus_buf[14] | (sbus_buf[15] << 8);			  //!< KeyBoard value
	rc_ctrl->rc.ch4 = sbus_buf[16] | (sbus_buf[17] << 8);				  //NULL

	rc_ctrl->rc.ch0 -= RC_CH_VALUE_OFFSET;
	rc_ctrl->rc.ch1 -= RC_CH_VALUE_OFFSET;
	rc_ctrl->rc.ch2 -= RC_CH_VALUE_OFFSET;
	rc_ctrl->rc.ch3 -= RC_CH_VALUE_OFFSET;
	rc_ctrl->rc.ch4 -= RC_CH_VALUE_OFFSET;

	/*  paser WASD key datato rocher value */
	Wasd_Key_To_Virtual_Rocker(rc_ctrl);

	/* limite the mouse x¡¢y value range */
	rc_ctrl->mouse.x = Int16_Limit(rc_ctrl->mouse.x, -5000, 5000);
	rc_ctrl->mouse.y = Int16_Limit(rc_ctrl->mouse.y, -5000, 5000);
}

/**
  * @brief      		clear idle it flag after uart receive a frame data
  * @param[in]  		huart: uart IRQHandler id
  * @retval  
  */
static void uart_rx_idle_callback(UART_HandleTypeDef *huart)
{
	/* clear idle it flag avoid idle interrupt all the time */
	__HAL_UART_CLEAR_IDLEFLAG(huart);

	/* handle received data in idle interrupt */
	if (huart == &DBUS_HUART)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);

		/* handle dbus data dbus_buf from DMA */
		if ((DBUS_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == DBUS_BUFLEN)
		{
			rc_callback_handler(dbus_buf, &rc);
		}

		/* restart dma transmission */
		__HAL_DMA_SET_COUNTER(huart->hdmarx, DBUS_MAX_LEN);
		__HAL_DMA_ENABLE(huart->hdmarx);
	}
}

/**
 * @brief 			check the remote data
 * @param 			Rc_ctrl_t : remote_controller
 * @return 			1:error!
 * 		   			0:no error!
 */
uint8_t Remoter_Data_Check(Rc_ctrl_t *remote_controller)
{
	if (ROCKER_DATA_CHECK(remote_controller->rc.ch0))
	{
		return 1;
	}
	if (ROCKER_DATA_CHECK(remote_controller->rc.ch1))
	{
		return 1;
	}
	if (ROCKER_DATA_CHECK(remote_controller->rc.ch2))
	{
		return 1;
	}
	if (ROCKER_DATA_CHECK(remote_controller->rc.ch3))
	{
		return 1;
	}
	if (SWITCH_DATA_CHECK(remote_controller->rc.s1))
	{
		return 1;
	}
	if (SWITCH_DATA_CHECK(remote_controller->rc.s2))
	{
		return 1;
	}
	return 0;
}

/**
 * @brief 			make the key value to rocher value
 * @param 			Rc_ctrl_t : rc
 * @return 			none
 */
void Wasd_Key_To_Virtual_Rocker(Rc_ctrl_t *rc)
{
	/* only press W, no press S */
	if (KEY_PRESSED(KEY_W) && !(KEY_PRESSED(KEY_S)))
	{
		if (rc->virtual_rocker.ch3 < 0)
		{
			rc->virtual_rocker.ch3 += VIRTUAL_ROCKER_STEP2;
		}
		if (rc->virtual_rocker.ch3 < 600)
		{
			rc->virtual_rocker.ch3 += VIRTUAL_ROCKER_STEP1;
		}
	}
	/* only press S, no press W */
	else if (KEY_PRESSED(KEY_S) && !(KEY_PRESSED(KEY_W)))
	{
		if (rc->virtual_rocker.ch3 > 0)
		{
			rc->virtual_rocker.ch3 -= VIRTUAL_ROCKER_STEP2;
		}
		if (rc->virtual_rocker.ch3 > -600)
		{
			rc->virtual_rocker.ch3 -= VIRTUAL_ROCKER_STEP1;
		}
	}
	/* no press W and S */
	else
	{
		if (rc->virtual_rocker.ch3 < 0)
		{
			rc->virtual_rocker.ch3 += VIRTUAL_ROCKER_STEP2;
			if (rc->virtual_rocker.ch3 > 0)
				rc->virtual_rocker.ch3 = 0;
		}
		else if (rc->virtual_rocker.ch3 > 0)
		{
			rc->virtual_rocker.ch3 -= VIRTUAL_ROCKER_STEP2;
			if (rc->virtual_rocker.ch3 < 0)
				rc->virtual_rocker.ch3 = 0;
		}
	}

	/* only press D */
	if (KEY_PRESSED(KEY_D) && !(KEY_PRESSED(KEY_A)))
	{
		if (rc->virtual_rocker.ch2 < 0)
		{
			rc->virtual_rocker.ch2 += VIRTUAL_ROCKER_STEP2;
		}
		if (rc->virtual_rocker.ch2 < 600)
		{
			rc->virtual_rocker.ch2 += VIRTUAL_ROCKER_STEP1;
		}
	}
	/* only press A */
	else if (KEY_PRESSED(KEY_A) && !(KEY_PRESSED(KEY_D)))
	{
		if (rc->virtual_rocker.ch2 > 0)
		{
			rc->virtual_rocker.ch2 -= VIRTUAL_ROCKER_STEP2;
		}
		if (rc->virtual_rocker.ch2 > -600)
		{
			rc->virtual_rocker.ch2 -= VIRTUAL_ROCKER_STEP1;
		}
	}
	/* no press A and D */
	else
	{
		if (rc->virtual_rocker.ch2 < 0)
		{
			rc->virtual_rocker.ch2 += VIRTUAL_ROCKER_STEP2;
			if (rc->virtual_rocker.ch2 > 0)
				rc->virtual_rocker.ch2 = 0;
		}
		else if (rc->virtual_rocker.ch2 > 0)
		{
			rc->virtual_rocker.ch2 -= VIRTUAL_ROCKER_STEP2;
			if (rc->virtual_rocker.ch2 < 0)
				rc->virtual_rocker.ch2 = 0;
		}
	}
}

/**
  * @brief      		callback this function when uart interrupt 
  * @param[in]  		huart: uart IRQHandler id
  * @retval  			none
  */
void uart_receive_handler(UART_HandleTypeDef *huart)
{
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
		__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
	{
		uart_rx_idle_callback(huart);
	}
}

/**
  * @brief   		initialize dbus uart device 
  * @param   		void
  * @retval  		none
  */
void dbus_uart_init(void)
{
	/* open uart idle it */
	__HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
	__HAL_UART_ENABLE_IT(&DBUS_HUART, UART_IT_IDLE);

	uart_receive_dma_no_it(&DBUS_HUART, dbus_buf, DBUS_MAX_LEN);
}
const Rc_ctrl_t* Get_Remote_DataPointer(void)
{
	return &rc;
}
