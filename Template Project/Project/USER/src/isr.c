#include "zf_uart.h"
#include "headfile.h"
#include "control.h"
#include "car.h"
#include "garage.h"

#define PI 3.1415926
extern void pit_handler(void);
void TM4_Isr() interrupt 20 
{
	static int dutyerror=0;
	static int lose = 0;
	static float temp_error=0;
	static int count = 0;
	static int count_10ms = 0;
	static int count_2s = 0;
    static int type_temp = 0;
	TIM4_CLEAR_FLAG; //清除中断标志
  	//pit_handler();
//	if(USART4_RX_LEN > 0 )return;
//	USART4_RX_STA = REFUSE_TO_RECEIVE;
	

	switch(car.work_mode)
	{
		case NORMAL: 	//普通寻迹模式
		{
			Normal_Tracing(); //普通赛道寻迹
			break;
		}
		case SCAN:		//扫描赛道模式
		{
			//AD_Scan(&car.adc_parameter);  //扫描赛道得各个电感最大值
			break;
		}
		case TEST:
		{

			//电感数据采集
			 ADC_Process(&car.adc_parameter);	   //均值滤波
			 ADC_Normalization(&car.adc_parameter); //归一化 0-100
			// //陀螺仪数据采集
			 ICM_OneOrderFilter();   
//	         pwm_duty(PWMB_CH4_P77, 600);						
//	         pwm_duty(PWMB_CH3_P33, 600); 
			 //hinder_judge(); 
			 //CitTie_judge();     
			// //电感偏差计算
			//get_error_normal(&car.adc_parameter); //计算偏差	
			// car.steering.error_now = car.adc_parameter.adc_error ;
			// //方向环计算	
			// car.steering.duty = car.steering.linear_error_kp   *car.steering.error_now +    							//一次偏差   
			// 				    car.steering.quadratic_error_kp*car.steering.error_now*abs(car.steering.error_now) +    //二次偏差
			// 				    car.steering.error_kd*(car.steering.error_now - car.steering.error_last) -  		    //偏差差分
			// 				    car.steering.gyro_kp *icmdata.YawVelocity; 												//陀螺仪抑制
			// car.steering.error_last = car.steering.error_now;
			// //转向PWM限幅
			// if(car.steering.duty>MOTOR_DUTY_MAX)car.steering.duty=MOTOR_DUTY_MAX;
			// else if(car.steering.duty<-MOTOR_DUTY_MAX)car.steering.duty=-MOTOR_DUTY_MAX;
			// //电机控速
			// //速度测量
			 EncoderPulseGet(); 
			//motor_control(-250,-150);//闭环速度150
			//Stop_after(2);
			// MotorPWM_Out(car.motor_average.duty, car.motor_average.duty);
			//MotorPWM_Out(1000,2000);
			// if(car.distance>100){
			// 	block_judge();
			// }
			

			break;
		}
		case REMOTE:	//遥控模式
		{
			break;
		}
	}

//    if(type_temp != car.road_type)
//        {
//            ips114_clear(WHITE);
//            type_temp = car.road_type;
//            switch(car.road_type)
//            {
//			case OUT_GARAGE:
//                ips114_showstr(0, 0, "出库\n");
//				break;	
//			case STRAIGHT : 		
//                ips114_showstr(0, 0, "直道\n");
//				break;	
//			case CIRCLE:
//                ips114_showstr(0, 0, "环岛\n");
//				break;	
////			case FORK :
////				//printf("cha路\n");
////				break;	
////			case STRAIGHT_FORK:
////				//printf("cha路内\n");
////				break;	
//			case ENTER_GARAGE :
//                ips114_showstr(0, 0, "入库\n");

//				break;	
////			case CROSS :
////				//printf("十字\n");
////				break;	
//			case RAMP:
//                ips114_showstr(0, 0, "坡道\n");
//				break;	
//            }
//        }

	//更新标志位 用于屏幕定时刷新
	if(car.screen.ready==0)
	{
		car.screen.count++;
		if(car.screen.count >= 200/car.screen.fps)
		{
			car.screen.count = 0;
			car.screen.ready = 1;
		}
	}
//	USART4_RX_STA = EMPTY;
}

//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF

	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;

		//接收数据寄存器为：S4BUF;
		if(wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback();           //无线转串口回调函数
        }
        else if(wireless_type == WIRELESS_CH9141)
        {
            bluetooth_ch9141_uart_callback();   //蓝牙转串口回调函数
        }
        else if(wireless_type == WIRELESS_CH573)
        {
            wireless_ch573_callback();          //CH573无线模块回调函数
        }
	}
}

void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}

void TM0_Isr() interrupt 1
{

}
void TM1_Isr() interrupt 3
{

}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //清除中断标志
	
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}

////UART4中断
//void UART4_Isr() interrupt 18
//{

