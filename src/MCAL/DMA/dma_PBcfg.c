

#include "dma.h"
#include "usart.h"

/* USART1 DMA Managers */
/* USART1_RX Init */

extern USART_ManagerStruct usart1Manager;

// Receive
DMA_HandleTypeDef dma2Manager_stream2_usart1_rx = {
	.Instance = DMA2_Stream2,
	.Init = {
		.Channel = DMA_CHANNEL_4,
		.Direction = DMA_PERIPH_TO_MEMORY,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Mode = DMA_NORMAL,
		.Priority = DMA_PRIORITY_HIGH,
		.FIFOMode = DMA_FIFOMODE_DISABLE

	},
	.State = HAL_DMA_STATE_RESET,
	.Parent = &usart1Manager

};

// Transmit
DMA_HandleTypeDef dma2Manager_stream7_usart1_tx = {
	.Instance = DMA2_Stream7,
	.Init = {
		.Channel = DMA_CHANNEL_4,
		.Direction = DMA_MEMORY_TO_PERIPH,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Mode = DMA_NORMAL,
		.Priority = DMA_PRIORITY_LOW,
		.FIFOMode = DMA_FIFOMODE_DISABLE

	},
	.State = HAL_DMA_STATE_RESET,
	.Parent = &usart1Manager

};

// /* USART1_TX Init */
// DMA_HandleTypeDef dma2Manager_stream2_usart1_tx={
// 	.Instance = DMA2_Stream7,
// 	.Init ={
// 		.Channel = DMA_CHANNEL_4,
// 		.Direction = DMA_MEMORY_TO_PERIPH,
// 		.PeriphInc 	= DMA_PINC_DISABLE,							//prephiral increment (disable because usart has 1 Data Reg so no need to inc)
// 		.MemInc 	= DMA_MINC_ENABLE,							//memory increment  (enable because we need to increment in memory)
// 		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,		//Data width options are Byte, HalfWord, Word
// 		.MemDataAlignment = DMA_MDATAALIGN_BYTE,		//Data width options are Byte, HalfWord, Word
// 		.Mode = DMA_NORMAL,				//options are Normal and Circular
// 		.Priority = DMA_PRIORITY_LOW,		//low, medium,high, very high
// 		.FIFOMode = DMA_FIFOMODE_DISABLE		//FIFO disable and enable

// 	},
// 	.State = HAL_DMA_STATE_RESET

// };

//   if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)

// #define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)               \
//                         do{                                                      \
//                               (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
//                               (__DMA_HANDLE__).Parent = (__HANDLE__);             \
//                           } while(0U)

// __HAL_LINKDMA(huart,hdmarx,dmaManager_usart1_rx);
// __HAL_LINKDMA(huart,hdmatx,dmaManager_usart1_tx);

// /* USART1 interrupt Init */
// HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
// HAL_NVIC_EnableIRQ(USART1_IRQn);

//   HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//   /* DMA2_Stream7_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
