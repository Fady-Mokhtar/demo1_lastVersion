#include "LCD.h"

/****************** macros for lcd commands ************/
#define LCD_FUNCTION_SET_MASK		0x38
#define LCD_DISPLAY_ON_OFF_MASK		0x0C
#define LCD_DISPLAY_CLEAR_MASK		0x01
#define LCD_ENTRY_MODE_MASK			0x06
#define LCD_SHIFT_LEFT_MASK			0x18
#define LCD_SHIFT_RIGHT_MASK		0x1C
#define LCD_CURSOR_OFF				0x0C
#define LCD_CURSOR_ON 				0x0E
#define LCD_SET_CURSOR_LOCATION 	0x80
#define LCD_SET_DDRAM_ADRESS		128
#define TURN_OF_CURSOR				0x0C
#define TURN_ON_CURSOR				0x0F	

/***************** macros for goto_XY function to set the location to write   ***************/
#define FIRST_LINE			0
#define SECOND_LINE			1

#define NUMBER_OF_REQUESTES				30

/*****************modes of LCD *********/
#define DATE_TIME_MODE					0
#define STOP_WATCH_MODE					1
//#define EDIT_MODE						2

#define CHANGED							1
#define NOT_CHANGED						0

#define IDLE_MESSAGE					0x01
/********** macros to position *********/

#define CHANGE_MODE     0x10
#define EDIT_MODE       0x20
#define START           0x30
#define STOP            0x40
#define LEFT            0x50
#define RIGHT           0x60
#define INC		     	0x70
#define DEC      		0x80
#define OK              0x90
#define RESET			0x80

#define MAX_LINE_SIZE	15
#define MIN_LINE_SIZE	0

 void Init_Sm();

 void LCD_Write_Command(uint8_t Command);


 void LCD_Test_2(){

 }


 void Handle_Time_Edit_For_Date_Time_Mode();

 void Handle_Time_Edit_For_Stop_Watch_Mode();

 void Display_Date_Time_Helper();

 void Display_Stop_Watch_Helper();

 void Sec_Increment_Task_For_Date_Time(void);

 void Sec_Increment_Task_For_Stop_Watch(void);

 void LCD_Write_Data(uint8_t Data);

 void LCD_Write_String_Helper(void);

 void LCD_Write_Number_Helper();

 void Write_Date_Time_Task();

 void LCD_Set_Cursor_Helper();

 void LCD_Clear_Display_Helper();

 void LCD_Turn_Off_Cursor_Asynch();

 void LCD_Turn_On_Cursor_Asynch();

 void Switches_Of_Keypad(void);


extern LCD_pin_Cfg LCD_Pins_Config[NUMBER_OF_LCD_PINS];


/*************** enable pin states **************/
#define ENABLE_PIN_LOW				0
#define ENABLE_PIN_HIGH				1


#define READY		0

#define BUSY		1

typedef enum{
	year,
	month,
	day,
	hours,
	minutes,
	seconds,
	milliseconds,
	end
}Date_Time_t;


typedef struct{
	uint8_t * s;
	uint8_t Number[10];
	uint8_t length;
	uint8_t state;
	uint8_t Type;
	uint8_t Cursor_Pos;
	uint8_t Command;
}User_Req;

typedef struct {
	uint8_t X_pos;
	uint8_t Y_pos;
}LCD_XY_Values;

/*********** for the lcd state options **************/
typedef enum{
	Init,
	Operation,
	Off
}LCD_State;

typedef struct{
	uint32_t	Year;
	uint32_t	Month;
	uint32_t	Day;
}Current_Date;

typedef struct
{
	uint32_t 	Hours;
	uint32_t	Minutes;
	uint32_t	Seconds;

}Current_Time;

/***************** request types enum *********************/

typedef enum{
	None,
	Write_Data,
	Write_Number,
	Set_Cursor,
	Clear_Display,
	Turn_Off_Cursor,
	Turn_On_Cursor

}LCD_Requests_options;

