/*   setting
modes:
Synchronous
Asynchronous
..
*
basic parameters:
	Baud rate
	word length
	parity
	stop bits
*
*/

/*** Information about USART
 * USART sends the least significient bit first
 */

#include "rcc.h"
#include "std_types.h"
#include "usart.h"
#include "usart_regs.h"
#include "common_macros.h"
#include "nvic.h"
#include "dma.h"

/*****************************************************************************************/
/*********************************** USART FUNCTIONS *************************************/
/*****************************************************************************************/

/** USART initialize function ******************************************************************/
void USART_Init(USART_ManagerStruct *usartxManger)
{

	switch ((uint32_t)usartxManger->Instance)
	{
	case USART1_BASE:
		RCC_enableAPB2Peripheral(RCC_APB2PERIPHERAL_USART1);
		break;

	case USART2_BASE:
		RCC_enableAPB1Peripheral(RCC_APB1PERIPHERAL_USART2);
		break;

	case USART6_BASE:
		RCC_enableAPB2Peripheral(RCC_APB2PERIPHERAL_USART6);
		break;

	default:
		break;
	}

	// disable the usart
	usartxManger->Instance->CR1 &= ~USART_CR1_UE;

	// Configure stop bits
	REG_CLEARANDSET_BYMASKS(usartxManger->Instance->CR2, USART_CR2_STOP_clrMsk, usartxManger->Init.StopBits);

	// configure Wordlength, parity, TxorRX mode
	REG_CLEARANDSET_BYMASKS(usartxManger->Instance->CR1,
							((uint32_t)(USART_CR1_M_clrMsk | USART_CR1_PCE_clrMsk | USART_CR1_PS_clrMsk | USART_CR1_TE_clrMsk | USART_CR1_RE_clrMsk)),
							((uint32_t)usartxManger->Init.WordLength | usartxManger->Init.Parity | usartxManger->Init.Mode));

	// configure baudrate
	usartxManger->Instance->BRR = UART_BRR_SAMPLING16(16000000, usartxManger->Init.BaudRate);

	// set usart states to ready
	usartxManger->TxState = USART_STATE_READY;
	usartxManger->RxState = USART_STATE_READY;

	// enable the usart
	usartxManger->Instance->CR1 |= USART_CR1_UE;

	// reset the error code
	usartxManger->ErrorCode = USART_ERROR_NONE;

	return;
}

