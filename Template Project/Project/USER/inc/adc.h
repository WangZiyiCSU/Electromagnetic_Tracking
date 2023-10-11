#ifndef _ADC_H_
#define _ADC_H_
#include "common.h"

#define INDUCTOR_NUM 5	//��Ŵ�������Ŀ
#define SAMPLING_TIMES 10 //��������

#define LEFT_H  0  //���
#define LEFT_X  1  //��б
#define MIDDLE  2  //�м�
#define RIGHT_X 3  //��б
#define RIGHT_H 4  //�Һ�


typedef struct{
	//���ԭʼֵ
	uint16 adc_original[INDUCTOR_NUM][SAMPLING_TIMES]; 
	float adc_normalized[INDUCTOR_NUM];//��һ��
	uint16 adc_ave[INDUCTOR_NUM];				//ƽ��ֵ
	uint16 adc_max[INDUCTOR_NUM];				//���ֵ
	uint16 adc_min[INDUCTOR_NUM];				//��Сֵ
	uint32 adc_strength;						//���ǿ��
	uint32 H_strength;							//����ǿ��
	uint32 X_strength;							//б���ǿ��
	float H_bias;  								//����ƫ��
	float X_bias;  								//б���ƫ��
	float adc_error; 						    //��ƫ��
	float error_caculate;                       //偏差计算系数
 
}ADC_Parameter;




void ADC_Normalization(ADC_Parameter* adc);//ADֵ��һ��
void Bubble_Sort(uint16 ad[],int len);//ð������
void ADC_Process(ADC_Parameter* adc);	//��ֵ�˲�

//adƫ�����
void get_error_normal(ADC_Parameter* adc);
void get_ad_error(ADC_Parameter* adc,float L_H,float L_V,float Mid,float R_H,float R_V);
void  get_error_sqrt(ADC_Parameter* adc,float H,float V);
//void  get_error_fork(ADC_Parameter* adc,float H,float V);
//void  get_error_sqrt_s(ADC_Parameter* adc,float H,float V);
void  get_error_sqrt_s(ADC_Parameter* adc,float H,float V,float S);
void  get_error_circle(ADC_Parameter* adc,float H,float V,float S);
void error_circle_lead(ADC_Parameter* adc,float H,float V,float S);
//ȥ����ֵ
float my_abs(float error);

//��Χ�ж�
int Limit_min_max(uint16 num,uint16 min,uint16 max);
#endif
