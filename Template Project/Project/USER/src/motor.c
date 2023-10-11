#include "headfile.h"
void Motor_Init()
{
    pwm_init(LEFT_MOTOR_PWM_PIN, 17000, 1);       //����pwm��ʼ��
 	pwm_init(RIGHT_MOTOR_PWM_PIN,17000, 1);       //�ҵ��pwm��ʼ��        
	gpio_mode(LEFT_MOTOR_DIR_PIN,GPO_PP);   		//����������ƹܽ��������
	gpio_mode(RIGHT_MOTOR_DIR_PIN,GPO_PP);			//�ҵ��������ƹܽ��������
}

//void MotorPID_Caculate(Motor *motor)
//{
//  motor->error_Sum += motor->error_Now;
//  if(motor->error_Sum > motor->error_Sum_Max)motor->error_Sum = motor->error_Sum_Max;
//  if(motor->error_Sum > motor->error_Sum_Mini)motor->error_Sum = motor->error_Sum_Mini;
//  motor->duty = motor->duty_Last + (int)(motor->kp*motor->error_Now + motor->ki*motor->error_Sum + motor->kd*(motor->error_Now - motor->error_Last));   //�õ�pwm���������
//  motor->error_Pre = motor->error_Last;//���������
//  motor->error_Last = motor->error_Now;
//  motor->duty_Last = motor->duty;
//}

void MotorPID_Caculate(Motor *motor)
{
    motor->error_Now = (motor->speed_Expected - motor->speed_Current);	
    motor->duty += (int)(motor->kp*(motor->error_Now - motor->error_Pre) 
    + motor->ki*motor->error_Now 
    + motor->kd*(motor->error_Now + motor->error_PrePre - 2 * motor->error_Pre));   //�õ�pwm���������
  //�޷�
	if(motor->duty > MOTOR_DUTY_MAX)motor->duty=MOTOR_DUTY_MAX;
	else if(motor->duty < -MOTOR_DUTY_MAX)motor->duty=-MOTOR_DUTY_MAX;
	motor->error_PrePre = motor->error_Pre;//���������
    motor->error_Pre = motor->error_Now;
}


void MotorPWM_Out(int left_duty,int right_duty)//����������
{
    // if(car.start_flag == STOP){
    //   LEFT_MOTOR_DIR_PIN = 0;
    //   RIGHT_MOTOR_DIR_PIN = 0;
    //   pwm_duty(LEFT_MOTOR_PWM_PIN, 0); 
	  // 	pwm_duty(RIGHT_MOTOR_PWM_PIN , 0); 
    // }
    // else{

      if(left_duty > MOTOR_DUTY_MAX)left_duty = MOTOR_DUTY_MAX;
      else if(left_duty < -MOTOR_DUTY_MAX)left_duty = -MOTOR_DUTY_MAX;

      if(right_duty > MOTOR_DUTY_MAX)right_duty = MOTOR_DUTY_MAX;
      else if(right_duty < -MOTOR_DUTY_MAX)right_duty = -MOTOR_DUTY_MAX;
      //}
    
      if(left_duty >= 0 )
      {
        LEFT_MOTOR_DIR_PIN = 0;
      }
      else if(left_duty < 0 )
      {
        LEFT_MOTOR_DIR_PIN = 1;
        left_duty = -left_duty;
      }

      if(right_duty >= 0 )
      {
        RIGHT_MOTOR_DIR_PIN = 0;
      }
      else if(right_duty < 0 )
      {
        RIGHT_MOTOR_DIR_PIN = 1;
        right_duty = -right_duty;
      }		
      pwm_duty(LEFT_MOTOR_PWM_PIN , left_duty); 
      pwm_duty(RIGHT_MOTOR_PWM_PIN, right_duty);   
}
