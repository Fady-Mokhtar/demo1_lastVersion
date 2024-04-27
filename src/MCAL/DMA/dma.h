#ifndef DMA_H
#define DMA_H

/**
 * options to configure
 * * priority
 * * stream   			//(1 or 2)based upon data sheet
 * * DMA Request 		// this mean Usart1_Rx, Usart1_Tx, SPI, I2C, .. and so on..
 *
 *
 *
 * then you can do the DMA request setting
 * 		mode:  normal or Circular
 *
 * 		increment Adress: True or False   (False by default in USART because there only one data register)
 * 					**VIP**	for: memory (OR) Prepheral //if possible for prephiral too  and if wanted  you disable both if you dont want increment. propably you need the memory.
 *
 * 		Data Width: Byte, Half-word, Or Word(32bit)
 * 					**VIP**	for: memory and Prepheral (if FIFo not enabled  then memory has **ONLY** the byte Option)
 *
 *
 *
 *
 * 		use FIFO or not if yes:
 * 			thereshold: One Quarter FUll, HalfFull, ThreeQuartersFull, Full.
 *
 * 			brust size: Single, 4_Increment, 8_Increment, 16_Increment.
 * 					**VIP**	for: memory and Prepheral
 *
 *
 */

#include "STD_TYPES.h"
#include "Common_Macros.h"
#include "Errors.h"
#include "dma_regs.h"

/** @defgroup DMA_Exported_Types DMA Exported Types
 * @brief    DMA Exported Types
 * @{
 */

/**
 * @brief  DMA Configuration Structure definition
 */
typedef struct
{
  uint32_t Channel; /*!< Specifies the channel used for the specified stream.
                         This parameter can be a value of @ref DMA_Channel_selection                    */

  uint32_t Direction; /*!< Specifies if the data will be transferred from memory to peripheral,
                           from memory to memory or from peripheral to memory.
                           This parameter can be a value of @ref DMA_Data_transfer_direction              */

  uint32_t PeriphInc; /*!< Specifies whether the Peripheral address register should be incremented or not.
                           This parameter can be a value of @ref DMA_Peripheral_incremented_mode          */

  uint32_t MemInc; /*!< Specifies whether the memory address register should be incremented or not.
                        This parameter can be a value of @ref DMA_Memory_incremented_mode              */

  uint32_t PeriphDataAlignment; /*!< Specifies the Peripheral data width.
                                     This parameter can be a value of @ref DMA_Peripheral_data_size                 */

  uint32_t MemDataAlignment; /*!< Specifies the Memory data width.
                                  This parameter can be a value of @ref DMA_Memory_data_size                     */

  uint32_t Mode; /*!< Specifies the operation mode of the DMAy Streamx.
                      This parameter can be a value of @ref DMA_mode
                      @note The circular buffer mode cannot be used if the memory-to-memory
                            data transfer is configured on the selected Stream                        */

  uint32_t Priority; /*!< Specifies the software priority for the DMAy Streamx.
                          This parameter can be a value of @ref DMA_Priority_level                       */

  uint32_t FIFOMode; /*!< Specifies if the FIFO mode or Direct mode will be used for the specified stream.
                          This parameter can be a value of @ref DMA_FIFO_direct_mode
                          @note The Direct mode (FIFO mode disabled) cannot be used if the
                                memory-to-memory data transfer is configured on the selected stream       */

  uint32_t FIFOThreshold; /*!< Specifies the FIFO threshold level.
                               This parameter can be a value of @ref DMA_FIFO_threshold_level                  */

  uint32_t MemBurst; /*!< Specifies the Burst transfer configuration for the memory transfers.
                          It specifies the amount of data to be transferred in a single non interruptible
                          transaction.
                          This parameter can be a value of @ref DMA_Memory_burst
                          @note The burst mode is possible only if the address Increment mode is enabled. */

  uint32_t PeriphBurst; /*!< Specifies the Burst transfer configuration for the peripheral transfers.
                             It specifies the amount of data to be transferred in a single non interruptible
                             transaction.
                             This parameter can be a value of @ref DMA_Peripheral_burst
                             @note The burst mode is possible only if the address Increment mode is enabled. */
} DMA_InitTypeDef;

