/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
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
uint32 speed_set=0;//�趨�ٶ�
extern pwmin_struct pwmin;
extern uint16 BLDC_accelerator;
extern unsigned int step;
extern PID speed_pid;
extern unsigned int last_step;
extern unsigned int step_counter;
/*
 * ϵͳƵ�ʣ��ɲ鿴board.h�е� FOSC �궨���޸ġ�
 * board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�����ϵͳƵ��Ϊ33.1776MHZ
 * ��board_init��,�Ѿ���P54��������Ϊ��λ
 * �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
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
	// �˴���д�û�����(���磺�����ʼ�������)
delay_ms(1000);	
    while(1)
	{
	if(step_counter==0)
		last_step=8;
	StepMotor(1);

    }
}

