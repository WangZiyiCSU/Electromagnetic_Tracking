/* Includes ------------------------------------------------------------------*/
#include "circle.h"
#include "control.h"
#include "car.h"
/* Includes ------------------------------------------------------------------*/

/****************************************************
 * @name	      Circle_parameter_init
 * @function      圆环参数初始化
 * @parameter     NULL
 * @return		  NULL
 * @date	      2023/7/27
 ***************************************************/
 void Circle_parameter_init()
 {
 
	//环岛数量(第几个环岛)
	car.circle.cur_num = 0;		           

   //圆环识别标志位：0为不识别，1为识别

	car.circle.enable[0]                       =1;
	car.circle.enable[1]                       =1;
	car.circle.enable[2]                       =1;

	//圆环横电感数值识别条件
	car.circle.PRE_ENTER_H_STRENGTH[0]        =140;
	car.circle.PRE_ENTER_H_STRENGTH[1]        =140;
	car.circle.PRE_ENTER_H_STRENGTH[2]        =140;
	
	//圆环中间电感数值识别条件
	car.circle.PRE_ENTER_MIDDLE[0]            =70;
	car.circle.PRE_ENTER_MIDDLE[1]            =70;
	car.circle.PRE_ENTER_MIDDLE[2]            =70;
	
	//圆环内的运行速度
	car.circle.speed[0]                       =120;
	car.circle.speed[1]                       =120;
	car.circle.speed[2]                       =120;

	//预入环到入环点的距离
	car.circle.Enter_DIST[0]                  =7;
	car.circle.Enter_DIST[1]                  =44;
	car.circle.Enter_DIST[2]                  =50;
	
	//入环点电感数值
	car.circle.Enter_MID[0]                   =95;
	car.circle.Enter_MID[1]                   =80;
	car.circle.Enter_MID[2]                   =70;

	//圆环初始状态
	car.circle.state                          =0 ;
	
	//入环点转角角度度数
	car.circle.Enter_ANGLE[0]                 =30;
	car.circle.Enter_ANGLE[1]                 =30;
	car.circle.Enter_ANGLE[2]                 =30;

	//入环转角(plan_B)
	car.circle.enter_angle_plan_B[0]          = 60;
	car.circle.enter_angle_plan_B[1]          = 60;
	car.circle.enter_angle_plan_B[2]          = 60;

	//入环半径
	car.circle.enter_R_circle[0]              = 35;
	car.circle.enter_R_circle[1]              = 45;
	car.circle.enter_R_circle[2]              = 50;
	
	//出环偏置
	car.circle.BIAS_ADJUST[0]                 = -10;
	car.circle.BIAS_ADJUST[1]                 = -10;
	car.circle.BIAS_ADJUST[2]                 = -10;

	//第一个环内kp和kd
	car.circle.kp[0]                          =20;
	car.circle.kd[0]                          =1;

	//第二个环内kp和kd
	car.circle.kp[1]                          =15;
	car.circle.kd[1]                          =0.7;

	//第三个环内kp和kd
	car.circle.kp[2]                          =25;
	car.circle.kd[2]                          =0.7;

	//环内循迹打角
	car.circle.angle_1[0]                     =300;
	car.circle.angle_1[1]                     =300;
	car.circle.angle_1[2]                     =330;

	//均值打角
	car.circle.angle_2[0]                     =310;
	car.circle.angle_2[1]                     =330;
	car.circle.angle_2[2]                     =350;

	car.circle.out_point                      = 0;
 }

