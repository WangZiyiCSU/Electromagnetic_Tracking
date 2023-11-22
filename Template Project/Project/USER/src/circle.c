/* Includes ------------------------------------------------------------------*/
#include "circle.h"
#include "control.h"
#include "car.h"
/* Includes ------------------------------------------------------------------*/

/****************************************************
 * @name	      Circle_parameter_init
 * @function      Բ��������ʼ��
 * @parameter     NULL
 * @return		  NULL
 * @date	      2023/7/27
 ***************************************************/
 void Circle_parameter_init()
 {
 
	//��������(�ڼ�������)
	car.circle.cur_num = 0;		           

   //Բ��ʶ���־λ��0Ϊ��ʶ��1Ϊʶ��

	car.circle.enable[0]                       =1;
	car.circle.enable[1]                       =1;
	car.circle.enable[2]                       =1;

	//Բ��������ֵʶ������
	car.circle.PRE_ENTER_H_STRENGTH[0]        =140;
	car.circle.PRE_ENTER_H_STRENGTH[1]        =140;
	car.circle.PRE_ENTER_H_STRENGTH[2]        =140;
	
	//Բ���м�����ֵʶ������
	car.circle.PRE_ENTER_MIDDLE[0]            =70;
	car.circle.PRE_ENTER_MIDDLE[1]            =70;
	car.circle.PRE_ENTER_MIDDLE[2]            =70;
	
	//Բ���ڵ������ٶ�
	car.circle.speed[0]                       =120;
	car.circle.speed[1]                       =120;
	car.circle.speed[2]                       =120;

	//Ԥ�뻷���뻷��ľ���
	car.circle.Enter_DIST[0]                  =7;
	car.circle.Enter_DIST[1]                  =44;
	car.circle.Enter_DIST[2]                  =50;
	
	//�뻷������ֵ
	car.circle.Enter_MID[0]                   =95;
	car.circle.Enter_MID[1]                   =80;
	car.circle.Enter_MID[2]                   =70;

	//Բ����ʼ״̬
	car.circle.state                          =0 ;
	
	//�뻷��ת�ǽǶȶ���
	car.circle.Enter_ANGLE[0]                 =30;
	car.circle.Enter_ANGLE[1]                 =30;
	car.circle.Enter_ANGLE[2]                 =30;

	//�뻷ת��(plan_B)
	car.circle.enter_angle_plan_B[0]          = 60;
	car.circle.enter_angle_plan_B[1]          = 60;
	car.circle.enter_angle_plan_B[2]          = 60;

	//�뻷�뾶
	car.circle.enter_R_circle[0]              = 35;
	car.circle.enter_R_circle[1]              = 45;
	car.circle.enter_R_circle[2]              = 50;
	
	//����ƫ��
	car.circle.BIAS_ADJUST[0]                 = -10;
	car.circle.BIAS_ADJUST[1]                 = -10;
	car.circle.BIAS_ADJUST[2]                 = -10;

	//��һ������kp��kd
	car.circle.kp[0]                          =20;
	car.circle.kd[0]                          =1;

	//�ڶ�������kp��kd
	car.circle.kp[1]                          =15;
	car.circle.kd[1]                          =0.7;

	//����������kp��kd
	car.circle.kp[2]                          =25;
	car.circle.kd[2]                          =0.7;

	//����ѭ�����
	car.circle.angle_1[0]                     =300;
	car.circle.angle_1[1]                     =300;
	car.circle.angle_1[2]                     =330;

	//��ֵ���
	car.circle.angle_2[0]                     =310;
	car.circle.angle_2[1]                     =330;
	car.circle.angle_2[2]                     =350;

	car.circle.out_point                      = 0;
 }