/** USART functions that uses the interrupts ***************************************************/
// Transmit functions by interrupts
MCALStatus_t USART_startTransmit_IT(USART_ManagerStruct *usartxManger, const uint8_t *pData, uint16_t Size)
{
	if (usartxManger->TxState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	usartxManger->pTxBuffPtr = pData;
	usartxManger->TxXferSize = Size;
	usartxManger->TxXferCount = Size;

	// reset error state
	usartxManger->ErrorCode = USART_ERROR_NONE;

	// set busy state to indiacte that the USart is in use
	usartxManger->TxState = USART_STATE_BUSY_TX;

	// enable interrupt TXE when the tx is empty and ready to write the new data
	usartxManger->Instance->CR1 |= USART_CR1_TXEIE;

	//Enable_Interrupts();
	switch ((uint32_t) usartxManger->Instance)
	{
	case USART1_BASE:
		NVIC_EnableIRQ(USART1_IRQn);
		break;
	case USART2_BASE:
		NVIC_EnableIRQ(USART2_IRQn);
		break;
	case USART6_BASE:
		NVIC_EnableIRQ(USART6_IRQn);
		break;
	default:
		return MCAL_ERROR;
		break;
	}
	

	return MCAL_OK;
}
static MCALStatus_t USART_dataTransmit_IT(USART_ManagerStruct *usartxManger)
{
	const uint16_t *tmp;

	/* Check that a Tx process is ongoing */
	if (usartxManger->TxState == USART_STATE_BUSY_TX)
	{
		if ((usartxManger->Init.WordLength == USART_WORDLENGTH_9B) && (usartxManger->Init.Parity == USART_PARITY_NONE))
		{
			tmp = (const uint16_t *)usartxManger->pTxBuffPtr;
			usartxManger->Instance->DR = (uint16_t)(*tmp & (uint16_t)0x01FF);
			usartxManger->pTxBuffPtr += 2U;
		}
		else
		{
			usartxManger->Instance->DR = (uint8_t)(*usartxManger->pTxBuffPtr++ & (uint8_t)0x00FF);
		}

		if (--(usartxManger->TxXferCount) == 0U)
		{
			/* Disable the UART Transmit Data Register Empty Interrupt */
			usartxManger->Instance->CR1 &= ~USART_CR1_TXEIE_clrMsk;

			/* Enable the UART Transmit Complete Interrupt */
			usartxManger->Instance->CR1 |= USART_CR1_TCIE;
		}
		return MCAL_OK;
	}
	else
	{
		return MCAL_BUSY;
	}
}

// Recieve functions by interrupts
MCALStatus_t USART_startReceive_IT(USART_ManagerStruct *usartxManger, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (usartxManger->RxState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL_PTR) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	/* Set Reception type to Standard reception */
	// usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;
	usartxManger->pRxBuffPtr = pData;
	usartxManger->RxXferSize = Size;
	usartxManger->RxXferCount = Size;

	// to reset the error state if you handle it in the manager
	usartxManger->ErrorCode = USART_ERROR_NONE;
	usartxManger->RxState = USART_STATE_BUSY_RX;

	// 111111111111111 enable ERROR interrupts
	if (usartxManger->Init.Parity != USART_PARITY_NONE)
	{
		/* Enable the UART Parity Error Interrupt */
		usartxManger->Instance->CR1 |= (USART_CR1_PEIE_Msk);
	}

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	usartxManger->Instance->CR1 |= (USART_CR3_EIE_Msk);
	// 111111111111111

	/* Enable the UART Data Register not empty Interrupt */
	usartxManger->Instance->CR1 |= (USART_CR1_RXNEIE_Msk);

	/* enable usart global interrupt*/
	NVIC_EnableIRQ(USART1_IRQn); // usart1 you need to be be hadle with all usarts

	return MCAL_OK;
}
static MCALStatus_t USART_dataReceive_IT(USART_ManagerStruct *usartxManger)
{
	uint8_t *pdata8bits;
	uint16_t *pdata16bits;

	/* Check that a Rx process is ongoing */
	if (usartxManger->RxState == USART_STATE_BUSY_RX)
	{
		if ((usartxManger->Init.WordLength == USART_WORDLENGTH_9B) && (usartxManger->Init.Parity == USART_PARITY_NONE))
		{
			pdata8bits = NULL;
			pdata16bits = (uint16_t *)usartxManger->pRxBuffPtr;
			*pdata16bits = (uint16_t)(usartxManger->Instance->DR & (uint16_t)0x01FF);
			usartxManger->pRxBuffPtr += 2U;
		}
		else
		{
			pdata8bits = (uint8_t *)usartxManger->pRxBuffPtr;
			pdata16bits = NULL;

			if ((usartxManger->Init.WordLength == USART_WORDLENGTH_9B) || ((usartxManger->Init.WordLength == USART_WORDLENGTH_8B) && (usartxManger->Init.Parity == USART_PARITY_NONE)))
			{
				*pdata8bits = (uint8_t)(usartxManger->Instance->DR & (uint8_t)0x00FF);
			}
			else
			{
				*pdata8bits = (uint8_t)(usartxManger->Instance->DR & (uint8_t)0x007F);
			}
			usartxManger->pRxBuffPtr += 1U;
		}

		if (--usartxManger->RxXferCount == 0U)
		{
			/* Disable the UART Data Register not empty Interrupt */
			usartxManger->Instance->CR1 &= ~(USART_CR1_RXNEIE_clrMsk);

			// 11111111111111111 Disable Error Interrupts
			/* Disable the UART Parity Error Interrupt */
			usartxManger->Instance->CR1 &= ~(USART_CR1_PEIE_clrMsk);

			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			usartxManger->Instance->CR1 &= ~(USART_CR3_EIE_clrMsk);
			// 11111111111111111

			/* Rx process is completed, restore usartxManger->RxState to Ready */
			usartxManger->RxState = USART_STATE_READY;

			//   /* Initialize type of RxEvent to Transfer Complete */
			//   usartxManger->RxEventType = HAL_UART_RXEVENT_TC;

			// /* Check current reception Mode :
			// 	If Reception till IDLE event has been selected : */
			// if (usartxManger->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
			// {
			// 	/* Set reception type to Standard */
			// 	usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

			// 	/* Disable IDLE interrupt */
			// 	ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

			// 	/* Check if IDLE flag is set */
			// 	if (__HAL_UART_GET_FLAG(usartxManger, UART_FLAG_IDLE))
			// 	{
			// 		/* Clear IDLE flag in ISR */
			// 		__HAL_UART_CLEAR_IDLEFLAG(usartxManger);
			// 	}

			// }

			return MCAL_OK;
		}

		return MCAL_OK;
	}
	else
	{
		return MCAL_BUSY;
	}
}

static void UART_EndTxTransfer(USART_ManagerStruct *usartxManger)
{
	/* Disable TXEIE and TCIE interrupts */
	ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));

	/* At end of Tx process, restore huart->gState to Ready */
	usartxManger->TxState = USART_STATE_READY;

	/*maybe you can add a callbackfunction and call it*/
}
static void UART_EndRxTransfer(USART_ManagerStruct *usartxManger)
{
	/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
	//   ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
	//   ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);

	Disable_Interrupts();
	usartxManger->Instance->CR1 &= ~(USART_CR1_RXNEIE_clrMsk | USART_CR1_PEIE_clrMsk);
	usartxManger->Instance->CR3 &= ~(USART_CR3_EIE);
	Enable_Interrupts();

	/* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
	//   if (usartxManger->ReceptionType == USART_RECEPTION_TOIDLE)
	//   {
	// 		Disable_Interrupts();
	//=		ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);
	//		Enable_Interrupts();
	//   }

	/* At end of Rx process, restore huart->RxState to Ready */
	usartxManger->RxState = USART_STATE_READY;
	//   usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;
}