typedef enum
{
  HAL_DMA_STATE_RESET = 0x00U,   /*!< DMA not yet initialized or disabled */
  HAL_DMA_STATE_READY = 0x01U,   /*!< DMA initialized and ready for use   */
  HAL_DMA_STATE_BUSY = 0x02U,    /*!< DMA process is ongoing              */
  HAL_DMA_STATE_TIMEOUT = 0x03U, /*!< DMA timeout state                   */
  HAL_DMA_STATE_ERROR = 0x04U,   /*!< DMA error state                     */
  HAL_DMA_STATE_ABORT = 0x05U,   /*!< DMA Abort state                     */
} HAL_DMA_StateTypeDef;

////////////////////////////////////
typedef struct __DMA_HandleTypeDef
{

  DMA_Stream_TypeDef *Instance; /*!< Register base address                  */

  DMA_InitTypeDef Init; /*!< DMA communication parameters           */

  //    HAL_LockTypeDef            Lock;                                                             /*!< DMA locking object                     */

  __IO HAL_DMA_StateTypeDef State; /*!< DMA transfer state                     */

  void *Parent; /*!< Parent object state                    */

  //   void                       (* XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);         /*!< DMA transfer complete callback         */

  //   void                       (* XferHalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);     /*!< DMA Half transfer complete callback    */

  //   void                       (* XferM1CpltCallback)( struct __DMA_HandleTypeDef * hdma);       /*!< DMA transfer complete Memory1 callback */

  //   void                       (* XferM1HalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);   /*!< DMA transfer Half complete Memory1 callback */

  //   void                       (* XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer error callback            */

  //   void                       (* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer Abort callback            */

  __IO uint32_t ErrorCode; /*!< DMA Error code                          */

  uint32_t StreamBaseAddress; /*!< DMA Stream Base Address                */

  uint32_t StreamIndex; /*!< DMA Stream Index                       */

} DMA_HandleTypeDef;

#define PERIPH_BASE 0x40000000UL /*!< Peripheral base address in the alias region                                */
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)

#define DMA1_BASE (AHB1PERIPH_BASE + 0x6000UL)
#define DMA1_Stream0_BASE (DMA1_BASE + 0x010UL)
#define DMA1_Stream1_BASE (DMA1_BASE + 0x028UL)
#define DMA1_Stream2_BASE (DMA1_BASE + 0x040UL)
#define DMA1_Stream3_BASE (DMA1_BASE + 0x058UL)
#define DMA1_Stream4_BASE (DMA1_BASE + 0x070UL)
#define DMA1_Stream5_BASE (DMA1_BASE + 0x088UL)
#define DMA1_Stream6_BASE (DMA1_BASE + 0x0A0UL)
#define DMA1_Stream7_BASE (DMA1_BASE + 0x0B8UL)
#define DMA2_BASE (AHB1PERIPH_BASE + 0x6400UL)
#define DMA2_Stream0_BASE (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE (DMA2_BASE + 0x0B8UL)

#define DMA1 ((DMA_RegStruct *)DMA1_BASE)
#define DMA1_Stream0 ((DMA_Stream_TypeDef *)DMA1_Stream0_BASE)
#define DMA1_Stream1 ((DMA_Stream_TypeDef *)DMA1_Stream1_BASE)
#define DMA1_Stream2 ((DMA_Stream_TypeDef *)DMA1_Stream2_BASE)
#define DMA1_Stream3 ((DMA_Stream_TypeDef *)DMA1_Stream3_BASE)
#define DMA1_Stream4 ((DMA_Stream_TypeDef *)DMA1_Stream4_BASE)
#define DMA1_Stream5 ((DMA_Stream_TypeDef *)DMA1_Stream5_BASE)
#define DMA1_Stream6 ((DMA_Stream_TypeDef *)DMA1_Stream6_BASE)
#define DMA1_Stream7 ((DMA_Stream_TypeDef *)DMA1_Stream7_BASE)
#define DMA2 ((DMA_RegStruct *)DMA2_BASE)
#define DMA2_Stream0 ((DMA_Stream_TypeDef *)DMA2_Stream0_BASE)
#define DMA2_Stream1 ((DMA_Stream_TypeDef *)DMA2_Stream1_BASE)
#define DMA2_Stream2 ((DMA_Stream_TypeDef *)DMA2_Stream2_BASE)
#define DMA2_Stream3 ((DMA_Stream_TypeDef *)DMA2_Stream3_BASE)
#define DMA2_Stream4 ((DMA_Stream_TypeDef *)DMA2_Stream4_BASE)
#define DMA2_Stream5 ((DMA_Stream_TypeDef *)DMA2_Stream5_BASE)
#define DMA2_Stream6 ((DMA_Stream_TypeDef *)DMA2_Stream6_BASE)
#define DMA2_Stream7 ((DMA_Stream_TypeDef *)DMA2_Stream7_BASE)

