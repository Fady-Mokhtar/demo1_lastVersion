
#include "Sched.h"


extern Runnable_t RunnableList[_Number_of_Runnables];
static volatile u32 PendingTicks;

static void Sched();
static void TickCb();


typedef struct
{
    const Runnable_t *  runnable;
    u32 remainingTimeMs;

}RunnableInfo_RunTime_t;

static RunnableInfo_RunTime_t RunnablesList_RunTime[_Number_of_Runnables];


SCHED_enuErrorStatus_t Sched_Init(void)
{
	SCHED_enuErrorStatus_t Ret_enuErrorStatusSCHED = SCHED_enuOK;

	u32 index;
	for (index=0;index<_Number_of_Runnables;index++)
	{
		RunnablesList_RunTime[index].runnable= &RunnableList[index];
		RunnablesList_RunTime[index].remainingTimeMs=RunnableList[index].DelayMs;
	}
	SYSTICK_SetConfig(SYSTICK_MODE_PERIODIC,SYSTICK_CLK_SOURCE_AHB,SYSTICK_ASSERT_ON);
	SYSTICK_SetTimeMS(TICK_TIME_MS);
	SYSTICK_setCallBackFN(TickCb);

	return Ret_enuErrorStatusSCHED;

}


SCHED_enuErrorStatus_t Sched_Start(void)
{
	SCHED_enuErrorStatus_t Ret_enuErrorStatusSCHED = SCHED_enuOK;
	SYSTICK_start();
	while (1)
	{
		if (PendingTicks)
		{
			PendingTicks--;

			Sched();

		}


	}

    return Ret_enuErrorStatusSCHED;

}

static void Sched()

{
	u32 index;
	for (index=0;index<_Number_of_Runnables;index++)
		{
		  if((RunnableList[index].CB)&&(RunnablesList_RunTime[index].remainingTimeMs==0))
		  {
			  RunnableList[index].CB();
			  RunnablesList_RunTime[index].remainingTimeMs=RunnableList[index].PeriodicityMS;

		  }

		  RunnablesList_RunTime[index].remainingTimeMs-=TICK_TIME_MS;
		}

}

static void TickCb()
{
	PendingTicks++;
}
