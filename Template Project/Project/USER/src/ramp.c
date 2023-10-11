/* Includes ------------------------------------------------------------------*/
#include "ramp.h"
#include "control.h"
#include "car.h"
/* Includes ------------------------------------------------------------------*/

/****************************************************
 * @name	      Ramp_parameter_init
 * @function    坡道检测
 * @parameter   NULL
 * @return		  NULL
 * @date		    2023/7/27
 ***************************************************/
 void Ramp_parameter_init(){
 
	//坡道使能
	car.ramp.enable                            = 1;

	//坡道状态机
	car.ramp.state                             = 0; 

	//坡道初始俯仰角
	car.ramp.angle                             = 0;

	//坡道计数
	car.ramp.count                             = 0;

	//坡道kp和kd
	car.ramp.kp                                =15;
	car.ramp.kd                                =0.7;

	//上坡道速度
	car.ramp.speed                             =100;

	//下坡道速度
	car.ramp.speed2                            =200;

	//识别条件
	car.ramp.adc_middle                        =75;
	car.ramp.adc_left                          =50; 
	car.ramp.adc_right                         =50;

	car.ramp.distance_passed                   = 0;
	car.ramp.in_point                          = 0;
 }
 
/****************************************************
 * @name	      Ramp_judge
 * @function      坡道检测
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/27
 ***************************************************/
void Ramp_judge()
{
	
	static int step= 0;
	static int judge_count = 0;
	//下坡后一段距离再打开检测
	// if(car.ramp.enable == 0)
	// {

	// 	if ( car.distance -  car.ramp.out_point > 250 )
	// 	{
	// 		car.ramp.enable = 1;
	// 	}	
	// }
	if(car.ramp.enable == 1){
		//电感条件
		// if(car.adc_parameter.adc_normalized[MIDDLE]>80 &&
		// 	car.adc_parameter.adc_normalized[LEFT_H]<30 &&
		// 	car.adc_parameter.adc_normalized[RIGHT_H]<30 
		//if( distance_data.Dist!=0&&distance_data.Dist < 40 &&car.adc_parameter.adc_normalized[MIDDLE]>80 && my_abs(car.servo.error_Now) < 20)
		if( car.adc_parameter.adc_normalized[MIDDLE]>car.ramp.adc_middle &&
		 	car.adc_parameter.adc_normalized[LEFT_H]<car.ramp.adc_left   &&
		 	car.adc_parameter.adc_normalized[RIGHT_H]<car.ramp.adc_right )
		{
				judge_count++;
				if(judge_count>2){
//					if(car.hinder.count==0)
//					{
//						Force_Stop();
//					}
					judge_count = 0;
					car.ramp.count++;
					car.road_type = RAMP;
					car.ramp.in_point = car.distance;
					car.ramp.enable = 0;  //关闭检测
				  
					BEEP_ON;
				}
		}
		else{
			judge_count = 0;
		}
	}
}

/****************************************************
 * @name	      Ramp_control
 * @function      坡道
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/5
 ***************************************************/
void Ramp_control()
{
	//static int step_ramp = 0;

	car.ramp.distance_passed =  car.distance  - car.ramp.in_point;
	car.adc_parameter.adc_min[LEFT_H]  = 200;
	car.adc_parameter.adc_min[MIDDLE]  = 200;
	car.adc_parameter.adc_min[RIGHT_H] = 200;
	error_calculate(car.ramp.kp,car.ramp.kd);
	car.ramp.angle = icmdata.Pitch;
	// car.ramp.speed = 100+ 0.9*(car.ramp.distance_passed);
	// if(car.ramp.speed>250)car.ramp.speed = 250;


	motor_control(car.ramp.speed-car.steering.duty,car.ramp.speed+car.steering.duty);
	// if(car.ramp.distance_passed>200)
	// {
	// 	car.road_type = STRAIGHT;
	// 	BEEP_OFF;
	// }

	switch(car.ramp.state){

		case 0:
		/***********************状态0：上坡减速*************************/
		car.ramp.speed = (-3)*car.ramp.angle+250;
		if(car.ramp.speed<150)car.ramp.speed = 150;
		if(car.ramp.distance_passed>90){
			car.ramp.state = 1;
		}
		break;
		/***********************状态0：上坡减速*************************/

		case 1:
		/***********************状态1：下坡加速*************************/
		car.ramp.speed = (-1)*car.ramp.angle+150;
		if(car.ramp.speed>250)car.ramp.speed = 250;
		if(car.ramp.speed<150)car.ramp.speed = 150;
		/*****走过180距离切换直道*****/
		if(car.ramp.distance_passed>120){
			car.ramp.state  = 0;
			car.road_type   = STRAIGHT; 
			car.element.ELEMENT_NUM++;  //元素表个数加1
			car.ramp.enable = 1 ;
		}
		break;
		/***********************状态1：下坡加速*************************/

	}


}