#ifndef USART_REGS_H
#define USART_REGS_H



#include "std_types.h"

# define __IO volatile


/*uasrat REGs.h*/
/**********************
 * CR1
 * bit13	: UE	uart enable
 * bit12	: M		Word length
 * bit10	: PCE	parity control enable
 * bit9		: PS	parity selection
 * bit3		: TE	transimitter enable
 * bit2		: RE	Reciver enable
 * 
*/
typedef struct
{
  volatile uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  volatile uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  volatile uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  volatile uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  volatile uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  volatile uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  volatile uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_RegStruct;
/********/





#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region                                */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)

/*!< APB1 peripherals */
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)

/*!< APB2 peripherals */
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400UL)










/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for USART_SR register  *******************/

/** @defgroup UART_Flags   UART FLags
  *        Elements values convention: 0xXXXX
  *           - 0xXXXX  : Flag mask in the SR register
  * @{
  */
#define UART_FLAG_CTS                       ((uint32_t)USART_SR_CTS)
#define UART_FLAG_LBD                       ((uint32_t)USART_SR_LBD)
#define UART_FLAG_TXE                       ((uint32_t)USART_SR_TXE)
#define UART_FLAG_TC                        ((uint32_t)USART_SR_TC)
#define UART_FLAG_RXNE                      ((uint32_t)USART_SR_RXNE)
#define UART_FLAG_IDLE                      ((uint32_t)USART_SR_IDLE)
#define UART_FLAG_ORE                       ((uint32_t)USART_SR_ORE)
#define UART_FLAG_NE                        ((uint32_t)USART_SR_NE)
#define UART_FLAG_FE                        ((uint32_t)USART_SR_FE)
#define UART_FLAG_PE                        ((uint32_t)USART_SR_PE)




/*******************  Bit definition for USART_SR register  *******************/
#define USART_SR_PE_Pos               (0U)
#define USART_SR_PE_Msk               (0x1UL << USART_SR_PE_Pos)                /*!< 0x00000001 */
#define USART_SR_PE                   USART_SR_PE_Msk                          /*!<Parity Error                 */
#define USART_SR_FE_Pos               (1U)
#define USART_SR_FE_Msk               (0x1UL << USART_SR_FE_Pos)                /*!< 0x00000002 */
#define USART_SR_FE                   USART_SR_FE_Msk                          /*!<Framing Error                */
#define USART_SR_NE_Pos               (2U)
#define USART_SR_NE_Msk               (0x1UL << USART_SR_NE_Pos)                /*!< 0x00000004 */
#define USART_SR_NE                   USART_SR_NE_Msk                          /*!<Noise Error Flag             */
#define USART_SR_ORE_Pos              (3U)
#define USART_SR_ORE_Msk              (0x1UL << USART_SR_ORE_Pos)               /*!< 0x00000008 */
#define USART_SR_ORE                  USART_SR_ORE_Msk                         /*!<OverRun Error                */
#define USART_SR_IDLE_Pos             (4U)
#define USART_SR_IDLE_Msk             (0x1UL << USART_SR_IDLE_Pos)              /*!< 0x00000010 */
#define USART_SR_IDLE                 USART_SR_IDLE_Msk                        /*!<IDLE line detected           */
#define USART_SR_RXNE_Pos             (5U)
#define USART_SR_RXNE_Msk             (0x1UL << USART_SR_RXNE_Pos)              /*!< 0x00000020 */
#define USART_SR_RXNE                 USART_SR_RXNE_Msk                        /*!<Read Data Register Not Empty */
#define USART_SR_TC_Pos               (6U)
#define USART_SR_TC_Msk               (0x1UL << USART_SR_TC_Pos)                /*!< 0x00000040 */
#define USART_SR_TC                   USART_SR_TC_Msk                          /*!<Transmission Complete        */
#define USART_SR_TXE_Pos              (7U)
#define USART_SR_TXE_Msk              (0x1UL << USART_SR_TXE_Pos)               /*!< 0x00000080 */
#define USART_SR_TXE                  USART_SR_TXE_Msk                         /*!<Transmit Data Register Empty */
#define USART_SR_LBD_Pos              (8U)
#define USART_SR_LBD_Msk              (0x1UL << USART_SR_LBD_Pos)               /*!< 0x00000100 */
#define USART_SR_LBD                  USART_SR_LBD_Msk                         /*!<LIN Break Detection Flag     */
#define USART_SR_CTS_Pos              (9U)
#define USART_SR_CTS_Msk              (0x1UL << USART_SR_CTS_Pos)               /*!< 0x00000200 */
#define USART_SR_CTS                  USART_SR_CTS_Msk                         /*!<CTS Flag                     */


