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
						MY_SDA                 查看SEEKFREE_IIC文件内的SEEKFREE_SDA宏定义
					------------------------------------ 
 ********************************************************************************************************************/


#include "MY_IIC.h"



#define MY_SDA   			MY_SIMIIC_SDA_PIN
#define MY_SDA0()          MY_SIMIIC_SDA_PIN = 0		//IO口输出低电平
#define MY_SDA1()          MY_SIMIIC_SDA_PIN = 1		//IO口输出高电平  
#define MY_SCL0()          MY_SIMIIC_SCL_PIN = 0		//IO口输出低电平
#define MY_SCL1()          MY_SIMIIC_SCL_PIN = 1		//IO口输出高电平
     
#define ack 1      //主应答
#define no_ack 0   //从应答	 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC初始化
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void  my_simiic_init(void)
{
	//GPIO已在board_init()中初始化。
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC延时
//  @return     void						
//  @since      v1.0
//  Sample usage:				如果IIC通讯失败可以尝试增加j的值
//-------------------------------------------------------------------------------------------------------------------
void my_simiic_delay(void)
{
    uint16 j=0;   
	while(j--);
}


//内部使用，用户无需调用
void my_simiic_start(void)
{
	MY_SDA1();
	MY_SCL1();
	my_simiic_delay();
	MY_SDA0();
	my_simiic_delay();
	MY_SCL0();
}

//内部使用，用户无需调用
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

//主应答(包含ack:MY_SDA=0和no_ack:MY_SDA=0)
//内部使用，用户无需调用
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
	
    if(MY_SDA)           //应答为高电平，异常，通信失败
    {

        MY_SCL0();
        return 0;
    }

    MY_SCL0();
	my_simiic_delay();
    return 1;
}

//字节发送程序
//发送c(可以是数据也可是地址)，送完后接收从应答
//不考虑从应答位
//内部使用，用户无需调用
void my_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	MY_SDA1();//MY_SDA 输出数据
        else			MY_SDA0();
        c <<= 1;
        my_simiic_delay();
        MY_SCL1();                //SCL 拉高，采集信号
        my_simiic_delay();
        MY_SCL0();                //SCL 时钟线拉低
    }
	sccb_waitack();
}

//字节接收程序
//接收器件传来的数据，此程序应配合|主应答函数|使用
//内部使用，用户无需调用
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
        MY_SCL0();         //置时钟线为低，准备接收数据位
        my_simiic_delay();
        MY_SCL1();         //置时钟线为高，使数据线上数据有效
        my_simiic_delay();
        c<<=1;
        if(MY_SDA) 
        {
            c+=1;   //读数据位，将接收的数据存c
        }
    }

	MY_SCL0();
	my_simiic_delay();
	my_simiic_sendack(ack_x);
	
    return c;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC写数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      dat				写入的数据
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void my_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	my_simiic_start();
  my_send_ch( (dev_add<<1) | 0x00);   //发送器件地址加写位
	my_send_ch( reg );   				 //发送从机寄存器地址
	my_send_ch( dat );   				 //发送需要写入的数据
	my_simiic_stop();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC从设备寄存器读取数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      type			选择通信方式是IIC  还是 SCCB
//  @return     uint8			返回寄存器的数据			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint8 my_simiic_read_reg(uint8 dev_add, uint8 reg, MY_IIC_type type)
{
	uint8 dat;
	my_simiic_start();
    my_send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	my_send_ch( reg );   				//发送从机寄存器地址
	if(type == SCCB)my_simiic_stop();
	
	my_simiic_start();
	my_send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
	dat = my_read_ch(no_ack);   				//读取数据
	my_simiic_stop();
	
	return dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC读取多字节数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      dat_add			数据保存的地址指针
//  @param      num				读取字节数量
//  @param      type			选择通信方式是IIC  还是 SCCB
//  @return     uint8			返回寄存器的数据			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void my_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, MY_IIC_type type)
{
	my_simiic_start();
    my_send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	my_send_ch( reg );   				//发送从机寄存器地址
	if(type == SCCB)my_simiic_stop();
	
	my_simiic_start();
	my_send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
    while(--num)
    {
        *dat_add = my_read_ch(ack); //读取数据
        dat_add++;
    }
    *dat_add = my_read_ch(no_ack); //读取数据
	my_simiic_stop();
}



