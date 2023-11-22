/* Includes ------------------------------------------------------------------*/
#include "garage.h"
#include "control.h"
#include "car.h"
/* Includes ------------------------------------------------------------------*/

/****************************************************
 * @name	      garage_parameter_init
 * @function      出入库参数初始化
 * @parameter     NULL
 * @return		  NULL
 * @date	      2023/7/27
 ***************************************************/
void garage_parameter_init()
{
    /**************************出库*********************/

	/****方案1参数******/
	car.garage.out_garage_angle               = 50;
	car.garage.out_garage_diff                = 50;
	car.garage.out_garage_distance            = 15;
	car.garage.out_garage_speed               = 100;
	/****方案1参数******/


	/****方案2参数******/
	car.garage.out_garage_R_circle            = 35;
	car.garage.out_garage_curve_angle         = 85;
	car.garage.out_garage_curve_speed         = 100;
	/****方案2参数******/

	/**************************出库*********************/

	/**************************入库*********************/
	/*****左入库参数*****/
	car.garage.enter_garage_distance_1_left   = -10;
	car.garage.enter_garage_angle_left        = 40;
	car.garage.enter_garage_distance_2_left   = 80;
	/*****左入库参数*****/

	/*****右·入库参数*****/
	car.garage.enter_garage_distance_1_right  = 5;
	car.garage.enter_garage_angle_right       = 10;
	car.garage.enter_garage_distance_2_right  = 25;	
	car.garage.enter_garage_diff              = 100;
	/*****右入库参数*****/ 

	car.garage.enter_garage_speed             = 100;


	car.garage.Element_Select                 = CIRCLE;
	car.garage.Element_Count                  =  1;
	car.garage.MODE                           =  0;
	car.garage.Distance_to_Garage             =800;
	car.garage.state                          =  0;

	car.garage.element_flag                   =  0;

	car.garage.forward_state                  =  0;
	car.garage.zebra_flag                     =  0;
	car.garage.zebra_distance                 =  0;
	car.garage.distance_to_zebra              =  8000;
	car.garage.turn_point                     =  100;
	
	car.garage.forward_speed                  =  100;
	car.garage.forward_R                      =  15;
	car.garage.forward_distance               =  0;
	/**************************入库*********************/
}
/****************************************************
 * @name	       Magnet_Judge
 * @function       磁铁检测
 * @parameter      NULL
 * @return		   NULL
 * @date		   2023/7/4
 ***************************************************/
void Magnet_Judge()
{
	static int garage_flag = 0;

	if(garage_flag == 0)
	{
		 if((HALL_PIN == 0)&&(car.garage.zebra_flag == 0))
		 {
			//暂时关闭磁铁检测
			garage_flag = 1;
			//记录磁铁位置
			car.garage.in_point = car.distance;
			
			//记录检测的次数
			car.garage.count ++;
			if(car.garage.count >= 1)
			{
				car.road_type = ENTER_GARAGE;//切换到入库状态
			}
			BEEP_ON;	
		}
	}
	//延时一段距离再检测磁铁
	else if(garage_flag == 1)
	{
		if (my_abs(car.distance - car.garage.in_point) > 100)
		{
			garage_flag = 0;
			BEEP_OFF;
		}
	}
}
 /****************************************************
 * @name	    Enter_Garage_Pre
 * @function    正向入库距离检测
 * @parameter   NULL
 * @return		NULL
 * @date		2023/8/12
 ***************************************************/
void Enter_Garage_Pre()
{
	if(car.garage.element_flag == 0)
	{
		car.garage.last_elemnent_point = car.distance;
		car.garage.element_flag = 1;
	}
	car.garage.distance_passed = car.distance - car.garage.last_elemnent_point;

	if((car.adc_parameter.adc_error < 40)&&(car.garage.distance_passed > car.garage.distance_to_zebra))
	{
		car.straight.base_speed = 120;
		if(/*(P50 == 0)||(P51 == 0)*/ HALL_PIN == 0)
		{				
			car.garage.zebra_flag = 1;
			car.road_type = ENTER_GARAGE_forward;
			car.garage.angle_forward = icmdata.Yaw;
		}
	}
}
 /****************************************************
 * @name	    Enter_Garage_control
 * @function    正向入库控制
 * @parameter   NULL
 * @return		NULL
 * @date		2023/8/12
 ***************************************************/