/*******************  Bit definition for USART_DR register  *******************/

#define USART_DR_DR						0x000001FF							/*!<Data value */

/******************  Bit definition for USART_BRR register  *******************/
#define USART_BRR_DIV_Fraction_Pos    (0U)
#define USART_BRR_DIV_Fraction_Msk    (0xFUL << USART_BRR_DIV_Fraction_Pos)     /*!< 0x0000000F */
#define USART_BRR_DIV_Fraction        USART_BRR_DIV_Fraction_Msk               /*!<Fraction of USARTDIV */
#define USART_BRR_DIV_Mantissa_Pos    (4U)
#define USART_BRR_DIV_Mantissa_Msk    (0xFFFUL << USART_BRR_DIV_Mantissa_Pos)   /*!< 0x0000FFF0 */
#define USART_BRR_DIV_Mantissa        USART_BRR_DIV_Mantissa_Msk               /*!<Mantissa of USARTDIV */

/******************  Bit definition for USART_CR1 register  *******************/
#define USART_CR1_SBK_Pos             (0U)
#define USART_CR1_SBK_Msk             (0x1UL << USART_CR1_SBK_Pos)              /*!< 0x00000001 */
#define USART_CR1_SBK                 USART_CR1_SBK_Msk                        /*!<Send Break                             */
#define USART_CR1_RWU_Pos             (1U)
#define USART_CR1_RWU_Msk             (0x1UL << USART_CR1_RWU_Pos)              /*!< 0x00000002 */
#define USART_CR1_RWU                 USART_CR1_RWU_Msk                        /*!<Receiver wakeup                        */
#define USART_CR1_RE_Pos              (2U)
#define USART_CR1_RE_clrMsk           (0x1UL << USART_CR1_RE_Pos)               /*!< 0x00000004 */
#define USART_CR1_RE_Msk              (0x1UL << USART_CR1_RE_Pos)               /*!< 0x00000004 */
#define USART_CR1_RE                  USART_CR1_RE_Msk                         /*!<Receiver Enable                        */
#define USART_CR1_TE_Pos              (3U)
#define USART_CR1_TE_clrMsk              (0x1UL << USART_CR1_TE_Pos)               /*!< 0x00000008 */
#define USART_CR1_TE_Msk              (0x1UL << USART_CR1_TE_Pos)               /*!< 0x00000008 */
#define USART_CR1_TE                  USART_CR1_TE_Msk                         /*!<Transmitter Enable                     */
#define USART_CR1_IDLEIE_Pos          (4U)
#define USART_CR1_IDLEIE_clrMsk          (0x1UL << USART_CR1_IDLEIE_Pos)           /*!< 0x00000010 */
#define USART_CR1_IDLEIE_Msk          (0x1UL << USART_CR1_IDLEIE_Pos)           /*!< 0x00000010 */
#define USART_CR1_IDLEIE              USART_CR1_IDLEIE_Msk                     /*!<IDLE Interrupt Enable                  */
#define USART_CR1_RXNEIE_Pos          (5U)
#define USART_CR1_RXNEIE_clrMsk          (0x1UL << USART_CR1_RXNEIE_Pos)           /*!< 0x00000020 */
#define USART_CR1_RXNEIE_Msk          (0x1UL << USART_CR1_RXNEIE_Pos)           /*!< 0x00000020 */
#define USART_CR1_RXNEIE              USART_CR1_RXNEIE_Msk                     /*!<RXNE Interrupt Enable                  */
#define USART_CR1_TCIE_Pos            (6U)
#define USART_CR1_TCIE_clrMsk         (0x1UL << USART_CR1_TCIE_Pos)             /*!< 0x00000040 */
#define USART_CR1_TCIE_Msk            (0x1UL << USART_CR1_TCIE_Pos)             /*!< 0x00000040 */
#define USART_CR1_TCIE                USART_CR1_TCIE_Msk                       /*!<Transmission Complete Interrupt Enable */
#define USART_CR1_TXEIE_Pos           (7U)
#define USART_CR1_TXEIE_clrMsk        (0x1UL << USART_CR1_TXEIE_Pos)            /*!< 0x00000080 */
#define USART_CR1_TXEIE_Msk           (0x1UL << USART_CR1_TXEIE_Pos)            /*!< 0x00000080 */
#define USART_CR1_TXEIE               USART_CR1_TXEIE_Msk                      /*!<TXE Interrupt Enable                   */
#define USART_CR1_PEIE_Pos            (8U)
#define USART_CR1_PEIE_Msk            (0x1UL << USART_CR1_PEIE_Pos)             /*!< 0x00000100 */
#define USART_CR1_PEIE_clrMsk         USART_CR1_PEIE_Msk                       /*!<PE Interrupt Enable                    */
#define USART_CR1_PEIE                USART_CR1_PEIE_Msk                       /*!<PE Interrupt Enable                    */
#define USART_CR1_PS_Pos              (9U)
#define USART_CR1_PS_clrMsk           (0x1UL << USART_CR1_PS_Pos)               /*!< 0x00000200 */
#define USART_CR1_PS_Msk              (0x1UL << USART_CR1_PS_Pos)               /*!< 0x00000200 */
#define USART_CR1_PS                  USART_CR1_PS_Msk                         /*!<Parity Selection                       */
#define USART_CR1_PCE_Pos             (10U)
#define USART_CR1_PCE_clrMsk          (0x1UL << USART_CR1_PCE_Pos)              /*!< 0x00000400 */
#define USART_CR1_PCE_Msk             (0x1UL << USART_CR1_PCE_Pos)              /*!< 0x00000400 */
#define USART_CR1_PCE                 USART_CR1_PCE_Msk                        /*!<Parity Control Enable                  */
#define USART_CR1_WAKE_Pos            (11U)
#define USART_CR1_WAKE_Msk            (0x1UL << USART_CR1_WAKE_Pos)             /*!< 0x00000800 */
#define USART_CR1_WAKE                USART_CR1_WAKE_Msk                       /*!<Wakeup method                          */
#define USART_CR1_M_Pos               (12U)
#define USART_CR1_M_clrMsk            (0x1UL << USART_CR1_M_Pos)                /*!< 0x00001000 */
#define USART_CR1_M_Msk               (0x1UL << USART_CR1_M_Pos)                /*!< 0x00001000 */
#define USART_CR1_M                   USART_CR1_M_Msk                          /*!<Word length                            */
#define USART_CR1_UE_Pos              (13U)
#define USART_CR1_UE_clrMsk           (0x1UL << USART_CR1_UE_Pos)               /*!< 0x00002000 */
#define USART_CR1_UE_Msk              (0x1UL << USART_CR1_UE_Pos)               /*!< 0x00002000 */
#define USART_CR1_UE                  USART_CR1_UE_Msk                         /*!<USART Enable                           */
#define USART_CR1_OVER8_Pos           (15U)
#define USART_CR1_OVER8_Msk           (0x1UL << USART_CR1_OVER8_Pos)            /*!< 0x00008000 */
#define USART_CR1_OVER8               USART_CR1_OVER8_Msk                      /*!<USART Oversampling by 8 enable         */

