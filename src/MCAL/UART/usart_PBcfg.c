/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "usart.h"
#include "usart_regs.h"//for dev phase only
// #include "usart_cfg.h"
#include "STD_TYPES.h"
#include "dma.h"

extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
extern DMA_HandleTypeDef dma2Manager_stream7_usart1_tx;

//USART
// this must be extern in the main or in the .h file
USART_ManagerStruct usart1Manager={
	.Instance	= (USART_RegStruct*)USART1_BASE,
	.Init		= {
		.BaudRate = 9600,
		.WordLength = USART_WORDLENGTH_8B,
		.StopBits = USART_STOPBITS_1,
		.Parity = USART_PARITY_NONE,
		.Mode = USART_MODE_TX_RX

	},
	.pTxBuffPtr	= NULL_PTR,
	.TxXferSize	= 0,
	.TxXferCount= 0,
	.pRxBuffPtr	= NULL_PTR,
	.RxXferSize	= 0,
	.RxXferCount= 0,
	.TxState	= USART_STATE_RESET,
	.RxState	= USART_STATE_RESET,
	.ErrorCode	= USART_ERROR_NONE,
	.hdmarx = &dma2Manager_stream2_usart1_rx,
	.hdmatx = &dma2Manager_stream7_usart1_tx
};

// usart1Manager.ErrorCode = 0;
