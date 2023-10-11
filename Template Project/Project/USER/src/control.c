/* Includes ------------------------------------------------------------------*/
#include "control.h"
#include "math.h"
#include "circle.h"
#include "hinder.h"
#include "ramp.h"
#include "garage.h"
/* Includes ------------------------------------------------------------------*/

//电机控制相关
void Speed_Control(float left, float right);		//电机闭环控制
void Speed_diff_control();							//电机闭环+差速控制

void Stop_after(float distance);					//运行指定距离自动停车 单位m

//条件检测相关
void Circle_judge_dir();   //有方向环岛判断
void Out_Track_judge();	   //出界检测

/****************************************************
 * @name	      堵转检测
 * @function      block_judge
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/27
 ***************************************************/
void block_judge(){

	static int count = 0;

	if((my_abs(car.motor_left.speed_Current)+my_abs(car.motor_right.speed_Current))/2<50){
		count++;
		if(count>80){
			Force_Stop();
		}
	}
}
/****************************************************
 * @name	      走一段距离停止
 * @function      Stop_after
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/27
 ***************************************************/
void Stop_after(float distance)
{
	if (car.distance > distance * 100)
	{
		Force_Stop();
		car.distance = 0;
	}
}
/****************************************************
 * @name	      Out_Track_judge
 * @function      出界保护
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/12
 ***************************************************/
void Out_Track_judge()
{
	static int out_count = 0;

	if (car.road_type != OUT_GARAGE &&car.road_type != RAMP&& car.road_type != ENTER_GARAGE)
	{
		if (car.adc_parameter.adc_strength <= 10 )
		{
			out_count++;
			if (out_count > 50)
			{
				out_count = 0;
				Force_Stop();
				car.road_type = OUT_TRACK;
			}
		}
		else
		{
			out_count = 0;
		}
	}
}
/****************************************************
 * @name	      Straight_control
 * @function      直道转向控制
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Straight_control()
{
	static int count_10ms = 0;

	/***************************元素检测****************************/
    if(car.distance > 5)
    {		
	  /*******停车********/
//		if(car.garage.MODE==0)
	if((car.element.excel[car.element.ELEMENT_NUM]==9)||(car.element.excel[car.element.ELEMENT_NUM]==10))
	{
		if(car.element.excel[car.element.ELEMENT_NUM]==9)//左入库
		{
			car.garage.DIR=LEFT;
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==10)//右入库
		{
			car.garage.DIR=RIGHT;
		}
	  /******正向入库距离检测******/
	  	Enter_Garage_Pre();
	  /******正向入库距离检测******/
		CitTie_judge();
	}
//		else if(car.garage.MODE==1)
//		Enter_Garage_Pre();
     /*******停车********/

	 /*******圆环********/
	 if((car.element.excel[car.element.ELEMENT_NUM]==2)||(car.element.excel[car.element.ELEMENT_NUM]==3)||(car.element.excel[car.element.ELEMENT_NUM]==4)||(car.element.excel[car.element.ELEMENT_NUM]==5)||(car.element.excel[car.element.ELEMENT_NUM]==11)||(car.element.excel[car.element.ELEMENT_NUM]==12))
	{
		Circle_judge_dir();
	} 
	 /*******圆环********/
		
	 /*******坡道********/
	 if(car.element.excel[car.element.ELEMENT_NUM]==8)
	 {
		Ramp_judge();
	 }
	 /*******坡道********/

	 /*******堵转*********/
	   block_judge();
	 /*******堵转*********/
		 
	 /*******出界保护******/
	  Out_Track_judge();
	 /*******出界保护******/
/*4320&&(car.distance>4450)*/
	/*******障碍物*********/
	if(((car.element.excel[car.element.ELEMENT_NUM]==6)||(car.element.excel[car.element.ELEMENT_NUM]==7))
	&&((car.distance-car.circle.out_point)>car.circle_distance)
	&&((car.distance-car.ramp.in_point)>car.ramp_distance))
	{
		//左避障
		if(car.element.excel[car.element.ELEMENT_NUM]==6)
		{
			car.hinder.dir=LEFT;
		}
		//右避障
		if(car.element.excel[car.element.ELEMENT_NUM]==7)
		{
			car.hinder.dir=RIGHT;
		}
		hinder_judge();
	}
	 /*******障碍物********/
}	

    Stop_after(car.meter_distance);


	/***************************元素检测****************************/
	car.adc_parameter.adc_min[LEFT_H]  = 400;
	car.adc_parameter.adc_min[MIDDLE]  = 400;
	car.adc_parameter.adc_min[RIGHT_H] = 400;
	error_calculate(car.straight.kp,car.straight.kd);
	/******转向控制******/
	motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
	//motor_control(car.straight.base_speed,car.straight.base_speed);
	/******转向控制******/
}