/****************************************************
 * @name	      Circle_judge_dir
 * @function      环岛检测
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Circle_judge_dir()
{
	static int v_error = 0;
	static int h_error = 0;
		/*-----
		左小环(R50)：  2
		右小环(R50):   3 
		左大环(R60)：  4
		右大环(R60):   5
		------*/
		/***********元素表判断圆环方向和大小*************/
		if(car.element.excel[car.element.ELEMENT_NUM]==2)
		{
			car.circle.dir     = LEFT; //方向：左
			car.circle.cur_num = 0;    //小环
		} 
		if(car.element.excel[car.element.ELEMENT_NUM]==3)
		{
			car.circle.dir     = RIGHT;//方向：右
			car.circle.cur_num = 0;    //小环
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==4)
		{
			car.circle.dir     = LEFT;//方向：左
			car.circle.cur_num = 1;   //大环
		}
	    if(car.element.excel[car.element.ELEMENT_NUM]==5)
		{
			car.circle.dir     = RIGHT;//方向：右
			car.circle.cur_num = 1;    //大环
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==11)
		{
			car.circle.dir     = LEFT; //方向：左
			car.circle.cur_num = 2;    //大环(R90)
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==12)
		{
			car.circle.dir     = RIGHT;//方向：右
			car.circle.cur_num = 2;    //大环(R90)
		}
		/******左右圆环判断条件(元素表判断方向)******/

    /****************环岛检测条件*********************/
	if(	car.adc_parameter.H_strength               > car.circle.PRE_ENTER_H_STRENGTH[car.circle.cur_num]    //横电感条件
		&&car.adc_parameter.adc_normalized[MIDDLE] > car.circle.PRE_ENTER_MIDDLE[car.circle.cur_num]   )    //中间电感条件
	{ 
		/************左右圆环判断条件*******************/
		// if(car.circle.cur_num%2==0)
		// {
		// 	car.circle.dir = RIGHT;
			
		// }
		// if(car.circle.cur_num%2==1)
		// {
		// 	car.circle.dir = LEFT;
		// }
	 	/************左右圆环判断条件*******************/
		BEEP_ON;//蜂鸣器开启
		car.circle.enter_point = car.distance;     //记录预入环点
		car.road_type = CIRCLE;			      	   //切换到环岛
		
	}
    /****************环岛检测条件*********************/
}
/****************************************************
 * @name	      Circle_control
 * @function      圆环
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Circle_control()
{
	
	static float error_ave = 0;
	static int error_count=0;
	static float ENTER_DIFF;
	
	//转过的角度
	car.circle.angle_turned =  icmdata.Yaw-car.circle.enter_angle;
	
	switch (car.circle.state)
	{
	/***********************状态0:预入环到入环点*************************/
	case 0: 
		
	
		error_calculate(car.circle.kp[car.circle.cur_num],car.circle.kd[car.circle.cur_num]);	

		if(car.circle.dir == RIGHT)    car.steering.error_now = car.adc_parameter.adc_error   ; 
		else if(car.circle.dir == LEFT)car.steering.error_now = car.adc_parameter.adc_error   ;


		//motor_control(car.straight.base_speed-car.steering.duty-30,car.straight.base_speed+car.steering.duty-30);
		motor_control(car.circle.speed[0]-car.steering.duty,car.circle.speed[0]+car.steering.duty);
		car.circle.distance_passed = car.distance - car.circle.enter_point;
		if (car.circle.distance_passed             > car.circle.Enter_DIST[car.circle.cur_num]  //入环编码器条件
		&&car.adc_parameter.adc_normalized[MIDDLE] < car.circle.Enter_MID[car.circle.cur_num])  //入环电感条件
		{
			//入环点
			BEEP_ON;
			car.circle.enter_angle = icmdata.Yaw;//记录入环点时刻的陀螺仪值
			car.circle.state = 1;                //切换下一个状态
		}
		break;
		/***********************状态0:预入环到入环点*************************/

		/***********************状态1:入环点到打角结束***********************/

		/*******方案1：变速差速入环*********/
	 case 1:
	 	if(car.circle.cur_num == 0)     car.speed_set =  car.circle.speed[car.circle.cur_num%2];
	 	else if(car.circle.cur_num == 1)car.speed_set =  car.circle.speed[car.circle.cur_num%2];
	 	ENTER_DIFF = 300-my_abs(car.circle.angle_turned)*6.6;
	 	//左环岛
	 	if (car.circle.dir == LEFT)
	 	{			
	 		motor_control(car.speed_set-ENTER_DIFF,car.speed_set+ENTER_DIFF);
	 	}
	 	//右环岛
	 	else if (car.circle.dir == RIGHT)
	 	{
	 		motor_control(car.speed_set+ENTER_DIFF,car.speed_set-ENTER_DIFF);
	 	}
	 	//打角满足设定值 切换回正常寻迹
	 	if( my_abs(car.circle.angle_turned) > car.circle.Enter_ANGLE[car.circle.cur_num%2])
	 	{
	 		car.circle.state =2;
	 	}
	 	break;
		/*******方案1：变速差速入环*********/


		/*******方案2：惯导匀速入环*********/
