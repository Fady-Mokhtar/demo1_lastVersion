/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */

/* Driver Libraries   */
#include "NVIC.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define __NVIC_PRIO_BITS          4U       /*!< STM32F4XX uses 4 Bits for the Priority Levels */
/**
  \brief   NVIC Register Structure
  \details Structure representing the NVIC (Nested Vectored Interrupt Controller) registers.
           It provides volatile access to various NVIC registers for interrupt control and management.
 */
typedef struct 
{
    volatile uint32_t NVIC_ISER[8U];       /**< Interrupt Set-Enable Registers */
    volatile uint32_t RESERVED1[24U];      /**< Reserved space */
    volatile uint32_t NVIC_ICER[8U];       /**< Interrupt Clear-Enable Registers */
    volatile uint32_t RESERVED2[24U];      /**< Reserved space */
    volatile uint32_t NVIC_ISPR[8U];       /**< Interrupt Set-Pending Registers */
    volatile uint32_t RESERVED3[24U];      /**< Reserved space */
    volatile uint32_t NVIC_ICPR[8U];       /**< Interrupt Clear-Pending Registers */
    volatile uint32_t RESERVED4[24U];      /**< Reserved space */
    volatile uint32_t NVIC_IABR[8U];       /**< Interrupt Active Bit Registers */
    volatile uint32_t RESERVED5[56U];      /**< Reserved space */
    volatile uint8_t NVIC_IPR[240U];       /**< Interrupt Priority Registers */
    volatile uint32_t RESERVED6[644U];     /**< Reserved space */
    volatile uint32_t NVIC_STIR;           /**< Software Trigger Interrupt Register */

}MNVIC;

#define NVIC_BASE_ADDR      (0xE000E100UL)                  /**< Base address of the NVIC */
#define NVIC    ((volatile MNVIC* const)(NVIC_BASE_ADDR))   /**< Pointer to the NVIC registers */

/*
    #define NVIC_STIR_OFFSET    0xE00UL
    #define NVIC_STIR   (volatile uint32_t NVIC_STIR)(NVIC_BASE_ADDR + NVIC_STIR_OFFSET)
*/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


//void NVIC_SetPriorityGrouping(uint32_t priority_grouping);  

/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
*/           
MCALStatus_t NVIC_EnableIRQ(IRQn_t IRQn)                                                 
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;

    if(IRQn >= 0)
    {
        NVIC->NVIC_ISER[IRQn/32] = 1 << (IRQn % 32); 
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;
    }
    return Loc_enumReturnStatus;
}

    // STMCube implementation 
/*
__STATIC_INLINE void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    __COMPILER_BARRIER();
    NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __COMPILER_BARRIER();
  }
}
*/

/**
  \brief   Disable Interrupt
  \details Disables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/           
MCALStatus_t NVIC_DisableIRQ(IRQn_t IRQn)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK; 

    if(IRQn >= 0)
    {
        NVIC->NVIC_ICER[IRQn/32] = 1 << (IRQn % 32);  // Disable the interrupt
    }  
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;  // Invalid input parameter
    }
    return Loc_enumReturnStatus;
}       


/**
  \brief   Set Pending Interrupt
  \details Sets a device specific interrupt as pending in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/           
MCALStatus_t NVIC_SetPendingIRQ (IRQn_t IRQn)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;

    if(IRQn >= 0)
    {
        NVIC->NVIC_ISPR[IRQn/32] = 1 << (IRQn % 32);  // Set the interrupt as pending
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;  // Invalid input parameter
    }
    return Loc_enumReturnStatus;
}   
    

/**
  \brief   Clear Pending Interrupt
  \details Clears a device specific interrupt as pending in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/           
MCALStatus_t NVIC_ClearPendingIRQ (IRQn_t IRQn)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;

    if(IRQn >= 0)
    {
        NVIC->NVIC_ICPR[IRQn/32] = 1 << (IRQn % 32);  // Clear the pending interrupt
    } 
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;  // Invalid input parameter
    }

    return Loc_enumReturnStatus;
}


/**
  \brief   Get Pending Interrupt Status
  \details Retrieves the pending status of a device specific interrupt in the NVIC interrupt controller.
  \param [in]   IRQn            Device specific interrupt number.
  \param [out]  Copy_PendingState  Pointer to store the pending state (1 or 0) of the interrupt.
  \return                       Status indicating whether the operation was successful or not.
                                - MCAL_OK: Operation successful.
                                - MCAL_WRONG_INPUTS: Invalid input parameters.
*/           
MCALStatus_t NVIC_GetPendingIRQ (IRQn_t IRQn, uint32_t* Copy_PendingState)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;
    if(IRQn >= 0)
    {
        *Copy_PendingState = (NVIC->NVIC_ISPR[IRQn/32] << (IRQn % 32));  // Get the pending state of the interrupt
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;  // Invalid input parameter
    }

    return Loc_enumReturnStatus;
}


