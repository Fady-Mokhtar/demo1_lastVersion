// {
// 	__HAL_RCC_DMA2_CLK_ENABLE();

//   /* DMA interrupt init */
//   /* DMA2_Stream2_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
// }

// Microcontroller Specific Part

#include "dma.h"
#include "dma_regs.h"
#include "common_macros.h"
#include "STD_TYPES.h"
#include "rcc.h"
#include "NVIC.h"
#include "Errors.h"


enum FlagStatus
{
	RESET = 0U,
	SET = !RESET
}FlagStatus;

/*------------------------------------- DMA Static helping functions --------------------------------------------*/
static MCALStatus_t DMA_CheckFifoParam(DMA_HandleTypeDef *hdma)
{
	MCALStatus_t status = MCAL_OK;
	uint32_t tmp = hdma->Init.FIFOThreshold;

	/* Memory Data size equal to Byte */
	if (hdma->Init.MemDataAlignment == DMA_MDATAALIGN_BYTE)
	{
		switch (tmp)
		{
		case DMA_FIFO_THRESHOLD_1QUARTERFULL:
		case DMA_FIFO_THRESHOLD_3QUARTERSFULL:
			if ((hdma->Init.MemBurst & DMA_SxCR_MBURST_1) == DMA_SxCR_MBURST_1)
			{
				status = MCAL_ERROR;
			}
			break;
		case DMA_FIFO_THRESHOLD_HALFFULL:
			if (hdma->Init.MemBurst == DMA_MBURST_INC16)
			{
				status = MCAL_ERROR;
			}
			break;
		case DMA_FIFO_THRESHOLD_FULL:
			break;
		default:
			break;
		}
	}

	/* Memory Data size equal to Half-Word */
	else if (hdma->Init.MemDataAlignment == DMA_MDATAALIGN_HALFWORD)
	{
		switch (tmp)
		{
		case DMA_FIFO_THRESHOLD_1QUARTERFULL:
		case DMA_FIFO_THRESHOLD_3QUARTERSFULL:
			status = MCAL_ERROR;
			break;
		case DMA_FIFO_THRESHOLD_HALFFULL:
			if ((hdma->Init.MemBurst & DMA_SxCR_MBURST_1) == DMA_SxCR_MBURST_1)
			{
				status = MCAL_ERROR;
			}
			break;
		case DMA_FIFO_THRESHOLD_FULL:
			if (hdma->Init.MemBurst == DMA_MBURST_INC16)
			{
				status = MCAL_ERROR;
			}
			break;
		default:
			break;
		}
	}

	/* Memory Data size equal to Word */
	else
	{
		switch (tmp)
		{
		case DMA_FIFO_THRESHOLD_1QUARTERFULL:
		case DMA_FIFO_THRESHOLD_HALFFULL:
		case DMA_FIFO_THRESHOLD_3QUARTERSFULL:
			status = MCAL_ERROR;
			break;
		case DMA_FIFO_THRESHOLD_FULL:
			if ((hdma->Init.MemBurst & DMA_SxCR_MBURST_1) == DMA_SxCR_MBURST_1)
			{
				status = MCAL_ERROR;
			}
			break;
		default:
			break;
		}
	}

	return status;
}

static uint32_t DMA_CalcBaseAndBitshift(DMA_HandleTypeDef *hdma)
{
	uint32_t stream_number = (((uint32_t)hdma->Instance & 0xFFU) - 16U) / 24U;

	/* lookup table for necessary bitshift of flags within status registers */
	static const uint8_t flagBitshiftOffset[8U] = {0U, 6U, 16U, 22U, 0U, 6U, 16U, 22U};
	hdma->StreamIndex = flagBitshiftOffset[stream_number];

	if (stream_number > 3U)
	{
		/* return pointer to HISR and HIFCR */
		hdma->StreamBaseAddress = (((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU)) + 4U);
	}
	else
	{
		/* return pointer to LISR and LIFCR */
		hdma->StreamBaseAddress = ((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU));
	}

	return hdma->StreamBaseAddress;
}

