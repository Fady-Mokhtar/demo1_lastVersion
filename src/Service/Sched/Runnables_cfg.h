
#ifndef _RUNNABLES_H_
#define _RUNNABLES_H_


#define TICK_TIME_MS       2


typedef enum
{
	LCD_INIT_RUNNABLE,
	Write_Date_Time_Task_RUNNABLE,
	Sec_Increment_Task_For_Date_Time_RUNNABLE,
	Keypad_Task_RUNNABLE,
	Sec_Increment_Task_For_Stop_Watch_RUNNABLE,
	Switches_Task_RUNNABLE,
	Keypad_TRANSMIT_Task_RUNNABLE,
	//Uart_Task,
	Uart_transmit_task,
	Uart_receive_task,
    _Number_of_Runnables
}RunnablesNUM_t;


#endif /* RUNNABLES_H_ */