/** USART functions that uses polling ***********************************************************/
void USART_sendByte_polling(USART_ManagerStruct *usartxManger, const uint8_t data)
{
	/* 1. wait til the TDR is empty and ready to take-in data (wait for the TXE flag).
	 * 2. then write the data in the Data Register.
	 */

	// stop while the TX line is full  (wait while the TXE flag is clear)
	while ((usartxManger->Instance->SR & USART_SR_TXE) == 0)
	{
	} 

	// writing the data in the data Register DR
	usartxManger->Instance->DR = data;

	// not nessecary    only nessecaery after the last byte sent by the uart to  indiacte that all is complete and its okay to disable the usart after it
	// while (!((USART_basePtr->SR)&(USART_SR_TC_Pos)))
}
uint8_t USART_recieveByte_polling(USART_ManagerStruct *usartxManger)
{
	/* 1. wait for the module to detect a byte on the line. that will raise a flag (RXNE) RX Not Empty. wait for the flag.
	 * 2. then read and return the recieved data.
	 */

	// stop while the RX line is empty  (wait while the RXNE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->Instance->SR, USART_SR_RXNE_Pos))
	{
	}

	// read and return the recieved data
	return usartxManger->Instance->DR;
}

/** USART Interrupt Handler Function ************************************************************/
enum FlagStatus
{
	RESET = 0U,
	SET = !RESET
};