/******************* for the init function to initilize the lcd **************/
typedef enum {
	Power_On,
	Function_Set,
	Display_On_Off,
	Display_Clear,
	End
}Init_Modes;

/************************ global variables *********************/

LCD_State Lcd_State=Off;
 
Init_Modes Mode_Of_Init=Power_On;

uint8_t Enable_Pin_State=ENABLE_PIN_LOW;


//static uint8_t Time_Out_Counter=0;

LCD_XY_Values Cordinates={
.X_pos=0,
.Y_pos=6
};

Date_Time_t Current=year;

/************** to get the current time to display *******************/
Current_Time Time={
.Hours=11,
.Minutes=22,
.Seconds=14
};

Current_Time Stop_Watch_Time={
	.Hours=0,
	.Minutes=0,
	.Seconds=0
};


Current_Date Date={
		.Year=2024,
		.Month=4,
		.Day=5
};

static User_Req Req[NUMBER_OF_REQUESTES]
={
	[0]={.s=NULL,
		.length=0,
		.state=READY,
		.Type=None,
		.Cursor_Pos=0,
		.Command=0,
		.Number={0}
	},

	[1]={.s=NULL,
		.length=0,
		.state=READY,
		.Type=None,
		.Cursor_Pos=0,
		.Command=0,
		.Number={0}
	},

	[2]={.s=NULL,
		.length=0,
		.state=READY,
		.Type=None,
		.Cursor_Pos=0,
		.Command=0,
		.Number={0}
	},
	[3]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[4]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
			},
			[5]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
				[6]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[7]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[8]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[9]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[10]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[11]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[12]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},
		
		[13]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[14]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[15]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[16]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},
			[17]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},
			[18]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},
		[19]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},
		
		[20]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[21]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[22]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[23]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},

			[24]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}}
		,
		[25]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}},
		
		[26]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[27]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[28]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[29]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}}
	
};
/*********** variable to hold the index needed to be executed *******/
volatile static uint8_t Request_Index=0;


/******** to simulate the recevied buffer from uart*/
volatile uint8_t UART_Buffer[2] = {0};

/*************** to set the cursor position on the first time of edit mode***/
volatile static uint8_t Counter_to_Set_Cursor_In_Edit_Mode=0;

volatile uint8_t Current_Display_Mode=DATE_TIME_MODE;

void LCD_Test(void){
	static uint8_t c=0;
	c++;
	if(c==5){
		// change mode
		UART_Buffer[1] = 0x10;
	}
	if(c==10){
		//Begin
		UART_Buffer[1] = 0x30;
		//c=0;
	}
}

uint8_t sign1[1]={"/"};
uint8_t sign2[1]={":"};

uint8_t clock_Mode[6]="Date: ";
uint8_t Time_Mode[6]="Time: ";

uint8_t Stop_Watch[]={"StoP Watch"};

uint8_t Change_Of_Time=CHANGED;


/// flag for the stop watch to start counting or to continue
uint8_t Begin=0;