/** @defgroup DMA_Error_Code DMA Error Code
 * @brief    DMA Error Code
 * @{
 */
#define HAL_DMA_ERROR_NONE 0x00000000U          /*!< No error                               */
#define HAL_DMA_ERROR_TE 0x00000001U            /*!< Transfer error                         */
#define HAL_DMA_ERROR_FE 0x00000002U            /*!< FIFO error                             */
#define HAL_DMA_ERROR_DME 0x00000004U           /*!< Direct Mode error                      */
#define HAL_DMA_ERROR_TIMEOUT 0x00000020U       /*!< Timeout error                          */
#define HAL_DMA_ERROR_PARAM 0x00000040U         /*!< Parameter error                        */
#define HAL_DMA_ERROR_NO_XFER 0x00000080U       /*!< Abort requested with no Xfer ongoing   */
#define HAL_DMA_ERROR_NOT_SUPPORTED 0x00000100U /*!< Not supported mode                     */

/** @defgroup DMA_Channel_selection DMA Channel selection
 * @brief    DMA channel selection
 * @{
 */
#define DMA_CHANNEL_0 0x00000000U /*!< DMA Channel 0 */
#define DMA_CHANNEL_1 0x02000000U /*!< DMA Channel 1 */
#define DMA_CHANNEL_2 0x04000000U /*!< DMA Channel 2 */
#define DMA_CHANNEL_3 0x06000000U /*!< DMA Channel 3 */
#define DMA_CHANNEL_4 0x08000000U /*!< DMA Channel 4 */
#define DMA_CHANNEL_5 0x0A000000U /*!< DMA Channel 5 */
#define DMA_CHANNEL_6 0x0C000000U /*!< DMA Channel 6 */
#define DMA_CHANNEL_7 0x0E000000U /*!< DMA Channel 7 */
#if defined(DMA_SxCR_CHSEL_3)
#define DMA_CHANNEL_8 0x10000000U  /*!< DMA Channel 8 */
#define DMA_CHANNEL_9 0x12000000U  /*!< DMA Channel 9 */
#define DMA_CHANNEL_10 0x14000000U /*!< DMA Channel 10 */
#define DMA_CHANNEL_11 0x16000000U /*!< DMA Channel 11 */
#define DMA_CHANNEL_12 0x18000000U /*!< DMA Channel 12 */
#define DMA_CHANNEL_13 0x1A000000U /*!< DMA Channel 13 */
#define DMA_CHANNEL_14 0x1C000000U /*!< DMA Channel 14 */
#define DMA_CHANNEL_15 0x1E000000U /*!< DMA Channel 15 */
#endif                             /* DMA_SxCR_CHSEL_3 */

/** @defgroup DMA_Data_transfer_direction DMA Data transfer direction
 * @brief    DMA data transfer direction
 * @{
 */
#define DMA_PERIPH_TO_MEMORY 0x00000000U                /*!< Peripheral to memory direction */
#define DMA_MEMORY_TO_PERIPH ((uint32_t)DMA_SxCR_DIR_0) /*!< Memory to peripheral direction */
#define DMA_MEMORY_TO_MEMORY ((uint32_t)DMA_SxCR_DIR_1) /*!< Memory to memory direction     */
/**
 * @}
 */

/** @defgroup DMA_Peripheral_incremented_mode DMA Peripheral incremented mode
 * @brief    DMA peripheral incremented mode
 * @{
 */
#define DMA_PINC_ENABLE ((uint32_t)DMA_SxCR_PINC) /*!< Peripheral increment mode enable  */
#define DMA_PINC_DISABLE 0x00000000U              /*!< Peripheral increment mode disable */
/**
 * @}
 */

