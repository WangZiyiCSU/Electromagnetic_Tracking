/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "zf_pwm.h"
#include "math.h"
/* Includes ------------------------------------------------------------------*/

//Servo servo;
#define PID_DUTY_MAX 9000
//设置pid参数
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

//积分分离阈值
#define SPE_INTEGRAL_START_ERR 100
#define SPE_INTEGRAL_MAX_VAL 4000
//位置式PID控制器
void Position_PID_Caculate(PD_LOOP *pid)
{ 
  //计算偏差
  pid->error_Now = pid->current - pid->expected;
  /*积分项，积分分离，偏差较大时去掉积分作用*/
  if(pid->error_Now > -SPE_INTEGRAL_START_ERR && pid->error_Now < SPE_INTEGRAL_START_ERR) //阈值范围内 使用积分
  {
      pid->integral += pid->error_Now;  
      /*积分范围限定，防止积分饱和*/
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
  //   pid->integral = 0;//偏差较大 不使用积分
  // }   

  //计算输出
  pid->duty = pid->kp*pid->error_Now +                   //比例
              pid->ki*pid->integral  +                   //积分
              pid->kd*(pid->error_Now - pid->error_Last);  //微分

  pid->error_Last = pid->error_Now;
}

//位置式PD控制器
void Position_PD_Caculate(PD_LOOP *pd)
{ 
  //计算偏差
  pd->error_Now = pd->current - pd->expected;
  //计算输出
  pd->duty = pd->kp*pd->error_Now + pd->kd*(pd->error_Now - pd->error_Last);
  pd->error_Last = pd->error_Now;
}

//增量式PID控制器
void Incremental_PID_Caculate(PD_LOOP *pid)
{
  //计算偏差
  pid->error_Now = pid->expected-pid->current ;
  //计算输出
  pid->duty += (  pid->kp*(pid->error_Now - pid->error_Last)+
                  pid->ki*pid->error_Now +
                  pid->kd*(pid->error_Now+pid->error_LastLast-2*pid->error_Last)
                );

  pid->error_Last = pid->error_Now;
  pid->error_LastLast = pid->error_Last;          
  //死区
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
//舵机输出
void Servo_Out(int duty){
  if(duty > SERVO_DUTY_MAX)duty = SERVO_DUTY_MAX;
  if(duty < SERVO_DUTY_MINI)duty = SERVO_DUTY_MINI;
	pwm_duty(PWMB_CH4_P77, duty);
}
/******************************************************************************
**********舵机初始化函数
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
//  FTM_PWM_init(FTM1 , CH0, 100,SERVO_DUTY_MIDDLE);//初始化时，舵机对中
}