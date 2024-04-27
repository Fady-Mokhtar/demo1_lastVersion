/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "Errors.h"
#include "STD_TYPES.h"

/* Driver Libraries   */
#include "SCB.h"
#include "STM32F401CC.h"



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/


//void NVIC_SetPriorityGrouping(uint32_t priority_grouping);                        // Set the priority grouping

/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_EnableIRQ(IRQn_t IRQn);

/**
  \brief   Disable Interrupt
  \details Disables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_DisableIRQ(IRQn_t IRQn);

/**
  \brief   Get Pending Interrupt
  \details Checks if a device specific interrupt is pending in the NVIC interrupt controller.
  \param [in]       IRQn  Device specific interrupt number.
  \param [out]  Copy_PendingState  Pointer to store the pending state (1 if pending, 0 if not).
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_GetPendingIRQ (IRQn_t IRQn, uint32_t* Copy_PendingState);

/**
  \brief   Set Pending Interrupt
  \details Sets a device specific interrupt as pending in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_SetPendingIRQ (IRQn_t IRQn);

/**
  \brief   Clear Pending Interrupt
  \details Clears the pending status of a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_ClearPendingIRQ (IRQn_t IRQn);

/**
  \brief   Get Active Interrupt
  \details Retrieves the active interrupt number from the NVIC interrupt controller.
  \param [in]       IRQn  Device specific interrupt number.
  \param [out]  Copy_PendingState  Pointer to store the active state (1 if active, 0 if not).
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_GetActive (IRQn_t IRQn, uint32_t* Copy_PendingState);

/**
  \brief   Set Interrupt Priority
  \details Sets the priority of a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \param [in]  priority  Priority to set.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_SetPriority (IRQn_t IRQn, uint32_t priority);

/**
  \brief   Get Interrupt Priority
  \details Retrieves the priority of a device specific interrupt from the NVIC interrupt controller.
  \param [in]       IRQn  Device specific interrupt number.
  \param [out]  Copy_PendingState  Pointer to store the priority value.
  \return                 Status indicating whether the operation was successful or not.
                          - MCAL_OK: Operation successful.
                          - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t NVIC_GetPriority (IRQn_t IRQn, uint32_t* Copy_PendingState);

/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
void NVIC_SystemReset (void);

