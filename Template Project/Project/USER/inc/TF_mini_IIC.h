   /*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		模拟IIC
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看common.h内VERSION宏定义
 * @Software 		MDK FOR C251 V5.60
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					接线定义：
					------------------------------------ 
						SCL                 查看SEEKFREE_IIC文件内的SEEKFREE_SCL宏定义
						SDA                 查看SEEKFREE_IIC文件内的SEEKFREE_SDA宏定义
					------------------------------------ 
 ********************************************************************************************************************/




#ifndef _TF_IIC_h_
#define _TF_IIC_h_

#include "common.h"
#define TF_SIMIIC_SCL_PIN P33
#define TF_SIMIIC_SDA_PIN P36



typedef enum TF_IIC       //DAC模块
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