/// task every 120 m
void Switches_Of_Keypad(void){
	if(Current_Display_Mode== DATE_TIME_MODE){
		//// UARTFARME[1] is the recievied command from the uart
		 if (UART_Buffer[1] == CHANGE_MODE)//Mode Button
            {
                Current_Display_Mode = STOP_WATCH_MODE;
                Display_Stop_Watch_Helper();
            }
            else if (UART_Buffer[1] == EDIT_MODE) //Edit Mode Button
            {
                Current_Display_Mode = EDIT_MODE;
				LCD_Turn_On_Cursor_Asynch();
				LCD_Set_Cursor_Asynch(0, 6);
				Cordinates.X_pos=0;
				Cordinates.Y_pos=6;
            }
            else{
            			/* do nothing */
            		}
		 UART_Buffer[1] = IDLE_MESSAGE;
	}
	else if (Current_Display_Mode == STOP_WATCH_MODE){
		//// Begin Button
		if(UART_Buffer[1] == START){
			Begin=1;
		}
		// Mode Button
		else if(UART_Buffer[1] == CHANGE_MODE){
			Current_Display_Mode= DATE_TIME_MODE;
			Change_Of_Time = CHANGED;
		}
		// stop button
		else if (UART_Buffer[1]==0x40){
			Begin=0;
		}
		//for reset
		else if (UART_Buffer[1]==RESET){
			Begin=0;
			Stop_Watch_Time.Seconds=0;
			Stop_Watch_Time.Minutes=0;
			Stop_Watch_Time.Hours=0;
			Display_Stop_Watch_Helper();
		}
		else{
			/* do nothing */
		}
		UART_Buffer[1] = IDLE_MESSAGE;
	}
		else if (Current_Display_Mode==EDIT_MODE){
			/******** to find if the edit bit equal 1 ************/

			switch (UART_Buffer[1])
			{
			case RIGHT:

				Cordinates.Y_pos++;
				if (Cordinates.Y_pos > MAX_LINE_SIZE)
				{
					/**** to set the column to the first one****/
					Cordinates.Y_pos = 6;
					/****** to increment the rows if the columns exceeds the max*/
					Cordinates.X_pos++;
					/****** to check if exceeded the number of rows available (2)***/
					if (Cordinates.X_pos > 1)
					{
						Cordinates.X_pos = 0;
					}
					
				}
				LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
				break;
			case LEFT:

				Cordinates.Y_pos--;
				if (Cordinates.Y_pos < 6)
				{
					/**** to set the column to the first one****/
					Cordinates.Y_pos = MAX_LINE_SIZE;
					/****** to increment the rows if the columns exceeds the max*/
					Cordinates.X_pos--;
					/****** to check if exceeded the number of rows available (2)***/
					if (Cordinates.X_pos < MIN_LINE_SIZE)
					{
						Cordinates.X_pos = 1;
					}
				}
				LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
				break;
			case INC:
				if (Cordinates.X_pos == 0 && (5 < Cordinates.Y_pos && Cordinates.Y_pos < 10))
				{
					Date.Year++;
				}
				else if (Cordinates.X_pos == 0 && (10 < Cordinates.Y_pos && Cordinates.Y_pos  < 13))
				{
					Date.Month++;
				}
				else if (Cordinates.X_pos == 0 && (13 < Cordinates.Y_pos && Cordinates.Y_pos  < 15))
				{
					Date.Day++;
				}
				else if (Cordinates.X_pos == 1 && (5 < Cordinates.Y_pos && Cordinates.Y_pos  < 8))
				{
					Time.Hours++;
				}

				else if (Cordinates.X_pos == 1 && (8 < Cordinates.Y_pos && Cordinates.Y_pos  < 11))
				{
					Time.Minutes++;
				}
				else if (Cordinates.X_pos == 1 && (11 < Cordinates.Y_pos && Cordinates.Y_pos  < 14))
				{
					Time.Seconds++;
				}
				Handle_Time_Edit_For_Date_Time_Mode();
				Display_Date_Time_Helper();
				LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
				break;

			case DEC:
				if (Cordinates.X_pos == 0 && (5 < Cordinates.Y_pos && Cordinates.Y_pos  < 10))
				{
					Date.Year--;
				}
				else if (Cordinates.X_pos == 0 && (10 < Cordinates.Y_pos && Cordinates.Y_pos  < 13))
				{
					Date.Month--;
				}
				else if (Cordinates.X_pos == 0 && (13 < Cordinates.Y_pos && Cordinates.Y_pos  < 15))
				{
					Date.Day--;
				}
				else if (Cordinates.X_pos == 1 && (5 < Cordinates.Y_pos && Cordinates.Y_pos  < 8))
				{
					Time.Hours--;
				}

				else if (Cordinates.X_pos == 1 && (8 < Cordinates.Y_pos && Cordinates.Y_pos  < 11))
				{
					Time.Minutes--;
				}
				else if (Cordinates.X_pos == 1 && (11 < Cordinates.Y_pos && Cordinates.Y_pos  < 14))
				{
					Time.Seconds--;
				}
				Handle_Time_Edit_For_Date_Time_Mode();
				Display_Date_Time_Helper();
				LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
				break;

				case OK:
				Current_Display_Mode = DATE_TIME_MODE;
				Change_Of_Time = CHANGED;
				LCD_Turn_Off_Cursor_Asynch();
				break;

			}
		}
}

