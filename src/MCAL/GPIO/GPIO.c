/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */

/* Driver Libraries   */
#include "GPIO.h"
#include "GPIO_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/**
  * @brief  GPIO peripheral register map.
  * @note   This structure defines the memory-mapped registers of a GPIO peripheral.
  *         Each register corresponds to a specific function of the GPIO peripheral,
  *         such as pin mode, output type, output speed, pull-up/pull-down configuration,
  *         input data, output data, etc.
  *         The structure is designed to be used with direct memory access (DMA) to
  *         interact with the GPIO peripheral registers efficiently.
  */
typedef struct 
{
    volatile uint32_t GPIOx_MODER;    /*!< GPIO port mode register */
    volatile uint32_t GPIOx_OTYPER;   /*!< GPIO port output type register */
    volatile uint32_t GPIOx_OSPEEDR;  /*!< GPIO port output speed register */
    volatile uint32_t GPIOx_PUPDR;    /*!< GPIO port pull-up/pull-down register */
    volatile uint32_t GPIOx_IDR;      /*!< GPIO port input data register */
    volatile uint32_t GPIOx_ODR;      /*!< GPIO port output data register */
    volatile uint32_t GPIOx_BSRR;     /*!< GPIO port bit set/reset register */
    volatile uint32_t GPIOx_LCKR;     /*!< GPIO port configuration lock register */
    volatile uint32_t GPIOx_AFRL;     /*!< GPIO port alternate function low register */
    volatile uint32_t GPIOx_AFRH;     /*!< GPIO port alternate function high register */
    /*  we cannot make AFRL & AFRH one uint64_t Register because shifting operators's max limit is 32 bit only */
} MGPIO;


/* Init_Masks */

/*
 * RCC port mask for GPIO initialization.
 * This mask is used to enable the clock for the GPIO port being initialized.
 * The RCC port mask is calculated based on the base address of the GPIO port.
 */
#define GPIO_MASK_RCC_PORT    (1 << (((Copy_strCfg_ptr[i].port) - GPIO_PORTA) /0x400))

/*
 * GPIO register access macro.
 * This macro is used to access the GPIO registers for a specific port.
 * It casts the base address of the GPIO port to a pointer to the MGPIO structure.
 */
#define GPIO    ((volatile MGPIO* const)(Copy_strCfg_ptr[i].port))


/* SetPin_GetPin_Masks */

/*
 * GPIO pin register access macro.
 * This macro is used to access the GPIO registers for a specific pin.
 * It casts the base address of the GPIO port to a pointer to the MGPIO structure.
 */
#define GPIOPINx    ((volatile MGPIO* const)(port))

/*
 * Mask for bit manipulation.
 * This mask is used for setting or clearing individual bits in a register.
 */

#define MASK0           0x00
#define MASK1           0x01

/*
 * Bit set/reset register clear mask.
 * This mask is used to clear a specific bit in the BSRR (bit set/reset) register.
 */
#define BSRR_CLR_MASK   0x10

#define AFR_SIZE        0x04

#define CLR_AFRL_MASK(AFNum)      (GPIO->GPIOx_AFRL &= ~(0b1111 << (AFNum * AFR_SIZE)))
#define SET_AFRL_MASK(reqAF, AFNum)      (GPIO->GPIOx_AFRL |= ( reqAF << (AFNum * AFR_SIZE)))

#define CLR_AFRH_MASK(AFNum)      (GPIO->GPIOx_AFRH &= ~(0b1111 << (AFNum * AFR_SIZE)))
#define SET_AFRH_MASK(reqAF, AFNum)      (GPIO->GPIOx_AFRH |= ( reqAF << (AFNum * AFR_SIZE)))

#define AFRL1_MASK  0
#define AFRL2_MASK  1
#define AFRL3_MASK  2
#define AFRL4_MASK  3
#define AFRL5_MASK  4
#define AFRL6_MASK  5
#define AFRL7_MASK  6
#define AFRL8_MASK  7

