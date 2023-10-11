/* Includes ------------------------------------------------------------------*/
#include "hinder.h"
#include "control.h"
#include "car.h"
/* Includes ------------------------------------------------------------------*/

/****************************************************
 * @name	      hinder_parameter_init
 * @function      障碍物初始化参数
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/27
 ***************************************************/
 void hinder_parameter_init(){
 
	 //使能标记位置
	car.hinder.enable_flag                     = 0;
	//障碍状态
	car.hinder.state                           = 0;

	//初始距离
	car.hinder.distance                        = 100000;

	//识别障碍物距离
	car.hinder.judge_distance                  = 950;

	//识别障碍物速度
	car.hinder.speed                           = 250;

	//识别障碍物陀螺仪角度
	car.hinder.gyro_kp                         = 0;  
	
	//状态1拐点转角和前进距离
	car.hinder.point_1.angle                   = 50;  
	car.hinder.point_1.distance                = 10; 

	//状态2拐点转角和前进距离
	car.hinder.point_2.angle                   = 100;  
	car.hinder.point_2.distance                = 40;

	//临时赋值
	car.hinder.enter_angle                     = 0;

	//第一段惯性导航半径
	car.hinder.R_circle                        = 35;

	//第二段惯性导航半径
	car.hinder.R_circle_2                      = 50;

	//电感条件数值
	car.hinder.adc                             = 50;
         
	//避障方向
	car.hinder.dir                             = LEFT;
	
	//避障次数
	car.hinder.count                           = RIGHT;
 }
/****************************************************
 * @name	      hinder_judge
 * @function      障碍检测条件
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/6
 ***************************************************/
void hinder_judge()
{
	static int count_hinder =0;
	dl1a_get_distance();//读取距离
	if(dl1a_finsh_flag == 1)//判断数据是否有效
	{
		car.hinder.distance = dl1a_distance_mm;
	}
	if(car.hinder.distance<car.hinder.judge_distance){//距离条件
            BEEP_ON;
			count_hinder++;
			car.road_type = HINDER;
			car.hinder.enter_angle = icmdata.Yaw;
			car.hinder.initial_angle = icmdata.Yaw;
			//car.hinder.enable_flag = 1;
			car.hinder.count++;
			
	}
}
/****************************************************
 * @name	    turn_angle
 * @function    expect_angle: 期望角度
				current_angle:实际角度
				speed：       速度
				R_circle:     转弯半径
 * @parameter   NULL
 * @return		NULL
 * @date		2023/7/10
 ***************************************************/
 void turn_angle(float expect_angle,float current_angle,float speed,float R_circle)
 {
	 static int count = 0;
	 static float w = 0 ;
	 static float diff_angle;

		if(count>=3){
			diff_angle=expect_angle-current_angle;
			count= 0 ;
		}
	
		count++;
		car.pd_loop.expected = car.pd_loop.circle_p*diff_angle;

		w = my_abs(speed/R_circle)*57.3;
		if(car.pd_loop.expected > w )      car.pd_loop.expected = w;
		else if(car.pd_loop.expected < -w )car.pd_loop.expected = -w;
		// //内环
		car.pd_loop.current  = icmdata.YawVelocity;
		if(car.start_flag==START)
		{	
		Incremental_PID_Caculate(&car.pd_loop);//增量式pid
		 car.steering.duty    = car.pd_loop.duty;
		 }	
	//转向PWM限幅
	if(car.steering.duty>300)      car.steering.duty = 300;
	else if(car.steering.duty<-300)car.steering.duty =-300;
 }
 
/****************************************************
 * @name	      hinder_control
 * @function      障碍物状态机	
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/6
 ***************************************************/
void hinder_control()
{
	static int DIR;
	//左避障
	if(car.hinder.dir==LEFT)
	{
		DIR=1;
	} 
	//右避障
		if(car.hinder.dir==RIGHT)
	{
		DIR=-1;
	}

	switch (car.hinder.state)
		{
		 /***********************状态0：左转打角圆弧*************************/
			case 0:/***逆时针：正数*******/
			turn_angle(DIR*car.hinder.point_1.angle,icmdata.Yaw-car.hinder.enter_angle,car.hinder.speed,car.hinder.R_circle);
			motor_control(car.hinder.speed-car.steering.duty,car.hinder.speed+car.steering.duty);		
		 	if(my_abs(icmdata.Yaw-car.hinder.enter_angle)>car.hinder.point_1.angle*0.9){
				car.hinder.state = 1;
				car.hinder.point = car.distance;
			    car.hinder.enter_angle = icmdata.Yaw;
		 	}
			break;
		 /***********************状态0：左转打角圆弧*************************/


		/************************状态1：前进一段距离*************************/
			case 1:
			motor_control(car.hinder.speed,car.hinder.speed);
			if(my_abs(car.distance-car.hinder.point)>car.hinder.point_1.distance){
				car.hinder.state = 2;
				car.hinder.enter_angle = icmdata.Yaw;
			}
			break;
		/************************状态1：前进一段距离*************************/


		/************************状态2：右转打角圆弧*************************/
			case 2:
		   turn_angle((-DIR)*car.hinder.point_2.angle,icmdata.Yaw-car.hinder.enter_angle,150,car.hinder.R_circle_2);
			motor_control(car.hinder.speed-car.steering.duty,car.hinder.speed+car.steering.duty);	
			if(my_abs(icmdata.Yaw-car.hinder.enter_angle)>car.hinder.point_2.angle*0.7){
				car.hinder.state= 3;
				car.hinder.enter_angle = icmdata.Yaw;
				car.hinder.point = car.distance;
			}
			break;
		/***********************状态2：右转打角圆弧***************************/
			

		/***********************状态3：前进一段距离****************************/
			case 3:
			motor_control(car.hinder.speed,car.hinder.speed);
			 if(my_abs(car.distance-car.hinder.point)>car.hinder.point_2.distance)
			 {	
				car.hinder.state = 4;			
			 }	
			break;
		/***********************状态3：前进一段距离****************************/

		/***********************状态4：转弯打角*******************************/
			case 4:
			turn_angle(DIR*car.hinder.point_1.angle,icmdata.Yaw-car.hinder.enter_angle,car.hinder.speed,30);
			motor_control(car.hinder.speed-car.steering.duty,car.hinder.speed+car.steering.duty);	
			if((my_abs(icmdata.Yaw-car.hinder.enter_angle)>car.hinder.point_1.angle*0.9)||(my_abs(icmdata.Yaw-car.hinder.initial_angle)<10))
			{
				  car.hinder.state = 5;
				  car.hinder.point = car.distance;
			    car.hinder.enter_angle = icmdata.Yaw;
		 	}
			break;
		/***********************状态4：转弯打角*******************************/

		/***********************状态5：转弯***********************************/
			case 5:
	    	error_calculate(5+0.7*(car.distance-car.hinder.point),0.3+0.015*(car.distance-car.hinder.point));
			motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
			if(car.distance-car.hinder.point>30)
			{
			car.hinder.state = 6;
			}
			break;
		/***********************状态5：转弯***********************************/


		/***********************状态4：切换直道*******************************/
			case 6:
			car.hinder.state = 0;
			car.road_type = STRAIGHT;
			car.hinder.distance = 100000;
			car.element.ELEMENT_NUM++;      //元素表个数加1
			BEEP_OFF;
			break;
		/***********************状态4：切换直道*******************************/
	}
}