/**
  \brief   Get Active Interrupt
  \details Retrieves the active status of a device specific interrupt in the NVIC interrupt controller.
  \param [in]   IRQn            Device specific interrupt number.
  \param [out]  Copy_PendingState  Pointer to store the active state (1 or 0) of the interrupt.
  \return                       Status indicating whether the operation was successful or not.
                                - MCAL_OK: Operation successful.
                                - MCAL_WRONG_INPUTS: Invalid input parameters.
*/           
MCALStatus_t NVIC_GetActive (IRQn_t IRQn, uint32_t* Copy_PendingState)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;
    if(IRQn >= 0)
    {
        *Copy_PendingState = (NVIC->NVIC_IABR[IRQn/32] << (IRQn % 32));  // Get the active state of the interrupt
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;  // Invalid input parameter
    } 

    return Loc_enumReturnStatus;
}


/**
  \brief   Set Interrupt Priority
  \details Sets the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]      IRQn  Interrupt number.
  \param [in]  priority  Priority to set.
  \note    The priority cannot be set for every processor exception.
 */
MCALStatus_t NVIC_SetPriority (IRQn_t IRQn, uint32_t priority)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;

    if ((sint32_t)(IRQn) >= 0)
    {
      NVIC->NVIC_IPR[((uint32_t)IRQn)]               = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
    }
    else if ((sint32_t)(IRQn) < 0 && (sint32_t)(IRQn) >= -14)
    {
      SCB->SCB_SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;
    }
    return Loc_enumReturnStatus;
}           

// implement if we will make it in .h file it must be a static inline
/*
__STATIC_INLINE void __NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->IP[((uint32_t)IRQn)]               = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
  else
  {
    SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
}
*/

/**
  \brief   Get Interrupt Priority
  \details Reads the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]   IRQn  Interrupt number.
  \return             Interrupt Priority.
                      Value is aligned automatically to the implemented priority bits of the microcontroller.
 */
uint8_t NVIC_GetPriority (IRQn_t IRQn, uint32_t* PeriorityStatus)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;
    if ((sint32_t)(IRQn) >= 0)
    {
      *PeriorityStatus = (((uint32_t)NVIC->NVIC_IPR[((uint32_t)IRQn)]               >> (8U - __NVIC_PRIO_BITS)));
    }
    else if ((sint32_t)(IRQn) < 0 && (sint32_t)(IRQn) >= -14)
    {
      *PeriorityStatus = (((uint32_t)SCB->SCB_SHP[(((uint32_t)IRQn) & 0xFUL)-4UL]   >> (8U - __NVIC_PRIO_BITS)));
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;
    }
    return Loc_enumReturnStatus;
}            

/*
__STATIC_INLINE uint32_t __NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) >= 0)
  {
    return(((uint32_t)NVIC->IP[((uint32_t)IRQn)]               >> (8U - __NVIC_PRIO_BITS)));
  }
  else
  {
    return(((uint32_t)SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] >> (8U - __NVIC_PRIO_BITS)));
  }
}
*/

/**
  \brief   Set Software Interrupt
  \details Generates a software interrupt for a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/           
MCALStatus_t SET_Software_Interrupt(IRQn_t IRQn)
{
    MCALStatus_t Loc_enumReturnStatus = MCAL_OK;
    
    if (IRQn >= 0)
    {
        /* Generate a Software Interrupt */
        NVIC->NVIC_STIR = IRQn;  // Set the software interrupt
    }
    else
    {
        Loc_enumReturnStatus = MCAL_WRONG_INPUTS;  // Invalid input parameter
    }
    return Loc_enumReturnStatus;
}


/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
void NVIC_SystemReset (void)
{
  __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
  SCB->SCB_AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)    |
                           (SCB->SCB_AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                            SCB_AIRCR_SYSRESETREQ_Msk    );         /* Keep priority group unchanged */
  __DSB();                                                          /* Ensure completion of memory access */

  for(;;)                                                           /* wait until reset */
  {
    __NOP();                                                        // Do nothing
  }

}
                                       