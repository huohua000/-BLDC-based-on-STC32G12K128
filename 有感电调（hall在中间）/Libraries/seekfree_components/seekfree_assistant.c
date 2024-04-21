/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		seekfree_assistant
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/
 

#include "seekfree_assistant.h"

seekfree_assistant_oscilloscope_struct          seekfree_assistant_oscilloscope_data;                         // ����ʾ��������


static fifo_struct      seekfree_assistant_fifo;
static uint8            seekfree_assistant_buffer[SEEKFREE_ASSISTANT_BUFFER_SIZE];              		// ���ݴ������
float                   seekfree_assistant_parameter[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT] = {0};    	// ������յ��Ĳ���
vuint8					seekfree_assistant_parameter_update_flag[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT] = {0};
//-------------------------------------------------------------------------------------------------------------------
// �������     ���������ͺ���
// ����˵��     *buffer         ��ҪУ������ݵ�ַ
// ����˵��     length          У�鳤��
// ���ز���     uint8           ��ֵ
// ʹ��ʾ��
//-------------------------------------------------------------------------------------------------------------------
static uint8 seekfree_assistant_sum (uint8 *buffer, uint32 length)
{
    uint8 temp_sum = 0;

    while(length--)
    {
        temp_sum += *buffer++;
    }

    return temp_sum;
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������� ����ʾ�������ͺ���
// ����˵��     *seekfree_assistant_oscilloscope  ʾ�������ݽṹ��
// ���ز���     void
// ʹ��ʾ��     seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_oscilloscope_send (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope)
{
    uint8 packet_size;
	uint8 i = 0;
	seekfree_assistant_oscilloscope_struct temp_oscilloscope;
	
	temp_oscilloscope.channel_num = seekfree_assistant_oscilloscope->channel_num & 0x0f;
	
	// ���ڴ�С�˲�ƥ�䣬������Ҫ������
	for(i = 0; i < temp_oscilloscope.channel_num; i++)
	{
		((uint8 *)&temp_oscilloscope.dat[i])[0] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[3];
		((uint8 *)&temp_oscilloscope.dat[i])[1] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[2];
		((uint8 *)&temp_oscilloscope.dat[i])[2] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[1];
		((uint8 *)&temp_oscilloscope.dat[i])[3] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[0];
	}

    // ֡ͷ
    temp_oscilloscope.head         = SEEKFREE_ASSISTANT_SEND_HEAD;

    // д���������Ϣ
    packet_size                     = sizeof(temp_oscilloscope) - (SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT - temp_oscilloscope.channel_num) * 4;
    temp_oscilloscope.length       = packet_size;

    // д�빦������ͨ������
    temp_oscilloscope.channel_num |= SEEKFREE_ASSISTANT_CAMERA_OSCILLOSCOPE;

    // ��У�����
    temp_oscilloscope.check_sum    = 0;
    temp_oscilloscope.check_sum    = seekfree_assistant_sum((uint8 *)&temp_oscilloscope, packet_size);

    // �����ڵ��ñ�����֮ǰ�����û�����Ҫ���͵�����д��temp_oscilloscope.data[]

    seekfree_assistant_transfer((const uint8 *)&temp_oscilloscope, packet_size);
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������ֽ������յ�������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     ����ֻ��Ҫ�ŵ��������е�PIT�жϻ�����ѭ������
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_data_analysis (void)
{
    uint8  temp_sum;
    uint32 read_length;
    seekfree_assistant_parameter_struct *receive_packet;

    // ����ʹ��uint32���ж��壬Ŀ����Ϊ�˱�֤�������ֽڶ���
    uint32  temp_buffer[SEEKFREE_ASSISTANT_BUFFER_SIZE / 4];

	uint32 i = 0;
    // ���Զ�ȡ����, ��������Զ���Ĵ��䷽ʽ��ӽ��ջص��ж�ȡ����
    read_length = seekfree_assistant_receive((uint8 *)temp_buffer, SEEKFREE_ASSISTANT_BUFFER_SIZE);
    if(read_length)
    {
        // ����ȡ��������д��FIFO
        fifo_write_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, read_length);
    }

    while(sizeof(seekfree_assistant_parameter_struct) <= fifo_used(&seekfree_assistant_fifo))
    {
        read_length = sizeof(seekfree_assistant_parameter_struct);
        fifo_read_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, &read_length, FIFO_READ_ONLY);
        if(SEEKFREE_ASSISTANT_RECEIVE_HEAD != ((uint8 *)temp_buffer)[0])
        {
            // û��֡ͷ���FIFO��ȥ����һ������
            read_length = 1;
        }
        else
        {

            // �ҵ�֡ͷ
            receive_packet = (seekfree_assistant_parameter_struct *)temp_buffer;
            temp_sum = receive_packet->check_sum;
            receive_packet->check_sum = 0;

            if(temp_sum == seekfree_assistant_sum((uint8 *)temp_buffer, sizeof(seekfree_assistant_parameter_struct)))
            {
                // ��У��ɹ���������
			
				// ���ڴ�С�˲�ƥ�䣬������Ҫ������
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[3] = ((uint8 *)&receive_packet->dat)[0];
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[2] = ((uint8 *)&receive_packet->dat)[1];
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[1] = ((uint8 *)&receive_packet->dat)[2];
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[0] = ((uint8 *)&receive_packet->dat)[3];
				seekfree_assistant_parameter_update_flag[receive_packet->channel - 1] = 1;
            }
            else
            {
                read_length = 1;
            }
        }

        // ��������ʹ�õ�����
        fifo_read_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, &read_length, FIFO_READ_AND_CLEAN);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������� ��ʼ��
// ����˵��
// ���ز���     void
// ʹ��ʾ��
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_init ()
{
    fifo_init(&seekfree_assistant_fifo,  FIFO_DATA_8BIT, seekfree_assistant_buffer, SEEKFREE_ASSISTANT_BUFFER_SIZE);
}


