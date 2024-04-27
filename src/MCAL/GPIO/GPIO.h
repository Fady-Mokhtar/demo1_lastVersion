#ifndef MCAL_GPIO_GPIO_H_
#define MCAL_GPIO_GPIO_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "Errors.h"
#include "STD_TYPES.h"

/* Driver Libraries   */
#include "RCC.h"
#include "GPIO_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/


/*
 * Base addresses for each GPIO port.
 * Note: These addresses are specific to the STM32 family of microcontrollers.
 */
#define GPIO_PORTA     (void*)(0x40020000UL)
#define GPIO_PORTB     (void*)(0x40020400UL)
#define GPIO_PORTC     (void*)(0x40020800UL)
#define GPIO_PORTD     (void*)(0x40020C00UL)
#define GPIO_PORTE     (void*)(0x40021000UL)
#define GPIO_PORTH     (void*)(0x40021C00UL)

/*
 * AFRH & AFRL selection Masks for configuring alternate function for GPIO pins.
 */
#define      GPIO_MASK_AF0         0b0000
#define      GPIO_MASK_AF1         0b0001
#define      GPIO_MASK_AF2         0b0010
#define      GPIO_MASK_AF3         0b0011
#define      GPIO_MASK_AF4         0b0100
#define      GPIO_MASK_AF5         0b0101
#define      GPIO_MASK_AF6         0b0110
#define      GPIO_MASK_AF7         0b0111
#define      GPIO_MASK_AF8         0b1000
#define      GPIO_MASK_AF9         0b1001
#define      GPIO_MASK_AF10        0b1010
#define      GPIO_MASK_AF11        0b1011
#define      GPIO_MASK_AF12        0b1100
#define      GPIO_MASK_AF13        0b1101
#define      GPIO_MASK_AF14        0b1110
#define      GPIO_MASK_AF15        0b1111




/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
  * @brief  GPIO pin configuration structure.
  * @note   This structure defines the configuration parameters for a GPIO pin.
  */
typedef struct
{
    void* port;        /*!< Pointer to the GPIO port base address */
    uint32_t pin;      /*!< Pin number */
    uint32_t speed;    /*!< GPIO output speed */
    uint32_t mode;     /*!< GPIO pin mode */
    uint32_t AF;       /*!< Alternate function selection mask */
    uint32_t out_type; /*!< GPIO output type */
    uint32_t pupd;     /*!< GPIO pull-up/pull-down configuration */
} GPIO_StrCfg_t;


/**
  * @brief  GPIO pin numbers enumeration.
  * @note   This enumeration lists the available GPIO pin numbers.
  */
typedef enum
{
    GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7,
    GPIO_PIN8, GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, GPIO_PIN12, GPIO_PIN13, GPIO_PIN14, GPIO_PIN15
} GPIO_PINS_t;

/**
  * @brief  GPIO pin state enumeration.
  * @note   This enumeration defines the possible states of a GPIO pin.
  */
typedef enum
{
    GPIO_PINSTATE_LOW, 
    GPIO_PINSTATE_HIGH
} GPIO_PinState_t;


/**
  * @brief  GPIO pin mode enumeration.
  * @note   This enumeration defines the possible modes of a GPIO pin.
  */
typedef enum
{
    GPIO_MODE_Default = 0,
    GPIO_MODE_Input = 0,            // (reset state)
    GPIO_MODE_Output,
    GPIO_MODE_Alternatefunction,
    GPIO_MODE_Analog
} GPIO_ModeState_t;


/**
  * @brief  GPIO output type enumeration.
  * @note   This enumeration defines the possible output types of a GPIO pin.
  */
typedef enum
{
    GPIO_OUTPUT_Default = 0,
    GPIO_OUTPUT_PushPull = 0,  // (reset state)
    GPIO_OUTPUT_OpenDrain
} GPIO_OutputType_t;


/**
  * @brief  GPIO output speed enumeration.
  * @note   This enumeration defines the possible output speeds of a GPIO pin.
  */
typedef enum
{
    GPIO_SPEED_Default = 0,
    GPIO_SPEED_Low = 0,
    GPIO_SPEED_Medium,
    GPIO_SPEED_High,
    GPIO_SPEED_VeryHigh
} GPIO_SpeedState_t;


/**
  * @brief  GPIO pull-up/pull-down configuration enumeration.
  * @note   This enumeration defines the possible pull-up/pull-down configurations of a GPIO pin.
  */
