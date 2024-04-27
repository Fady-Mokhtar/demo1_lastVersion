/*===============================================================================================================
*
* @date 10/2/2024
*
* Author: Fady
*
* Layer: MCAL
*
* Module: RCC
*
* File Name: RCC_cfg.h
*
*===============================================================================================================*/

/* Include Libraries  */

/* Driver Libraries   */
#include "RCC.h"

/*==============================================================================================================*/

/*==============================================================================================================*/

/* Register Definitions (1st method using macros) */
/*
#define RCC_CR            *((volatile uint32_t*)0x40023800)
#define RCC_PLLCFGR       *((volatile uint32_t*)0x40023804)
#define RCC_CFGR          *((volatile uint32_t*)0x40023804)
#define RCC_CIR           *((volatile uint32_t*)0x40023808)
   //     and so on
*/
/*==============================================================================================================*/

/* Register Definitions (2nd method using struct) */

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t RESERVED1[2];;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED2[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t RESERVED3[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t RESERVED4[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t RESERVED5[2];
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t RESERVED6[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t RESERVED7[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t RESERVED8;
    volatile uint32_t DCKCFGR;
}MRCC;

#define RCC_BASE_ADDR   (0x40023800UL)
#define RCC     ((volatile MRCC* const)(RCC_BASE_ADDR))

/*==============================================================================================================*/
/*                                                    Defines                                                   */
/*==============================================================================================================*/
#define RCC_CFGR_SW_MASK  ((uint32_t)0x3 << 0)
#define RCC_CFGR_SWS_MASK ((uint32_t)0x3 << 2)

#define RCC_PLLCFGR_PLLSRC_OFFSET (22)
#define RCC_PLLCFGR_PLLSRC_MASK ((uint32_t)1 << RCC_PLLCFGR_PLLSRC_OFFSET)

#define RCC_PLLCFGR_PLLM_OFFSET (0)
#define RCC_PLLCFGR_PLLM_MASK ((uint32_t)0x3F << RCC_PLLCFGR_PLLM_OFFSET)

#define RCC_PLLCFGR_PLLN_OFFSET (6)
#define RCC_PLLCFGR_PLLN_MASK ((uint32_t)0x1FF << RCC_PLLCFGR_PLLN_OFFSET)

#define RCC_PLLCFGR_PLLP_OFFSET (16)
#define RCC_PLLCFGR_PLLP_MASK ((uint32_t)0x3 << RCC_PLLCFGR_PLLP_OFFSET)

#define RCC_PLLCFGR_PLLQ_OFFSET (24)
#define RCC_PLLCFGR_PLLQ_MASK ((uint32_t)0xF << RCC_PLLCFGR_PLLQ_OFFSET)

#define RCC_CFGR_HPRE_OFFSET (4)
#define RCC_CFGR_HPRE_MASK ((uint32_t)0xF << RCC_CFGR_HPRE_OFFSET)

#define RCC_CFGR_PPRE1_OFFSET (10)
#define RCC_CFGR_PPRE1_MASK ((uint32_t)0x7 << RCC_CFGR_PPRE1_OFFSET)

#define RCC_CFGR_PPRE2_OFFSET (13)
#define RCC_CFGR_PPRE2_MASK ((uint32_t)0x7 << RCC_CFGR_PPRE2_OFFSET)

#define RCC_TIMEOUT_VAL     (600)




/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

MCALStatus_t RCC_enableClock(RCC_SystemClock_t Clock)
{
    /* Enable the clock*/
    RCC->CR |= Clock;
    uint32_t clockStatusMask = (Clock << 1);

    uint32_t timeout = RCC_TIMEOUT_VAL;
    while(!(RCC->CR & clockStatusMask) && timeout){timeout--;}

    return (RCC->CR & clockStatusMask)? MCAL_OK : MCAL_ERROR;
}

MCALStatus_t RCC_disableClock(RCC_SystemClock_t Clock)
{
    RCC->CR &= ~Clock;
    return MCAL_OK;
}


MCALStatus_t RCC_selectSystemClock(RCC_SystemClock_t SystemClock)
{
    uint32_t CFGRtmp = RCC->CFGR;
    CFGRtmp = (CFGRtmp & ~RCC_CFGR_SW_MASK) | (uint32_t)SystemClock;

    RCC->CFGR = CFGRtmp;
    return MCAL_OK;
}


MCALStatus_t RCC_getSystemClock(RCC_SystemClock_t *SystemClock)
{
    *SystemClock = (((RCC->CFGR) & RCC_CFGR_SWS_MASK) >> 2);

    return MCAL_OK;
}


MCALStatus_t RCC_configurePLLClock(RCC_PLLConfig_t *PLLConfig)
{
    uint32_t PLLCFGRtmp = RCC->PLLCFGR;

    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLSRC_MASK) | (uint32_t)PLLConfig->PLLSrc;

    /* Trim the prescaler value using its mask */
    uint32_t PLLM_value = ((uint32_t)PLLConfig->PLLM & (RCC_PLLCFGR_PLLM_MASK >> RCC_PLLCFGR_PLLM_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLM_MASK) | (PLLM_value << RCC_PLLCFGR_PLLM_OFFSET);

    uint32_t PLLN_value = ((uint32_t)PLLConfig->PLLN & (RCC_PLLCFGR_PLLN_MASK >> RCC_PLLCFGR_PLLN_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLN_MASK) | (PLLN_value << RCC_PLLCFGR_PLLN_OFFSET);

    uint32_t PLLP_value = ((uint32_t)PLLConfig->PLLP & (RCC_PLLCFGR_PLLP_MASK >> RCC_PLLCFGR_PLLP_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLP_MASK) | (PLLP_value << RCC_PLLCFGR_PLLP_OFFSET);

    uint32_t PLLQ_value = ((uint32_t)PLLConfig->PLLQ & (RCC_PLLCFGR_PLLQ_MASK >> RCC_PLLCFGR_PLLQ_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLQ_MASK) | (PLLQ_value << RCC_PLLCFGR_PLLQ_OFFSET);   

    RCC->PLLCFGR = PLLCFGRtmp;

    return MCAL_OK;
}


MCALStatus_t RCC_enableAHB1Peripheral(RCC_AHB1Peri_t AHB1Peripheral)
{
    RCC->AHB1ENR |= AHB1Peripheral;
    return MCAL_OK;
}


MCALStatus_t RCC_enableAHB2Peripheral(RCC_AHB2Peri_t AHB2Peripheral)
{
    RCC->AHB2ENR |= AHB2Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_enableAPB1Peripheral(RCC_APB1Peri_t APB1Peripheral)
{
    RCC->APB1ENR |= APB1Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_enableAPB2Peripheral(RCC_APB2Peri_t APB2Peripheral)
{
    RCC->APB2ENR |= APB2Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_disableAHB1Peripheral(RCC_AHB1Peri_t AHB1Peripheral)
{
    RCC->AHB1ENR &= ~AHB1Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_disableAHB2Peripheral(RCC_AHB2Peri_t AHB2Peripheral)
{
    RCC->AHB2ENR &= ~AHB2Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_disableAPB1Peripheral(RCC_APB1Peri_t APB1Peripheral)
{
    RCC->APB1ENR &= ~APB1Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_disableAPB2Peripheral(RCC_APB2Peri_t APB2Peripheral)
{
    RCC->APB2ENR &= ~APB2Peripheral;
    return MCAL_OK;
}

MCALStatus_t RCC_selectSystemClockPrescalers(RCC_AHB1Prescaler_t AHB1Prescaler, RCC_APB1Prescaler_t APB1Prescaler, RCC_APB2Prescaler_t APB2Prescaler)
{
    uint32_t CFGRtmp = RCC->CFGR;

    CFGRtmp = (CFGRtmp & ~RCC_CFGR_HPRE_MASK)  | AHB1Prescaler;
    CFGRtmp = (CFGRtmp & ~RCC_CFGR_PPRE1_MASK) | APB1Prescaler;
    CFGRtmp = (CFGRtmp & ~RCC_CFGR_PPRE2_MASK) | APB2Prescaler;

    RCC->CFGR = CFGRtmp;

    return MCAL_OK;
}