/** @defgroup DMA_Memory_incremented_mode DMA Memory incremented mode
 * @brief    DMA memory incremented mode
 * @{
 */
#define DMA_MINC_ENABLE ((uint32_t)DMA_SxCR_MINC) /*!< Memory increment mode enable  */
#define DMA_MINC_DISABLE 0x00000000U              /*!< Memory increment mode disable */
/**
 * @}
 */

/** @defgroup DMA_Peripheral_data_size DMA Peripheral data size
 * @brief    DMA peripheral data size
 * @{
 */
#define DMA_PDATAALIGN_BYTE 0x00000000U                      /*!< Peripheral data alignment: Byte     */
#define DMA_PDATAALIGN_HALFWORD ((uint32_t)DMA_SxCR_PSIZE_0) /*!< Peripheral data alignment: HalfWord */
#define DMA_PDATAALIGN_WORD ((uint32_t)DMA_SxCR_PSIZE_1)     /*!< Peripheral data alignment: Word     */
/**
 * @}
 */

/** @defgroup DMA_Memory_data_size DMA Memory data size
 * @brief    DMA memory data size
 * @{
 */
#define DMA_MDATAALIGN_BYTE 0x00000000U                      /*!< Memory data alignment: Byte     */
#define DMA_MDATAALIGN_HALFWORD ((uint32_t)DMA_SxCR_MSIZE_0) /*!< Memory data alignment: HalfWord */
#define DMA_MDATAALIGN_WORD ((uint32_t)DMA_SxCR_MSIZE_1)     /*!< Memory data alignment: Word     */
/**
 * @}
 */

/** @defgroup DMA_mode DMA mode
 * @brief    DMA mode
 * @{
 */
#define DMA_NORMAL 0x00000000U                 /*!< Normal mode                  */
#define DMA_CIRCULAR ((uint32_t)DMA_SxCR_CIRC) /*!< Circular mode                */
#define DMA_PFCTRL ((uint32_t)DMA_SxCR_PFCTRL) /*!< Peripheral flow control mode */
/**
 * @}
 */

/** @defgroup DMA_Priority_level DMA Priority level
 * @brief    DMA priority levels
 * @{
 */
#define DMA_PRIORITY_LOW 0x00000000U                   /*!< Priority level: Low       */
#define DMA_PRIORITY_MEDIUM ((uint32_t)DMA_SxCR_PL_0)  /*!< Priority level: Medium    */
#define DMA_PRIORITY_HIGH ((uint32_t)DMA_SxCR_PL_1)    /*!< Priority level: High      */
#define DMA_PRIORITY_VERY_HIGH ((uint32_t)DMA_SxCR_PL) /*!< Priority level: Very High */
/**
 * @}
 */

/** @defgroup DMA_FIFO_direct_mode DMA FIFO direct mode
 * @brief    DMA FIFO direct mode
 * @{
 */
#define DMA_FIFOMODE_DISABLE 0x00000000U                /*!< FIFO mode disable */
#define DMA_FIFOMODE_ENABLE ((uint32_t)DMA_SxFCR_DMDIS) /*!< FIFO mode enable  */
/**
 * @}
 */

/** @defgroup DMA_FIFO_threshold_level DMA FIFO threshold level
 * @brief    DMA FIFO level
 * @{
 */
#define DMA_FIFO_THRESHOLD_1QUARTERFULL 0x00000000U                  /*!< FIFO threshold 1 quart full configuration  */
#define DMA_FIFO_THRESHOLD_HALFFULL ((uint32_t)DMA_SxFCR_FTH_0)      /*!< FIFO threshold half full configuration     */
#define DMA_FIFO_THRESHOLD_3QUARTERSFULL ((uint32_t)DMA_SxFCR_FTH_1) /*!< FIFO threshold 3 quarts full configuration */
#define DMA_FIFO_THRESHOLD_FULL ((uint32_t)DMA_SxFCR_FTH)            /*!< FIFO threshold full configuration          */
/**
 * @}
 */

/** @defgroup DMA_Memory_burst DMA Memory burst
 * @brief    DMA memory burst
 * @{
 */