/******************  Bit definition for USART_CR2 register  *******************/
#define USART_CR2_ADD_Pos             (0U)
#define USART_CR2_ADD_Msk             (0xFUL << USART_CR2_ADD_Pos)              /*!< 0x0000000F */
#define USART_CR2_ADD                 USART_CR2_ADD_Msk                        /*!<Address of the USART node            */
#define USART_CR2_LBDL_Pos            (5U)
#define USART_CR2_LBDL_Msk            (0x1UL << USART_CR2_LBDL_Pos)             /*!< 0x00000020 */
#define USART_CR2_LBDL                USART_CR2_LBDL_Msk                       /*!<LIN Break Detection Length           */
#define USART_CR2_LBDIE_Pos           (6U)
#define USART_CR2_LBDIE_Msk           (0x1UL << USART_CR2_LBDIE_Pos)            /*!< 0x00000040 */
#define USART_CR2_LBDIE               USART_CR2_LBDIE_Msk                      /*!<LIN Break Detection Interrupt Enable */
#define USART_CR2_LBCL_Pos            (8U)
#define USART_CR2_LBCL_Msk            (0x1UL << USART_CR2_LBCL_Pos)             /*!< 0x00000100 */
#define USART_CR2_LBCL                USART_CR2_LBCL_Msk                       /*!<Last Bit Clock pulse                 */
#define USART_CR2_CPHA_Pos            (9U)
#define USART_CR2_CPHA_Msk            (0x1UL << USART_CR2_CPHA_Pos)             /*!< 0x00000200 */
#define USART_CR2_CPHA                USART_CR2_CPHA_Msk                       /*!<Clock Phase                          */
#define USART_CR2_CPOL_Pos            (10U)
#define USART_CR2_CPOL_Msk            (0x1UL << USART_CR2_CPOL_Pos)             /*!< 0x00000400 */
#define USART_CR2_CPOL                USART_CR2_CPOL_Msk                       /*!<Clock Polarity                       */
#define USART_CR2_CLKEN_Pos           (11U)
#define USART_CR2_CLKEN_Msk           (0x1UL << USART_CR2_CLKEN_Pos)            /*!< 0x00000800 */
#define USART_CR2_CLKEN               USART_CR2_CLKEN_Msk                      /*!<Clock Enable                         */

