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
	TIM4_CLEAR_FLAG; //����жϱ�־
  	//pit_handler();
//	if(USART4_RX_LEN > 0 )return;
//	USART4_RX_STA = REFUSE_TO_RECEIVE;
	

	switch(car.work_mode)
	{
		case NORMAL: 	//��ͨѰ��ģʽ
		{
			Normal_Tracing(); //��ͨ����Ѱ��
			break;
		}
		case SCAN:		//ɨ������ģʽ
		{
			//AD_Scan(&car.adc_parameter);  //ɨ�������ø���������ֵ
			break;
		}
		case TEST:
		{

			//������ݲɼ�
			 ADC_Process(&car.adc_parameter);	   //��ֵ�˲�
			 ADC_Normalization(&car.adc_parameter); //��һ�� 0-100
			// //���������ݲɼ�
			 ICM_OneOrderFilter();   
//	         pwm_duty(PWMB_CH4_P77, 600);						
//	         pwm_duty(PWMB_CH3_P33, 600); 
			 //hinder_judge(); 
			 //CitTie_judge();     
			// //���ƫ�����
			//get_error_normal(&car.adc_parameter); //����ƫ��	
			// car.steering.error_now = car.adc_parameter.adc_error ;
			// //���򻷼���	
			// car.steering.duty = car.steering.linear_error_kp   *car.steering.error_now +    							//һ��ƫ��   
			// 				    car.steering.quadratic_error_kp*car.steering.error_now*abs(car.steering.error_now) +    //����ƫ��
			// 				    car.steering.error_kd*(car.steering.error_now - car.steering.error_last) -  		    //ƫ����
			// 				    car.steering.gyro_kp *icmdata.YawVelocity; 												//����������
			// car.steering.error_last = car.steering.error_now;
			// //ת��PWM�޷�
			// if(car.steering.duty>MOTOR_DUTY_MAX)car.steering.duty=MOTOR_DUTY_MAX;
			// else if(car.steering.duty<-MOTOR_DUTY_MAX)car.steering.duty=-MOTOR_DUTY_MAX;
			// //�������
			// //�ٶȲ���
			 EncoderPulseGet(); 
			//motor_control(-250,-150);//�ջ��ٶ�150
			//Stop_after(2);
			// MotorPWM_Out(car.motor_average.duty, car.motor_average.duty);
			//MotorPWM_Out(1000,2000);
			// if(car.distance>100){
			// 	block_judge();
			// }
			

			break;
		}
		case REMOTE:	//ң��ģʽ
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
//                ips114_showstr(0, 0, "����\n");
//				break;	
//			case STRAIGHT : 		
//                ips114_showstr(0, 0, "ֱ��\n");
//				break;	
//			case CIRCLE:
//                ips114_showstr(0, 0, "����\n");
//				break;	
////			case FORK :
////				//printf("cha·\n");
////				break;	
////			case STRAIGHT_FORK:
////				//printf("cha·��\n");
////				break;	
//			case ENTER_GARAGE :
//                ips114_showstr(0, 0, "���\n");

//				break;	
////			case CROSS :
////				//printf("ʮ��\n");
////				break;	
//			case RAMP:
//                ips114_showstr(0, 0, "�µ�\n");
//				break;	
//            }
//        }

	//���±�־λ ������Ļ��ʱˢ��
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

//UART1�ж�
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
        //�����Զ�����
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

//UART2�ж�
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
		//�������ݼĴ���Ϊ��S2BUF

	}
}


//UART3�ж�
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
		//�������ݼĴ���Ϊ��S3BUF

	}
}


//UART4�ж�
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

		//�������ݼĴ���Ϊ��S4BUF;
		if(wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback();           //����ת���ڻص�����
        }
        else if(wireless_type == WIRELESS_CH9141)
        {
            bluetooth_ch9141_uart_callback();   //����ת���ڻص�����
        }
        else if(wireless_type == WIRELESS_CH573)
        {
            wireless_ch573_callback();          //CH573����ģ��ص�����
        }
	}
}

void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //����жϱ�־
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //����жϱ�־
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //����жϱ�־
}

void TM0_Isr() interrupt 1
{

}
void TM1_Isr() interrupt 3
{

}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //����жϱ�־
	
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //����жϱ�־
	
}

////UART4�ж�
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











////UART1�ж�
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
//        //�����Զ�����
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
////		if((USART_RX_STA&0x8000) == 0){//����δ���
////				if(USART_RX_STA<(UART1_RX_BUFFER_SIZE)){
////					  TL1 = 0x00;
////						TH1 = 0x00;
////						if(USART_RX_STA == 0){
////							pit_timer_ms(TIM_1, 10); //�򿪶�ʱ��3
////						}
////						uart1_rx_buff[USART_RX_STA++]=res;//��¼���յ���ֵ
////				}
////				else{
////					USART_RX_STA|=0x8000; //������������ ǿ�Ʊ��Ϊ�������
////				}	
////		}
//	}
//}

////UART2�ж�
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
//		//�������ݼĴ���Ϊ��S2BUF

//	}
//}

////UART3�ж�
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
//		//�������ݼĴ���Ϊ��S3BUF

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
////�ⲿ�ж�2  
//void INT2_Isr() interrupt 10
//{
//	INT2_CLEAR_FLAG;  //����жϱ�־

//}

//void INT3_Isr() interrupt 11
//{
//	INT3_CLEAR_FLAG;  //����жϱ�־
//}

//void INT4_Isr() interrupt 16
//{
//	INT4_CLEAR_FLAG;  //����жϱ�־
//}

//void TM0_Isr() interrupt 1  //�޷�ʹ�� ��ʱ��0�����ڱ������ⲿ����
//{
//	
//}

//void TM1_Isr() interrupt 3     //����ͨ������ж�
//{
////	static int count = 0;
////	count++;
////	if(count >= 50){
////		count = 0;
////		USART_RX_STA |=0x8000;		//��ǽ������
////		USART4_RX_STA |=0x8000;		//��ǽ������
////		car.uart_busy = 0;
////		TR1 = 0; //�رն�ʱ��
////	}
//}

//void TM2_Isr() interrupt 12  //���ڴ���4�Ĳ����ʲ���
//{
//	TIM2_CLEAR_FLAG;  //����жϱ�־
//}

//void TM3_Isr() interrupt 19 //�޷�ʹ�� ��ʱ��3�����ڱ������ⲿ����
//{
//	TIM3_CLEAR_FLAG; //����жϱ�־
//}
////extern void pit_handler(void);
////void TM4_Isr() interrupt 20
////{
////	TIM4_CLEAR_FLAG; //����жϱ�־
//////	ccd_collect();	 //CCD�ɼ�����

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