/* Includes ------------------------------------------------------------------*/
#include "control.h"
#include "math.h"
#include "circle.h"
#include "hinder.h"
#include "ramp.h"
#include "garage.h"
/* Includes ------------------------------------------------------------------*/

//����������
void Speed_Control(float left, float right);		//����ջ�����
void Speed_diff_control();							//����ջ�+���ٿ���

void Stop_after(float distance);					//����ָ�������Զ�ͣ�� ��λm

//����������
void Circle_judge_dir();   //�з��򻷵��ж�
void Out_Track_judge();	   //������

/****************************************************
 * @name	      ��ת���
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
 * @name	      ��һ�ξ���ֹͣ
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
 * @function      ���籣��
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
 * @function      ֱ��ת�����
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Straight_control()
{
	static int count_10ms = 0;

	/***************************Ԫ�ؼ��****************************/
    if(car.distance > 5)
    {		
	  /*******ͣ��********/
//		if(car.garage.MODE==0)
	if((car.element.excel[car.element.ELEMENT_NUM]==9)||(car.element.excel[car.element.ELEMENT_NUM]==10))
	{
		if(car.element.excel[car.element.ELEMENT_NUM]==9)//�����
		{
			car.garage.DIR=LEFT;
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==10)//�����
		{
			car.garage.DIR=RIGHT;
		}
	  /******������������******/
	  	Enter_Garage_Pre();
	  /******������������******/
		CitTie_judge();
	}
//		else if(car.garage.MODE==1)
//		Enter_Garage_Pre();
     /*******ͣ��********/

	 /*******Բ��********/
	 if((car.element.excel[car.element.ELEMENT_NUM]==2)||(car.element.excel[car.element.ELEMENT_NUM]==3)||(car.element.excel[car.element.ELEMENT_NUM]==4)||(car.element.excel[car.element.ELEMENT_NUM]==5)||(car.element.excel[car.element.ELEMENT_NUM]==11)||(car.element.excel[car.element.ELEMENT_NUM]==12))
	{
		Circle_judge_dir();
	} 
	 /*******Բ��********/
		
	 /*******�µ�********/
	 if(car.element.excel[car.element.ELEMENT_NUM]==8)
	 {
		Ramp_judge();
	 }
	 /*******�µ�********/

	 /*******��ת*********/
	   block_judge();
	 /*******��ת*********/
		 
	 /*******���籣��******/
	  Out_Track_judge();
	 /*******���籣��******/
/*4320&&(car.distance>4450)*/
	/*******�ϰ���*********/
	if(((car.element.excel[car.element.ELEMENT_NUM]==6)||(car.element.excel[car.element.ELEMENT_NUM]==7))
	&&((car.distance-car.circle.out_point)>car.circle_distance)
	&&((car.distance-car.ramp.in_point)>car.ramp_distance))
	{
		//�����
		if(car.element.excel[car.element.ELEMENT_NUM]==6)
		{
			car.hinder.dir=LEFT;
		}
		//�ұ���
		if(car.element.excel[car.element.ELEMENT_NUM]==7)
		{
			car.hinder.dir=RIGHT;
		}
		hinder_judge();
	}
	 /*******�ϰ���********/
}	

    Stop_after(car.meter_distance);


	/***************************Ԫ�ؼ��****************************/
	car.adc_parameter.adc_min[LEFT_H]  = 400;
	car.adc_parameter.adc_min[MIDDLE]  = 400;
	car.adc_parameter.adc_min[RIGHT_H] = 400;
	error_calculate(car.straight.kp,car.straight.kd);
	/******ת�����******/
	motor_control(car.straight.base_speed-car.steering.duty,car.straight.base_speed+car.steering.duty);
	//motor_control(car.straight.base_speed,car.straight.base_speed);
	/******ת�����******/
}

/****************************************************
 * @name	      motor_control
 * @function      Բ��
 * @parameter     left_speed  �����ٶ�
 *                right_speed �����ٶ�
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void motor_control(float left_speed,float right_speed){

	//�����ҵ������ת��
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
 * @function      ƫ�����
 * @parameter     error_kp  һ����ϵ��
 *                gyro_kp   ����������
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void error_calculate(float error_kp,float gyro_kp)
{

	car.steering.linear_error_kp = error_kp;
	car.steering.gyro_kp         = gyro_kp;

	get_error_normal(&car.adc_parameter); //����ƫ��	
	car.steering.error_now = car.adc_parameter.adc_error ;  
	//���򻷼���
	if(car.lose_flag)	//����״̬
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
		car.steering.duty = car.steering.linear_error_kp*car.steering.error_now +                                               //һ��ƫ��   
										car.steering.quadratic_error_kp*car.steering.error_now*abs(car.steering.error_now) +    //����ƫ��
										car.steering.error_kd*(car.steering.error_now - car.steering.error_last) -  		    //ƫ����
										car.steering.gyro_kp *icmdata.YawVelocity; 												                      //����������
		car.steering.error_last = car.steering.error_now;
	}
	//ת��PWM�޷�
	if(car.steering.duty>300)      car.steering.duty = 300;
	else if(car.steering.duty<-300)car.steering.duty =-300;
}
/****************************************************
 * @name	      Normal_Tracing
 * @function      Ѱ��ģʽ
 * @parameter     NULL
 * @return		  NULL
 * @date		  2023/7/4
 ***************************************************/
void Normal_Tracing()
{

	//������ݲɼ�
	ADC_Process(&car.adc_parameter);	   //��ֵ�˲�
	ADC_Normalization(&car.adc_parameter); //��һ�� 0-100
	//���������ݲɼ�
	ICM_OneOrderFilter();   
	//�ٶȲ���
	EncoderPulseGet();
	//����

	//״̬��
	switch (/*car.road_type*/STRAIGHT)
	{
		//ֱ��
		case STRAIGHT:
			Straight_control(); //ֱ���������
			break;
		//���
		case ENTER_GARAGE:
			Enter_garage_back(car.garage.DIR); //1 �ҿ� 0 ���
			break;
		//����
		case OUT_GARAGE:
			
		/*****����ⷽ�����Ԫ�ر��ж�*******/
		if(car.element.excel[car.element.ELEMENT_NUM]==0)
		{
			car.garage.DIR= 0;   //����0��1��
		}
		if(car.element.excel[car.element.ELEMENT_NUM]==1)
		{
			car.garage.DIR= 1;   //����0��1��
		}
		/*****����ⷽ�����Ԫ�ر��ж�*******/

			Out_garage(car.garage.DIR);
			break;
		//Բ��
		case CIRCLE:
			Circle_control();
			car.road_type_last = CIRCLE;
			break;
		//�µ�
		case RAMP:
			Ramp_control();
			car.road_type_last = RAMP;
		  break;
		//�ϰ�
		case HINDER:
			hinder_control();
			break;
		//�������
		case ENTER_GARAGE_forward:
			Enter_Garage_control();
			break;

	}
}