#define DMA_MBURST_SINGLE 0x00000000U
#define DMA_MBURST_INC4 ((uint32_t)DMA_SxCR_MBURST_0)
#define DMA_MBURST_INC8 ((uint32_t)DMA_SxCR_MBURST_1)
#define DMA_MBURST_INC16 ((uint32_t)DMA_SxCR_MBURST)
/**
 * @}
 */

/** @defgroup DMA_Peripheral_burst DMA Peripheral burst
 * @brief    DMA peripheral burst
 * @{
 */
#define DMA_PBURST_SINGLE 0x00000000U
#define DMA_PBURST_INC4 ((uint32_t)DMA_SxCR_PBURST_0)
#define DMA_PBURST_INC8 ((uint32_t)DMA_SxCR_PBURST_1)
#define DMA_PBURST_INC16 ((uint32_t)DMA_SxCR_PBURST)
/**
 * @}
 */

/** @defgroup DMA_interrupt_enable_definitions DMA interrupt enable definitions
 * @brief    DMA interrupts definition
 * @{
 */
#define DMA_IT_TC ((uint32_t)DMA_SxCR_TCIE)
#define DMA_IT_HT ((uint32_t)DMA_SxCR_HTIE)
#define DMA_IT_TE ((uint32_t)DMA_SxCR_TEIE)
#define DMA_IT_DME ((uint32_t)DMA_SxCR_DMEIE)
#define DMA_IT_FE 0x00000080U
/**
 * @}
 */

/** @defgroup DMA_flag_definitions DMA flag definitions
 * @brief    DMA flag definitions
 * @{
 */
#define DMA_FLAG_FIFO_ERROR 0x00000001U
#define DMA_FLAG_DIRECT_MODE_ERROR 0x00000004U
#define DMA_FLAG_TRANSFER_ERROR 0x00000008U
#define DMA_FLAG_HALF_TRANSFER 0x00000010U
#define DMA_FLAG_TRANSFER_COMPLETE 0x00000020U

#define DMA_FLAG_FEIF 0x00000001U
#define DMA_FLAG_DMEIF 0x00000004U
#define DMA_FLAG_TEIF 0x00000008U
#define DMA_FLAG_HTIF 0x00000010U
#define DMA_FLAG_TCIF 0x00000020U

#define DMA_FLAG_FEIF0_4 0x00000001U
#define DMA_FLAG_DMEIF0_4 0x00000004U
#define DMA_FLAG_TEIF0_4 0x00000008U
#define DMA_FLAG_HTIF0_4 0x00000010U
#define DMA_FLAG_TCIF0_4 0x00000020U
#define DMA_FLAG_FEIF1_5 0x00000040U
#define DMA_FLAG_DMEIF1_5 0x00000100U
#define DMA_FLAG_TEIF1_5 0x00000200U
#define DMA_FLAG_HTIF1_5 0x00000400U
#define DMA_FLAG_TCIF1_5 0x00000800U
#define DMA_FLAG_FEIF2_6 0x00010000U
#define DMA_FLAG_DMEIF2_6 0x00040000U
#define DMA_FLAG_TEIF2_6 0x00080000U
#define DMA_FLAG_HTIF2_6 0x00100000U
#define DMA_FLAG_TCIF2_6 0x00200000U
#define DMA_FLAG_FEIF3_7 0x00400000U
#define DMA_FLAG_DMEIF3_7 0x01000000U
#define DMA_FLAG_TEIF3_7 0x02000000U
#define DMA_FLAG_HTIF3_7 0x04000000U
#define DMA_FLAG_TCIF3_7 0x08000000U

typedef enum
{
  HAL_DMA_FULL_TRANSFER = 0x00U, /*!< Full transfer     */
  HAL_DMA_HALF_TRANSFER = 0x01U  /*!< Half Transfer     */
} HAL_DMA_LevelCompleteTypeDef;

void DMA_Init(DMA_HandleTypeDef *streamManager);

void DMA_start(DMA_HandleTypeDef *streamManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
MCALStatus_t DMA_PollForTransfer(DMA_HandleTypeDef *hdma, HAL_DMA_LevelCompleteTypeDef CompleteLevel);

void DMA_Start_IT(DMA_HandleTypeDef *streamManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
void MCAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
MCALStatus_t DMA_Abort_IT(DMA_HandleTypeDef *hdma);

#endif /*DMA_H*/