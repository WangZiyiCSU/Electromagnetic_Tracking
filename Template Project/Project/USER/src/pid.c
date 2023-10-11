/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "zf_pwm.h"
#include "math.h"
/* Includes ------------------------------------------------------------------*/

//Servo servo;
#define PID_DUTY_MAX 9000
//����pid����
void Set_Pid(Servo *servo,float kp,float kd,float ki){
	servo->kp = kp;
	servo->kd = kd;
	servo->ki = ki;
}
void Set_Pid1(Servo *servo,float kp,float kd,float ki){
	servo->kp = kp;
	servo->kd = kd+5;
	servo->ki = ki;
}

void  Clear_PID_Para(Servo *servo)
{
	servo->error_Now = 0;
	servo->error_Last = 0;
}

void Set_Pd(PD_LOOP *pd,float kp,float kd){
  pd->kp = kp;
  pd->kd = kd;
}

//���ַ�����ֵ
#define SPE_INTEGRAL_START_ERR 100
#define SPE_INTEGRAL_MAX_VAL 4000
//λ��ʽPID������
void Position_PID_Caculate(PD_LOOP *pid)
{ 
  //����ƫ��
  pid->error_Now = pid->current - pid->expected;
  /*��������ַ��룬ƫ��ϴ�ʱȥ����������*/
  if(pid->error_Now > -SPE_INTEGRAL_START_ERR && pid->error_Now < SPE_INTEGRAL_START_ERR) //��ֵ��Χ�� ʹ�û���
  {
      pid->integral += pid->error_Now;  
      /*���ַ�Χ�޶�����ֹ���ֱ���*/
      if(pid->integral > SPE_INTEGRAL_MAX_VAL)
      {
          pid->integral = SPE_INTEGRAL_MAX_VAL;
      }
      else if(pid->integral < -SPE_INTEGRAL_MAX_VAL)
      {
          pid->integral = -SPE_INTEGRAL_MAX_VAL;
      }
  }
  // else
  // {
  //   pid->integral = 0;//ƫ��ϴ� ��ʹ�û���
  // }   

  //�������
  pid->duty = pid->kp*pid->error_Now +                   //����
              pid->ki*pid->integral  +                   //����
              pid->kd*(pid->error_Now - pid->error_Last);  //΢��

  pid->error_Last = pid->error_Now;
}

//λ��ʽPD������
void Position_PD_Caculate(PD_LOOP *pd)
{ 
  //����ƫ��
  pd->error_Now = pd->current - pd->expected;
  //�������
  pd->duty = pd->kp*pd->error_Now + pd->kd*(pd->error_Now - pd->error_Last);
  pd->error_Last = pd->error_Now;
}

//����ʽPID������
void Incremental_PID_Caculate(PD_LOOP *pid)
{
  //����ƫ��
  pid->error_Now = pid->expected-pid->current ;
  //�������
  pid->duty += (  pid->kp*(pid->error_Now - pid->error_Last)+
                  pid->ki*pid->error_Now +
                  pid->kd*(pid->error_Now+pid->error_LastLast-2*pid->error_Last)
                );

  pid->error_Last = pid->error_Now;
  pid->error_LastLast = pid->error_Last;          
  //����
  //if(abs(pid->current)<1)pid->duty = 0;
  if(pid->duty > PID_DUTY_MAX)pid->duty = PID_DUTY_MAX;
  else if(pid->duty < -PID_DUTY_MAX)pid->duty = -PID_DUTY_MAX;
}



void ServoPID_Caculate(Servo *servo)
{
  //servo->duty = SERVO_DUTY_MIDDLE + (servo->kp*servo->error_Now + servo->kd*(servo->error_Now - servo->error_Last));
  servo->duty = servo->kp*servo->error_Now + servo->kd*(servo->error_Now - servo->error_Last);
  // if(servo->duty > 3000)servo->duty = SERVO_DUTY_MAX;
  // if(servo->duty < SERVO_DUTY_MINI)servo->duty = SERVO_DUTY_MINI;
  servo->error_Last = servo->error_Now;
}
//������
void Servo_Out(int duty){
  if(duty > SERVO_DUTY_MAX)duty = SERVO_DUTY_MAX;
  if(duty < SERVO_DUTY_MINI)duty = SERVO_DUTY_MINI;
	pwm_duty(PWMB_CH4_P77, duty);
}
/******************************************************************************
**********�����ʼ������
2017.3.13 MonkeyKing
******************************************************************************/
void Servo_Init(Servo *servo)
{
  servo->duty           = SERVO_DUTY_MIDDLE;
  servo->error_Now      = 0;
  servo->error_Last     = 0;
  servo->kp             = 1;
  servo->ki             = 0;
  servo->kd             = 0;
//  FTM_PWM_init(FTM1 , CH0, 100,SERVO_DUTY_MIDDLE);//��ʼ��ʱ���������
}