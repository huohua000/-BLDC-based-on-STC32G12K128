/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library ����CH32V307VCT6 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ���CH32V307VCT6 ��Դ���һ����
*
* CH32V307VCT6 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          seekfree_assistant
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio V1.8.1
* ����ƽ̨          CH32V307VCT6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����                                      ����                                     ��ע
* 2023-11-29     ��W             first version
********************************************************************************************************************/
#ifndef _seekfree_assistant_h_
#define _seekfree_assistant_h_


#include "headfile.h"

// �������FIFO��С
#define SEEKFREE_ASSISTANT_BUFFER_SIZE             ( 0x80 )

// ����ʾ���������ͨ������
#define SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT  ( 0x08 )

// ����������Ե����ͨ������
#define SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT      ( 0x08 )

// ����ͼ������������
#define SEEKFREE_ASSISTANT_CAMERA_MAX_BOUNDARY     ( 0x08 )

// ��Ƭ������λ�����͵�֡ͷ
#define SEEKFREE_ASSISTANT_SEND_HEAD               ( 0xAA )

// ����ͷ��
#define SEEKFREE_ASSISTANT_CAMERA_FUNCTION         ( 0x02 )
#define SEEKFREE_ASSISTANT_CAMERA_DOT_FUNCTION     ( 0x03 )
#define SEEKFREE_ASSISTANT_CAMERA_OSCILLOSCOPE     ( 0x10 )

// ��λ������Ƭ�����͵�֡ͷ
#define SEEKFREE_ASSISTANT_RECEIVE_HEAD            ( 0x55 )

// ����������
#define SEEKFREE_ASSISTANT_RECEIVE_SET_PARAMETER   ( 0x20 )


typedef struct
{
    uint8 head;                                     // ֡ͷ
    uint8 channel_num;                              // ����λΪ������  ����λΪͨ������
    uint8 check_sum;                                // ��У��
    uint8 length;                                   // ������
    float dat[SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT];    // ͨ������
}seekfree_assistant_oscilloscope_struct;


typedef struct
{
    uint8 head;                                     // ֡ͷ
    uint8 function;                                 // ������
    uint8 channel;                                  // ͨ��
    uint8 check_sum;                                // ��У��
    float dat;                                      // ����
}seekfree_assistant_parameter_struct;


extern seekfree_assistant_oscilloscope_struct               seekfree_assistant_oscilloscope_data;                         // ����ʾ��������
extern float                                                seekfree_assistant_parameter[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT];    // ������յ��Ĳ���
extern vuint8												seekfree_assistant_parameter_update_flag[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT];

extern uint32 (*seekfree_assistant_transfer)  	 			(const uint8 *buff, uint32 length);
extern uint32 (*seekfree_assistant_receive)    				(uint8 *buff, uint32 length);

void    seekfree_assistant_oscilloscope_send                (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope);

void    seekfree_assistant_data_analysis                    (void);
void    seekfree_assistant_init                             (void);


#endif
