
#include "port.h"

// #define NULL ((void*)0)


#define GPIO_GETMODE_MASK       0x00000003
#define GPIO_GETOUT_TYPE_MASK   0x00000004
#define GPIO_GETPULL_TYPE_MASK  0x00000018

#define GPIO_OUT_TYPE_SHIFT         2
#define GPIO_PULL_TYPE_SHIFT        3

#define GPIO_CLEAR_MODE_BITS        0x00000003
#define GPIO_CLEAR_OUTTYPE_BIT      0x00000001
#define GPIO_CLEAR_PUPD_BITS        0x00000003
#define GPIO_CLEAR_SPEED_BITS       0x00000003

#define SHIFT_MULTIPLICATION_FACTOR       					0x00000002



typedef struct
{
    u32 volatile GPIO_MODER;
    u32 volatile GPIO_OTYPER;
    u32 volatile GPIO_OSPEEDR;
    u32 volatile GPIO_PUPDR;
    u32 volatile GPIO_IDR;
    u32 volatile GPIO_ODR;
    u32 volatile GPIO_BSRR;
    u32 volatile GPIO_LCKR;
    u32 volatile GPIO_AFRL;
    u32 volatile GPIO_AFRH;
}GPIO_PortRegisters;

#define GPIO_A							((volatile GPIO_PortRegisters*)(GPIOA_BASEADD))
#define GPIO_B							((volatile GPIO_PortRegisters*)(GPIOB_BASEADD))
#define GPIO_C							((volatile GPIO_PortRegisters*)(GPIOC_BASEADD))
#define GPIO_D							((volatile GPIO_PortRegisters*)(GPIOD_BASEADD))
#define GPIO_E							((volatile GPIO_PortRegisters*)(GPIOE_BASEADD))
#define GPIO_H							((volatile GPIO_PortRegisters*)(GPIOH_BASEADD))



// #define HIGH 1
// #define LOW  0


GPIO_enuErrorStatus_t GPIO_InitPin (GPIO_PinConfig_t *ptrToPinConfig)
{
	GPIO_enuErrorStatus_t Ret_enuErrorStatusGPIO = GPIO_enuOk;

	  if (ptrToPinConfig == NULL)
	   		    {
	   		         Ret_enuErrorStatusGPIO = GPIO_NULLPOINTER ;
	   		    }
	   		    else if ((ptrToPinConfig->Port > GPIOH_BASEADD) || (ptrToPinConfig->Pin > GPIO_PIN15))
	   		    {
	   		    	Ret_enuErrorStatusGPIO = GPIO_WRONGPortORPin ;
	   		    }
	   		    else
	   		    {
    u32 Loc_MODER=0;
    u32 Loc_OTYPER=0;
	u32 Loc_OSPEEDR=0;
	u32 Loc_PUPDR=0;
	u32 Loc_ModeValue=0x00000000;
	u32 Loc_OutTypeValue=0x00000000;
	u32 Loc_PUPDValue=0x00000000;

	/*put the first 2 bits in loc_Modevalue to indicate mode of operation (input,output,AF,Analog)*/
	Loc_ModeValue= (ptrToPinConfig->Mode) & GPIO_GETMODE_MASK ;

	/*put the next 1 bits in loc_OutTypevalue to indicate the output status (Output pushpull, Output opendrain)*/
	Loc_OutTypeValue=((ptrToPinConfig->Mode) & GPIO_GETOUT_TYPE_MASK)>>GPIO_OUT_TYPE_SHIFT;

	/*put the next 2 bits in loc_PUPDValue to indicate the pull type status (no PU or PD, PU , PD)*/
	Loc_PUPDValue=((ptrToPinConfig->Mode)& GPIO_GETPULL_TYPE_MASK)>>GPIO_PULL_TYPE_SHIFT;

	/*COPY the address of MODER according to the provided port base address (GPIOA_BASEADD,GPIOB_BASEADD,GPIOC_BASEADD,GPIOD_BASEADD,GPIOE_BASEADD,GPIOH_BASEADD)to Loc_MODER */
    Loc_MODER=((GPIO_PortRegisters*)(ptrToPinConfig->Port))->GPIO_MODER;

    /*COPY the address of OTYPER REG according to the provided port base address (GPIOA_BASEADD,GPIOB_BASEADD,GPIOC_BASEADD,GPIOD_BASEADD,GPIOE_BASEADD,GPIOH_BASEADD)to Loc_OTYPER */
    Loc_OTYPER=((GPIO_PortRegisters*)(ptrToPinConfig->Port))->GPIO_OTYPER;

    /*COPY the address of OSPEED REG according to the provided port base address (GPIOA_BASEADD,GPIOB_BASEADD,GPIOC_BASEADD,GPIOD_BASEADD,GPIOE_BASEADD,GPIOH_BASEADD)to Loc_OSPEEDR*/
    Loc_OSPEEDR=((GPIO_PortRegisters*)(ptrToPinConfig->Port))->GPIO_OSPEEDR;

    /*COPY the address of PUPDR REG according to the provided port base address (GPIOA_BASEADD,GPIOB_BASEADD,GPIOC_BASEADD,GPIOD_BASEADD,GPIOE_BASEADD,GPIOH_BASEADD)to Loc_PUPDR*/
    Loc_PUPDR=((GPIO_PortRegisters*)(ptrToPinConfig->Port))->GPIO_PUPDR;



    /*Clear bits which will be configured according to the provided configuration from the user*/
    Loc_MODER    &= ~((GPIO_CLEAR_MODE_BITS)  <<((ptrToPinConfig->Pin)*SHIFT_MULTIPLICATION_FACTOR));
    Loc_OSPEEDR  &= ~((GPIO_CLEAR_SPEED_BITS) <<((ptrToPinConfig->Pin)*SHIFT_MULTIPLICATION_FACTOR));
    Loc_PUPDR    &= ~((GPIO_CLEAR_PUPD_BITS)  <<((ptrToPinConfig->Pin)*SHIFT_MULTIPLICATION_FACTOR));
    Loc_OTYPER   &= ~((GPIO_CLEAR_OUTTYPE_BIT)<<((ptrToPinConfig->Pin)));

    /*SET bits which will be configured according to the provided configuration from the user*/
     Loc_MODER    |= ((Loc_ModeValue)  <<((ptrToPinConfig->Pin)*SHIFT_MULTIPLICATION_FACTOR));
     Loc_OSPEEDR  |= ((ptrToPinConfig->Speed) <<((ptrToPinConfig->Pin)*SHIFT_MULTIPLICATION_FACTOR));
     Loc_PUPDR    |= ((Loc_PUPDValue)  <<((ptrToPinConfig->Pin)*SHIFT_MULTIPLICATION_FACTOR));
     Loc_OTYPER   |= ((Loc_OTYPER)<<((ptrToPinConfig->Pin)));

/*write the configuration on the register(configured by the user) to init the pin*/
    ((GPIO_PortRegisters *)(ptrToPinConfig -> Port)) -> GPIO_MODER   = Loc_MODER;
    ((GPIO_PortRegisters *)(ptrToPinConfig -> Port)) -> GPIO_OTYPER  = Loc_OTYPER;
    ((GPIO_PortRegisters *)(ptrToPinConfig -> Port)) -> GPIO_OSPEEDR = Loc_OSPEEDR;
    ((GPIO_PortRegisters *)(ptrToPinConfig -> Port)) -> GPIO_PUPDR   = Loc_PUPDR;
   		    }

return Ret_enuErrorStatusGPIO;


}


