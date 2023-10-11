/* Includes ------------------------------------------------------------------*/
#include "hinder.h"
#include "control.h"
#include "car.h"
/* Includes ------------------------------------------------------------------*/

/****************************************************
 * @name	      hinder_parameter_init
 * @function      �ϰ����ʼ������
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/27
 ***************************************************/
 void hinder_parameter_init(){
 
	 //ʹ�ܱ��λ��
	car.hinder.enable_flag                     = 0;
	//�ϰ�״̬
	car.hinder.state                           = 0;

	//��ʼ����
	car.hinder.distance                        = 100000;

	//ʶ���ϰ������
	car.hinder.judge_distance                  = 950;

	//ʶ���ϰ����ٶ�
	car.hinder.speed                           = 250;

	//ʶ���ϰ��������ǽǶ�
	car.hinder.gyro_kp                         = 0;  
	
	//״̬1�յ�ת�Ǻ�ǰ������
	car.hinder.point_1.angle                   = 50;  
	car.hinder.point_1.distance                = 10; 

	//״̬2�յ�ת�Ǻ�ǰ������
	car.hinder.point_2.angle                   = 100;  
	car.hinder.point_2.distance                = 40;

	//��ʱ��ֵ
	car.hinder.enter_angle                     = 0;

	//��һ�ι��Ե����뾶
	car.hinder.R_circle                        = 35;

	//�ڶ��ι��Ե����뾶
	car.hinder.R_circle_2                      = 50;

	//���������ֵ
	car.hinder.adc                             = 50;
         
	//���Ϸ���
	car.hinder.dir                             = LEFT;
	
	//���ϴ���
	car.hinder.count                           = RIGHT;
 }
/****************************************************
 * @name	      hinder_judge
 * @function      �ϰ��������
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/6
 ***************************************************/
void hinder_judge()
{
	static int count_hinder =0;
	dl1a_get_distance();//��ȡ����
	if(dl1a_finsh_flag == 1)//�ж������Ƿ���Ч
	{
		car.hinder.distance = dl1a_distance_mm;
	}
	if(car.hinder.distance<car.hinder.judge_distance){//��������
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
 * @function    expect_angle: �����Ƕ�
				current_angle:ʵ�ʽǶ�
				speed��       �ٶ�
				R_circle:     ת��뾶
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
		// //�ڻ�
		car.pd_loop.current  = icmdata.YawVelocity;
		if(car.start_flag==START)
		{	
		Incremental_PID_Caculate(&car.pd_loop);//����ʽpid
		 car.steering.duty    = car.pd_loop.duty;
		 }	
	//ת��PWM�޷�
	if(car.steering.duty>300)      car.steering.duty = 300;
	else if(car.steering.duty<-300)car.steering.duty =-300;
 }
 
/****************************************************
 * @name	      hinder_control
 * @function      �ϰ���״̬��	
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/6
 ***************************************************/
void hinder_control()
{
	static int DIR;
	//�����
	if(car.hinder.dir==LEFT)
	{
		DIR=1;
	} 
	//�ұ���
		if(car.hinder.dir==RIGHT)
	{
		DIR=-1;
	}

	switch (car.hinder.state)
		{
		 /***********************״̬0����ת���Բ��*************************/
			case 0:/***��ʱ�룺����*******/
			turn_angle(DIR*car.hinder.point_1.angle,icmdata.Yaw-car.hinder.enter_angle,car.hinder.speed,car.hinder.R_circle);
			motor_control(car.hinder.speed-car.steering.duty,car.hinder.speed+car.steering.duty);		
		 	if(my_abs(icmdata.Yaw-car.hinder.enter_angle)>car.hinder.point_1.angle*0.9){
				car.hinder.state = 1;
				car.hinder.point = car.distance;
			    car.hinder.enter_angle = icmdata.Yaw;
		 	}
			break;
		 /***********************״̬0����ת���Բ��*************************/


		/************************״̬1��ǰ��һ�ξ���*************************/
			case 1:
			motor_control(car.hinder.speed,car.hinder.speed);
			if(my_abs(car.distance-car.hinder.point)>car.hinder.point_1.distance){
				car.hinder.state = 2;
				car.hinder.enter_angle = icmdata.Yaw;
			}
			break;
		/************************״̬1��ǰ��һ�ξ���*************************/


		/************************״̬2����ת���Բ��*************************/
			case 2:
		   turn_angle((-DIR)*car.hinder.point_2.angle,icmdata.Yaw-car.hinder.enter_angle,150,car.hinder.R_circle_2);
			motor_control(car.hinder.speed-car.steering.duty,car.hinder.speed+car.steering.duty);	
			if(my_abs(icmdata.Yaw-car.hinder.enter_angle)>car.hinder.point_2.angle*0.7){
				car.hinder.state= 3;
				car.hinder.enter_angle = icmdata.Yaw;
				car.hinder.point = car.distance;
			}
			break;
		/***********************״̬2����ת���Բ��***************************/
			

		/***********************״̬3��ǰ��һ�ξ���****************************/
			case 3:
			motor_control(car.hinder.speed,car.hinder.speed);
			 if(my_abs(car.distance-car.hinder.point)>car.hinder.point_2.distance)
			 {	
				car.hinder.state = 4;			
			 }	
			break;
		/***********************״̬3��ǰ��һ�ξ���****************************/

		/***********************״̬4��ת����*******************************/
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
		/***********************״̬4��ת����*******************************/

		/***********************״̬5��ת��***********************************/
			case 5:
	    	error_calculate(5+0.7*(car.distance-car.hinder.point),0.3+0.015*(car.distance-car.hinder.point));
			motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
			if(car.distance-car.hinder.point>30)
			{
			car.hinder.state = 6;
			}
			break;
		/***********************״̬5��ת��***********************************/


		/***********************״̬4���л�ֱ��*******************************/
			case 6:
			car.hinder.state = 0;
			car.road_type = STRAIGHT;
			car.hinder.distance = 100000;
			car.element.ELEMENT_NUM++;      //Ԫ�ر������1
			BEEP_OFF;
			break;
		/***********************״̬4���л�ֱ��*******************************/
	}
}