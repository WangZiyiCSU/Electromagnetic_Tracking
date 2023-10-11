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
						TF_SDA                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
					------------------------------------ 
 ********************************************************************************************************************/


#include "TF_mini_IIC.h"



#define TF_SDA   			TF_SIMIIC_SDA_PIN
#define TF_SDA0()          TF_SIMIIC_SDA_PIN = 0		//IO������͵�ƽ
#define TF_SDA1()          TF_SIMIIC_SDA_PIN = 1		//IO������ߵ�ƽ  
#define TF_SCL0()          TF_SIMIIC_SCL_PIN = 0		//IO������͵�ƽ
#define TF_SCL1()          TF_SIMIIC_SCL_PIN = 1		//IO������ߵ�ƽ
     
#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��	 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʼ��
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void  tf_simiic_init(void)
{
	//GPIO����board_init()�г�ʼ����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʱ
//  @return     void						
//  @since      v1.0
//  Sample usage:				���IICͨѶʧ�ܿ��Գ�������j��ֵ
//-------------------------------------------------------------------------------------------------------------------
void tf_simiic_delay(void)
{
    uint16 j=0;   
	while(j--);
}


//�ڲ�ʹ�ã��û��������
void tf_simiic_start(void)
{
	TF_SDA1();
	TF_SCL1();
	tf_simiic_delay();
	TF_SDA0();
	tf_simiic_delay();
	TF_SCL0();
}

//�ڲ�ʹ�ã��û��������
void tf_simiic_stop(void)
{
	TF_SDA0();
	TF_SCL0();
	tf_simiic_delay();
	TF_SCL1();
	tf_simiic_delay();
	TF_SDA1();
	tf_simiic_delay();
}

//��Ӧ��(����ack:TF_SDA=0��no_ack:TF_SDA=0)
//�ڲ�ʹ�ã��û��������
void tf_simiic_sendack(unsigned char ack_dat)
{
    TF_SCL0();
	tf_simiic_delay();
	if(ack_dat) TF_SDA0();
    else    	TF_SDA1();

    TF_SCL1();
    tf_simiic_delay();
    TF_SCL0();
    tf_simiic_delay();
}


static int sccb_waitack(void)
{
    TF_SCL0();

	tf_simiic_delay();
	
	TF_SCL1();
    tf_simiic_delay();
	
    if(TF_SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {

        TF_SCL0();
        return 0;
    }

    TF_SCL0();
	tf_simiic_delay();
    return 1;
}

//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
//�ڲ�ʹ�ã��û��������
void tf_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	TF_SDA1();//TF_SDA �������
        else			TF_SDA0();
        c <<= 1;
        tf_simiic_delay();
        TF_SCL1();                //SCL ���ߣ��ɼ��ź�
        tf_simiic_delay();
        TF_SCL0();                //SCL ʱ��������
    }
	sccb_waitack();
}

//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|ʹ��
//�ڲ�ʹ�ã��û��������
uint8 tf_read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    TF_SCL0();
    tf_simiic_delay();
    TF_SDA1();             

    for(i=0;i<8;i++)
    {
        tf_simiic_delay();
        TF_SCL0();         //��ʱ����Ϊ�ͣ�׼����������λ
        tf_simiic_delay();
        TF_SCL1();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        tf_simiic_delay();
        c<<=1;
        if(TF_SDA) 
        {
            c+=1;   //������λ�������յ����ݴ�c
        }
    }

	TF_SCL0();
	tf_simiic_delay();
	tf_simiic_sendack(ack_x);
	
    return c;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IICд���ݵ��豸�Ĵ�������
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat				д�������
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void tf_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	tf_simiic_start();
  tf_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	tf_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ
	tf_send_ch( dat );   				 //������Ҫд�������
	tf_simiic_stop();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC���豸�Ĵ�����ȡ����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint8 tf_simiic_read_reg(uint8 dev_add, uint8 reg, TF_IIC_type type)
{
	uint8 dat;
	tf_simiic_start();
    tf_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	tf_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)tf_simiic_stop();
	
	tf_simiic_start();
	tf_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
	dat = tf_read_ch(no_ack);   				//��ȡ����
	tf_simiic_stop();
	
	return dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ȡ���ֽ�����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat_add			���ݱ���ĵ�ַָ��
//  @param      num				��ȡ�ֽ�����
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void tf_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, TF_IIC_type type)
{
	tf_simiic_start();
    tf_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	tf_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)tf_simiic_stop();
	
	tf_simiic_start();
	tf_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = tf_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = tf_read_ch(no_ack); //��ȡ����
	tf_simiic_stop();
}