void MCAL_USART_IRQHandler(USART_ManagerStruct *usartxManger)
{
	uint32_t isrflags = (usartxManger->Instance->SR);
	uint32_t cr1its = (usartxManger->Instance->CR1);
	uint32_t cr3its = (usartxManger->Instance->CR3);
	uint32_t errorflags = 0x00U;
	uint32_t dmarequest = 0x00U;

	errorflags = (isrflags & (uint32_t)(USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE));

	/* receive if no error and recieve mode */
	if (errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
		if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			USART_dataReceive_IT(usartxManger);
			return; // no error and recieve mode receives
		}
	}

	// if error interrupts are enables
	// handle error if there is an error interrupts are enabled
	/* If some errors occur Abort (cancel all recive interrupts. and fn. and return)*/
	if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
	{
		/******************** SET ERROR CODE ********************/
		/* UART parity error interrupt occurred ----------------------------------*/
		if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
		{
			usartxManger->ErrorCode |= USART_ERROR_PE;
		}

		/* UART frame error flag is raised ------------------------------------*/
		if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			usartxManger->ErrorCode |= USART_ERROR_FE;
		}

		/* UART noise error flag is raised --*/ // the noise flag doesn't generate interrupt. it is raised at receiving as same time as RXNE interrupt.
		if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			usartxManger->ErrorCode |= USART_ERROR_NE;
		}

		/* UART Over-Run interrupt occurred --------------------------------------*/
		if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))
		{
			usartxManger->ErrorCode |= USART_ERROR_ORE;
		}
		/******************************************************/

		/* Call UART Error Call back function if need to --------------------------*/
		// if there is an error and its interrupt is enabled   receive if there is data in DR and then ABORT
		if (usartxManger->ErrorCode != USART_ERROR_NONE)
		{
			/* UART in mode Receiver -----------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
			{
				USART_dataReceive_IT(usartxManger);
			}

			/* If Overrun error occurs, or if any error occurs in DMA mode reception,
			   consider error as blocking */
			dmarequest = (usartxManger->Instance->CR3 & USART_CR3_DMAR);
			if (((usartxManger->ErrorCode & USART_ERROR_ORE) != RESET) || dmarequest)
			{
				/* Blocking error : transfer is aborted
				   Set the UART state ready to be able to start again the process,
				   Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
				UART_EndRxTransfer(usartxManger);

				/* Disable the UART DMA Rx request mode if enabled */
				if (dmarequest)
				{
					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_DMAR);

					/* Abort the UART DMA Rx stream */
					if (usartxManger->hdmarx != NULL)
					{
						/* Set the UART DMA Abort callback :
						   will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
						// usartxManger->hdmarx->XferAbortCallback = UART_DMAAbortOnError;
						if (DMA_Abort_IT(usartxManger->hdmarx) != MCAL_OK) // dma abort
						{
							/* Call Directly XferAbortCallback function in case of error */
							// usartxManger->hdmarx->XferAbortCallback(usartxManger->hdmarx);
						}
					}
					else
					{
						/* Call user USART error callback */
					}
				}
				else
				{
					/* Call user USART error callback */
				}
			}
			else // if not overrun error
			{
				/* Non Blocking error : transfer could go on.
				   Error is notified to user through user error callback */

				usartxManger->ErrorCode = USART_ERROR_NONE;
			}
		}
		return;
	} /* End if some error occurs */


	// idle
	{
	// /* Check current reception Mode :
	//    If Reception till IDLE event has been selected : */
	// if ((usartxManger->ReceptionType == HAL_UART_RECEPTION_TOIDLE) && ((isrflags & USART_SR_IDLE) != 0U) && ((cr1its & USART_CR1_IDLEIE) != 0U))
	// {
	// 	__HAL_UART_CLEAR_IDLEFLAG(usartxManger);

	// 	/* Check if DMA mode is enabled in UART */
	// 	if (HAL_IS_BIT_SET(usartxManger->Instance->CR3, USART_CR3_DMAR))
	// 	{
	// 		/* DMA mode enabled */
	// 		/* Check received length : If all expected data are received, do nothing,
	// 		   (DMA cplt callback will be called).
	// 		   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 		uint16_t nb_remaining_rx_data = (uint16_t)__HAL_DMA_GET_COUNTER(usartxManger->hdmarx);
	// 		if ((nb_remaining_rx_data > 0U) && (nb_remaining_rx_data < usartxManger->RxXferSize))
	// 		{
	// 			/* Reception is not complete */
	// 			usartxManger->RxXferCount = nb_remaining_rx_data;

	// 			/* In Normal mode, end DMA xfer and HAL UART Rx process*/
	// 			if (usartxManger->hdmarx->Init.Mode != DMA_CIRCULAR)
	// 			{
	// 				/* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_PEIE);
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);

	// 				/* Disable the DMA transfer for the receiver request by resetting the DMAR bit
	// 				   in the UART CR3 register */
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_DMAR);

	// 				/* At end of Rx process, restore usartxManger->RxState to Ready */
	// 				usartxManger->RxState = USART_STATE_READY;
	// 				usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

	// 				/* Last bytes received, so no need as the abort is immediate */
	// 				(void)HAL_DMA_Abort(usartxManger->hdmarx);
	// 			}

	// 			/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 			In this case, Rx Event type is Idle Event */
	// 			usartxManger->RxEventType = HAL_UART_RXEVENT_IDLE;

	// 			///////////////if you want to add a cbf on event you can add it here
	// 		}
	// 	}
	// 	else
	// 	{
	// 		/* DMA mode not enabled */
	// 		/* Check received length : If all expected data are received, do nothing.
	// 		   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 		uint16_t nb_rx_data = usartxManger->RxXferSize - usartxManger->RxXferCount;
	// 		if ((usartxManger->RxXferCount > 0U) && (nb_rx_data > 0U)) // ebtada w lessa fadlo bytes lw 7asl kda fa abort
	// 		{
	// 			//////////////////////////
	// 			/* Disable the UART Parity Error Interrupt and RXNE interrupts */
	// 			ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

	// 			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	// 			ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);
	// 			//////////////////////////

	// 			/* Rx process is completed, restore usartxManger->RxState to Ready */
	// 			usartxManger->RxState = USART_STATE_READY;
	// 			usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 			ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

	// 			/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 			   In this case, Rx Event type is Idle Event */
	// 			usartxManger->RxEventType = HAL_UART_RXEVENT_IDLE;
	// 		}

	// 	}




	// 	return;
	// }
	;}




	/* UART in mode Transmitter ------------------------------------------------*/
	if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
	{
		USART_dataTransmit_IT(usartxManger);
		return;
	}

	/* UART in mode Transmitter end --------------------------------------------*/
	if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
	{
		UART_EndTxTransfer(usartxManger);
		return;
	}
}

