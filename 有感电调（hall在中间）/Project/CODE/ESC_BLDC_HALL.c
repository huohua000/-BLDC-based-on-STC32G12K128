#include "headfile.h"
#include "common.h"
#include "board.h"
#include "STC32Gxx.h"
void PWM_IO_SET0();
uint16 BLDC_accelerator = 2600;
unsigned int step = 0;
unsigned int last_step = 8;
unsigned int step_counter=0;
void ESC_init()
{
	pwm_init(PWMA_CH1P_P20, 24000, 0);
	pwm_init(PWMA_CH2P_P22, 24000, 0);
	pwm_init(PWMA_CH3P_P24, 24000, 0);
	PWM_IO_SET0();
}
void PWM_IO_SET0()
{
	pwm_duty(PWMA_CH1P_P20, 0); // AH
	pwm_duty(PWMA_CH2P_P22, 0); // BH
	pwm_duty(PWMA_CH3P_P24, 0); // CH
	AL_IO = 0;
	BL_IO = 0;
	CL_IO = 0;
}
void step_test_ex()
{
	LEDA = P35; // HALL_C
	LEDB = P36; // HALL_B
	LEDC = P37; // HALL_A
}
void delay_500ns(void)
{
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	
}
void StepMotor(int direction) // 换相序列函数
{
	//	PWMB_IER   = 0;
	//	PWMB_CCER1 = 0;
	//	PWMB_CCER2 = 0;
	step = 0;
	step |= P35; // HALL_C
	step <<= 1;
	step |= P36; // HALL_B
	step <<= 1;
	step |= P37; // HALL_A
	//	step = P2 & 0x07;	//P2.0-HALL_A P2.1-HALL_B P2.2-HALL_C
	if (last_step != step)
	{
		last_step = step;
		step_counter++;
		LEDB=LEDA;
		LEDA=!LEDA;
		
		if (direction) // 顺时针
		{
			switch (step)
			{
			case 4: // 010, P2.0-HALL_A下降沿  PWM3, PWM2_L=1		//顺时针
				// PWMA_ENO = 0x00;	PWM1_L=0;	PWM3_L=0;
				// PWM_IO_SET0();
				//				pwm_duty(PWMA_CH3P_P24, 0);
				
				AL_IO = 0;
				pwm_duty(PWMA_CH3P_P24, 0);
				delay_500ns();
				pwm_duty(PWMA_CH3P_P24, BLDC_accelerator); // 打开C相的高端PWM
				BL_IO = 1;								   // 打开B相的低端
														   //				PWMB_CCER2 = (0x01+0x00);	//P2.2 0x01:允许输入捕获, +0x00:上升沿, +0x02:下降沿
														   //				PWMB_IER   = 0x08;			//P2.2 使能中断
				break;
			case 0: // 110, P2.2-HALL_C上升沿  PWM3, PWM1_L=1
					//				PWMA_ENO = 0x10;	PWM2_L=0;	PWM3_L=0;	// 打开C相的高端PWM
					//				Delay_500ns();
					//				PWM1_L = 1;			// 打开A相的低端
					//				PWMB_CCER1 = (0x10+0x20);	//P2.1 0x10:允许输入捕获, +0x00:上升沿, +0x20:下降沿
					//				PWMB_IER   = 0x04;			//P2.1 使能中断
				// PWM_IO_SET0();
				
				pwm_duty(PWMA_CH2P_P22, 0);
				AL_IO = 0;
				delay_500ns();
				pwm_duty(PWMA_CH3P_P24, BLDC_accelerator);
				AL_IO = 1;
				break;
			case 2: // 100, P2.1-HALL_B下降沿  PWM2, PWM1_L=1
					//				PWMA_ENO = 0x00;	PWM2_L=0;	PWM3_L=0;
					//				Delay_500ns();
					//				PWMA_ENO = 0x04;	// 打开B相的高端PWM
					//				PWM1_L = 1;			// 打开A相的低端
					//				PWMB_CCER1 = (0x01+0x00);	//P2.0 0x01:允许输入捕获, +0x00:上升沿, +0x02:下降沿
					//				PWMB_IER   = 0x02;			//P2.0 使能中断
					//				PWM_IO_SET0();
				// pwm_duty(PWMA_CH2P_P22, 0);
				
				CL_IO = 0;
				pwm_duty(PWMA_CH2P_P22, 0);
				delay_500ns();
				pwm_duty(PWMA_CH2P_P22, BLDC_accelerator);
				AL_IO = 1;
				break;
			case 3: // 101, P2.0-HALL_A上升沿  PWM2, PWM3_L=1
					//				PWMA_ENO = 0x04;	PWM1_L=0;	PWM2_L=0;	// 打开B相的高端PWM
					//				Delay_500ns();
					//				PWM3_L = 1;			// 打开C相的低端
					//				PWMB_CCER2 = (0x01+0x02);	//P2.2 0x01:允许输入捕获, +0x00:上升沿, +0x02:下降沿
					//				PWMB_IER   = 0x08;			//P2.2 使能中断
					//				PWM_IO_SET0();
				
				pwm_duty(PWMA_CH1P_P20, 0);
				CL_IO = 0;
				delay_500ns();
				pwm_duty(PWMA_CH2P_P22, BLDC_accelerator);
				CL_IO = 1;
				break;
			case 7: // 001, P2.2-HALL_C下降沿  PWM1, PWM3_L=1
					//				PWMA_ENO = 0x00;	PWM1_L=0;	PWM2_L=0;
					//				Delay_500ns();
					//				PWMA_ENO = 0x01;	// 打开A相的高端PWM
					//				PWM3_L = 1;			// 打开C相的低端
					//				PWMB_CCER1 = (0x10+0x00);	//P2.1 0x10:允许输入捕获, +0x00:上升沿, +0x20:下降沿
					//				PWMB_IER   = 0x04;			//P2.1 使能中断
					//				PWM_IO_SET0();
					//				pwm_duty(PWMA_CH1P_P20, 0);
				
				BL_IO = 0;
				pwm_duty(PWMA_CH1P_P20, 0);
				delay_500ns();
				pwm_duty(PWMA_CH1P_P20, BLDC_accelerator);
				CL_IO = 1;
				break;
			case 5: // 011, P2.1-HALL_B上升沿  PWM1, PWM2_L=1
					//				PWMA_ENO = 0x01;	PWM1_L=0;	PWM3_L=0;	// 打开A相的高端PWM
					//				Delay_500ns();
					//				PWM2_L = 1;			// 打开B相的低端
					//				PWMB_CCER1 = (0x01+0x02);	//P2.0 0x01:允许输入捕获, +0x00:上升沿, +0x02:下降沿
					//				PWMB_IER   = 0x02;			//P2.0 使能中断
					//				PWM_IO_SET0();
				
				pwm_duty(PWMA_CH3P_P24, 0);
				BL_IO = 0; // 打开C相的高端PWM
				delay_500ns();
				pwm_duty(PWMA_CH1P_P20, BLDC_accelerator);
				BL_IO = 1;
				break;

			default:
				break;
			}
		}

		else // 逆时针
		{
			switch (step)
			{
			case 4: // 100, P2.0-HALL_A下降沿  PWM1, PWM2_L=1		//逆时针
				pwm_duty(PWMA_CH3P_P24, 0);
				BL_IO = 0; // 打开C相的高端PWM
				delay_500ns();
				pwm_duty(PWMA_CH1P_P20, BLDC_accelerator);
				BL_IO = 1;
				break;
			case 6: // 110, P2.1-HALL_B上升沿  PWM1, PWM3_L=1
				BL_IO = 0;
				pwm_duty(PWMA_CH1P_P20, 0);
				delay_500ns();
				pwm_duty(PWMA_CH1P_P20, BLDC_accelerator);
				CL_IO = 1;
				break;
			case 2: // 010, P2.2-HALL_C下降沿  PWM2, PWM3_L=1
				pwm_duty(PWMA_CH1P_P20, 0);
				CL_IO = 0;
				delay_500ns();
				pwm_duty(PWMA_CH2P_P22, BLDC_accelerator);
				CL_IO = 1;
				break;
			case 3: // 011, P2.0-HALL_A上升沿  PWM2, PWM1_L=1
				CL_IO = 0;
				pwm_duty(PWMA_CH2P_P22, 0);
				delay_500ns();
				pwm_duty(PWMA_CH2P_P22, BLDC_accelerator);
				AL_IO = 1;
				break;
			case 1: // 001, P2.1-HALL_B下降沿  PWM3, PWM1_L=1
				pwm_duty(PWMA_CH2P_P22, 0);
				AL_IO = 0;
				delay_500ns();
				pwm_duty(PWMA_CH3P_P24, BLDC_accelerator);
				AL_IO = 1;
				break;
			case 5: // 101, P2.2-HALL_C上升沿  PWM3, PWM2_L=1
				AL_IO = 0;
				pwm_duty(PWMA_CH3P_P24, 0);
				delay_500ns();
				pwm_duty(PWMA_CH3P_P24, BLDC_accelerator); // 打开C相的高端PWM
				BL_IO = 1;								   // 打开B相的低端
														   //				PWMB_CCER2 = (0x01+0x00);	//P2.2 0x01:允许输入捕获, +0x00:上升沿, +0x02:下降沿
														   //				PWMB_IER   = 0x08;			//P2.2 使能中断
				break;

			default:
				break;
			}
		}
	}
}


int clip(int num, int min, int max)
{
	if (num > max)
		return max;
	else if (num < min)
		return min;
	else
		return num;
}