/*** task to write the date and time on lcd ***/
void Write_Date_Time_Task()
{

	if (Current_Display_Mode == DATE_TIME_MODE)
	{
		if (Change_Of_Time == CHANGED)
		{

			Display_Date_Time_Helper();
			Change_Of_Time = NOT_CHANGED;
		}

		}
		else if (Current_Display_Mode == STOP_WATCH_MODE)
		{
			//Display_Stop_Watch_Helper();
		}
	
		}


	void Display_Stop_Watch_Helper(){
		LCD_Clear_Display_Asynch();
		LCD_Set_Cursor_Asynch(0, 4);
		LCD_Write_String_NoCopy(Stop_Watch,10);
		LCD_Set_Cursor_Asynch(1, 4);
		LCD_Wrtite_Number_Asynch(Stop_Watch_Time.Hours);
		LCD_Write_String_NoCopy(sign2,1);
		//LCD_Set_Cursor_Asynch(1, 7);
		LCD_Wrtite_Number_Asynch(Stop_Watch_Time.Minutes);
		LCD_Write_String_NoCopy(sign2,1);
		//LCD_Set_Cursor_Asynch(1,10);
		LCD_Wrtite_Number_Asynch(Stop_Watch_Time.Seconds);
	}

	void Display_Date_Time_Helper(){
		LCD_Clear_Display_Asynch();
			/**** to init the cursor position */
			LCD_Set_Cursor_Asynch(0, 0);
			LCD_Write_String_NoCopy(clock_Mode,6);
			/* to print the date : YEAR/MONTH/DAY */
			LCD_Wrtite_Number_Asynch(Date.Year);
			LCD_Write_String_NoCopy(sign1,1);
			//LCD_Set_Cursor_Asynch(0, 5);
			// LCD_Write_Data('/');
			LCD_Wrtite_Number_Asynch(Date.Month);
			LCD_Write_String_NoCopy(sign1,1);
			//LCD_Set_Cursor_Asynch(0, 8);
			// LCD_Write_Data('/');
			LCD_Wrtite_Number_Asynch(Date.Day);

			/* Set the cursor on the second Line */
			LCD_Set_Cursor_Asynch(1, 0);
			LCD_Write_String_NoCopy(Time_Mode,6);

			/* to print the Time : HOURS:MINUTES:SECONDS */
			LCD_Wrtite_Number_Asynch(Time.Hours);
			// LCD_Write_Data(':');
			//LCD_Set_Cursor_Asynch(1, 3);
			LCD_Write_String_NoCopy(sign2,1);
			LCD_Wrtite_Number_Asynch(Time.Minutes);
			// LCD_Write_Data(':');
			LCD_Write_String_NoCopy(sign2,1);
			//LCD_Set_Cursor_Asynch(1, 6);
			LCD_Wrtite_Number_Asynch(Time.Seconds);
			//LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
	}
	// task every 1000 miliSecond
	void Sec_Increment_Task_For_Date_Time(void)
	{
		
		Handle_Time_Edit_For_Date_Time_Mode();
		Change_Of_Time = CHANGED;
	}

	void Sec_Increment_Task_For_Stop_Watch(void)
	{
		Handle_Time_Edit_For_Stop_Watch_Mode();
	}