void Enter_Garage_control()
{
	static int DIR = 0;
	static int temp_distance = 0;
	if(car.garage.DIR==LEFT)
	{
		DIR = 1;
	}
	if(car.garage.DIR==RIGHT)
	{
		DIR = -1;
	}
	switch(car.garage.forward_state)
	{
		case 0:

		//turn_angle(DIR*car.garage.turn_point,icmdata.Yaw-car.garage.angle_forward,car.garage.forward_speed,car.garage.forward_R);
        //motor_control(-car.garage.enter_garage_speed-car.garage.enter_garage_diff,-car.garage.enter_garage_speed+car.garage.enter_garage_diff);
		car.motor_left.ki                         = 10;
		car.motor_right.ki                        = 10;
		motor_control(car.garage.forward_speed - DIR*car.garage.forward_R,car.garage.forward_speed+DIR*car.garage.forward_R);

		if(my_abs(icmdata.Yaw-car.garage.angle_forward) > car.garage.turn_point*0.9)
		{
			car.garage.forward_state = 1;
			temp_distance = car.distance;
	 	}
		break;

		case 1:

			motor_control(car.garage.forward_speed + 50,car.garage.forward_speed + 50);
			if((car.distance-temp_distance)>car.garage.forward_distance)
			{
				car.garage.forward_state = 2;
			}

		break;

		case 2:

			pwm_duty(PWMB_CH4_P77, 0);						
	        pwm_duty(PWMB_CH3_P33, 0); 
			motor_control(0,0);
	}
}
/****************************************************
 * @name	      Out_garage(int dir)
 * @function      出库
 * @parameter     方向dir
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Out_garage(int dir)
{

	static int step = 0;
	static int count = 0;
	static int a;
	switch (step)
	{
	case 0: //没发车
		if (IsStarted())
		{
			car.garage.in_point = car.distance;
			step = 1;
		}
		break;
	case 1: //发车 先往前冲
		motor_control(car.garage.out_garage_speed,car.garage.out_garage_speed);

		if(car.distance - car.garage.in_point > car.garage.out_garage_distance)
		{
			car.garage.base_angle = icmdata.Yaw;
			step = 2;
            car.element.ELEMENT_NUM++;     //元素表个数加1
		}
		break;
	case 2: //转弯
/*************方案1：固定差速转弯，转角达到期望值切到直道********************/
	if(dir == LEFT)
	{
		motor_control(car.garage.out_garage_speed-car.garage.out_garage_diff,car.garage.out_garage_speed+car.garage.out_garage_diff);
 	}
 	else if(dir == RIGHT)
 	{
 		motor_control(car.garage.out_garage_speed+car.garage.out_garage_diff,car.garage.out_garage_speed-car.garage.out_garage_diff);
 	}
 	if (my_abs(icmdata.Yaw - car.garage.base_angle) >car.garage.out_garage_angle)
 	{
        car.road_type = STRAIGHT;
 		step = 0;
 	}
/*************方案1：固定差速转弯，转角达到期望值切到直道********************/


/*************方案2：固定转角，达到期望值切到直道****************************/
		
//		error_calculate(car.straight.kp,car.straight.kd);
//		//左正右负
//		if(dir == LEFT)
//		{
//			turn_angle(car.garage.out_garage_curve_angle,icmdata.Yaw-car.garage.base_angle,car.garage.out_garage_curve_speed,car.garage.out_garage_R_circle);
//			motor_control(car.garage.out_garage_curve_speed-car.steering.duty,car.garage.out_garage_curve_speed+car.steering.duty);	
//		}
//		else if(dir == RIGHT)
//		{
//			turn_angle(-car.garage.out_garage_curve_angle,icmdata.Yaw-car.garage.base_angle,car.garage.out_garage_curve_speed,car.garage.out_garage_R_circle);
//			motor_control(car.garage.out_garage_curve_speed-car.steering.duty,car.garage.out_garage_curve_speed+car.steering.duty);	
//		}