#define AFRH1_MASK  8
#define AFRH2_MASK  9
#define AFRH3_MASK  10
#define AFRH4_MASK  11
#define AFRH5_MASK  12
#define AFRH6_MASK  13
#define AFRH7_MASK  14
#define AFRH8_MASK  15

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/**
  \brief       Initialize GPIO Pin
  \details     Initializes a GPIO pin based on the provided configuration.
  \param [in]  Copy_strCfg_ptr Pointer to the GPIO_StrCfg_t structure containing pin configuration.
  \return      Status indicating whether the operation was successful or not.
               - MCAL_OK: Operation successful.
               - MCAL_NULL_PTR: Null pointer provided as input.
               - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t GPIO_Init(GPIO_StrCfg_t *Copy_strCfg_ptr, uint8_t NUM_OF_PINS)
{ 
    MCALStatus_t Loc_GPIOErrorState = MCAL_OK;

    if(Copy_strCfg_ptr == NULL_t)
    {
        Loc_GPIOErrorState = MCAL_NULL_PTR;
    }
    else if (Copy_strCfg_ptr->port == NULL_t)
    {
        Loc_GPIOErrorState = MCAL_NULL_PTR;
    }
    else if (Copy_strCfg_ptr->port < GPIO_PORTA || Copy_strCfg_ptr->port > GPIO_PORTH)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else if (Copy_strCfg_ptr->pin < GPIO_PIN0 || Copy_strCfg_ptr->pin > GPIO_PIN15)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else if (Copy_strCfg_ptr->mode < GPIO_MODE_Input || Copy_strCfg_ptr->mode > GPIO_MODE_Analog)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else if (Copy_strCfg_ptr->speed < GPIO_SPEED_Low || Copy_strCfg_ptr->speed > GPIO_SPEED_VeryHigh)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else
    {
        
        uint8_t i;
        for (i = 0; i < NUM_OF_PINS; i++)
        {
            RCC_enableAHB1Peripheral(GPIO_MASK_RCC_PORT);               //1st Method
            GPIO->GPIOx_OSPEEDR |= (Copy_strCfg_ptr[i].speed << (Copy_strCfg_ptr[i].pin * 2));
            GPIO->GPIOx_PUPDR |= (Copy_strCfg_ptr[i].pupd << (Copy_strCfg_ptr[i].pin * 2));
            GPIO->GPIOx_MODER |= (Copy_strCfg_ptr[i].mode << (Copy_strCfg_ptr[i].pin * 2));
            GPIO->GPIOx_OTYPER |= (Copy_strCfg_ptr[i].out_type << (Copy_strCfg_ptr[i].pin * 2));
            if (Copy_strCfg_ptr[i].mode == GPIO_MODE_Alternatefunction)
            {
                if (Copy_strCfg_ptr[i].pin < GPIO_PIN8)
                {
                    CLR_AFRL_MASK(Copy_strCfg_ptr[i].pin);
                    SET_AFRL_MASK(Copy_strCfg_ptr[i].AF, Copy_strCfg_ptr[i].pin);
                }
                else if((Copy_strCfg_ptr[i].pin > GPIO_PIN7) && (Copy_strCfg_ptr[i].pin <= GPIO_PIN15))
                {
                    CLR_AFRH_MASK((Copy_strCfg_ptr[i].pin - 0x08));
                    SET_AFRH_MASK((Copy_strCfg_ptr[i].AF), (Copy_strCfg_ptr[i].pin - 0x08));
                }
                else
                {
                    Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
                }
            }

        }
        
    }
    

    return Loc_GPIOErrorState;
}

/**
  \brief       Set GPIO Pin State
  \details     Sets the state of a GPIO pin to either high or low.
  \param [in]  port Pointer to the GPIO port.
  \param [in]  Copy_PinNum Pin number.
  \param [in]  Copy_PinState Desired state of the pin (high or low).
  \return      Status indicating whether the operation was successful or not.
               - MCAL_OK: Operation successful.
               - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t GPIO_SetPinState(void *port, GPIO_PINS_t Copy_PinNum, GPIO_PinState_t Copy_PinState)
{
    MCALStatus_t Loc_GPIOErrorState = MCAL_OK;

    if (Copy_PinState == 0)
    {
        GPIOPINx->GPIOx_BSRR = Copy_PinState << (Copy_PinNum + BSRR_CLR_MASK);
    }
    else if (Copy_PinState == 1)   
    {
        GPIOPINx->GPIOx_BSRR = Copy_PinState << Copy_PinNum;
    }
    else    
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }

    /*     another method (old one)         */
 /*   
    if (Copy_PinState == 0)
    {
        GPIOPINx->GPIOx_ODR &= ~(Copy_PinState << Copy_PinNum);
    }
    else if (Copy_PinState == 1)   
    {
         GPIOPINx->GPIOx_ODR |=  Copy_PinState << Copy_PinNum;
    }
    else    
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
*/    
    return Loc_GPIOErrorState;
}

/**
  \brief       Get GPIO Pin State
  \details     Retrieves the current state of a GPIO pin (high or low).
  \param [in]  port Pointer to the GPIO port.
  \param [in]  Copy_PinNum Pin number.
  \param [out] Copy_PinState Pointer to where the pin state will be stored.
  \return      Status indicating whether the operation was successful or not.
               - MCAL_OK: Operation successful.
*/
MCALStatus_t GPIO_GetPinState(void *port, GPIO_PINS_t Copy_PinNum, uint8_t* Copy_PinState)
{
    MCALStatus_t Loc_GPIOErrorState = MCAL_OK;

    *Copy_PinState = ((GPIOPINx->GPIOx_IDR >> Copy_PinNum) & MASK1);

    return Loc_GPIOErrorState;
}

/**
  \brief       Toggle GPIO Pin State
  \details     Toggles the state of a GPIO pin.
  \param [in]  port Pointer to the GPIO port.
  \param [in]  Copy_PinNum Pin number.
  \return      Status indicating whether the operation was successful or not.
               - MCAL_OK: Operation successful.
*/
MCALStatus_t GPIO_TogglePinState(void *port, GPIO_PINS_t Copy_PinNum)
{
    MCALStatus_t Loc_GPIOErrorState = MCAL_OK;

    GPIOPINx->GPIOx_ODR ^= MASK1 << Copy_PinNum;
    
    return Loc_GPIOErrorState;
}