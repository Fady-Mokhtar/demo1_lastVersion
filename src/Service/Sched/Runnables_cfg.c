
#include "Runnables_cfg.h"
#include "Sched.h"

extern void LCD_Task(void);
extern void Switches_Of_Keypad(void);
extern void Sec_Increment_Task_For_Date_Time(void);
extern void Sec_Increment_Task_For_Stop_Watch(void);
extern void Write_Date_Time_Task(void);
extern void Get_Value_and_transmit_runnable(void);
extern void Uart_receive_runnable(void);
extern void Uart_transmit_runnable(void);
extern void keypad_runnable(void);
extern void Uart_runnable(void);

extern void dma_receive_runnable();
extern void dma_transmit_runnable();

// Runnable_t RunnableList[2] =

// 	{

// 		[0] =
// 			{
// 				.Runnable_Name = "dma_receive",
// 				.DelayMs = 0,
// 				.PeriodicityMS = 2,

// 				.CB = dma_receive_runnable},

// 		[1] =
// 			{
// 				.Runnable_Name = "dma_transmit",
// 				.DelayMs = 0,
// 				.PeriodicityMS = 2,

// 				.CB = dma_transmit_runnable}};

/*ARRAY FOR THE USER TO CONFIGURE THE RUNNABLES*/

Runnable_t RunnableList[_Number_of_Runnables]=

{

	[LCD_INIT_RUNNABLE] =
		{
			.Runnable_Name = "LCD_Init",
			.DelayMs = 0,
			.PeriodicityMS = 2,

			.CB = LCD_Task},

	[Write_Date_Time_Task_RUNNABLE] =
		{
			.Runnable_Name = "Write_Date_Time_Task",
			.DelayMs = 50,
			.PeriodicityMS = 2,

			.CB = Write_Date_Time_Task},

	[Sec_Increment_Task_For_Date_Time_RUNNABLE] =
		{
			.Runnable_Name = "Sec_Increment_Task_For_Date_Time",
			.DelayMs = 70,
			.PeriodicityMS = 1000,
			.CB = Sec_Increment_Task_For_Date_Time},

	[Keypad_Task_RUNNABLE] =
		{
			.Runnable_Name = "Keypad_Task",
			.DelayMs = 150,
			.PeriodicityMS =10,

			.CB = keypad_runnable},

	[Sec_Increment_Task_For_Stop_Watch_RUNNABLE] =
		{
			.Runnable_Name = "Sec_Increment_Task_For_Stop_Watch",
			.DelayMs = 350,
			.PeriodicityMS = 1000,

			.CB = Sec_Increment_Task_For_Stop_Watch},

	[Switches_Task_RUNNABLE] =
		{
			.Runnable_Name = "Switches_Task",
			.DelayMs = 150,
			.PeriodicityMS = 200,

			.CB = Switches_Of_Keypad},

	[Keypad_TRANSMIT_Task_RUNNABLE] =
		{
			.Runnable_Name = "Keypad_TRANSMIT_Task",
			.DelayMs = 200,
			.PeriodicityMS = 250,

			.CB = Get_Value_and_transmit_runnable},

	// [Uart_Task] =
	// 	{
	// 		.Runnable_Name = "Uart_runnable",
	// 		.DelayMs = 250,
	// 		.PeriodicityMS = 250,

	// 		.CB = Uart_runnable},

	[Uart_receive_task] =
		{
			.Runnable_Name = "Uart_receive_runnable",
			.DelayMs = 250,
			.PeriodicityMS = 250,

			.CB = Uart_receive_runnable},

	[Uart_transmit_task] =
		{
				.Runnable_Name = "Uart_transmit_runnable",
				.DelayMs = 252,
				.PeriodicityMS = 258,

				.CB = Uart_transmit_runnable},

				};