/****************************************************
 * @name	      Circle_judge_dir
 * @function      �������
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Circle_judge_dir()
{
	static int v_error = 0;
	static int h_error = 0;
		/*-----
		��С��(R50)��  2
		��С��(R50):   3 
		���(R60)��  4
		�Ҵ�(R60):   5
		------*/
		/***********Ԫ�ر��ж�Բ������ʹ�С*************/
		if(car.element.excel[car.element.ELEMENT_NUM]==2)
		{
			car.circle.dir     = LEFT; //������
			car.circle.cur_num = 0;    //С��
		} 
		if(car.element.excel[car.element.ELEMENT_NUM]==3)
		{
			car.circle.dir     = RIGHT;//������
			car.circle.cur_num = 0;    //С��
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==4)
		{
			car.circle.dir     = LEFT;//������
			car.circle.cur_num = 1;   //��
		}
	    if(car.element.excel[car.element.ELEMENT_NUM]==5)
		{
			car.circle.dir     = RIGHT;//������
			car.circle.cur_num = 1;    //��
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==11)
		{
			car.circle.dir     = LEFT; //������
			car.circle.cur_num = 2;    //��(R90)
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==12)
		{
			car.circle.dir     = RIGHT;//������
			car.circle.cur_num = 2;    //��(R90)
		}
		/******����Բ���ж�����(Ԫ�ر��жϷ���)******/

    /****************�����������*********************/
	if(	car.adc_parameter.H_strength               > car.circle.PRE_ENTER_H_STRENGTH[car.circle.cur_num]    //��������
		&&car.adc_parameter.adc_normalized[MIDDLE] > car.circle.PRE_ENTER_MIDDLE[car.circle.cur_num]   )    //�м�������
	{ 
		/************����Բ���ж�����*******************/
		// if(car.circle.cur_num%2==0)
		// {
		// 	car.circle.dir = RIGHT;
			
		// }
		// if(car.circle.cur_num%2==1)
		// {
		// 	car.circle.dir = LEFT;
		// }
	 	/************����Բ���ж�����*******************/
		BEEP_ON;//����������
		car.circle.enter_point = car.distance;     //��¼Ԥ�뻷��
		car.road_type = CIRCLE;			      	   //�л�������
		
	}
    /****************�����������*********************/
}
/****************************************************
 * @name	      Circle_control
 * @function      Բ��
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Circle_control()
{
	
	static float error_ave = 0;
	static int error_count=0;
	static float ENTER_DIFF;
	
	//ת���ĽǶ�
	car.circle.angle_turned =  icmdata.Yaw-car.circle.enter_angle;
	
	switch (car.circle.state)
	{
	/***********************״̬0:Ԥ�뻷���뻷��*************************/
	case 0: 
		
	
		error_calculate(car.circle.kp[car.circle.cur_num],car.circle.kd[car.circle.cur_num]);	

		if(car.circle.dir == RIGHT)    car.steering.error_now = car.adc_parameter.adc_error   ; 
		else if(car.circle.dir == LEFT)car.steering.error_now = car.adc_parameter.adc_error   ;


		//motor_control(car.straight.base_speed-car.steering.duty-30,car.straight.base_speed+car.steering.duty-30);
		motor_control(car.circle.speed[0]-car.steering.duty,car.circle.speed[0]+car.steering.duty);
		car.circle.distance_passed = car.distance - car.circle.enter_point;
		if (car.circle.distance_passed             > car.circle.Enter_DIST[car.circle.cur_num]  //�뻷����������
		&&car.adc_parameter.adc_normalized[MIDDLE] < car.circle.Enter_MID[car.circle.cur_num])  //�뻷�������
		{
			//�뻷��
			BEEP_ON;
			car.circle.enter_angle = icmdata.Yaw;//��¼�뻷��ʱ�̵�������ֵ
			car.circle.state = 1;                //�л���һ��״̬
		}
		break;
		/***********************״̬0:Ԥ�뻷���뻷��*************************/

		/***********************״̬1:�뻷�㵽��ǽ���***********************/

		/*******����1�����ٲ����뻷*********/
	 case 1:
	 	if(car.circle.cur_num == 0)     car.speed_set =  car.circle.speed[car.circle.cur_num%2];
	 	else if(car.circle.cur_num == 1)car.speed_set =  car.circle.speed[car.circle.cur_num%2];
	 	ENTER_DIFF = 300-my_abs(car.circle.angle_turned)*6.6;
	 	//�󻷵�
	 	if (car.circle.dir == LEFT)
	 	{			
	 		motor_control(car.speed_set-ENTER_DIFF,car.speed_set+ENTER_DIFF);
	 	}
	 	//�һ���
	 	else if (car.circle.dir == RIGHT)
	 	{
	 		motor_control(car.speed_set+ENTER_DIFF,car.speed_set-ENTER_DIFF);
	 	}
	 	//��������趨ֵ �л�������Ѱ��
	 	if( my_abs(car.circle.angle_turned) > car.circle.Enter_ANGLE[car.circle.cur_num%2])
	 	{
	 		car.circle.state =2;
	 	}
	 	break;
		/*******����1�����ٲ����뻷*********/


		/*******����2���ߵ������뻷*********/