static void DMA_SetConfig(DMA_HandleTypeDef *streamManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	/* Clear DBM bit */
	streamManager->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

	/* Configure DMA Stream data length */
	streamManager->Instance->NDTR = DataLength;

	/* Memory to Peripheral */
	if ((streamManager->Init.Direction) == DMA_MEMORY_TO_PERIPH)
	{
		/* Configure DMA Stream destination address */
		streamManager->Instance->PAR = DstAddress;

		/* Configure DMA Stream source address */
		streamManager->Instance->M0AR = SrcAddress;
	}
	/* Peripheral to Memory */
	else
	{
		/* Configure DMA Stream source address */
		streamManager->Instance->PAR = SrcAddress;

		/* Configure DMA Stream destination address */
		streamManager->Instance->M0AR = DstAddress;
	}
}

/*------------------------------------------- DMA Intialization function --------------------------------------------*/
void DMA_Init(DMA_HandleTypeDef *streamManager)
{

	switch (((uint32_t)streamManager->Instance & (uint32_t)(~0x3FFU)))
	{
	case (uint32_t)DMA1:
		RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_DMA1);
		break;

	case (uint32_t)DMA2:
		RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_DMA2);
		break;

	default:
		/*ERROR wrong inputs*/
		break;
	}

	/* Change DMA peripheral state */
	streamManager->State = HAL_DMA_STATE_BUSY;

	// disable the module
	(streamManager->Instance->CR &= ~DMA_SxCR_EN);
	/* Check if the DMA Stream is effectively disabled */
	while ((streamManager->Instance->CR & DMA_SxCR_EN) != 0)
		; // maybe you should do time out

	// configure the Control Register
	CLEAR_AND_SET_BYMASKS(streamManager->Instance->CR,
						  (DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | DMA_SxCR_PL | DMA_SxCR_MSIZE | DMA_SxCR_PSIZE |
						   DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_CT | DMA_SxCR_DBM | DMA_SxCR_DIR | DMA_SxCR_CIRC),
						  (streamManager->Init.Channel | streamManager->Init.Priority | streamManager->Init.MemDataAlignment | streamManager->Init.PeriphDataAlignment |
						   streamManager->Init.MemInc | streamManager->Init.PeriphInc | streamManager->Init.Direction | streamManager->Init.Mode));
	;

	/** not handling fifo mode right now***********************************************************************/
	// ffffffffffffffffffffffffffffffff
	if (streamManager->Init.FIFOMode == DMA_FIFOMODE_ENABLE)
	{
		/* Get memory burst and peripheral burst */
		streamManager->Instance->CR |= streamManager->Init.MemBurst | streamManager->Init.PeriphBurst;
	}
	// fffffffffffffffffffffffffffffffff

	/* Clear Direct mode and FIFO threshold bits */
	/* Prepare the DMA Stream FIFO configuration */ /*(no thershold added!!)*/
	CLEAR_AND_SET_BYMASKS(streamManager->Instance->FCR, (DMA_SxFCR_DMDIS | DMA_SxFCR_FTH), (streamManager->Init.FIFOMode));

	// ffffffffffffffffffffffffffffffffff
	if (streamManager->Init.FIFOMode == DMA_FIFOMODE_ENABLE)
	{
		/* Get the FIFO threshold */
		streamManager->Instance->FCR |= streamManager->Init.FIFOThreshold;

		if (streamManager->Init.MemBurst != DMA_MBURST_SINGLE)
		{
			if (DMA_CheckFifoParam(streamManager) != MCAL_OK)
			{
				/* Update error code */
				streamManager->ErrorCode = HAL_DMA_ERROR_PARAM;

				/* Change the DMA state */
				streamManager->State = HAL_DMA_STATE_READY;

				// return MCAL_ERROR;
			}
		}
	}
	// fffffffffffffffffffffffffffffffffff

	/***************************************************************************/

	// 	/* Clear all interrupt flags */
	/* Initialize StreamBaseAddress and StreamIndex parameters to be used to calculate
	 DMA steam Base Address needed by HAL_DMA_IRQHandler() and HAL_DMA_PollForTransfer() */
	DMA_Base_Registers *regs;
	regs = (DMA_Base_Registers *)DMA_CalcBaseAndBitshift(streamManager);

	/* Clear all interrupt flags */
	regs->IFCR = 0x3FU << streamManager->StreamIndex;

	/* Initialize the error code */
	streamManager->ErrorCode = HAL_DMA_ERROR_NONE;

	/* Initialize the DMA state */
	streamManager->State = HAL_DMA_STATE_READY;

	// return MCAL_OK;
}

