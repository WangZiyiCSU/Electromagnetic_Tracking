/* Includes ------------------------------------------------------------------*/
#include "zf_uart.h"
#include "car.h"
#include "circle.h"
#include "hinder.h"
#include "ramp.h"
#include "garage.h"
/* Includes ------------------------------------------------------------------*/

CAR car;	
uint16 adc_max[INDUCTOR_NUM]={4000,4000,4000,4000,4000};
uint16 adc_min[INDUCTOR_NUM]={0,0,0,0,0};

/***********************Ԫ�ر�**************************/
/*-----
����⣺       0
�ҳ��⣺       1

��С��(R50)��  2
��С��(R50):   3 

���(R60)��  4
�Ҵ�(R60):   5

�ϰ�(���)��   6
�ϰ�(�Ҳ�)��   7

�µ���         8

����⣺       9
����⣺       10

���(R90):   11
�Ҵ�(R90)��  12

------*/
/***********************Ԫ�ر�**************************/

/****************************************************
 * @name	      Circle_parameter_init
 * @function      Բ��������ʼ��
 * @parameter     NULL
 * @return		  NULL
 * @date	      2023/7/27
 ***************************************************/
 void element_parameter_init(){

 	car.element.ELEMENT_NUM = 0;
	
	/******ʵ����328����(��ʱ��)**********/	
	//�ҳ��⡪����С��������󻷡����ϰ������µ����������
	/******����������******************/
	//����⡪����С�������µ�������󻷡����ϰ�(��)��������µ������������

	car.element.excel[0]       = 0;
	car.element.excel[1]       = 3;
	car.element.excel[2]       = 8;
	car.element.excel[3]       = 4;
	car.element.excel[4]       = 11;
	car.element.excel[5]       = 6;
	car.element.excel[6]       = 8;
	car.element.excel[7]       = 9;
    // car.element.excel[6]    = 7;
    // car.element.excel[7]    = 5;
    // car.element.excel[8]    = 2;
	// car.element.excel[9]    = 8;
	// car.element.excel[10]   = 6;
	// car.element.excel[11]   = 5;
	// car.element.excel[11]   = 2;
	// car.element.excel[7]    = 6;
	// car.element.excel[8]    = 7;
	/******ʵ����328����(��ʱ��)**********/	

 }