/****************************************************
 * @name	      motor_control
 * @function      圆环
 * @parameter     left_speed  左轮速度
 *                right_speed 右轮速度
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void motor_control(float left_speed,float right_speed){

	//给左右电机期望转速
	car.motor_left.speed_Expected  = left_speed;
	car.motor_right.speed_Expected = right_speed;

	if(car.start_flag){
		MotorPID_Caculate(&car.motor_left);
		MotorPID_Caculate(&car.motor_right);
	}
	MotorPWM_Out(car.motor_left.duty, car.motor_right.duty);
}
/****************************************************
 * @name	      error_calculate
 * @function      偏差计算
 * @parameter     error_kp  一次项系数
 *                gyro_kp   陀螺仪抑制
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void error_calculate(float error_kp,float gyro_kp)
{

	car.steering.linear_error_kp = error_kp;
	car.steering.gyro_kp         = gyro_kp;

	get_error_normal(&car.adc_parameter); //计算偏差	
	car.steering.error_now = car.adc_parameter.adc_error ;  
	//方向环计算
	if(car.lose_flag)	//丢线状态
	{	
		if(car.adc_parameter.adc_normalized[LEFT_H]>3&&car.adc_parameter.adc_normalized[RIGHT_H]>3)
		{
			car.lose_flag = 0;
			//BEEP_OFF;
		}
	}
	else
	{
		if(car.adc_parameter.adc_normalized[LEFT_H]<1&&car.adc_parameter.adc_normalized[RIGHT_H]<1)
		{
			car.lose_flag = 1;
			//BEEP_ON;
		}
		car.steering.duty = car.steering.linear_error_kp*car.steering.error_now +                                               //一次偏差   
										car.steering.quadratic_error_kp*car.steering.error_now*abs(car.steering.error_now) +    //二次偏差
										car.steering.error_kd*(car.steering.error_now - car.steering.error_last) -  		    //偏差差分
										car.steering.gyro_kp *icmdata.YawVelocity; 												                      //陀螺仪抑制
		car.steering.error_last = car.steering.error_now;
	}
	//转向PWM限幅
	if(car.steering.duty>300)      car.steering.duty = 300;
	else if(car.steering.duty<-300)car.steering.duty =-300;
}
/****************************************************
 * @name	      Normal_Tracing
 * @function      寻迹模式
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Normal_Tracing()
{

	//电感数据采集
	ADC_Process(&car.adc_parameter);	   //均值滤波
	ADC_Normalization(&car.adc_parameter); //归一化 0-100
	//陀螺仪数据采集
	ICM_OneOrderFilter();   
	//速度测量
	EncoderPulseGet();
	//风扇

	//状态机
	switch (/*car.road_type*/STRAIGHT)
	{
		//直道
		case STRAIGHT:
			Straight_control(); //直道方向控制
			break;
		//入库
		case ENTER_GARAGE:
			Enter_garage_back(car.garage.DIR); //1 右库 0 左库
			break;
		//出库
		case OUT_GARAGE:
			
		/*****出入库方向根据元素表判断*******/
		if(car.element.excel[car.element.ELEMENT_NUM]==0)
		{
			car.garage.DIR= 0;   //（左0右1）
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==1)
		{
			car.garage.DIR= 1;   //（左0右1）
		}
		/*****出入库方向根据元素表判断*******/

			Out_garage(car.garage.DIR);
			break;
		//圆环
		case CIRCLE:
			Circle_control();
			car.road_type_last = CIRCLE;
			break;
		//坡道
		case RAMP:
			Ramp_control();
			car.road_type_last = RAMP;
		  break;
		//障碍
		case HINDER:
			hinder_control();
			break;
		//正向入库
		case ENTER_GARAGE_forward:
			Enter_Garage_control();
			break;

	}
}