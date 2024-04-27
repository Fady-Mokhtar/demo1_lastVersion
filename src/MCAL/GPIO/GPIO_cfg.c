/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "GPIO_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
GPIO_StrCfg_t Loc_arrStrGpios[__NUM_OF_PINS_DEMO] = {
    [0] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN0,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [1] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN1,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [2] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN2,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [3] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN3,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [4] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN4,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [5] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN5,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [6] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN6,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},
    //  Dma & Uart
    [7] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN9,
            .mode = GPIO_MODE_Alternatefunction,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_MASK_AF7},

    [8] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN10,
            .mode = GPIO_MODE_Alternatefunction,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_MASK_AF7},

    [9] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN0,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default}};

//  DMA
GPIO_StrCfg_t Loc_DMAGpios[__NUM_OF_PINS_DMA] = {
    [0] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN9,
            .mode = GPIO_MODE_Alternatefunction,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_MASK_AF7},

    [1] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN10,
            .mode = GPIO_MODE_Alternatefunction,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_MASK_AF7}};

// lcd
GPIO_StrCfg_t Loc_LCDGpios[__NUM_OF_PINS_LCD] = {
    [0] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN0,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [1] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN1,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [2] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN2,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [3] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN3,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [4] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN4,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [5] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN5,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [6] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN6,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [7] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN7,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [8] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN11,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [9] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN12,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default},

    [10] =
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN13,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Medium,
            .AF = GPIO_Default}};

//  KPD
GPIO_StrCfg_t Keypad_row_pins[__NUM_OF_PINS_KPD_ROWS] = {
    [0] =
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN0,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_High,
            .AF = GPIO_Default},

    [1] =
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN1,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_High,
            .AF = GPIO_Default},

    [2] =
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN2,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_High,
            .AF = GPIO_Default}};

GPIO_StrCfg_t Keypad_Column_pins[__NUM_OF_PINS_KPD_COLS] = {
    [0] =
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN3,
            .mode = GPIO_MODE_Input,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_PD,
            .speed = GPIO_SPEED_High,
            .AF = GPIO_Default},

    [1] =
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN4,
            .mode = GPIO_MODE_Input,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_PD,
            .speed = GPIO_SPEED_High,
            .AF = GPIO_Default},

    [2] =
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN5,
            .mode = GPIO_MODE_Input,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_PD,
            .speed = GPIO_SPEED_High,
            .AF = GPIO_Default}};
