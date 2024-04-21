/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
#define LEDA P32
#define LEDB P33
#define LEDC P50
#define Clockwise_direction 1
#define Counterclockwise _direction 0
uint32 speed_set=0;//设定速度
extern pwmin_struct pwmin;
extern uint16 BLDC_accelerator;
extern unsigned int step;
extern PID speed_pid;
extern unsigned int last_step;
extern unsigned int step_counter;
/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */


void main()
{
    CKCON = 0;
    WTST = 0;               // ??????????,???0??CPU????????????
	DisableGlobalIRQ();		// ?????
    sys_clk = 33177600;     // ???????33177600Hz
	board_init();			// ??????
    EnableGlobalIRQ();		// ?????
	ESC_init();
	PID_Init(&speed_pid,70,20,0,2000,30000);
	pit_timer_ms(TIM_4, 10);
//	pwm_input_init();
	LEDA=0;
	LEDB=0;
	LEDC=0;	
//	uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 115200, TIM_2);
//	pit_timer_ms(TIM_0,2);
//	Speed_Init();
//	start_counter=0;
	// 此处编写用户代码(例如：外设初始化代码等)
delay_ms(1000);	
    while(1)
	{
	if(step_counter==0)
		last_step=8;
	StepMotor(1);

    }
}