//	case 1:
//		error_calculate(car.circle.kp[car.circle.cur_num],car.circle.kd[car.circle.cur_num]);

//		if(car.circle.cur_num == 0)     car.speed_set =  car.circle.speed[car.circle.cur_num];
//		else if(car.circle.cur_num == 1)car.speed_set =  car.circle.speed[car.circle.cur_num];
//		//左环岛
//		if (car.circle.dir == LEFT)
//		{			
//			turn_angle(car.circle.enter_angle_plan_B[car.circle.cur_num],car.circle.angle_turned,car.speed_set,car.circle.enter_R_circle[car.circle.cur_num]);
//			motor_control(car.speed_set-car.steering.duty,car.speed_set+car.steering.duty);	
//		}
//		//右环岛
//		 if (car.circle.dir == RIGHT)
//		{
//			turn_angle(-car.circle.enter_angle_plan_B[car.circle.cur_num],car.circle.angle_turned,car.speed_set,car.circle.enter_R_circle[car.circle.cur_num]);
//			motor_control(car.speed_set-car.steering.duty,car.speed_set+car.steering.duty);
//		}
//		//打角满足设定值 切换回正常寻迹
//		if( my_abs(car.circle.angle_turned) > car.circle.Enter_ANGLE[car.circle.cur_num]*0.9)
//		{
//			car.circle.state =2;
//		}
//		break;
		/*******方案2：惯导匀速入环*********/

		/***********************状态1:入环点到打角结束***********************/

		/***********************状态2:环内循迹******************************/
	case 2://环内寻迹

		motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
		if (my_abs(car.circle.angle_turned) < car.circle.angle_1[car.circle.cur_num])  //25度-280度 限幅
		{
			if(car.adc_parameter.adc_normalized[MIDDLE]<5)car.adc_parameter.adc_normalized[MIDDLE] = 5;

			car.adc_parameter.adc_min[LEFT_H]  = 100;
			car.adc_parameter.adc_min[MIDDLE]  = 100;
			car.adc_parameter.adc_min[RIGHT_H] = 100;
			error_calculate(car.circle.kp[car.circle.cur_num],car.circle.kd[car.circle.cur_num]);
			
			error_count++;
			error_ave = error_ave + (car.steering.duty - error_ave )/error_count;
		}
		else if (my_abs(car.circle.angle_turned) < car.circle.angle_2[car.circle.cur_num])  //310度-330之间 均值打角
		{
			BEEP_ON;
			//car.speed_set = car.circle.speed[car.circle.cur_num] ;
			//用平均值出环
			car.steering.duty  =  error_ave + car.circle.BIAS_ADJUST[car.circle.cur_num];		
		}
		else //大于330度 出环
		{
			car.circle.state = 3;
			car.circle.out_point = car.distance;
			BEEP_OFF;
		}
		break;
		/***********************状态2:环内循迹******************************/

		/***********************状态3:出环*********************************/
  	case 3:
		//出环结束
		car.adc_parameter.adc_min[LEFT_H]  = 400;
		car.adc_parameter.adc_min[MIDDLE]  = 400;
		car.adc_parameter.adc_min[RIGHT_H] = 400;
  		error_calculate(car.straight.kp,car.straight.kd);
		motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
		//切换为直道
		car.circle.distance_passed =   car.distance -  car.circle.out_point;
		if (car.circle.distance_passed > 120)
		{
			car.circle.state = 0;
			//当前环岛个数加一
			car.circle.cur_num ++ ;
			//元素表个数加一
			car.element.ELEMENT_NUM++;
			car.road_type = STRAIGHT;			
			car.circle.out_point = car.distance;
			error_count =0;
			error_ave = 0;
			BEEP_OFF;
		}
		break;
		/***********************状态3:出环*********************************/

  	}
}