//		if(my_abs(icmdata.Yaw-car.garage.base_angle)>car.garage.out_garage_curve_angle*0.9)
//        {
//      		
//			car.road_type = STRAIGHT;
//			step = 0;
//		}
/*************方案2：固定转角，达到期望值切到直道****************************/
		break;
	}
}
/****************************************************
 * @name	      Enter_garage_back
 * @function      倒车入库
 * @parameter     方向dir
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Enter_garage_back(int dir)
{
    static int count = 0;
	static int dis_2 = 0;
	
	switch (car.garage.state)
	{
	case 0: //阶段0 刹车
		BEEP_ON;
		if(car.garage.DIR == RIGHT)
		{
		//速度控制 缓慢刹车
			motor_control(0, 0);	
			//判断是否刹停
			if (my_abs(car.speed_ave) < 10)
			{
				count++;
				if (count > 2)
				{
					BEEP_OFF;
					car.garage.state = 1;
					count = 0;
					car.garage.in_point   = car.distance;
					car.garage.base_angle = icmdata.Yaw;
					break;
				}
			}
		}

		if(car.garage.DIR == LEFT)
		{
						//速度控制 缓慢刹车
			motor_control(0,0);	
			//判断是否刹停
			if (my_abs(car.speed_ave)< 10)
			{
				count++;
				if (count > 2)
				{
					BEEP_OFF;
					car.garage.state = 1;
					count = 0;
					car.garage.in_point   = car.distance;
					car.garage.base_angle = icmdata.Yaw;
					break;
				}
			}
		}
		break;
	
	case 1: //阶段1 倒车
		BEEP_ON;
		//速度控制 缓慢刹车
		motor_control(-car.garage.enter_garage_speed,-car.garage.enter_garage_speed);
		//判断是否刹停
		//倒车距离达到
		if(car.garage.DIR == LEFT)
		{
			if(my_abs(car.garage.in_point - car.distance) > car.garage.enter_garage_distance_1_left)
			{
				car.garage.in_point = car.distance;
				car.garage.base_angle = icmdata.Yaw;
				car.garage.state = 2;
			}
		}
		if(car.garage.DIR == RIGHT)
		{
			if(my_abs(car.garage.in_point - car.distance) > car.garage.enter_garage_distance_1_right)
			{
				car.garage.in_point = car.distance;
				car.garage.base_angle = icmdata.Yaw;
				car.garage.state = 2;
			}
		}
		break;
	case 2: //阶段2 转向速度控制
	/***********************************方案1：固定转角入库*****************************************/
	   if(car.garage.DIR == RIGHT)
		{
			motor_control(-car.garage.enter_garage_speed-car.garage.enter_garage_diff,-car.garage.enter_garage_speed+car.garage.enter_garage_diff);
			
				if (my_abs(icmdata.Yaw - car.garage.base_angle) > car.garage.enter_garage_angle_right*0.9) 
			{	
				car.garage.in_point = car.distance;
				car.garage.state = 3;
			}
		}	
		// //转向和速度控制
		if(car.garage.DIR==LEFT)
		{	motor_control(-car.garage.enter_garage_speed+car.garage.enter_garage_diff,-car.garage.enter_garage_speed-car.garage.enter_garage_diff);
			
				if (my_abs(icmdata.Yaw - car.garage.base_angle) > car.garage.enter_garage_angle_left*0.9) 
			{	
			car.garage.in_point = car.distance;
			car.garage.state = 3;
			}
		}

	/***********************************方案2：固定转角入库*******************************************/
		break;

	case 3: //阶段2 倒车
		//转向和速度控制	
		if(car.garage.DIR==LEFT)
		{
			dis_2 = car.garage.enter_garage_distance_2_left;
		}
		if(car.garage.DIR==RIGHT)
		{
			dis_2 = car.garage.enter_garage_distance_2_right;
		}


		motor_control(-car.garage.enter_garage_speed,-car.garage.enter_garage_speed);
		//倒车距离达到
		if (my_abs(car.garage.in_point - car.distance) > dis_2)
		{
			car.garage.state = 4;
		}
		break;
	case 4://阶段3 倒车并关闭风扇
		motor_control(0,0);

		pwm_duty(PWMB_CH4_P77, 0);						
		pwm_duty(PWMB_CH3_P33, 0); 
		break;
 	}
 }

/****************************************************
 * @name	      Enter_garage_front
 * @function      正向入库
 * @parameter     方向dir
 * @return		  NULL
 * @date		  2023/7/10
 ***************************************************/
void Enter_garage_front(int dir)
{
	static int step =0;
	switch (step)
	{
	case 0: 
		if (dir == LEFT)
		{
			motor_control(car.garage.enter_garage_speed-car.garage.enter_garage_diff,car.garage.enter_garage_speed+car.garage.enter_garage_diff);
			if (icmdata.Yaw - car.garage.base_angle > 40)
			{
				step = 1;
				car.garage.in_point = car.distance;
			}
		}
		else if (dir == RIGHT)
		{
			motor_control(car.garage.enter_garage_speed+car.garage.enter_garage_diff,car.garage.enter_garage_speed-car.garage.enter_garage_diff);
			if (icmdata.Yaw - car.garage.base_angle < -40)
			{
				step = 1;
				car.garage.in_point = car.distance;
			}
		}
		break;
	case 1:
    motor_control(car.garage.enter_garage_speed,car.garage.enter_garage_speed);
		if (car.distance - car.garage.in_point > 10)
		{
			step = 2;
		}
		break;
	case 2:
    motor_control(0,0);
		Force_Stop();
		break;
	}
}