void Handle_Time_Edit_For_Stop_Watch_Mode(){
	if(Begin==1){
		Stop_Watch_Time.Seconds++;
	if (Stop_Watch_Time.Seconds == 60)
		{
			Stop_Watch_Time.Minutes++;
			Stop_Watch_Time.Seconds = 0;
		}
		else
		{
			// do nothing
		}
		if (Stop_Watch_Time.Minutes == 60)
		{
			Stop_Watch_Time.Hours++;
			Stop_Watch_Time.Minutes = 0;
		}
		else
		{
			// do nothing
		}
	}
	else{
		//do noting
	}
	if(Current_Display_Mode == STOP_WATCH_MODE&&Begin){
	Display_Stop_Watch_Helper();
	}
}


	void Handle_Time_Edit_For_Date_Time_Mode()
	{
		if(Current_Display_Mode==DATE_TIME_MODE||Current_Display_Mode==STOP_WATCH_MODE){
		Time.Seconds++;
		}
		if (Time.Seconds == 60)
		{
			Time.Minutes++;
			Time.Seconds = 0;
		}
		else
		{
			// do nothing
		}
		if (Time.Minutes == 60)
		{
			Time.Hours++;
			Time.Minutes = 0;
		}
		else
		{
			// do nothing
		}
		if (Time.Hours == 24)
		{
			Date.Day++;
			Time.Hours = 0;
		}
		else
		{
			// do nothing
		}
		if (Date.Day > 30)
		{
			Date.Month++;
			Date.Day = 1;
		}
		else
		{
			// do nothing
		}
		if (Date.Month >12)
		{
			Date.Year++;
			Date.Month = 1;
		}
		else
		{
			// do nothing
		}
	}

void LCD_Init_Asynch(){
	 Lcd_State=Init;
}

/******************* LCD Runnable *****************/
void LCD_Task(){
	if(Lcd_State==Init){
		Init_Sm();
	}
	else if (Lcd_State==Operation){
		switch(Req[Request_Index].Type){
		case Write_Data:
			//LCD_enuGotoDDRAM_XY(Cordinates.X_pos,Cordinates.Y_pos);
			LCD_Write_String_Helper();
		break;
		case Clear_Display:
			LCD_Clear_Display_Helper();
			break;
		case Set_Cursor:
			LCD_Set_Cursor_Helper();
			break;
		case Write_Number:
			LCD_Write_Number_Helper();
		break;
		case Turn_Off_Cursor:
			LCD_Set_Cursor_Helper();
			break;
		case Turn_On_Cursor:
			LCD_Set_Cursor_Helper();
			break;	
		default :
					Request_Index=0;
					break;
		}

	}
//to reset the Request index
	if(Request_Index==NUMBER_OF_REQUESTES){
		Request_Index=0;
	}

}

void LCD_Set_Cursor_Asynch(uint8_t Copy_u8Row, uint8_t Copy_u8Column){
	uint8_t Loc_u8Location =0;
	uint8_t counter=0;
	for(counter=0;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){
		Req[counter].state=BUSY;
		Req[counter].Type=Set_Cursor;
		if(Copy_u8Row==FIRST_LINE){
			Loc_u8Location=Copy_u8Column;
		}
		else if (Copy_u8Row==SECOND_LINE){
			Loc_u8Location=Copy_u8Column+0x40 ;
		}
		else{
			//do noting
		}
		Req[counter].Command=LCD_SET_DDRAM_ADRESS+Loc_u8Location;
		counter=NUMBER_OF_REQUESTES; // to terminate from the loop
	}
	}
}





