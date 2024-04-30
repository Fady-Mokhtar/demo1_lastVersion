
#include "port.h"
#include "Keypad_cfg.h"
#include "Keypad.h"
#include "led.h"

// extern uint8_t UART_Buffer[2] ;

volatile u8 UARTFRAME[2] = {START_OF_FRAME, IDLE_MESSAGE};

#define NOT_INIT_State 0
#define OPERATIONAL_STATE 1

#define BTN_NOT_PRESSED 0
#define BTN_PRESSED 1

#define DATE_TIME_MODE 0
#define STOP_WATCH_MODE 3

// static u8 input = 0;
// static u8 *PTR_TO_Input = &input;
static u8 Keypadstate = NOT_INIT_State;
// static u8 KeypadCurrentMode = DATE_TIME_MODE;
static u8 BTN_state[No_of_Desired_Rows][No_of_Desired_Columns] = {0};

static Keypad_Errorstates_t Keypad_init();
static Keypad_Errorstates_t Keypad_get_value();
static Keypad_Errorstates_t IsPressed(u8 Row_No, u8 Column_No, u32 *switch_current_state);

/*
 * array of two first byte start second byte input from get value send to uart
 *
 */

// uint8_t UARTFARME[1] = &UARTFARME[1];

static const u8 keypad_values[No_of_Desired_Rows][No_of_Desired_Columns] = {
    {CHANGE_MODE, INCREMENT, EDIT_MODE},
    {LEFT, OK, RIGHT},
    {START, DECREMENT, STOP}};
/*
        btns layout={MODE,INCREMENT,EDIT},
                    {RIGHT,OK,LEFT},
                    {BEGIN,DECREMENT,STOP}
*/
extern KeyPad_CFG_t KeyPad_CFG[NO_OF_KEYPADS];
// every 120 ms runnable
void Get_Value_and_transmit_runnable(void)
{
    u8 sent_data_Flag = 0;

    for (u8 Nested_LOC_iterartor_1 = 0; Nested_LOC_iterartor_1 < No_of_Desired_Rows; Nested_LOC_iterartor_1++)
    {
        if (sent_data_Flag == 1)
        {
            break;
        }
        else
        {
            for (u8 Nested_LOC_iterartor_2 = 0; Nested_LOC_iterartor_2 < No_of_Desired_Columns; Nested_LOC_iterartor_2++)
            {
                if (BTN_state[Nested_LOC_iterartor_1][Nested_LOC_iterartor_2] == BTN_PRESSED)
                {
                    BTN_state[Nested_LOC_iterartor_1][Nested_LOC_iterartor_2] = BTN_NOT_PRESSED;

                    // LED_SetStatus(LED_1, LED_ON);
                    sent_data_Flag = 1;
                    UARTFRAME[1] = keypad_values[Nested_LOC_iterartor_1][Nested_LOC_iterartor_2];
                    // USART_Transmit_DMA(&usart1Manager, &UARTFARME, (uint16_t)0x02);

                    break;
                }
                else
                {
                    UARTFRAME[1] = IDLE_MESSAGE;
                }
            }
        }
    }
    // LED_SetStatus(LED_1, LED_OFF);
    // USART_Transmit_DMA(&usart1Manager, &UART_Buffer, (uint16_t)0x02);
    // USART_startTransmit_IT(&usart1Manager, &UARTFARME[0], (uint16_t)0x02);
}

// EVERY 6 MS
void keypad_runnable(void)
{
    switch (Keypadstate)
    {
    case NOT_INIT_State:
        LED_Init();
        Keypad_init();
        Keypadstate = OPERATIONAL_STATE;
        break;
    case OPERATIONAL_STATE:
        Keypad_get_value();
        break;

    default:
        break;
    }
}