#define USART_CR2_STOP_Pos            (12U)
#define USART_CR2_STOP_Msk            (0x3UL << USART_CR2_STOP_Pos)             /*!< 0x00003000 */
#define USART_CR2_STOP_clrMsk         USART_CR2_STOP_Msk                       /*!<STOP[1:0] bits (STOP bits) */
#define USART_CR2_STOP                USART_CR2_STOP_Msk                       /*!<STOP[1:0] bits (STOP bits) */
#define USART_CR2_STOP_0              (0x1UL << USART_CR2_STOP_Pos)             /*!< 0x1000 */
#define USART_CR2_STOP_1              (0x2UL << USART_CR2_STOP_Pos)             /*!< 0x2000 */

#define USART_CR2_LINEN_Pos           (14U)
#define USART_CR2_LINEN_Msk           (0x1UL << USART_CR2_LINEN_Pos)            /*!< 0x00004000 */
#define USART_CR2_LINEN               USART_CR2_LINEN_Msk                      /*!<LIN mode enable */

/******************  Bit definition for USART_CR3 register  *******************/
#define USART_CR3_EIE_Pos             (0U)
#define USART_CR3_EIE_Msk             (0x1UL << USART_CR3_EIE_Pos)              /*!< 0x00000001 */
#define USART_CR3_EIE_clrMsk          USART_CR3_EIE_Msk                        /*!<Error Interrupt Enable      */
#define USART_CR3_EIE                 USART_CR3_EIE_Msk                        /*!<Error Interrupt Enable      */
#define USART_CR3_IREN_Pos            (1U)
#define USART_CR3_IREN_Msk            (0x1UL << USART_CR3_IREN_Pos)             /*!< 0x00000002 */
#define USART_CR3_IREN                USART_CR3_IREN_Msk                       /*!<IrDA mode Enable            */
#define USART_CR3_IRLP_Pos            (2U)
#define USART_CR3_IRLP_Msk            (0x1UL << USART_CR3_IRLP_Pos)             /*!< 0x00000004 */
#define USART_CR3_IRLP                USART_CR3_IRLP_Msk                       /*!<IrDA Low-Power              */
#define USART_CR3_HDSEL_Pos           (3U)
#define USART_CR3_HDSEL_Msk           (0x1UL << USART_CR3_HDSEL_Pos)            /*!< 0x00000008 */
#define USART_CR3_HDSEL               USART_CR3_HDSEL_Msk                      /*!<Half-Duplex Selection       */
#define USART_CR3_NACK_Pos            (4U)
#define USART_CR3_NACK_Msk            (0x1UL << USART_CR3_NACK_Pos)             /*!< 0x00000010 */
#define USART_CR3_NACK                USART_CR3_NACK_Msk                       /*!<Smartcard NACK enable       */
#define USART_CR3_SCEN_Pos            (5U)
#define USART_CR3_SCEN_Msk            (0x1UL << USART_CR3_SCEN_Pos)             /*!< 0x00000020 */
#define USART_CR3_SCEN                USART_CR3_SCEN_Msk                       /*!<Smartcard mode enable       */
#define USART_CR3_DMAR_Pos            (6U)
#define USART_CR3_DMAR_Msk            (0x1UL << USART_CR3_DMAR_Pos)             /*!< 0x00000040 */
#define USART_CR3_DMAR                USART_CR3_DMAR_Msk                       /*!<DMA Enable Receiver         */
#define USART_CR3_DMAT_Pos            (7U)
#define USART_CR3_DMAT_Msk            (0x1UL << USART_CR3_DMAT_Pos)             /*!< 0x00000080 */
#define USART_CR3_DMAT                USART_CR3_DMAT_Msk                       /*!<DMA Enable Transmitter      */
#define USART_CR3_RTSE_Pos            (8U)
#define USART_CR3_RTSE_Msk            (0x1UL << USART_CR3_RTSE_Pos)             /*!< 0x00000100 */
#define USART_CR3_RTSE                USART_CR3_RTSE_Msk                       /*!<RTS Enable                  */
#define USART_CR3_CTSE_Pos            (9U)
#define USART_CR3_CTSE_Msk            (0x1UL << USART_CR3_CTSE_Pos)             /*!< 0x00000200 */
#define USART_CR3_CTSE                USART_CR3_CTSE_Msk                       /*!<CTS Enable                  */
#define USART_CR3_CTSIE_Pos           (10U)
#define USART_CR3_CTSIE_Msk           (0x1UL << USART_CR3_CTSIE_Pos)            /*!< 0x00000400 */
#define USART_CR3_CTSIE               USART_CR3_CTSIE_Msk                      /*!<CTS Interrupt Enable        */
#define USART_CR3_ONEBIT_Pos          (11U)
#define USART_CR3_ONEBIT_Msk          (0x1UL << USART_CR3_ONEBIT_Pos)           /*!< 0x00000800 */
#define USART_CR3_ONEBIT              USART_CR3_ONEBIT_Msk                     /*!<USART One bit method enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define USART_GTPR_PSC_Pos            (0U)
#define USART_GTPR_PSC_Msk            (0xFFUL << USART_GTPR_PSC_Pos)            /*!< 0x000000FF */
#define USART_GTPR_PSC                USART_GTPR_PSC_Msk                       /*!<PSC[7:0] bits (Prescaler value) */
#define USART_GTPR_PSC_0              (0x01UL << USART_GTPR_PSC_Pos)            /*!< 0x0001 */
#define USART_GTPR_PSC_1              (0x02UL << USART_GTPR_PSC_Pos)            /*!< 0x0002 */
#define USART_GTPR_PSC_2              (0x04UL << USART_GTPR_PSC_Pos)            /*!< 0x0004 */
#define USART_GTPR_PSC_3              (0x08UL << USART_GTPR_PSC_Pos)            /*!< 0x0008 */
#define USART_GTPR_PSC_4              (0x10UL << USART_GTPR_PSC_Pos)            /*!< 0x0010 */
#define USART_GTPR_PSC_5              (0x20UL << USART_GTPR_PSC_Pos)            /*!< 0x0020 */
#define USART_GTPR_PSC_6              (0x40UL << USART_GTPR_PSC_Pos)            /*!< 0x0040 */
#define USART_GTPR_PSC_7              (0x80UL << USART_GTPR_PSC_Pos)            /*!< 0x0080 */

#define USART_GTPR_GT_Pos             (8U)
#define USART_GTPR_GT_Msk             (0xFFUL << USART_GTPR_GT_Pos)             /*!< 0x0000FF00 */
#define USART_GTPR_GT                 USART_GTPR_GT_Msk                        /*!<Guard time value */

/**************************************************************************************************************************/

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            ((uint32_t)((((uint64_t)(_PCLK_))*25U)/(4U*((uint64_t)(_BAUD_)))))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U)\
                                                         + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            ((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U) + (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))


#endif //USART_REGS_H