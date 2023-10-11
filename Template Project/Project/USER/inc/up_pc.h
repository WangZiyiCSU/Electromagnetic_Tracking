#ifndef _UPPC_H_
#define _UPPC_H_
#include "common.h"
#include "zf_uart.h"


void Uart1_Process();//串口1通信
void Uart4_Process();//串口4通信
void Data_upload(int choice);//数据上传
void get_Road_type();  //赛道类型
void get_Speed();  //速度
#endif