GPIO_enuErrorStatus_t  GPIO_Set_PinValue(void *Port , u32 PinNum , u32 PinStatus )
     {
		GPIO_enuErrorStatus_t Ret_enuErrorStatusGPIO = GPIO_enuOk;
		 if (Port == NULL)
		    {
		         Ret_enuErrorStatusGPIO = GPIO_NULLPOINTER ;
		    }
		    else if ((Port > GPIOH_BASEADD) || (PinNum > GPIO_PIN15))
		    {
		    	Ret_enuErrorStatusGPIO = GPIO_WRONGPortORPin ;
		    }
		    else if ((PinStatus != GPIO_HIGH ) && (PinStatus != GPIO_LOW ))
		    {
		    	Ret_enuErrorStatusGPIO = GPIO_WRONGStatus ;
		    }
		    else
		    {
		    	switch (PinStatus)
		    	{
		    	case GPIO_HIGH: ((GPIO_PortRegisters *)Port )->GPIO_BSRR = (BSSR_BIT_VALUE<< PinNum) ;
		    		break;

		    	case GPIO_LOW:
		    		((GPIO_PortRegisters *)Port )->GPIO_BSRR = (BSSR_BIT_VALUE<< (PinNum+BSSR_OFFSET)) ;
		    		break;

		    	default: break;




		    	}


		    }
		    return Ret_enuErrorStatusGPIO ;

 }

 GPIO_enuErrorStatus_t GPIO_GetPinValue(void *Port,u32 PinNum,  u32 * PinStatus)

 {
	 GPIO_enuErrorStatus_t Ret_enuErrorStatusGPIO = GPIO_enuOk;

	 		 if (Port == NULL)
	 		    {
	 		         Ret_enuErrorStatusGPIO = GPIO_NULLPOINTER ;
	 		    }
	 		    else if ((Port > GPIOH_BASEADD) || (PinNum > GPIO_PIN15))
	 		    {
	 		    	Ret_enuErrorStatusGPIO = GPIO_WRONGPortORPin ;
	 		    }
	 		    else if (PinStatus==NULL)
	 		    {
	 		    	Ret_enuErrorStatusGPIO = GPIO_WRONGStatus ;
	 		    }

	 		    else
	 		    {
					 *PinStatus=(((GPIO_PortRegisters *)Port)->GPIO_IDR>>PinNum)&1;
	 		    }



    return Ret_enuErrorStatusGPIO;


 }


