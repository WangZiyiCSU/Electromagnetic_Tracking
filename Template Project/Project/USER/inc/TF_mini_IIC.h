   /*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ģ��IIC
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴common.h��VERSION�궨��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					���߶��壺
					------------------------------------ 
						SCL                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SCL�궨��
						SDA                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
					------------------------------------ 
 ********************************************************************************************************************/




#ifndef _TF_IIC_h_
#define _TF_IIC_h_

#include "common.h"
#define TF_SIMIIC_SCL_PIN P33
#define TF_SIMIIC_SDA_PIN P36



typedef enum TF_IIC       //DACģ��
{
    SIMIIC,
    SCCB
} TF_IIC_type;



void  tf_simiic_start(void);
void  tf_simiic_stop(void);
void  tf_simiic_ack_main(uint8 ack_main);
void  tf_send_ch(uint8 c);
uint8 tf_read_ch(uint8 ack);
void  tf_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat);
uint8 tf_simiic_read_reg(uint8 dev_add, uint8 reg, TF_IIC_type type);
void  tf_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, TF_IIC_type type);
void  tf_simiic_init(void);











#endif