/***********************Ԫ�ر�**************************/
//*******************************������ʼ��*************************************
void car_Parameter_Init()
{

	// car.steering.linear_error_kp 			   = 300;   //һ�������ϵ��
	// car.steering.quadratic_error_kp 		       = 0.2; 	//���������ϵ��
	// car.steering.error_kd 					   = 5;     //���΢��ϵ��
	// car.steering.gyro_kp 				       = 25;  	//������ϵ��
	car.steering.linear_error_kp 			       = 7.5;    //һ�������ϵ��
	car.steering.quadratic_error_kp 		       = 0;     //���������ϵ��
	car.steering.error_kd 					       = 10;     //���΢��ϵ��
	car.steering.gyro_kp 					       = 0;   //������ϵ��
	car.steering.error_now 				           = 0;     //��ǰƫ��
	car.steering.error_last 	         		   = 0;     //�ϴ�ƫ��
	car.steering.duty 						       = 0;     //���
	
    car.pd_loop.circle_p                           = 5;

	//ƽ���������
	car.motor_average.kp                     = 25;
	car.motor_average.ki                     = 3;
	car.motor_average.kd                     = 0;
	car.motor_average.integral               = 0;
	car.motor_average.speed_Expected         = 150;
	car.motor_average.distance 			     = 0;
	car.motor_average.speed_Current          = 0;
	car.motor_average.speed_Last             = 0;
	car.motor_average.error_Now              = 0;
	car.motor_average.error_Pre              = 0;
	car.motor_average.error_PrePre           = 0;
	car.motor_average.duty                   = 0;
	car.motor_average.duty_Last              = 0;

	//��������
	car.motor_left.kp                        = 30;
	car.motor_left.ki                        = 1;
	car.motor_left.kd                        = 0;
	car.motor_left.distance 			     = 0;
	car.motor_left.speed_Expected            = 0;
	car.motor_left.speed_Current             = 0;
	car.motor_left.speed_Last                = 0;
	car.motor_left.error_Now                 = 0;
	car.motor_left.error_Pre                 = 0;
	car.motor_left.error_PrePre              = 0;
	car.motor_left.duty                      = 0;
	car.motor_left.duty_Last                 = 0;

	//�ҵ������
	car.motor_right.kp                       = 30;
	car.motor_right.ki                       = 1;
	car.motor_right.kd                       = 0;
	car.motor_right.distance 			     = 0;
	car.motor_right.speed_Expected           = 0;
	car.motor_right.speed_Current            = 0;
	car.motor_right.speed_Last               = 0;
	car.motor_right.error_Now                = 0;
	car.motor_right.error_Pre                = 0;
	car.motor_right.error_PrePre             = 0;
	car.motor_right.duty                     = 0;
	car.motor_right.duty_Last                = 0;




	//����
	car.garage.count                         = 0;
	car.garage.in_point                      = 0;
	car.garage.base_angle                    = 0;
	car.garage.enter_flag                    = 0;
	car.garage.out_circle_point              = 0;
	//car.garage.DIR                           = RIGHT;

	car.meter_distance                       = 100;
	
	//���߱��λ
	car.lose_flag = 0;

	/************************Ԫ�ر�*********************/
	element_parameter_init();
	/************************Ԫ�ر�*********************/

	/**************************����*********************/
	Circle_parameter_init();  
	/**************************����*********************/
	
	/**************************�µ�*********************/
	Ramp_parameter_init();
	/**************************�µ�*********************/
	
	/**************************����*********************/
	car.fan.left_duty                           = 200;
	car.fan.right_duty                          = 200;
	/**************************����*********************/

	/**************************�ϰ�*********************/
	hinder_parameter_init();
	/**************************�ϰ�*********************/

	/**************************�����*******************/
	garage_parameter_init();
	/**************************�����*******************/

	car.ramp_distance      = 50;
	car.circle_distance    = 50;


	/**************************���*********************/
	//��й�һ������
	//���ֵ
	car.adc_parameter.adc_max[LEFT_H]        = 4000; 
	car.adc_parameter.adc_max[LEFT_X]        = 4000; 
	car.adc_parameter.adc_max[MIDDLE]        = 4000; 
	car.adc_parameter.adc_max[RIGHT_X]       = 4000; 
	car.adc_parameter.adc_max[RIGHT_H]       = 4000; 

	//��Сֵ
	car.adc_parameter.adc_min[LEFT_H]        = 400;
	car.adc_parameter.adc_min[LEFT_X]        = 1;
	car.adc_parameter.adc_min[MIDDLE]        = 400;
	car.adc_parameter.adc_min[RIGHT_X]       = 1;
	car.adc_parameter.adc_min[RIGHT_H]       = 400;
	
	//ƫ�����
	car.adc_parameter.error_caculate         = 1000;
	/**************************���*********************/


    //�ɵ�����

	//ֱ������
	car.straight.kp       	 	                 = 1.0;	
	car.straight.kd                              = 0.5;
	car.straight.base_speed                      = 200;


	/*************************���ٶȻ�*******************/
	//���ٶȻ� (λ��ʽ)
	car.pd_loop.kp              = 2;          //����
	car.pd_loop.ki              = 0.1;          //����
	car.pd_loop.kd              = 0;            //΢��
	car.pd_loop.duty            = 0;
	car.pd_loop.error_Now       = 0;
	car.pd_loop.error_Last      = 0;
	car.pd_loop.error_LastLast  = 0;
	car.pd_loop.expected        = 0;
	car.pd_loop.current         = 0;
	/*************************���ٶȻ�*******************/

	
	
	//��־λ
	//����
	car.start_flag    = 0;
	//��������ģʽ
	car.work_mode     = NORMAL;    	//��ʼ��ͨѰ��ģʽ
	//���ߴ���
	car.upload_enable = 0;		 	//���ߴ���ʹ��
	
	//����æµ��
	car.uart_busy     = 1;  
	//�������ѭ��ģʽ
	car.road_type     = OUT_GARAGE;
	//��̼Ƴ�ֵ
	car.distance = 0;
    

	//������
	icmdata.ACCEL_X =0;
	icmdata.ACCEL_Y =0;
	icmdata.ACCEL_Z =0;
	icmdata.GYRO_X 	=0;
	icmdata.GYRO_Y 	=0;
	icmdata.GYRO_Z  =0;
	//�Ƕ�
	icmdata.Pitch =0;
	icmdata.Yaw =0;
	icmdata.Roll =0;
	//���ٶ�
	icmdata.PitchVelocity =0;
	icmdata.RollVelocity =0;
	icmdata.YawVelocity =0;
	icmdata.YawVelocity_offset =0;//ƫ�����ٶ�ƫ��
	//��Ļ����
	car.screen.fps = 20;
	car.screen.ready = 0;
	car.screen.count = 0;
	
	
}
//�����ʼ��
void car_System_Init(){
	Beep_Init();
	//��������
	BEEP_ON;
    delay_ms(2500);
	//eeprom��ʼ��
	iap_init();
	//��Ļ��ʼ��	
	ips114_init();
	//�˵���ʼ��
//	MenuInit();
	//����4��ʼ��
    wireless_uart_init();
	//��������ʼ��
	EncoderInit();	
	//�����ʼ��
	Motor_Init();
	//adc��ʼ��
	adc_init(ADC_P00, ADC_SYSclk_DIV_2);
	adc_init(ADC_P01, ADC_SYSclk_DIV_2);
	adc_init(ADC_P05, ADC_SYSclk_DIV_2);
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);
	adc_init(ADC_P13, ADC_SYSclk_DIV_2);
	//�����ǳ�ʼ��
	//mpu6050_init();
	icm20602_init();
	//���ö�ʱ��4 ��������5ms
	pit_timer_ms(TIM_4, 5);	
    //�����ж����ȼ�
	NVIC_SetPriority(TIM_4,2);
	NVIC_SetPriority(UART4_IRQn,3);
	//��������
	BEEP_OFF;
	//tof��ʼ��
	dl1a_init();
	//��ѹ��ʼ��
	pwm_init(PWMB_CH4_P77, 50, 0);						
	pwm_init(PWMB_CH3_P33, 50, 0);  
}
//��������ʼ��
void Beep_Init()
{
	gpio_mode(BEEP_PIN,GPO_PP);
}

void Force_Stop()      //ǿ��ͣ�����رշ���
{    
	car.start_flag = STOP;
	//car.distance  = 0;
	car.steering.duty = 0;
	car.motor_average.duty = 0;
	car.motor_left.duty = 0;
	car.motor_right.duty = 0;
	pwm_duty(PWMB_CH4_P77,0);	
	pwm_duty(PWMB_CH3_P33,0);	
	MotorPWM_Out(0,0);
}

void Start_Up()     	//����
{
	BEEP_ON;
    pwm_duty(PWMB_CH4_P77,car.fan.left_duty);	
	pwm_duty(PWMB_CH3_P33,car.fan.right_duty);	
	delay_ms(2500);
    BEEP_OFF;
	car.start_flag = START;
}

Start_Flag IsStarted()//��ȡС����ǰʹ��״̬
{
	return car.start_flag;
}