/***********************************************************************************************************************/
/*********************************************** USART using DMA *******************************************************/
/***********************************************************************************************************************/

MCALStatus_t USART_Transmit_DMA(USART_ManagerStruct *huart, const uint8_t *pData, uint16_t Size)
{
	// const uint32_t *tmp;

	/* Check that a Tx process is not already ongoing */
	if (huart->TxState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	huart->pTxBuffPtr = pData;
	huart->TxXferSize = Size;
	huart->TxXferCount = Size;

	huart->ErrorCode = USART_ERROR_NONE;
	huart->TxState = USART_STATE_BUSY_TX;

	// /* Set the UART DMA transfer complete callback */
	// huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;

	// /* Set the UART DMA Half transfer complete callback */
	// huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;

	// /* Set the DMA error callback */
	// huart->hdmatx->XferErrorCallback = UART_DMAError;

	// /* Set the DMA abort callback */
	// huart->hdmatx->XferAbortCallback = NULL;

	/* Enable the UART transmit DMA stream */
	DMA_Start_IT(huart->hdmatx, (uint32_t)(pData+1), (uint32_t)&huart->Instance->DR, Size-1);

	/* Clear the TC flag in the USART SR register by writing 0 to it */
	(huart)->Instance->SR = ~(UART_FLAG_TC);

	/* Enable the DMA transfer for transmit request by setting the DMAT bit
		in the UART CR3 register */
	ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);

	// sending the 1st byte by polling then the DMA will send the rest of the buffer from the 2nd byte 
	// we may need a while loop before sending to check the flag like in polling func we use if cond instead
	if (huart->Instance->SR & USART_SR_TXE)
	{
		huart->Instance->DR = *pData;
	}
	

	return MCAL_OK;
}

MCALStatus_t USART_Receive_DMA(USART_ManagerStruct *huart, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (huart->RxState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}
	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	/* Set Reception type to Standard reception */
	// huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	huart->pRxBuffPtr = pData;
	huart->RxXferSize = Size;

	huart->ErrorCode = USART_ERROR_NONE;
	huart->RxState = USART_STATE_BUSY_RX;

	/*<! mda call back functions >*/
	// /* Set the UART DMA transfer complete callback */
	// huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;

	// /* Set the UART DMA Half transfer complete callback */
	// huart->hdmarx->XferHalfCpltCallback = UART_DMARxHalfCplt;

	// /* Set the DMA error callback */
	// huart->hdmarx->XferErrorCallback = UART_DMAError;

	// /* Set the DMA abort callback */
	// huart->hdmarx->XferAbortCallback = NULL;

	/* Enable the DMA stream */
	DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

	/* Clear the usart Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
	// Clear USART Overrun error
	{
		__IO uint32_t tmpreg = 0x00U;
		tmpreg = huart->Instance->SR;
		tmpreg = huart->Instance->DR;
		(void)tmpreg;
	}

	if (huart->Init.Parity != USART_PARITY_NONE)
	{
		/* Enable the UART Parity Error Interrupt */
		ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);
	}

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

	/* Enable the DMA transfer for the receiver request by setting the DMAR bit
	in the UART CR3 register */
	ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

	return MCAL_OK;
}
