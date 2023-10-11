#ifndef __MOTOR_H
#define __MOTOR_H
#include "common.h"
#define  MOTOR_DUTY_MAX 9000

#define LEFT_MOTOR_PWM_PIN      PWMA_CH4P_P66       //左电机pwm输出
#define LEFT_MOTOR_DIR_PIN  	P64						    //左电机方向

#define RIGHT_MOTOR_PWM_PIN 	PWMA_CH2P_P62      //右电机pwm输出
#define RIGHT_MOTOR_DIR_PIN     P60						    //右电机方向
/*now*/

//#define  LEFT_MOTOR_PWM_PIN	    PWMA_CH4P_P66        //����pwm
//#define  RIGHT_MOTOR_PWM_PIN		PWMA_CH2P_P62        //�ҵ��pwm
//#define  LEFT_MOTOR_DIR_PIN	    P64 						     //��������
//#define  RIGHT_MOTOR_DIR_PIN    P60 						     //�ҵ������
//// #define LEFT_MOTOR_PWM_PIN      PWMA_CH4P_P66 
// #define LEFT_MOTOR_DIR_PIN  	  P64 						//????????

// #define RIGHT_MOTOR_PWM_PIN 		PWMA_CH2P_P62   //????pwm
// #define RIGHT_MOTOR_DIR_PIN     P60 						//????????


typedef struct
{
  float distance;  //?????��??
  float speed_Expected;   //????????
  float speed_Current;    //??????
  float speed_Last;       //?????????
  int duty;             //????????????????
  int duty_Last;

  int error_Now;        //??????
  int error_Pre;       //????????
  int error_PrePre;        //????????
  
  float integral;
  float kp;
  float ki;
  float kd;

}Motor;

//????
typedef struct
{
	float diff;//????
}Differential;

extern void Motor_Init();
extern void MotorPID_Caculate(Motor *motor);     //PID??????
//extern void Enable_Motor(Motor* motor);     //??????
//extern void Force_Stop(Motor* motor);     //??????,???????????
extern void MotorPWM_Out(int left_duty,int right_duty);//??????????
//extern Enable_flag Motor_IsEnable(Motor *motor);//??????????????
extern void MotorPWM_Out_Single(int duty,int dir);//??????????????
#endif