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
						MY_SDA                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
					------------------------------------ 
 ********************************************************************************************************************/


#include "MY_IIC.h"



#define MY_SDA   			MY_SIMIIC_SDA_PIN
#define MY_SDA0()          MY_SIMIIC_SDA_PIN = 0		//IO������͵�ƽ
#define MY_SDA1()          MY_SIMIIC_SDA_PIN = 1		//IO������ߵ�ƽ  
#define MY_SCL0()          MY_SIMIIC_SCL_PIN = 0		//IO������͵�ƽ
#define MY_SCL1()          MY_SIMIIC_SCL_PIN = 1		//IO������ߵ�ƽ
     
#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��	 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʼ��
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void  my_simiic_init(void)
{
	//GPIO����board_init()�г�ʼ����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʱ
//  @return     void						
//  @since      v1.0
//  Sample usage:				���IICͨѶʧ�ܿ��Գ�������j��ֵ
//-------------------------------------------------------------------------------------------------------------------
void my_simiic_delay(void)
{
    uint16 j=0;   
	while(j--);
}


//�ڲ�ʹ�ã��û��������
void my_simiic_start(void)
{
	MY_SDA1();
	MY_SCL1();
	my_simiic_delay();
	MY_SDA0();
	my_simiic_delay();
	MY_SCL0();
}

//�ڲ�ʹ�ã��û��������
void my_simiic_stop(void)
{
	MY_SDA0();
	MY_SCL0();
	my_simiic_delay();
	MY_SCL1();
	my_simiic_delay();
	MY_SDA1();
	my_simiic_delay();
}

//��Ӧ��(����ack:MY_SDA=0��no_ack:MY_SDA=0)
//�ڲ�ʹ�ã��û��������
void my_simiic_sendack(unsigned char ack_dat)
{
    MY_SCL0();
	my_simiic_delay();
	if(ack_dat) MY_SDA0();
    else    	MY_SDA1();

    MY_SCL1();
    my_simiic_delay();
    MY_SCL0();
    my_simiic_delay();
}


static int sccb_waitack(void)
{
    MY_SCL0();

	my_simiic_delay();
	
	MY_SCL1();
    my_simiic_delay();
	
    if(MY_SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {

        MY_SCL0();
        return 0;
    }

    MY_SCL0();
	my_simiic_delay();
    return 1;
}

//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
//�ڲ�ʹ�ã��û��������
void my_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	MY_SDA1();//MY_SDA �������
        else			MY_SDA0();
        c <<= 1;
        my_simiic_delay();
        MY_SCL1();                //SCL ���ߣ��ɼ��ź�
        my_simiic_delay();
        MY_SCL0();                //SCL ʱ��������
    }
	sccb_waitack();
}

//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|ʹ��
//�ڲ�ʹ�ã��û��������
uint8 my_read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    MY_SCL0();
    my_simiic_delay();
    MY_SDA1();             

    for(i=0;i<8;i++)
    {
        my_simiic_delay();
        MY_SCL0();         //��ʱ����Ϊ�ͣ�׼����������λ
        my_simiic_delay();
        MY_SCL1();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        my_simiic_delay();
        c<<=1;
        if(MY_SDA) 
        {
            c+=1;   //������λ�������յ����ݴ�c
        }
    }

	MY_SCL0();
	my_simiic_delay();
	my_simiic_sendack(ack_x);
	
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
void my_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	my_simiic_start();
  my_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	my_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ
	my_send_ch( dat );   				 //������Ҫд�������
	my_simiic_stop();
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
uint8 my_simiic_read_reg(uint8 dev_add, uint8 reg, MY_IIC_type type)
{
	uint8 dat;
	my_simiic_start();
    my_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	my_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)my_simiic_stop();
	
	my_simiic_start();
	my_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
	dat = my_read_ch(no_ack);   				//��ȡ����
	my_simiic_stop();
	
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
void my_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, MY_IIC_type type)
{
	my_simiic_start();
    my_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	my_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)my_simiic_stop();
	
	my_simiic_start();
	my_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = my_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = my_read_ch(no_ack); //��ȡ����
	my_simiic_stop();
}



