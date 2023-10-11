#include "TF_MINI.h"

// #include "bsp_tfminiplus.h"
//#include "delay.h"
/*获取固件版本 */
uint8_t tf_mini_Get_fir_ver[4]  = {0x5A,0x04,0x01,0x5F}; //5A 07 01 V1 V2 V3 SU 版本 V3.V2.V1
/*系统复位*/
uint8_t tf_mini_System_reset[4] = {0x5A,0x04,0x02,0x60}; //5A 05 02 00 60 配置成功 /
//5A 05 02 01 61 配置失败 /
/*输出帧率*/
uint8_t tf_mini_Output_fra_ra[6]= {0x5A,0x06,0x03,0,0,0}; //5A 06 03 LL HH SU 1-1000Hz 设置① 100Hz
/*单次触发指令*/
uint8_t tf_mini_One_shot[4] =    {0x5A,0x04,0x04,0x62};//数据帧将输出帧率设置为 0后，可通过本指令出发测试
/*输出模式*/
uint8_t tf_mini_Standard_9_bytes_c[5] = {0x5A,0x05,0x05,0x01,0x65};//5A 05 05 01 65 标准 9 字节(cm) √
uint8_t tf_mini_String_format[5] = 	    {0x5A,0x05,0x05,0x02,0x66}; //5A 05 05 02 66 字符串格式(m)② /
uint8_t tf_mini_Standard_9_bytes_m[5] = {0x5A,0x05,0x05,0x06,0x6A};//5A 05 05 06 6A 标准 9 字节(mm) /
/*波特率 5A 08 06 H1 H2 H3 H4 SU*/
uint8_t tf_mini_Baud_rate[8]= {0x5A,0x08,0x06,0,0,0,0,0}; //5A 08 06 H1 H2 H3 H4 SU 设置波特率③ 115200
/*输出开关*/
uint8_t tf_mini_Dis_en_data_out[5] = {0x5A,0x05,0x07,0x00,0x66};//5A 05 07 00 66 关闭数据输出 /
uint8_t tf_mini_En_data_out[5]  =    {0x5A,0x05,0x07,0x01,0x67};//5A 05 07 01 67 使能数据输出 √
/*通信接口设置*/
uint8_t tf_mini_Comm_set[5]= {0x5A,0x05,0x0A,0,0}; //0（UART） 1（I2C）

/*修改 I2C 从机地址 0x5A,0x05,0x0B,ADDR,SU*/
uint8_t tf_mini_I2c_slave_addr[5]= {0x5A,0x05,0x0B,0,0}; //原指令 修改 I2c_slave_addr
/*获取测距结果*/
uint8_t tf_mini_data_frames_c[5]= {0x5A,0x05,0x00,0x01,0x60}; //数据帧(标准 9 字节(cm)) 仅 IIC 模式下可用 /
uint8_t tf_mini_data_frames_m[5]= {0x5A,0x05,0x00,0x06,0x65}; //数据帧(标准 9 字节(mm))
/*I/O（开关量）模式使能 5A 09 3B MODE DL DH ZoneL ZoneH SU*/
uint8_t tf_mini_switch_value[9] = {0x5A,0x09,0x3B,0,0,0,0,0,0};
// 开启/关闭 I/O（开关量）输出模式
/*MODE：
0 – 标准数据模式
1 – I/O，近高远低
2 – I/O，近低远高
Zone：滞回区间
0（标准数
据模式）*/
/*恢复出厂设置*/
uint8_t tf_mini_Res_fac_set[4]= {0x5A,0x04,0x10,0x6E}; //5A 05 10 00 6E 配置成功
//5A 05 10 01 6F 配置失败
/*保存设置*/
uint8_t tf_mini_save_settings[4]= {0x5A,0x04,0x11,0x6F}; //5A 05 11 00 6F 配置成功
//5A 05 11 01 70 配置失败
Tf_Mini_Plus distance_data;
void mx_iic2_init(void)
{
//    GPIO_InitTypeDef  GPIO_InitStructure;

//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟

//    //GPIOF0,F1初始化设置
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化

//    I2C_SCL_H();
//    I2C_SDA_H();
}
//产生IIC起始信号
static void I2C_Generate_Start(void)
{
    //TF_SDA_OUT();

    I2C_SDA_H();
    I2C_SCL_H();
    delay_us(5);
    I2C_SDA_L();
    delay_us(5);
    I2C_SCL_L();
    delay_us(5);
}
//产生IIC停止信号
static void I2C_Generate_Stop(void)
{
    I2C_SCL_L();
    I2C_SDA_L();
    delay_us(5);
    I2C_SCL_H();
    delay_us(5);
    I2C_SDA_H();
    delay_us(5);
}
//产生ACK应答
static void I2C_Generate_Ack(iic_ack_typedef ack)
{
    if(ack == I2C_NACK)
        {
            I2C_SDA_H();
        }
    else
        {
            I2C_SDA_L();
        }

    delay_us(5);
    I2C_SCL_H();
    delay_us(5);
    I2C_SCL_L();
}

