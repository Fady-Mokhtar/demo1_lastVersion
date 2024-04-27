#ifndef _SCHED_H_
#define _SCHED_H_

#include "STD_TYPES.h"
#include "Runnables_cfg.h"
#include "Systick_cfg.h"
#include "Systick.h"

typedef void (*RunnableCb_t)(void);

typedef struct

{
	char *Runnable_Name;
	u32  PeriodicityMS;
	RunnableCb_t CB;
	u32 DelayMs;
}Runnable_t;

typedef enum
{
	SCHED_enuOK,
	SCHED_enuNOK

}SCHED_enuErrorStatus_t;

SCHED_enuErrorStatus_t Sched_Init(void);
SCHED_enuErrorStatus_t Sched_Start(void);




#endif /* SCHED_H_ */