/******************** function to init the lcd ************/
void Init_Sm(){
static uint8_t counter=0;
GPIO_PinConfig_t pin;
uint8_t idx=0;
counter++;
switch (Mode_Of_Init){
case Power_On:
/***** to initialize the lcd pins ************/
	if(counter==1){
	pin.Mode=GPIO_OUTPUT_PP;
	pin.Speed=GPIO_HIGH_SPEED;
	for(idx=0;idx<NUMBER_OF_LCD_PINS;idx++){
		pin.Pin=LCD_Pins_Config[idx].pin;
		pin.Port=LCD_Pins_Config[idx].port;
		//pin.Af=0;
		GPIO_InitPin(&pin);
	}
	}
	else if (counter==15){
		counter=0;
	Mode_Of_Init=Function_Set;
	}
	else{
		//do nothing
	}
	break;
case Function_Set:
	LCD_Write_Command(LCD_FUNCTION_SET_MASK);
		if(Enable_Pin_State==ENABLE_PIN_LOW){
		Mode_Of_Init=Display_On_Off;
		}
	break;
case Display_On_Off:
	LCD_Write_Command(LCD_DISPLAY_ON_OFF_MASK);
	if(Enable_Pin_State==ENABLE_PIN_LOW){
		Mode_Of_Init=Display_Clear;
	}
	break;
case Display_Clear:
	LCD_Write_Command(LCD_DISPLAY_CLEAR_MASK);
	if(Enable_Pin_State==ENABLE_PIN_LOW){
		Mode_Of_Init=End;
	}
	break;
case End:
	Lcd_State=Operation;
	break;

}
}


/********************* to get the string from the user **************/
void LCD_Write_String_NoCopy(uint8_t * str,uint32_t length){
	uint8_t counter=0;
	if(str){
			for(counter=0;counter<NUMBER_OF_REQUESTES;counter++){
if(Req[counter].state==READY){
	Req[counter].state=BUSY;
	Req[counter].s=str;
	Req[counter].length=length;
	Req[counter].Type=Write_Data;
	counter=NUMBER_OF_REQUESTES; // to terminate from the loop
}
			}
	}
}

/************************* to get command request from the user *************/
void LCD_Clear_Display_Asynch(){
	uint8_t counter=0;
	for(counter=0;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){
		Req[counter].state=BUSY;
		Req[counter].Command=LCD_DISPLAY_CLEAR_MASK;
		Req[counter].Type=Clear_Display;
		counter=NUMBER_OF_REQUESTES; // to terminate from the loop
}
	}
}

/****************** helper function to clear the displayy**********/
void LCD_Clear_Display_Helper(){
LCD_Write_Command(Req[Request_Index].Command);
if(Enable_Pin_State==ENABLE_PIN_LOW){
	Req[Request_Index].Type=None;
	Req[Request_Index].state=READY;
	Request_Index++;
	}
}

void LCD_Turn_Off_Cursor_Asynch(){
uint8_t counter=0;
	for(counter=0;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){
		Req[counter].state=BUSY;
		Req[counter].Command=TURN_OF_CURSOR;
		Req[counter].Type=Turn_Off_Cursor;
		counter=NUMBER_OF_REQUESTES; // to terminate from the loop
}
	}
}

void LCD_Turn_On_Cursor_Asynch(){
	uint8_t counter=0;
	for(counter=0;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){
		Req[counter].state=BUSY;
		Req[counter].Command=TURN_ON_CURSOR;
		Req[counter].Type=Turn_On_Cursor;
		counter=NUMBER_OF_REQUESTES; // to terminate from the loop
}
	}
}


