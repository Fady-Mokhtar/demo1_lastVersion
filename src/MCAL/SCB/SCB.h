/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "Errors.h"
#include "STD_TYPES.h"

/* Driver Libraries   */

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/* CMSIS compiler specific defines */

#ifndef   __ASM
  #define __ASM                                  __asm
#endif

/**
  \brief   No Operation
  \details No Operation does nothing. This instruction can be used for code alignment purposes.
 */
#define __NOP()                                 __ASM volatile ("nop")

#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                  __attribute__((always_inline)) static inline
#endif


/* SCB Vector Table Offset Register Definitions */
#define SCB_VTOR_TBLOFF_Pos                 7U                                            /*!< SCB VTOR: TBLOFF Position */
#define SCB_VTOR_TBLOFF_Msk                (0x1FFFFFFUL << SCB_VTOR_TBLOFF_Pos)           /*!< SCB VTOR: TBLOFF Mask */

/* SCB Application Interrupt and Reset Control Register Definitions */
#define SCB_AIRCR_VECTKEY_Pos              16U                                            /*!< SCB AIRCR: VECTKEY Position */
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)            /*!< SCB AIRCR: VECTKEY Mask */

#define SCB_AIRCR_PRIGROUP_Pos              8U                                            /*!< SCB AIRCR: PRIGROUP Position */
#define SCB_AIRCR_PRIGROUP_Msk             (7UL << SCB_AIRCR_PRIGROUP_Pos)                /*!< SCB AIRCR: PRIGROUP Mask */

#define SCB_AIRCR_SYSRESETREQ_Pos           2U                                            /*!< SCB AIRCR: SYSRESETREQ Position */
#define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)             /*!< SCB AIRCR: SYSRESETREQ Mask */

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
  \ingroup  CMSIS_core_register
  \defgroup CMSIS_SCB     System Control Block (SCB)
  \brief    Type definitions for the System Control Block Registers
  @{
 */

/**
  \brief  Structure type to access the System Control Block (SCB).
 */

typedef struct
{
    volatile uint32_t SCB_CPUID;
    volatile uint32_t SCB_ICSR;
    volatile uint32_t SCB_VTOR;
    volatile uint32_t SCB_AIRCR;
    volatile uint32_t SCB_SCR;
    volatile uint32_t SCB_CCR;
    /*SCB_
    volatile uint32_t SCB_SHPR1;
    volatile uint32_t SCB_SHPR2;
    volatile uint32_t SCB_SHPR3;
    */
    volatile uint8_t  SCB_SHP[12U];
    volatile uint32_t SCB_SHCSR;
    volatile uint32_t SCB_CFSR;
    volatile uint32_t SCB_HFSR;
    volatile uint32_t SCB_DFSR;
    volatile uint32_t SCB_MMAR;
    volatile uint32_t SCB_BFAR;
    volatile uint32_t SCB_AFSR;
}MSCB;

#define SCB_BASE_ADDR    0xE000ED00UL
#define SCB     ((volatile MSCB* const)(SCB_BASE_ADDR))


/**
  \ingroup  CMSIS_core_register
  \defgroup CMSIS_SCB     System Control Block (SCB)
  \brief    Type definitions for the System Control Block Registers
  @{
 */

/**
  \brief  Structure type to access the System Control Block (SCB).
 */

// typedef struct
// {
//   __IM  uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
//   __IOM uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
//   __IOM uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
//   __IOM uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
//   __IOM uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
//   __IOM uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
//   __IOM uint8_t  SHP[12U];               /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
//   __IOM uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
//   __IOM uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
//   __IOM uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
//   __IOM uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
//   __IOM uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
//   __IOM uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
//   __IOM uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
//   __IM  uint32_t PFR[2U];                /*!< Offset: 0x040 (R/ )  Processor Feature Register */
//   __IM  uint32_t DFR;                    /*!< Offset: 0x048 (R/ )  Debug Feature Register */
//   __IM  uint32_t ADR;                    /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
//   __IM  uint32_t MMFR[4U];               /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
//   __IM  uint32_t ISAR[5U];               /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
//         uint32_t RESERVED0[5U];
//   __IOM uint32_t CPACR;                  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
// } SCB_Type;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/


/**
  \brief   Instruction Synchronization Barrier
  \details Instruction Synchronization Barrier flushes the pipeline in the processor,
           so that all instructions following the ISB are fetched from cache or memory,
           after the instruction has been completed.
 */
__STATIC_FORCEINLINE void __ISB(void)
{
  __ASM volatile ("isb 0xF":::"memory");
}
/*
The __STATIC_FORCEINLINE keyword indicates that the function should be inlined if possible, and the __DSB function is a macro for a Data Synchronization Barrier (DSB) instruction in ARM assembly language.

Here's a breakdown of the function and its assembly instruction:

__ASM volatile ("dsb 0xF":::"memory");: 
__ASM ----> keyword to define an assembly instruction.
volatile ----> is used to prevent the compiler from optimizing out the assembly instruction.
"dsb 0xF" ----> is the actual assembly instruction. DSB stands for Data Synchronization Barrier, and 0xF is a parameter specifying the type of barrier to use. 0xF is a full system DSB, ensuring that all memory accesses are completed before the barrier completes.
The empty input and output operand lists (:::) indicate that the assembly instruction does not take any input or produce any output.
The "memory" clobber list informs the compiler that the assembly instruction may read from or write to memory, so it should not reorder memory operations across this point.
In summary, the __DSB function is a compiler intrinsic that generates a Data Synchronization Barrier instruction in ARM assembly, ensuring that memory accesses are completed and synchronized across all cores in a multi-core system.
*/

/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
__STATIC_FORCEINLINE void __DSB(void)
{
  __ASM volatile ("dsb 0xF":::"memory");
}


/**
  \brief   Data Memory Barrier
  \details Ensures the apparent order of the explicit memory operations before
           and after the instruction, without ensuring their completion.
 */
__STATIC_FORCEINLINE void __DMB(void)
{
  __ASM volatile ("dmb 0xF":::"memory");
}