//	case 1:
//		error_calculate(car.circle.kp[car.circle.cur_num],car.circle.kd[car.circle.cur_num]);

//		if(car.circle.cur_num == 0)     car.speed_set =  car.circle.speed[car.circle.cur_num];
//		else if(car.circle.cur_num == 1)car.speed_set =  car.circle.speed[car.circle.cur_num];
//		//�󻷵�
//		if (car.circle.dir == LEFT)
//		{			
//			turn_angle(car.circle.enter_angle_plan_B[car.circle.cur_num],car.circle.angle_turned,car.speed_set,car.circle.enter_R_circle[car.circle.cur_num]);
//			motor_control(car.speed_set-car.steering.duty,car.speed_set+car.steering.duty);	
//		}
//		//�һ���
//		 if (car.circle.dir == RIGHT)
//		{
//			turn_angle(-car.circle.enter_angle_plan_B[car.circle.cur_num],car.circle.angle_turned,car.speed_set,car.circle.enter_R_circle[car.circle.cur_num]);
//			motor_control(car.speed_set-car.steering.duty,car.speed_set+car.steering.duty);
//		}
//		//��������趨ֵ �л�������Ѱ��
//		if( my_abs(car.circle.angle_turned) > car.circle.Enter_ANGLE[car.circle.cur_num]*0.9)
//		{
//			car.circle.state =2;
//		}
//		break;
		/*******����2���ߵ������뻷*********/

		/***********************״̬1:�뻷�㵽��ǽ���***********************/

		/***********************״̬2:����ѭ��******************************/
	case 2://����Ѱ��

		motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
		if (my_abs(car.circle.angle_turned) < car.circle.angle_1[car.circle.cur_num])  //25��-280�� �޷�
		{
			if(car.adc_parameter.adc_normalized[MIDDLE]<5)car.adc_parameter.adc_normalized[MIDDLE] = 5;

			car.adc_parameter.adc_min[LEFT_H]  = 100;
			car.adc_parameter.adc_min[MIDDLE]  = 100;
			car.adc_parameter.adc_min[RIGHT_H] = 100;
			error_calculate(car.circle.kp[car.circle.cur_num],car.circle.kd[car.circle.cur_num]);
			
			error_count++;
			error_ave = error_ave + (car.steering.duty - error_ave )/error_count;
		}
		else if (my_abs(car.circle.angle_turned) < car.circle.angle_2[car.circle.cur_num])  //310��-330֮�� ��ֵ���
		{
			BEEP_ON;
			//car.speed_set = car.circle.speed[car.circle.cur_num] ;
			//��ƽ��ֵ����
			car.steering.duty  =  error_ave + car.circle.BIAS_ADJUST[car.circle.cur_num];		
		}
		else //����330�� ����
		{
			car.circle.state = 3;
			car.circle.out_point = car.distance;
			BEEP_OFF;
		}
		break;
		/***********************״̬2:����ѭ��******************************/

		/***********************״̬3:����*********************************/
  	case 3:
		//��������
		car.adc_parameter.adc_min[LEFT_H]  = 400;
		car.adc_parameter.adc_min[MIDDLE]  = 400;
		car.adc_parameter.adc_min[RIGHT_H] = 400;
  		error_calculate(car.straight.kp,car.straight.kd);
		motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
		//�л�Ϊֱ��
		car.circle.distance_passed =   car.distance -  car.circle.out_point;
		if (car.circle.distance_passed > 120)
		{
			car.circle.state = 0;
			//��ǰ����������һ
			car.circle.cur_num ++ ;
			//Ԫ�ر������һ
			car.element.ELEMENT_NUM++;
			car.road_type = STRAIGHT;			
			car.circle.out_point = car.distance;
			error_count =0;
			error_ave = 0;
			BEEP_OFF;
		}
		break;
		/***********************״̬3:����*********************************/

  	}
}
