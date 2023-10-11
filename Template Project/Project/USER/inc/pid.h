#ifndef _PID_H_
#define _PID_H_
#include "common.h"
// #define SERVO_DUTY_MAX          620  //����Ҽ��� ���
// #define SERVO_DUTY_MINI         490  //�������  �Ҵ�
// #define SERVO_DUTY_MIDDLE       555  //��ֵ
// #define SERVO_DUTY_MAX          826  //����Ҽ��� ���
// #define SERVO_DUTY_MINI         664  //�������  �Ҵ�
// #define SERVO_DUTY_MIDDLE       745  //��ֵ
  #define SERVO_DUTY_MAX          910  //����Ҽ��� ���
 #define SERVO_DUTY_MINI         740  //�������  �Ҵ�
 #define SERVO_DUTY_MIDDLE       825  //��ֵ
//#define SERVO_DUTY_MAX          1280  //����Ҽ��� �����
//#define SERVO_DUTY_MIDDLE       1040  //��ֵ
//#define SERVO_DUTY_MINI         800  //�������  �Ҵ���
typedef struct
{
//  int duty;         //����������ռ�ձȵ�ֵ

//  float error_Now;        //��ǰ���
//  float error_Last;       //��һ�����
//  
//  float kp;
//  float ki;
//  float kd;
	  float duty;         //����������ռ�ձȵ�ֵ
//    int duty_Last;
//    int duty_Pre;
//    int duty_Data[5];
//    int duty_Count;
    

    float error_Now;        //��ǰ���
    float error_Last;       //��һ�����
    float error_Delta;
//    float error_data[50];     //��ʷ����¼
//    float history_error_ave;  //��ʷ���ƽ��ֵ

//    int left_count; //�����ƫ�����
//    int right_count;



    
    
//    float error_Sum;        //������
//    float error_Sum_Max;    //��������
//    float error_Sum_Mini;   //��������
    
    float kp;
		float kp_basic;
    float ki;
    float kd;
  
}Servo;



//PDλ�ñջ�
typedef struct
{
    float kp;
    float kd;
    float ki;
    float integral; 
	  float duty;          //����������ռ�ձȵ�ֵ
    float error_Now;     //��ǰ���
    float error_Last;    //��һ�����
    float error_LastLast;    //��һ�����
    float expected;      //����ֵ
    float current;       //��ǰֵ
	  float circle_p;
}PD_LOOP;




extern void Clear_PID_Para(Servo *servo);
//extern void Servo_Init(Servo *servo);       //�����ʼ������
extern void ServoPID_Caculate(Servo *servo);//���pid���㺯��
extern void Set_Pid(Servo *servo,float kp,float kd,float ki);//����pid����
extern void Set_Pid1(Servo *servo,float kp,float kd,float ki);
//extern void Servo_Out(int duty);//������
extern void Position_PD_Caculate(PD_LOOP *pd);//λ��ʽPD������
extern void Position_PID_Caculate(PD_LOOP *pd);//λ��ʽPID������


extern void Set_Pd(PD_LOOP *pd,float kp,float kd);
extern void Incremental_PID_Caculate(PD_LOOP *pid);//����ʽPID������
#endif