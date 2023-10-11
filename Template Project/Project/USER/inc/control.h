#ifndef __CONTROL_H
#define __CONTROL_H

#include "headfile.h"
void Out_Track_judge();	
void Speed_Control(float left,float right);//����ջ�����
void Speed_Control_Average(float speed, float diff);//������ٱջ�����
void motor_control(float,float);//ѭ������
void error_calculate(float error_kp,float gyro_kp);//ƫ�����
void hinder_judge();//�ϰ���ʶ��
void hinder_control();//�ϰ���״̬��
void block_judge();
void Stop_after(float distance);
#endif