/******************* function to write number *************/
void LCD_Wrtite_Number_Asynch(uint32_t Copy_number){
	uint8_t counter=0;
	uint8_t digit=0;
	// Array to hold digits in reverse order
	uint32_t index = 0;
		for(counter=0;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){

		Req[counter].state=BUSY;
		Req[counter].Type=Write_Number;

		if(Copy_number == 0) {
			Req[counter].Number[0]='0';
			Req[counter].Number[1]='0';
			index=2;
				}

				if(Copy_number < 0) {
					LCD_Write_Data('-');
					Copy_number = -Copy_number; // Convert negative number to positive
				}


				if(0 <Copy_number&& Copy_number< 10) {
			Req[counter].Number[0]='0'+Copy_number;
			Req[counter].Number[1]='0';
			index=2;
				}
				else
				{
				// Extract digits from right to left
				while(Copy_number > 0) {
					digit = Copy_number % 10;
					Req[counter].Number[index++] = digit + '0'; // Convert digit to ASCII character
					Copy_number /= 10;
				}
				}
				Req[counter].length=index;
				counter=NUMBER_OF_REQUESTES; // to terminate from the loop
	}


	}

}

/************************ helper function to write number ************/
void LCD_Write_Number_Helper(){
		static uint8_t iterator=0;
/******************** set the needed position **************/


	if(iterator<Req[Request_Index].length){
			LCD_Write_Data(Req[Request_Index].Number[Req[Request_Index].length-iterator-1]);
			if(Enable_Pin_State==ENABLE_PIN_LOW){
			iterator++;
			}

	}
	// when writing the string finish
	else{
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		for (int i = 0; i <Req[Request_Index].length; i++)
		{
			Req[Request_Index].Number[i] = 0;
		}
		
		// memset(Req[Request_Index].Number, 0, sizeof(Req[Request_Index].Number));
		Request_Index++;
		iterator=0;
	}
}

/********************* helper function to write the string on lcd *******/
void LCD_Write_String_Helper(void){
	static uint8_t iterator=0;
/******************** set the needed position **************/


	if(iterator<Req[Request_Index].length){
			LCD_Write_Data(Req[Request_Index].s[iterator]);
			if(Enable_Pin_State==ENABLE_PIN_LOW){
			iterator++;
			}

	}
	// when writing the string finish
	else{
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		Request_Index++;
		iterator=0;
	}
}

void LCD_Write_Command_Helper(){
	LCD_Write_Command(Req[Request_Index].s[0]);
	if(Enable_Pin_State==ENABLE_PIN_LOW){
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		Request_Index++;
	}
}


void LCD_Set_Cursor_Helper()
{
LCD_Write_Command(Req[Request_Index].Command);
if(Enable_Pin_State==ENABLE_PIN_LOW){
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		Request_Index++;
	}
}