Keypad_Errorstates_t Keypad_init()
{

    Keypad_Errorstates_t ret_Errorstate = KEYPAD_OK;
    GPIO_PinConfig_t Keypad_row_pins;
    GPIO_PinConfig_t Keypad_Column_pins;
    Keypad_row_pins.Speed = GPIO_HIGH_SPEED;
    Keypad_Column_pins.Speed = GPIO_HIGH_SPEED;
    for (u8 LOC_iterartor = 0; LOC_iterartor < NO_OF_KEYPADS; LOC_iterartor++)
    {
        for (u8 Nested_LOC_iterartor = 0; Nested_LOC_iterartor < No_of_Desired_Rows; Nested_LOC_iterartor++)
        {
            Keypad_row_pins.Port = KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor].PORT;
            Keypad_row_pins.Pin = KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor].PIN;
            Keypad_row_pins.Mode = KeyPad_CFG[LOC_iterartor].ROWS_Default_state;
            Keypad_Column_pins.Port = KeyPad_CFG[LOC_iterartor].KeyPad_Columns_Connection_CFG[Nested_LOC_iterartor].PORT;
            Keypad_Column_pins.Pin = KeyPad_CFG[LOC_iterartor].KeyPad_Columns_Connection_CFG[Nested_LOC_iterartor].PIN;
            Keypad_Column_pins.Mode = KeyPad_CFG[LOC_iterartor].Columns_Default_state;
            if (Keypad_row_pins.Mode == GPIO_OUTPUT_PP)
            {
                GPIO_InitPin(&Keypad_row_pins);
                GPIO_Set_PinValue(Keypad_row_pins.Port, Keypad_row_pins.Pin, KeyPad_CFG[LOC_iterartor].ROWS_Column_Output_state);
            }
            else
            {
                GPIO_InitPin(&Keypad_row_pins);
            }
            if (Keypad_Column_pins.Mode == GPIO_OUTPUT_PP)
            {
                GPIO_InitPin(&Keypad_Column_pins);
                GPIO_Set_PinValue(Keypad_Column_pins.Port, Keypad_Column_pins.Pin, KeyPad_CFG[LOC_iterartor].ROWS_Column_Output_state);
            }
            else
            {
                GPIO_InitPin(&Keypad_Column_pins);
            }
        }
    }

    return ret_Errorstate;
}
Keypad_Errorstates_t Keypad_get_value()
{

    Keypad_Errorstates_t ret_Errorstate = KEYPAD_OK;
    u32 LOC_TEMP = 0;
    u32 *ptr_to_local_value = &LOC_TEMP;
    for (u8 LOC_iterartor = 0; LOC_iterartor < NO_OF_KEYPADS; LOC_iterartor++)
    {

        for (u8 Nested_LOC_iterartor_1 = 0; Nested_LOC_iterartor_1 < No_of_Desired_Rows; Nested_LOC_iterartor_1++)
        {
            for (u8 Nested_LOC_iterartor = 0; Nested_LOC_iterartor < No_of_Desired_Rows; Nested_LOC_iterartor++)
            {
                GPIO_Set_PinValue(KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor].PORT, KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor].PIN, GPIO_LOW);
            }

            GPIO_Set_PinValue(KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor_1].PORT, KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor_1].PIN, GPIO_HIGH);

            for (u8 Nested_LOC_iterartor_2 = 0; Nested_LOC_iterartor_2 < No_of_Desired_Columns; Nested_LOC_iterartor_2++)
            {

                GPIO_GetPinValue(KeyPad_CFG[LOC_iterartor].KeyPad_Rows_Connection_CFG[Nested_LOC_iterartor_2].PORT, KeyPad_CFG[LOC_iterartor].KeyPad_Columns_Connection_CFG[Nested_LOC_iterartor_2].PIN, ptr_to_local_value);
                IsPressed(Nested_LOC_iterartor_1, Nested_LOC_iterartor_2, ptr_to_local_value);
            }
        }
    }
    return ret_Errorstate;
}

static Keypad_Errorstates_t IsPressed(u8 Row_No, u8 Column_No, u32 *switch_current_state)
{
    Keypad_Errorstates_t ret_Errorstate = KEYPAD_OK;
    static u8 count[No_of_Desired_Rows][No_of_Desired_Columns] = {0};
    static u8 past_value[No_of_Desired_Rows][No_of_Desired_Columns] = {0};
    static u8 LOC_BTN_State[No_of_Desired_Rows][No_of_Desired_Columns] = {0};
    if (*switch_current_state == past_value[Row_No][Column_No])
    {
        count[Row_No][Column_No] = (count[Row_No][Column_No]) + 1;
        LOC_BTN_State[Row_No][Column_No] = *switch_current_state;
    }
    else
    {
        count[Row_No][Column_No] = 0;
    }
    if ((count[Row_No][Column_No]) % 26 == 0 && LOC_BTN_State[Row_No][Column_No])
    {
        BTN_state[Row_No][Column_No] = BTN_PRESSED;
    }
    past_value[Row_No][Column_No] = *switch_current_state;

    return ret_Errorstate;
}
