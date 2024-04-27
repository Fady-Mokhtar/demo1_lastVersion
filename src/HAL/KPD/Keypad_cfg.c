
#include"Keypad_cfg.h"
#include"port.h"

KeyPad_CFG_t KeyPad_CFG[NO_OF_KEYPADS]=
{
    [FIRST_KEYPAD]=
    {
        .KeyPad_Rows_Connection_CFG[Row0]=
        {
            .PORT=GPIOB_BASEADD,
            .PIN=GPIO_PIN0,
        },
        .KeyPad_Rows_Connection_CFG[Row1]=
        {
            .PORT=GPIOB_BASEADD,
            .PIN=GPIO_PIN1,
        },
        .KeyPad_Rows_Connection_CFG[Row2]=
        {
            .PORT=GPIOB_BASEADD,
            .PIN=GPIO_PIN2,
        },
        .KeyPad_Columns_Connection_CFG[Column0]=
        {
            .PORT=GPIOB_BASEADD,
            .PIN=GPIO_PIN5,
        },
        .KeyPad_Columns_Connection_CFG[Column1]=
        {
            .PORT=GPIOB_BASEADD,
            .PIN=GPIO_PIN6,
        },
        .KeyPad_Columns_Connection_CFG[Column2]=
        {
            .PORT=GPIOB_BASEADD,
            .PIN=GPIO_PIN7,
        },
        .ROWS_Default_state=GPIO_OUTPUT_PP,
        .Columns_Default_state=GPIO_INPUT_PD,
        .ROWS_Column_Output_state=GPIO_HIGH
    }
};