/******************* to send a one byte command to Lcd ***********/
void LCD_Write_Command(uint8_t Command){


if(Enable_Pin_State==ENABLE_PIN_LOW){


/******* to set the RW and RS pins to LOw ************/
GPIO_Set_PinValue(LCD_Pins_Config[RW_PIN].port, LCD_Pins_Config[RW_PIN].pin, GPIO_LOW);
GPIO_Set_PinValue(LCD_Pins_Config[RS_PIN].port, LCD_Pins_Config[RS_PIN].pin, GPIO_LOW);
/************* copying the command bits to the physical bits ************/
GPIO_Set_PinValue(LCD_Pins_Config[D0_PIN].port, LCD_Pins_Config[D0_PIN].pin,((Command>>D0_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D1_PIN].port, LCD_Pins_Config[D1_PIN].pin,((Command>>D1_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D2_PIN].port, LCD_Pins_Config[D2_PIN].pin,((Command>>D2_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D3_PIN].port, LCD_Pins_Config[D3_PIN].pin,((Command>>D3_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D4_PIN].port, LCD_Pins_Config[D4_PIN].pin,((Command>>D4_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D5_PIN].port, LCD_Pins_Config[D5_PIN].pin,((Command>>D5_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D6_PIN].port, LCD_Pins_Config[D6_PIN].pin,((Command>>D6_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D7_PIN].port, LCD_Pins_Config[D7_PIN].pin,((Command>>D7_PIN)&1));

/****************** set the enable pin to high ****************/
GPIO_Set_PinValue(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, GPIO_HIGH);
Enable_Pin_State=ENABLE_PIN_HIGH;
}
else if(Enable_Pin_State==ENABLE_PIN_HIGH){
		GPIO_Set_PinValue(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, GPIO_LOW);
		Enable_Pin_State=ENABLE_PIN_LOW;
}

}


/***************** to write a byte of data on lcd *************/

void LCD_Write_Data(uint8_t Data){

	if(Enable_Pin_State==ENABLE_PIN_LOW){
/******* to set the RW and RS pins to LOw ************/
GPIO_Set_PinValue(LCD_Pins_Config[RW_PIN].port, LCD_Pins_Config[RW_PIN].pin, GPIO_LOW);
GPIO_Set_PinValue(LCD_Pins_Config[RS_PIN].port, LCD_Pins_Config[RS_PIN].pin, GPIO_HIGH);
/************* copying the command bits to the physical bits ************/
GPIO_Set_PinValue(LCD_Pins_Config[D0_PIN].port, LCD_Pins_Config[D0_PIN].pin,((Data>>D0_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D1_PIN].port, LCD_Pins_Config[D1_PIN].pin,((Data>>D1_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D2_PIN].port, LCD_Pins_Config[D2_PIN].pin,((Data>>D2_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D3_PIN].port, LCD_Pins_Config[D3_PIN].pin,((Data>>D3_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D4_PIN].port, LCD_Pins_Config[D4_PIN].pin,((Data>>D4_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D5_PIN].port, LCD_Pins_Config[D5_PIN].pin,((Data>>D5_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D6_PIN].port, LCD_Pins_Config[D6_PIN].pin,((Data>>D6_PIN)&1));
GPIO_Set_PinValue(LCD_Pins_Config[D7_PIN].port, LCD_Pins_Config[D7_PIN].pin,((Data>>D7_PIN)&1));

/****************** set the enable pin to high ****************/
GPIO_Set_PinValue(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, GPIO_HIGH);
Enable_Pin_State=ENABLE_PIN_HIGH;
	}

	else if(Enable_Pin_State==ENABLE_PIN_HIGH){
			GPIO_Set_PinValue(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, GPIO_LOW);
			Enable_Pin_State=ENABLE_PIN_LOW;
	}

}
/*
else if(Current_Display_Mode==EDIT_MODE){
		if(Counter_to_Set_Cursor_In_Edit_Mode==0){
		LCD_Set_Cursor_Asynch(0, 0);
		/******* to set the cursor at first line in the first time *******/
		/*Counter_to_Set_Cursor_In_Edit_Mode++;
		}/*
		switch(UART_Buffer){
		case RIGHT:

			Cordinates.Y_pos++;
			if(Cordinates.Y_pos>MAX_LINE_SIZE){
	/**** to set the column to the first one****/
				//Cordinates.Y_pos=0;
	/****** to increment the rows if the columns exceeds the max*/
				//Cordinates.X_pos++;
	/****** to check if exceeded the number of rows available (2)***/
				/*if(Cordinates.X_pos>1){
					Cordinates.X_pos=0;
				}
			LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
			}
		break;
		case LEFT:

					Cordinates.Y_pos--;
					if(Cordinates.Y_pos<MIN_LINE_SIZE){
			/**** to set the column to the first one****/
					//	Cordinates.Y_pos=MAX_LINE_SIZE;
			/****** to increment the rows if the columns exceeds the max*/
						//Cordinates.X_pos--;
			/****** to check if exceeded the number of rows available (2)***/
						////*if(Cordinates.X_pos<MIN_LINE_SIZE){
							/* Cordinates.X_pos=1; */
						/*}
					LCD_Set_Cursor_Asynch(Cordinates.X_pos, Cordinates.Y_pos);
					}
					break;/*

		}

	}

	/******** to get the mode bit from the received buffer from uart***/
	//Current_Display_Mode = (modecomm & 1 << 6);
	/*to clear the display at the first time of this task */