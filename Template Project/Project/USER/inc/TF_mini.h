#ifndef _TF_MINI_h_
#define _TF_MINI_h_
#include "common.h"
#include "headfile.h"
//#include "TF_mini_IIC.h"
typedef  uint8 uint8_t;
typedef  uint16 uint16_t;
typedef  uint32 uint32_t;
// #define USER_I2C_SCL_PIN  GPIO_Pin_5
// #define USER_I2C_SDA_PIN  GPIO_Pin_4
// #define USER_I2C_GPIO     GPIOE

// #define I2C_SCL_H() 		GPIO_SetBits(USER_I2C_GPIO, USER_I2C_SCL_PIN)
// #define I2C_SCL_L()			GPIO_ResetBits(USER_I2C_GPIO, USER_I2C_SCL_PIN)

// #define I2C_SDA_H()			GPIO_SetBits(USER_I2C_GPIO, USER_I2C_SDA_PIN)
// #define I2C_SDA_L()			GPIO_ResetBits(USER_I2C_GPIO, USER_I2C_SDA_PIN)

// #define I2C_SCL_IN()		GPIO_ReadInputDataBit(USER_I2C_GPIO, USER_I2C_SCL_PIN)
// #define I2C_SDA_IN()		GPIO_ReadInputDataBit(USER_I2C_GPIO, USER_I2C_SDA_PIN)

// //IO方向设置
// #define TF_SDA_IN()  {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=0<<4*2;}	//PE4输入模式
// #define TF_SDA_OUT() {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=1<<4*2;} //PE4输出模式



// #define USER_I2C_SCL_PIN  GPIO_Pin_5
// #define USER_I2C_SDA_PIN  GPIO_Pin_4
// #define USER_I2C_GPIO     GPIOE
#define TF_SIMIIC_SCL_PIN P33
#define TF_SIMIIC_SDA_PIN P36
#define I2C_SCL_H() 		TF_SIMIIC_SCL_PIN = 1		//IO口输出高电平
#define I2C_SCL_L()			TF_SIMIIC_SCL_PIN = 0		//IO口输出低电平

#define I2C_SDA_H()			TF_SIMIIC_SDA_PIN = 1		//IO口输出高电平  
#define I2C_SDA_L()			 TF_SIMIIC_SDA_PIN = 0		//IO口输出低电平


#define Bit_RESET 0
#define Bit_SET 1

// #define I2C_SCL_IN()		GPIO_ReadInputDataBit(USER_I2C_GPIO, USER_I2C_SCL_PIN)
// #define I2C_SDA_IN()		GPIO_ReadInputDataBit(USER_I2C_GPIO, USER_I2C_SDA_PIN)

// //IO方向设置
// #define TF_SDA_IN()  {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=0<<4*2;}	//PE4输入模式
// #define TF_SDA_OUT() {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=1<<4*2;} //PE4输出模式


//#define TF_SDA   			TF_SIMIIC_SDA_PIN
//#define TF_SDA0()          TF_SIMIIC_SDA_PIN = 0		//IO口输出低电平
//#define TF_SDA1()          TF_SIMIIC_SDA_PIN = 1		//IO口输出高电平  
//#define TF_SCL0()          TF_SIMIIC_SCL_PIN = 0		//IO口输出低电平
//#define TF_SCL1()          TF_SIMIIC_SCL_PIN = 1		//IO口输出高电平


typedef enum
{
	I2C_OK = 0U,
	I2C_BUSY = 1U,
	I2C_TIMEOUT = 3U,
	I2C_ERROR = 4U,
}iic_status_typedef;

typedef enum
{
	I2C_ACK = 0U,
	I2C_NACK = 1U,
}iic_ack_typedef;

typedef enum
{
	I2C_Transmitter = 0U,//发送
	I2C_Receiver = 1U,   //接收
}iic_oper_typedef;



typedef struct
{
	uint8_t Mode; 	   //= buf[6];
	uint16_t Dist; 	   //= buf[2] | (buf[3] << 8);
	uint32_t Strength; //= buf[4] | (buf[5] << 8);
}Tf_Mini_Plus;

void mx_iic2_init(void);
iic_status_typedef I2C_SendBytes(uint8_t SlaveAddr, uint16_t RegAddr, uint8_t *TxBuf, uint8_t OperLen, uint32_t Timeout);
iic_status_typedef I2C_RecvBytes(uint8_t SlaveAddr, uint16_t RegAddr, uint8_t *Rxbuf, uint8_t OperLen, uint32_t Timeout);
void get_distance(Tf_Mini_Plus* Tf_Mini_Dis,uint8_t addr);
extern Tf_Mini_Plus distance_data;



#endif