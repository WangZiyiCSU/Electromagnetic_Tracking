#ifndef _PID_H_
#define _PID_H_
#include "common.h"
// #define SERVO_DUTY_MAX          620  //舵机右极限 左打
// #define SERVO_DUTY_MINI         490  //舵机左极限  右打
// #define SERVO_DUTY_MIDDLE       555  //中值
// #define SERVO_DUTY_MAX          826  //舵机右极限 左打
// #define SERVO_DUTY_MINI         664  //舵机左极限  右打
// #define SERVO_DUTY_MIDDLE       745  //中值
  #define SERVO_DUTY_MAX          910  //舵机右极限 左打
 #define SERVO_DUTY_MINI         740  //舵机左极限  右打
 #define SERVO_DUTY_MIDDLE       825  //中值
//#define SERVO_DUTY_MAX          1280  //舵机右极限 左打满
//#define SERVO_DUTY_MIDDLE       1040  //中值
//#define SERVO_DUTY_MINI         800  //舵机左极限  右打满
typedef struct
{
//  int duty;         //给定舵机输出占空比的值

//  float error_Now;        //当前误差
//  float error_Last;       //上一次误差
//  
//  float kp;
//  float ki;
//  float kd;
	  float duty;         //给定舵机输出占空比的值
//    int duty_Last;
//    int duty_Pre;
//    int duty_Data[5];
//    int duty_Count;
    

    float error_Now;        //当前误差
    float error_Last;       //上一次误差
    float error_Delta;
//    float error_data[50];     //历史误差记录
//    float history_error_ave;  //历史误差平均值

//    int left_count; //向左的偏差个数
//    int right_count;



    
    
//    float error_Sum;        //积分项
//    float error_Sum_Max;    //积分上限
//    float error_Sum_Mini;   //积分下限
    
    float kp;
		float kp_basic;
    float ki;
    float kd;
  
}Servo;



//PD位置闭环
typedef struct
{
    float kp;
    float kd;
    float ki;
    float integral; 
	  float duty;          //给定舵机输出占空比的值
    float error_Now;     //当前误差
    float error_Last;    //上一次误差
    float error_LastLast;    //上一次误差
    float expected;      //期望值
    float current;       //当前值
	  float circle_p;
}PD_LOOP;




extern void Clear_PID_Para(Servo *servo);
//extern void Servo_Init(Servo *servo);       //舵机初始化函数
extern void ServoPID_Caculate(Servo *servo);//舵机pid计算函数
extern void Set_Pid(Servo *servo,float kp,float kd,float ki);//设置pid参数
extern void Set_Pid1(Servo *servo,float kp,float kd,float ki);
//extern void Servo_Out(int duty);//舵机输出
extern void Position_PD_Caculate(PD_LOOP *pd);//位置式PD控制器
extern void Position_PID_Caculate(PD_LOOP *pd);//位置式PID控制器


extern void Set_Pd(PD_LOOP *pd,float kp,float kd);
extern void Incremental_PID_Caculate(PD_LOOP *pid);//增量式PID控制器
#endif