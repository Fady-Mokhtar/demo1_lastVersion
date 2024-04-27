#include "STD_TYPES.h"
#include "Systick.h"

#define SYSTICK_BASE_ADDRRESS            0xE000E010
#define CLK_sel_MASK                     0x00000004
#define ASSERT_MASK                      0x00000002
#define CLK_ENABLE_DISABLE_MASK          0x00000001
u8 systickmodeFLAG=0;
static SYSTICKCB_t CallBack;
typedef struct 
{
   u32 STK_CTRL;
   u32 STK_LOAD;
   u32 STK_VAL;
   u32 STK_CALIB;
}Systick_Registers;

static volatile Systick_Registers *const Systick =(Systick_Registers *)SYSTICK_BASE_ADDRRESS;


STK_ErrorState_t SYSTICK_SetTimeMS(u32 Copy_of_time)
{
    STK_ErrorState_t retErrorState=systick_OK;
    if(Copy_of_time<0)
    {
        retErrorState=systick_NOK;
    }
    else
    {
        u32 LOC_Cpu_Freq=CPU_FREQ_IN_HZ;
        u32 LOC_Load_Value=0;
        u32 test=Systick->STK_CTRL&CLK_sel_MASK;
        switch (test)
        {
        case SYSTICK_CLK_SOURCE_AHB:
            LOC_Load_Value=((LOC_Cpu_Freq/1000)*Copy_of_time)-1;
            if(LOC_Load_Value<0x00000001||LOC_Load_Value>0x00FFFFFF)
            {
                retErrorState=systick_NOK;
            }
            else
            {
                Systick->STK_LOAD=LOC_Load_Value;
            }
            break;
        case SYSTICK_CLK_SOURCE_AHB_DIV_8_:
            LOC_Load_Value=((LOC_Cpu_Freq/1000*8)*Copy_of_time)-1;
            if(LOC_Load_Value<0x00000001||LOC_Load_Value>0x00FFFFFF)
            {
                retErrorState=systick_NOK;
            }
            else
            {
                Systick->STK_LOAD=LOC_Load_Value;
            }
            break;
        
        default:
            retErrorState=systick_NOK;
            break;
        }
       

    }
    return retErrorState;
}
STK_ErrorState_t SYSTICK_SetConfig(u8 SYSTICK_MODE,u8 SYSTICK_CLK_SOURCE,u8 ASSERT_CFG)
{   
    STK_ErrorState_t retErrorState=systick_OK;
    if(SYSTICK_MODE<SYSTICK_MODE_PERIODIC || SYSTICK_MODE>SYSTICK_MODE_ONETIME ||ASSERT_CFG > SYSTICK_ASSERT_ON || ASSERT_CFG < SYSTICK_ASSERT_OFF || (SYSTICK_CLK_SOURCE !=SYSTICK_CLK_SOURCE_AHB && SYSTICK_CLK_SOURCE !=SYSTICK_CLK_SOURCE_AHB_DIV_8_))
    {
        retErrorState= systick_NOK;
    }
    else
    {
        systickmodeFLAG = SYSTICK_MODE;
        u32 LOC_temp = Systick->STK_CTRL;
        LOC_temp &= ~(CLK_sel_MASK);
        LOC_temp |= (SYSTICK_CLK_SOURCE);
        LOC_temp &= ~(ASSERT_MASK);
        LOC_temp |= (ASSERT_CFG);
        Systick->STK_CTRL=LOC_temp;
    }
    return retErrorState;
}
STK_ErrorState_t SYSTICK_start()
{
    STK_ErrorState_t retErrorState=systick_OK;
    u32 LOC_temp = Systick->STK_CTRL;
    LOC_temp |=(CLK_ENABLE_DISABLE_MASK);
    Systick->STK_CTRL=LOC_temp;
    return retErrorState;
}
STK_ErrorState_t SYSTICK_Stop()
{
    STK_ErrorState_t retErrorState=systick_OK;
    u32 LOC_temp = Systick->STK_CTRL;
    LOC_temp &= ~(CLK_ENABLE_DISABLE_MASK);
    Systick->STK_CTRL=LOC_temp;
    return retErrorState;
}
STK_ErrorState_t SYSTICK_setCallBackFN(SYSTICKCB_t CB)
{
    STK_ErrorState_t retErrorState=systick_OK;
    if(CB)
    {
        CallBack=CB;
    }
    else
    {
        retErrorState=systick_NOK;
    }
    return retErrorState;
}
void SysTick_Handler(void)
{
    if(CallBack)
    {
        CallBack();
    }
}