////	uint8 res;
////    if(UART4_GET_TX_FLAG)
////	{
////		UART4_CLEAR_TX_FLAG;
////		busy[4] = 0;
////	}
////    if(UART4_GET_RX_FLAG)
////	{
////        UART4_CLEAR_RX_FLAG;
////		res = WIRELESS_DATA_BUF;
////		if(USART4_RX_STA == EMPTY)
////		{
////			uart4_rx_buff[USART4_RX_LEN++] = res;
////			USART4_RX_STA = RECEIVING;
////		}
////		else if(USART4_RX_STA == RECEIVING)
////		{
////			uart4_rx_buff[USART4_RX_LEN++] = res;
////			if(uart4_rx_buff[USART4_RX_LEN-1]==0x0a)
////			{
////				if(USART4_RX_LEN>=2 && uart4_rx_buff[USART4_RX_LEN-2]==0x0d )
////				{
////					USART4_RX_STA = RECEIVING_COMPLETED;
////				}
////			}
////		}
////	}
//}
//	











////UART1中断
//void UART1_Isr() interrupt 4
//{
//    uint8 res;
//	static uint8 dwon_count;
//    if(UART1_GET_TX_FLAG)
//    {
//        UART1_CLEAR_TX_FLAG;
//        busy[1] = 0;
//    }
//    if(UART1_GET_RX_FLAG)
//    {
//        UART1_CLEAR_RX_FLAG;
//        res = SBUF;
//        //程序自动下载
//        // if(res == 0x7F)
//        // {
//        //     if(dwon_count++ > 20)
//        //         IAP_CONTR = 0x60;
//        // }
//        // else
//        // {
//        //     dwon_count = 0;
//        // }
////	
////		if((USART_RX_STA&0x8000) == 0){//接收未完成
////				if(USART_RX_STA<(UART1_RX_BUFFER_SIZE)){
////					  TL1 = 0x00;
////						TH1 = 0x00;
////						if(USART_RX_STA == 0){
////							pit_timer_ms(TIM_1, 10); //打开定时器3
////						}
////						uart1_rx_buff[USART_RX_STA++]=res;//记录接收到的值
////				}
////				else{
////					USART_RX_STA|=0x8000; //超出缓存容量 强制标记为接收完成
////				}	
////		}
//	}
//}

////UART2中断
//void UART2_Isr() interrupt 8
//{
//    if(UART2_GET_TX_FLAG)
//	{
//        UART2_CLEAR_TX_FLAG;
//		busy[2] = 0;
//	}
//    if(UART2_GET_RX_FLAG)
//	{
//        UART2_CLEAR_RX_FLAG;
//		//接收数据寄存器为：S2BUF

//	}
//}

////UART3中断
//void UART3_Isr() interrupt 17
//{
//    if(UART3_GET_TX_FLAG)
//	{
//        UART3_CLEAR_TX_FLAG;
//		busy[3] = 0;
//	}
//    if(UART3_GET_RX_FLAG)
//	{
//        UART3_CLEAR_RX_FLAG;
//		//接收数据寄存器为：S3BUF

//	}
//}
//	

//		
//	


//void INT0_Isr() interrupt 0
//{
//	INT0_CLEAR_FLAG;

//}



//void INT1_Isr() interrupt 2
//{
//	static int back = 0;
//	INT1_CLEAR_FLAG;


//}
////外部中断2  
//void INT2_Isr() interrupt 10
//{
//	INT2_CLEAR_FLAG;  //清除中断标志

//}

//void INT3_Isr() interrupt 11
//{
//	INT3_CLEAR_FLAG;  //清除中断标志
//}

//void INT4_Isr() interrupt 16
//{
//	INT4_CLEAR_FLAG;  //清除中断标志
//}

//void TM0_Isr() interrupt 1  //无法使用 定时器0已用于编码器外部计数
//{
//	
//}

//void TM1_Isr() interrupt 3     //用于通信完成判断
//{
////	static int count = 0;
////	count++;
////	if(count >= 50){
////		count = 0;
////		USART_RX_STA |=0x8000;		//标记接收完成
////		USART4_RX_STA |=0x8000;		//标记接收完成
////		car.uart_busy = 0;
////		TR1 = 0; //关闭定时器
////	}
//}

//void TM2_Isr() interrupt 12  //用于串口4的波特率产生
//{
//	TIM2_CLEAR_FLAG;  //清除中断标志
//}

//void TM3_Isr() interrupt 19 //无法使用 定时器3已用于编码器外部计数
//{
//	TIM3_CLEAR_FLAG; //清除中断标志
//}
////extern void pit_handler(void);
////void TM4_Isr() interrupt 20
////{
////	TIM4_CLEAR_FLAG; //清除中断标志
//////	ccd_collect();	 //CCD采集数据

////	pit_handler();
////}







//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;