static iic_ack_typedef I2C_RecvAck(uint32_t Timeout)
{
    uint32_t timeout = 0;
    iic_ack_typedef ackStatus;

    I2C_SDA_H();
    //TF_SDA_IN();

    delay_us(5);
    I2C_SCL_H();
    delay_us(5);
    while(1)
        {
            ackStatus = (TF_SIMIIC_SDA_PIN == Bit_RESET) ? I2C_ACK : I2C_NACK;
            if(ackStatus == I2C_ACK)
                {
                    break;
                }

            if((timeout++) > Timeout)
                {
                    ackStatus = I2C_NACK;
                    break;
                }
        }

    I2C_SCL_L();
    delay_us(5);
    //TF_SDA_OUT();
    return ackStatus;
}
//IIC发送一个字节
static iic_ack_typedef I2C_SendByte(uint8_t tf_data, uint32_t Timeout)
{
    uint32_t i;

    I2C_SCL_L();

    for (i = 0; i < 8; i++)
        {
            if((tf_data << i)&0x80)
                {
                    I2C_SDA_H();
                }
            else
                {
                    I2C_SDA_L();
                }
            I2C_SCL_H();
            delay_us(5);
            I2C_SCL_L();
            delay_us(5);
        }

    return I2C_RecvAck(Timeout);
}

//读1个字节
static uint8_t I2C_RecvByte(void)
{
    uint32_t i = 0;
    uint32_t tf_data = 0;
	  uint32_t BitStatus ;

    I2C_SDA_H();
    //TF_SDA_IN();
    delay_us(5);
    for (i = 0; i < 8; i++)
        {
            tf_data <<= 1;
            I2C_SCL_H();
            delay_us(5);

            BitStatus = (TF_SIMIIC_SDA_PIN == Bit_SET) ? 1 : 0;
            tf_data |= BitStatus;

            I2C_SCL_L();
            delay_us(5);
        }
    //TF_SDA_OUT();
    return tf_data;
}
//iic写
static iic_status_typedef I2C_DummyWrite(uint8_t SlaveAddr, uint16_t RegAddr, uint8_t OperLen, uint32_t Timeout)
{
    I2C_Generate_Start();
    if(I2C_SendByte((SlaveAddr<<1) | I2C_Transmitter, Timeout) != I2C_ACK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    if(I2C_SendByte((RegAddr >> 8) & 0xFF, Timeout) != I2C_ACK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    if(I2C_SendByte(RegAddr & 0xFF, Timeout) != I2C_ACK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    if(I2C_SendByte(OperLen, Timeout) != I2C_ACK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    return I2C_OK;
}

iic_status_typedef I2C_SendBytes(uint8_t SlaveAddr, uint16_t RegAddr, uint8_t *TxBuf, uint8_t OperLen, uint32_t Timeout)
{
    uint8_t i = 0;
    if(I2C_DummyWrite(SlaveAddr, RegAddr, OperLen, Timeout) != I2C_OK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    I2C_Generate_Start();
    if(I2C_SendByte((SlaveAddr<<1) | I2C_Transmitter, Timeout) != I2C_ACK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    for(i = 0; i < OperLen; i++)
        {
            if(I2C_SendByte(TxBuf[i], Timeout) != I2C_ACK)
                {
                    I2C_Generate_Stop();
                    return I2C_ERROR;
                }
        }
    I2C_Generate_Stop();
    return I2C_OK;
}

iic_status_typedef I2C_RecvBytes(uint8_t SlaveAddr, uint16_t RegAddr, uint8_t *Rxbuf, uint8_t OperLen, uint32_t Timeout)
{
    uint8_t i = 0;
    if(I2C_DummyWrite(SlaveAddr, RegAddr, OperLen, Timeout) != I2C_OK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    I2C_Generate_Start();
    if(I2C_SendByte((SlaveAddr<<1) | I2C_Receiver, Timeout) != I2C_ACK)
        {
            I2C_Generate_Stop();
            return I2C_ERROR;
        }
    for(i = 0; i < OperLen - 1; i++)
        {
            Rxbuf[i] = I2C_RecvByte();
            I2C_Generate_Ack(I2C_ACK);
        }
    Rxbuf[OperLen - 1] = I2C_RecvByte();
    I2C_Generate_Ack(I2C_NACK);
    I2C_Generate_Stop();
    return I2C_OK;
}





void get_distance(Tf_Mini_Plus* Tf_Mini_Dis,uint8_t addr)
{
    uint8_t buf[7];
    iic_status_typedef i2CStatus;
    i2CStatus = I2C_RecvBytes(addr,0x102, buf,7,100);
    if(i2CStatus == I2C_OK)
        {
            Tf_Mini_Dis->Mode = buf[6];
            Tf_Mini_Dis->Dist = buf[2] | (buf[3] << 8);
            Tf_Mini_Dis->Strength = buf[4] | (buf[5] << 8);
        }
}