/*---------------------------------------------- DMA Polling functions ----------------------------------------------*/
void DMA_start(DMA_HandleTypeDef *streamManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{

	// check size range
	if (((DataLength) < 0x01U) || (0xffffU < (DataLength)))
	{
		/*ERROR*/
	}
	//(or) assert_param(IS_DMA_BUFFER_SIZE(DataLength));

	if (HAL_DMA_STATE_READY == streamManager->State)
	{
		/* Change DMA peripheral state */
		streamManager->State = HAL_DMA_STATE_BUSY;

		/* Initialize the error code */
		streamManager->ErrorCode = HAL_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		DMA_SetConfig(streamManager, SrcAddress, DstAddress, DataLength);

		/* Enable the Peripheral */
		streamManager->Instance->CR |= DMA_SxCR_EN;
	}
}

MCALStatus_t DMA_PollForTransfer(DMA_HandleTypeDef *hdma, HAL_DMA_LevelCompleteTypeDef CompleteLevel /*, uint32_t Timeout*/)
{
	MCALStatus_t status = MCAL_OK;
	uint32_t mask_cpltlevel;
	//   uint32_t tickstart = HAL_GetTick();
	uint32_t tmpisr;

	/* calculate DMA base and stream number */
	DMA_Base_Registers *regs;

	if (HAL_DMA_STATE_BUSY != hdma->State)
	{
		/* No transfer ongoing */
		hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
		// __HAL_UNLOCK(hdma);
		return MCAL_ERROR;
	}

	/* Polling mode not supported in circular mode and double buffering mode */
	if ((hdma->Instance->CR & DMA_SxCR_CIRC) != RESET)
	{
		hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
		return MCAL_ERROR;
	}

	////set complete level mask
	/* Get the level transfer complete flag */
	if (CompleteLevel == HAL_DMA_FULL_TRANSFER)
	{
		/* Transfer Complete flag */
		mask_cpltlevel = DMA_FLAG_TRANSFER_COMPLETE << hdma->StreamIndex;
	}
	else
	{
		/* Half Transfer Complete flag */
		mask_cpltlevel = DMA_FLAG_HALF_TRANSFER << hdma->StreamIndex;
	}

	regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
	tmpisr = regs->ISR;

	// loop until transfer is complete or hlfcplt   or   if there is an error
	while (((tmpisr & mask_cpltlevel) == RESET) && ((hdma->ErrorCode & HAL_DMA_ERROR_TE) == RESET))
	{
		/* Check for the Timeout (Not applicable in circular mode)*/
		// if(Timeout != HAL_MAX_DELAY)
		// {
		//   if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))
		//   {
		//     /* Update error code */
		//     hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;

		//     /* Change the DMA state */
		//     hdma->State = HAL_DMA_STATE_READY;

		//     /* Process Unlocked */
		//     __HAL_UNLOCK(hdma);

		//     return HAL_TIMEOUT;
		//   }
		// }

		/* Get the ISR register value */
		tmpisr = regs->ISR;

		if ((tmpisr & (DMA_FLAG_TRANSFER_ERROR << hdma->StreamIndex)) != RESET)
		{
			/* Update error code */
			hdma->ErrorCode |= HAL_DMA_ERROR_TE;

			/* Clear the transfer error flag */
			regs->IFCR = DMA_FLAG_TRANSFER_ERROR << hdma->StreamIndex;
		}

		if ((tmpisr & (DMA_FLAG_FIFO_ERROR << hdma->StreamIndex)) != RESET)
		{
			/* Update error code */
			hdma->ErrorCode |= HAL_DMA_ERROR_FE;

			/* Clear the FIFO error flag */
			regs->IFCR = DMA_FLAG_FIFO_ERROR << hdma->StreamIndex;
		}

		if ((tmpisr & (DMA_FLAG_DIRECT_MODE_ERROR << hdma->StreamIndex)) != RESET)
		{
			/* Update error code */
			hdma->ErrorCode |= HAL_DMA_ERROR_DME;

			/* Clear the Direct Mode error flag */
			regs->IFCR = DMA_FLAG_DIRECT_MODE_ERROR << hdma->StreamIndex;
		}
	}

	// handle the error
	if (hdma->ErrorCode != HAL_DMA_ERROR_NONE)
	{
		if ((hdma->ErrorCode & HAL_DMA_ERROR_TE) != RESET)
		{
			//   HAL_DMA_Abort(hdma);//111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111

			/* Clear the half transfer and transfer complete flags */
			regs->IFCR = (DMA_FLAG_HALF_TRANSFER | DMA_FLAG_TRANSFER_COMPLETE) << hdma->StreamIndex;

			/* Change the DMA state */
			hdma->State = HAL_DMA_STATE_READY;

			/* Process Unlocked */
			//   __HAL_UNLOCK(hdma);

			return MCAL_ERROR;
		}
	}

	/* Get the level transfer complete flag */
	if (CompleteLevel == HAL_DMA_FULL_TRANSFER)
	{
		/* Clear the half transfer and transfer complete flags */
		regs->IFCR = (DMA_FLAG_HALF_TRANSFER | DMA_FLAG_TRANSFER_COMPLETE) << hdma->StreamIndex;

		hdma->State = HAL_DMA_STATE_READY;

		/* Process Unlocked */
		// __HAL_UNLOCK(hdma);
	}
	else
	{
		/* Clear the half transfer and transfer complete flags */
		regs->IFCR = (DMA_FLAG_HALF_TRANSFER) << hdma->StreamIndex;
	}

	return status;
}

/*--------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------- DMA Interrupts functions --------------------------------------------*/
void DMA_Start_IT(DMA_HandleTypeDef *streamManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{

	/* calculate DMA base and stream number */
	DMA_Base_Registers *regs = (DMA_Base_Registers *)streamManager->StreamBaseAddress;

	/* Check the parameters */
	// check size range
	if (((DataLength) < 0x01U) || (0xffffU < (DataLength)))
	{
		/*ERROR*/
	}
	//   assert_param(IS_DMA_BUFFER_SIZE(DataLength));

	/* Process locked */
	//   __HAL_LOCK(hdma);

	if (HAL_DMA_STATE_READY == streamManager->State)
	{
		/* Change DMA peripheral state */
		streamManager->State = HAL_DMA_STATE_BUSY;

		/* Initialize the error code */
		streamManager->ErrorCode = HAL_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		DMA_SetConfig(streamManager, SrcAddress, DstAddress, DataLength);

		/* Clear all interrupt flags at correct offset within the register */
		regs->IFCR = 0x3FU << streamManager->StreamIndex;

		/* Enable Common interrupts*/
		streamManager->Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;

		// half transfer callback function and interrupt enable
		// if(streamManager->XferHalfCpltCallback != NULL)
		// {
		//   streamManager->Instance->CR  |= DMA_IT_HT;
		// }

		/* Enable the Peripheral */
		streamManager->Instance->CR |= DMA_SxCR_EN;
	}
	//   else //state not ready
	//   {
	//     /* Process unlocked */
	//     __HAL_UNLOCK(hdma);

	//     /* Return error status */
	//     status = HAL_BUSY;
	//   }

	/* Enable DMA global interrupts */
        switch((uint32_t)streamManager->Instance){
            case (uint32_t)DMA1_Stream0:
                NVIC_EnableIRQ(DMA1_Stream0_IRQn);
                break;
            case (uint32_t)DMA1_Stream1:
                NVIC_EnableIRQ(DMA1_Stream1_IRQn);
                break;
            case (uint32_t)DMA1_Stream2:
                NVIC_EnableIRQ(DMA1_Stream2_IRQn);
                break;
            case (uint32_t)DMA1_Stream3:
                NVIC_EnableIRQ(DMA1_Stream3_IRQn);
                break;
            case (uint32_t)DMA1_Stream4:
                NVIC_EnableIRQ(DMA1_Stream4_IRQn);
                break;
            case (uint32_t)DMA1_Stream5:
                NVIC_EnableIRQ(DMA1_Stream5_IRQn);
                break;
            case (uint32_t)DMA1_Stream6:
                NVIC_EnableIRQ(DMA1_Stream6_IRQn);
                break;
            case (uint32_t)DMA1_Stream7:
                NVIC_EnableIRQ(DMA1_Stream7_IRQn);
                break;
            case (uint32_t)DMA2_Stream0:
                NVIC_EnableIRQ(DMA2_Stream0_IRQn);
                break;
            case (uint32_t)DMA2_Stream1:
                NVIC_EnableIRQ(DMA2_Stream1_IRQn);
                break;
            case (uint32_t)DMA2_Stream2:
                NVIC_EnableIRQ(DMA2_Stream2_IRQn);
                break;
            case (uint32_t)DMA2_Stream3:
                NVIC_EnableIRQ(DMA2_Stream3_IRQn);
                break;
            case (uint32_t)DMA2_Stream4:
                NVIC_EnableIRQ(DMA2_Stream4_IRQn);
                break;
            case (uint32_t)DMA2_Stream5:
                NVIC_EnableIRQ(DMA2_Stream5_IRQn);
                break;
            case (uint32_t)DMA2_Stream6:
                NVIC_EnableIRQ(DMA2_Stream6_IRQn);
                break;
            case (uint32_t)DMA2_Stream7:
                NVIC_EnableIRQ(DMA2_Stream7_IRQn);
                break;

        }

	return;
}

/**
 * @brief  Handles DMA interrupt request.
 * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA Stream.
 * @retval None
 */
void MCAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
	uint32_t tmpisr;
	//   __IO uint32_t count = 0U;
	//   uint32_t timeout = SystemCoreClock / 9600U;

	/* calculate DMA base and stream number */
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	tmpisr = regs->ISR;

	/* Transfer Error Interrupt management ***************************************/
	if ((tmpisr & (DMA_FLAG_TRANSFER_ERROR << hdma->StreamIndex)) != RESET)
	{
		// if this interrupt is enable
		if ((hdma->Instance->CR & DMA_IT_TE) != RESET)
		{
			/* Disable the transfer error interrupt */ // and abort the dma
			hdma->Instance->CR &= ~(DMA_IT_TE);

			/* Clear the transfer error flag */
			regs->IFCR = DMA_FLAG_TRANSFER_ERROR << hdma->StreamIndex;

			/* Update error code */
			hdma->ErrorCode |= HAL_DMA_ERROR_TE;
		}
	}

	/* FIFO Error Interrupt management ******************************************/
	if ((tmpisr & (DMA_FLAG_FIFO_ERROR << hdma->StreamIndex)) != RESET)
	{
		// if this interrupt is enable
		if ((hdma->Instance->FCR & DMA_IT_FE) != RESET)
		{
			/* Clear the FIFO error flag */
			regs->IFCR = DMA_FLAG_FIFO_ERROR << hdma->StreamIndex;

			/* Update error code */
			hdma->ErrorCode |= HAL_DMA_ERROR_FE;
		}
	}

	/* Direct Mode Error Interrupt management ***********************************/
	if ((tmpisr & (DMA_FLAG_DIRECT_MODE_ERROR << hdma->StreamIndex)) != RESET)
	{
		// if this interrupt is enable
		if ((hdma->Instance->CR & DMA_IT_DME) != RESET)
		{
			/* Clear the direct mode error flag */
			regs->IFCR = DMA_FLAG_DIRECT_MODE_ERROR << hdma->StreamIndex;

			/* Update error code */
			hdma->ErrorCode |= HAL_DMA_ERROR_DME;
		}
	}

	/* Half Transfer Complete Interrupt management ******************************/
	if ((tmpisr & (DMA_FLAG_HALF_TRANSFER << hdma->StreamIndex)) != RESET)
	{
		// if this interrupt is enable
		if ((hdma->Instance->CR & DMA_IT_HT) != RESET)
		{
			/* Clear the half transfer complete flag */
			regs->IFCR = DMA_FLAG_HALF_TRANSFER << hdma->StreamIndex;

			/* Multi_Buffering mode enabled */
			if (((hdma->Instance->CR) & (uint32_t)(DMA_SxCR_DBM)) != RESET)
			{
				/* Current memory buffer used is Memory 0 */
				if ((hdma->Instance->CR & DMA_SxCR_CT) == RESET)
				{
					//   if(hdma->XferHalfCpltCallback != NULL)
					//   {
					//     /* Half transfer callback */
					//     hdma->XferHalfCpltCallback(hdma);
					//   }
				}
				/* Current memory buffer used is Memory 1 */
				else
				{
					//   if(hdma->XferM1HalfCpltCallback != NULL)
					//   {
					//     /* Half transfer callback */
					//     hdma->XferM1HalfCpltCallback(hdma);
					//   }
				}
			}
			else
			{
				/* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
				if ((hdma->Instance->CR & DMA_SxCR_CIRC) == RESET)
				{
					/* Disable the half transfer interrupt */
					hdma->Instance->CR &= ~(DMA_IT_HT);
				}
				// if(hdma->XferHalfCpltCallback != NULL)
				// {
				// /* Half transfer callback */
				// hdma->XferHalfCpltCallback(hdma);
				// }
			}
		}
	}

	/* Transfer Complete Interrupt management ***********************************/
	if ((tmpisr & (DMA_FLAG_TRANSFER_COMPLETE << hdma->StreamIndex)) != RESET)
	{
		// if this interrupt is enable
		if ((hdma->Instance->CR & DMA_IT_TC) != RESET)
		{
			/* Clear the transfer complete flag */
			regs->IFCR = DMA_FLAG_TRANSFER_COMPLETE << hdma->StreamIndex;

			// if transfer error happend and then tranfer got completed then abort
			if (HAL_DMA_STATE_ABORT == hdma->State)
			{
				/* Disable all the transfer interrupts */
				hdma->Instance->CR &= ~(DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);
				hdma->Instance->FCR &= ~(DMA_IT_FE);

				// if((hdma->XferHalfCpltCallback != NULL) || (hdma->XferM1HalfCpltCallback != NULL))
				// {
				//   hdma->Instance->CR  &= ~(DMA_IT_HT);
				// }

				/* Clear all interrupt flags at correct offset within the register */
				regs->IFCR = 0x3FU << hdma->StreamIndex;

				/* Change the DMA state */
				hdma->State = HAL_DMA_STATE_READY;

				/* Process Unlocked */
				// __HAL_UNLOCK(hdma);

				// if(hdma->XferAbortCallback != NULL)
				// {
				//   hdma->XferAbortCallback(hdma);
				// }
				return;
			}

			// if no error and there is other modes, then handle them
			//  double buffer mode
			if (((hdma->Instance->CR) & (uint32_t)(DMA_SxCR_DBM)) != RESET)
			{
				/* Current memory buffer used is Memory 0 */
				if ((hdma->Instance->CR & DMA_SxCR_CT) == RESET)
				{
					//   if(hdma->XferM1CpltCallback != NULL)
					//   {
					//     /* Transfer complete Callback for memory1 */
					//     hdma->XferM1CpltCallback(hdma);
					//   }
				}
				/* Current memory buffer used is Memory 1 */
				else
				{
					//   if(hdma->XferCpltCallback != NULL)
					//   {
					//     /* Transfer complete Callback for memory0 */
					//     hdma->XferCpltCallback(hdma);
					//   }
				}
			}
			/* Disable the transfer complete interrupt if the DMA mode is not CIRCULAR */
			else
			{
				// if not circular mode
				if ((hdma->Instance->CR & DMA_SxCR_CIRC) == RESET)
				{
					/* Disable the transfer complete interrupt */
					hdma->Instance->CR &= ~(DMA_IT_TC);

					/* Change the DMA state */
					hdma->State = HAL_DMA_STATE_READY;

					/* Process Unlocked */
					//   __HAL_UNLOCK(hdma);
				}

				// if(hdma->XferCpltCallback != NULL)
				// {
				//   /* Transfer complete callback */
				//   hdma->XferCpltCallback(hdma);
				// }
			}
		}
	}

	/* manage error case */
	if (hdma->ErrorCode != HAL_DMA_ERROR_NONE)
	{
		if ((hdma->ErrorCode & HAL_DMA_ERROR_TE) != RESET)
		{
			hdma->State = HAL_DMA_STATE_ABORT;

			/* Disable the stream */
			//   __HAL_DMA_DISABLE(hdma);
			(hdma->Instance->CR &= ~DMA_SxCR_EN);

			do
			{
				// if (++count > timeout)
				// {
				//   break;
				// }
			} while ((hdma->Instance->CR & DMA_SxCR_EN) != RESET);

			/* Change the DMA state */
			hdma->State = HAL_DMA_STATE_READY;

			/* Process Unlocked */
			//   __HAL_UNLOCK(hdma);
		}

		// if(hdma->XferErrorCallback != NULL)
		// {
		//   /* Transfer error callback */
		//   hdma->XferErrorCallback(hdma);
		// }
	}
}

MCALStatus_t DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{
	if (hdma->State != HAL_DMA_STATE_BUSY)
	{
		hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
		return MCAL_ERROR;
	}
	else
	{
		/* Set Abort State  */
		hdma->State = HAL_DMA_STATE_ABORT;

		/* Disable the stream */
		// __HAL_DMA_DISABLE(hdma);
		(hdma->Instance->CR &= ~DMA_SxCR_EN);
	}

	return MCAL_OK;
}