typedef enum
{
    GPIO_Default = 0,
    GPIO_NO_PUPD = 0,
    GPIO_PU,
    GPIO_PD,
    GPIO_Reserved
} GPIO_PUPDRState_t;



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
  * @brief  Initializes a GPIO pin according to the provided configuration.
  * @param  Copy_strCfg_ptr: Pointer to a GPIO_StrCfg_t structure that contains the pin configuration.
  * @retval MCALStatus_t: Status of the initialization process, can be SUCCESS or ERROR.
  *
  * This function initializes a GPIO pin based on the configuration provided in the
  * GPIO_StrCfg_t structure pointed to by Copy_strCfg_ptr. The configuration includes
  * settings such as pin mode (input/output), output type (push-pull/open-drain), 
  * output speed, and pull-up/pull-down resistors. The function configures the
  * corresponding GPIO port and pin based on the specified settings.
  *
  * @note   This function assumes that the GPIO peripheral clock has already been enabled.
  *         It does not handle enabling the clock for the GPIO port.
  */

MCALStatus_t GPIO_Init(GPIO_StrCfg_t *Copy_strCfg_ptr, uint8_t NUM_OF_PINS);

/**
  * @brief  Sets the state of a GPIO pin.
  * @param  Port: Pointer to the GPIO port base address.
  * @param  Copy_PinNum: The pin number to set the state for.
  * @param  Copy_PinState: The desired state of the pin (GPIO_PIN_RESET or GPIO_PIN_SET).
  * @retval MCALStatus_t: Status of the operation, can be SUCCESS or ERROR.
  *
  * This function sets the state of the specified GPIO pin on the specified GPIO port
  * to the desired state (GPIO_PIN_RESET for low, GPIO_PIN_SET for high). It uses bitwise
  * operations to modify the specific pin in the GPIO port's output data register
  * without affecting the other pins. The function returns SUCCESS if the operation
  * is successful, or ERROR if an error occurs (e.g., invalid pin number).
  *
  * @note   This function assumes that the GPIO port has already been configured and initialized.
  *         It does not handle GPIO initialization or configuration.
  */

MCALStatus_t GPIO_SetPinState(void *Port, GPIO_PINS_t Copy_PinNum, GPIO_PinState_t Copy_PinState);

/**
  * @brief  Toggle the state of a GPIO pin.
  * @param  Port: Pointer to the GPIO port base address.
  * @param  Copy_PinNum: The pin number to set the state for.
  * @param  Copy_PinState: The desired state of the pin (GPIO_PIN_RESET or GPIO_PIN_SET).
  * @retval MCALStatus_t: Status of the operation, can be SUCCESS or ERROR.
  *
  * This function sets the state of the specified GPIO pin on the specified GPIO port
  * to the desired state (GPIO_PIN_RESET for low, GPIO_PIN_SET for high). It uses bitwise
  * operations to modify the specific pin in the GPIO port's output data register
  * without affecting the other pins. The function returns SUCCESS if the operation
  * is successful, or ERROR if an error occurs (e.g., invalid pin number).
  *
  * @note   This function assumes that the GPIO port has already been configured and initialized.
  *         It does not handle GPIO initialization or configuration.
  */
MCALStatus_t GPIO_TogglePinState(void *port, GPIO_PINS_t Copy_PinNum);

/**
  * @brief  Gets the state of a GPIO pin.
  * @param  Port: Pointer to the GPIO port base address.
  * @param  Copy_PinNum: The pin number to get the state for.
  * @param  Copy_PinState: Pointer to a variable to store the pin state (GPIO_PIN_RESET or GPIO_PIN_SET).
  * @retval MCALStatus_t: Status of the operation, can be SUCCESS or ERROR.
  *
  * This function retrieves the current state of the specified GPIO pin on the specified GPIO port
  * and stores it in the variable pointed to by Copy_PinState. It reads the specific pin's state
  * from the GPIO port's input data register using bitwise operations. The function returns
  * SUCCESS if the operation is successful, or ERROR if an error occurs (e.g., invalid pin number).
  *
  * @note   This function assumes that the GPIO port has already been configured and initialized.
  *         It does not handle GPIO initialization or configuration.
  */

MCALStatus_t GPIO_GetPinState(void *Port, GPIO_PINS_t Copy_PinNum, uint8_t *Copy_PinState);

MCALStatus_t GPIO_GetOutState(void *port, GPIO_PINS_t Copy_PinNum, uint8_t* Copy_PinState);








#endif // MCAL_GPIO_GPIO_H_