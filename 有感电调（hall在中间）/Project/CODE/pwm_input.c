#include "headfile.h"

#define PWMIN_PIN P00
extern uint16 BLDC_accelerator;
extern uint32 speed_set;
pwmin_struct pwmin;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMB???????
//  @param      void                        
//  @return     void          
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_init(void)
{
    PWMB_PS = 0x0A;		// ??????
    PWMB_CCMR1 = 0x01;	// CC5?????,????TI5FP5?
	PWMB_CCMR2 = 0x02;	// CC6?????,????TI5FP6?
    
	// CC5E ??????
	// CC5P ?????TI5F????
	// CC6E ??????
	// CC6P ?????TI5F????
    PWMB_CCER1 = 0x31;
    
    PWMB_PSCRH = 0;		// ???
	PWMB_PSCRL = 32;    // ???
    PWMB_SMCR = 0x54;	// TS=TI1FP1,SMS=TI1???????
	PWMB_CR1 = 0x01;	// ??PWMB,????
	PWMB_IER = 0x07;	// ??CC1?CC2?UIE??

    pwmin.period = 0;
    pwmin.high_value = 0;
    pwmin.high_time = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMB??????
//  @param      void                        
//  @return     void          
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void pwmb_isr()interrupt 27
{
    uint16 temp;
	if(PWMB_SR1 & 0x02)
	{
		pwmin.period = (PWMB_CCR1H << 8) + PWMB_CCR1L;	    // CC1??????
		PWMB_SR1 = 0;
        
        // ????PWM?????
        pwmin.frequency = sys_clk / (PWMB_PSCRL + 1) / pwmin.period;
	}
	
	if(PWMB_SR1 & 0x04)
	{
		pwmin.high_value = (PWMB_CCR2H << 8) + PWMB_CCR2L;   // CC2???????
		PWMB_SR1 = 0;
        
        // ????????????
        if((30 < pwmin.frequency) && (400 > pwmin.frequency))
        {
            // ??????? ????????1-2ms??? 
            // pwmin.high_time = pwmin.high_value * (PWMB_PSCRL + 1) * 1000 / (sys_clk/1000);
            
            // ??????????33 ???1000???1005us??,???????????,??????
            pwmin.high_time = pwmin.high_value;
            
            if((3000 < pwmin.high_time) || (1000 > pwmin.high_time))
            {
                // ???????????,??????0
                pwmin.throttle = 0;
            }
            else
            {
                if(2000 < pwmin.high_time)
                {
                    pwmin.high_time = 2000;
                }
                
                // ??????
                temp = pwmin.high_time - 1000;
                // ????????? ?????????????0
//                if(temp < ((uint32)1000 * 50 / 690))
//                {
//                    temp = 0;
//                }
                pwmin.throttle = temp;
            }
        }
        
        // ?????
        speed_set = (uint32)pwmin.throttle * 20 / 1000;////被乘数为结果的上限
	}
    
    if(PWMB_SR1 & 0x01)
    {
        PWMB_SR1 = 0;
        // ????????????????
        pwmin.throttle = 0;
        speed_set = 0;
    }
    
}
