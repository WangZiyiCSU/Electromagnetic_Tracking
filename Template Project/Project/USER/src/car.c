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

/***********************元素表**************************/
/*-----
左出库：       0
右出库：       1

左小环(R50)：  2
右小环(R50):   3 

左大环(R60)：  4
右大环(R60):   5

障碍(左侧)：   6
障碍(右侧)：   7

坡道：         8

左入库：       9
右入库：       10

左大环(R90):   11
右大环(R90)：  12

------*/
/***********************元素表**************************/

/****************************************************
 * @name	      Circle_parameter_init
 * @function      圆环参数初始化
 * @parameter     NULL
 * @return		  NULL
 * @date	      2023/7/27
 ***************************************************/
 void element_parameter_init(){

 	car.element.ELEMENT_NUM = 0;
	
	/******实验室328赛道(逆时针)**********/	
	//右出库——右小环——左大环——障碍——坡道——右入库
	/******华北理工赛道******************/
	//左出库——右小环——坡道——左大环——障碍(左)——大大坡道———左入库

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
	/******实验室328赛道(逆时针)**********/	

 }
/***********************元素表**************************/
//*******************************参数初始化*************************************
void car_Parameter_Init()
{

	// car.steering.linear_error_kp 			   = 300;   //一次项比例系数
	// car.steering.quadratic_error_kp 		       = 0.2; 	//二次项比例系数
	// car.steering.error_kd 					   = 5;     //误差微分系数
	// car.steering.gyro_kp 				       = 25;  	//陀螺仪系数
	car.steering.linear_error_kp 			       = 7.5;    //一次项比例系数
	car.steering.quadratic_error_kp 		       = 0;     //二次项比例系数
	car.steering.error_kd 					       = 10;     //误差微分系数
	car.steering.gyro_kp 					       = 0;   //陀螺仪系数
	car.steering.error_now 				           = 0;     //当前偏差
	car.steering.error_last 	         		   = 0;     //上次偏差
	car.steering.duty 						       = 0;     //输出
	
    car.pd_loop.circle_p                           = 5;

	//平均电机参数
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

	//左电机参数
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

	//右电机参数
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




	//车库
	car.garage.count                         = 0;
	car.garage.in_point                      = 0;
	car.garage.base_angle                    = 0;
	car.garage.enter_flag                    = 0;
	car.garage.out_circle_point              = 0;
	//car.garage.DIR                           = RIGHT;

	car.meter_distance                       = 100;
	
	//丢线标记位
	car.lose_flag = 0;

	/************************元素表*********************/
	element_parameter_init();
	/************************元素表*********************/

	/**************************环岛*********************/
	Circle_parameter_init();  
	/**************************环岛*********************/
	
	/**************************坡道*********************/
	Ramp_parameter_init();
	/**************************坡道*********************/
	
	/**************************风扇*********************/
	car.fan.left_duty                           = 200;
	car.fan.right_duty                          = 200;
	/**************************风扇*********************/

	/**************************障碍*********************/
	hinder_parameter_init();
	/**************************障碍*********************/

	/**************************出入库*******************/
	garage_parameter_init();
	/**************************出入库*******************/

	car.ramp_distance      = 50;
	car.circle_distance    = 50;


	/**************************电感*********************/
	//电感归一化参数
	//最大值
	car.adc_parameter.adc_max[LEFT_H]        = 4000; 
	car.adc_parameter.adc_max[LEFT_X]        = 4000; 
	car.adc_parameter.adc_max[MIDDLE]        = 4000; 
	car.adc_parameter.adc_max[RIGHT_X]       = 4000; 
	car.adc_parameter.adc_max[RIGHT_H]       = 4000; 

	//最小值
	car.adc_parameter.adc_min[LEFT_H]        = 400;
	car.adc_parameter.adc_min[LEFT_X]        = 1;
	car.adc_parameter.adc_min[MIDDLE]        = 400;
	car.adc_parameter.adc_min[RIGHT_X]       = 1;
	car.adc_parameter.adc_min[RIGHT_H]       = 400;
	
	//偏差计算
	car.adc_parameter.error_caculate         = 1000;
	/**************************电感*********************/


    //可调参数

	//直道参数
	car.straight.kp       	 	                 = 1.0;	
	car.straight.kd                              = 0.5;
	car.straight.base_speed                      = 200;


	/*************************角速度环*******************/
	//角速度环 (位置式)
	car.pd_loop.kp              = 2;          //比例
	car.pd_loop.ki              = 0.1;          //积分
	car.pd_loop.kd              = 0;            //微分
	car.pd_loop.duty            = 0;
	car.pd_loop.error_Now       = 0;
	car.pd_loop.error_Last      = 0;
	car.pd_loop.error_LastLast  = 0;
	car.pd_loop.expected        = 0;
	car.pd_loop.current         = 0;
	/*************************角速度环*******************/

	
	
	//标志位
	//发车
	car.start_flag    = 0;
	//开机工作模式
	car.work_mode     = NORMAL;    	//初始普通寻迹模式
	//无线串口
	car.upload_enable = 0;		 	//无线传输使能
	
	//串口忙碌否
	car.uart_busy     = 1;  
	//开机后的循迹模式
	car.road_type     = OUT_GARAGE;
	//里程计初值
	car.distance = 0;
    

	//陀螺仪
	icmdata.ACCEL_X =0;
	icmdata.ACCEL_Y =0;
	icmdata.ACCEL_Z =0;
	icmdata.GYRO_X 	=0;
	icmdata.GYRO_Y 	=0;
	icmdata.GYRO_Z  =0;
	//角度
	icmdata.Pitch =0;
	icmdata.Yaw =0;
	icmdata.Roll =0;
	//角速度
	icmdata.PitchVelocity =0;
	icmdata.RollVelocity =0;
	icmdata.YawVelocity =0;
	icmdata.YawVelocity_offset =0;//偏航角速度偏置
	//屏幕参数
	car.screen.fps = 20;
	car.screen.ready = 0;
	car.screen.count = 0;
	
	
}
//外设初始化
void car_System_Init(){
	Beep_Init();
	//蜂鸣器响
	BEEP_ON;
    delay_ms(2500);
	//eeprom初始化
	iap_init();
	//屏幕初始化	
	ips114_init();
	//菜单初始化
//	MenuInit();
	//串口4初始化
    wireless_uart_init();
	//编码器初始化
	EncoderInit();	
	//电机初始化
	Motor_Init();
	//adc初始化
	adc_init(ADC_P00, ADC_SYSclk_DIV_2);
	adc_init(ADC_P01, ADC_SYSclk_DIV_2);
	adc_init(ADC_P05, ADC_SYSclk_DIV_2);
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);
	adc_init(ADC_P13, ADC_SYSclk_DIV_2);
	//陀螺仪初始化
	//mpu6050_init();
	icm20602_init();
	//设置定时器4 控制周期5ms
	pit_timer_ms(TIM_4, 5);	
    //设置中断优先级
	NVIC_SetPriority(TIM_4,2);
	NVIC_SetPriority(UART4_IRQn,3);
	//蜂鸣器关
	BEEP_OFF;
	//tof初始化
	dl1a_init();
	//负压初始化
	pwm_init(PWMB_CH4_P77, 50, 0);						
	pwm_init(PWMB_CH3_P33, 50, 0);  
}
//蜂鸣器初始化
void Beep_Init()
{
	gpio_mode(BEEP_PIN,GPO_PP);
}

void Force_Stop()      //强制停车并关闭风扇
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

void Start_Up()     	//发车
{
	BEEP_ON;
    pwm_duty(PWMB_CH4_P77,car.fan.left_duty);	
	pwm_duty(PWMB_CH3_P33,car.fan.right_duty);	
	delay_ms(2500);
    BEEP_OFF;
	car.start_flag = START;
}

Start_Flag IsStarted()//获取小车当前使能状态
{
	return car.start_flag;
}



