#include "math.h"
#include "headfile.h"
void ADC_Process(ADC_Parameter* adc) //��ֵ�˲�
{
	uint8 i;
	//�ɼ�ʮ�ε��
	for(i = 0; i < SAMPLING_TIMES; i++)
	{
		adc->adc_original[LEFT_H][i]  = adc_once(ADC_P00, ADC_12BIT);  //���
		adc->adc_original[LEFT_X][i]  = 0;						            	   //��б
		adc->adc_original[MIDDLE][i]  = adc_once(ADC_P06, ADC_12BIT);  //�м�
		adc->adc_original[RIGHT_X][i] = 0;							               //��б
		adc->adc_original[RIGHT_H][i] = adc_once(ADC_P05, ADC_12BIT);  //�Һ�
	}	
	
	//����ֵ��С�������У�ð�ݷ���
	Bubble_Sort(adc->adc_original[LEFT_H] ,SAMPLING_TIMES);	   //���
	Bubble_Sort(adc->adc_original[MIDDLE] ,SAMPLING_TIMES);    //�м�
	Bubble_Sort(adc->adc_original[RIGHT_H],SAMPLING_TIMES);    //�Һ�

	//ȥ�������С��ֵ���
	for(i = 1; i < SAMPLING_TIMES - 1; i++)
	{
		adc->adc_ave[LEFT_H]  += adc->adc_original[LEFT_H][i];
		adc->adc_ave[LEFT_X]  += adc->adc_original[LEFT_X][i];
		adc->adc_ave[MIDDLE]  = 0;
		adc->adc_ave[RIGHT_X] += adc->adc_original[RIGHT_X][i];
		adc->adc_ave[RIGHT_H] += adc->adc_original[RIGHT_H][i];
	}
	//��ƽ��
	for(i = 0; i<INDUCTOR_NUM; i++)
	{
		adc->adc_ave[i] = adc->adc_ave[i]/(SAMPLING_TIMES-2);
	}
}

//ð������
void Bubble_Sort(uint16 ad[],int len)
{
	int i,j;
	int filter_temp;
	for(j = 0; j < len - 1; j++)
    {
        for(i = 0; i < len - 1 - j; i++)
        {
            if(ad[i] > ad[i + 1])
            {
                filter_temp = ad[i];
                ad[i] = ad[i + 1];
                ad[i + 1] = filter_temp;
            }
        }
    }
}

//��һ������
void ADC_Normalization(ADC_Parameter* adc)
{
	int i;
	float AD_Normalized[INDUCTOR_NUM];
	for(i=0;i<INDUCTOR_NUM;i++)
	{

		if(adc->adc_ave[i]<adc->adc_min[i])AD_Normalized[i]=0.01;
		else
		{
			AD_Normalized[i]=(float)(adc->adc_ave[i]-adc->adc_min[i])/(float)(adc->adc_max[i]-adc->adc_min[i]);
		}
		if(AD_Normalized[i]>1.0)
		{
				AD_Normalized[i]=1.0;
		}
		AD_Normalized[i]=100 * AD_Normalized[i];  //AD[i]��һ�����ֵ��0-100֮��
		adc->adc_normalized[i] = AD_Normalized[i];
	}
	adc->H_strength = adc->adc_normalized[LEFT_H]+adc->adc_normalized[RIGHT_H];
	adc->X_strength = adc->adc_normalized[LEFT_X]+adc->adc_normalized[RIGHT_X];
	adc->adc_strength =adc->H_strength+adc->adc_normalized[MIDDLE];
}

#define ERROR_MAX 100
//��Ⱥ�
void  get_error_normal(ADC_Parameter* adc)
{
	//���м���ƫ��
	adc->adc_error = car.adc_parameter.error_caculate*(adc->adc_normalized[LEFT_H] - adc->adc_normalized[RIGHT_H])/
					     ((adc->adc_normalized[LEFT_H] +adc->adc_normalized[MIDDLE]+adc->adc_normalized[RIGHT_H])*adc->adc_normalized[MIDDLE]);

	// if(adc->adc_normalized[LEFT_H]>5 && adc->adc_normalized[RIGHT_H]<5)adc->adc_error = ERROR_MAX;
	// else if(adc->adc_normalized[LEFT_H]<5 && adc->adc_normalized[RIGHT_H]>5)adc->adc_error = -ERROR_MAX;
	//ƫ���޷�
	if(adc->adc_error>ERROR_MAX)adc->adc_error=ERROR_MAX;
	else if(adc->adc_error<-ERROR_MAX)adc->adc_error=-ERROR_MAX;
}

//ȥ����ֵ
float my_abs(float error){
	if(error<0)error = -error;
	return error;
}
//��Χ�ж�
int Limit_min_max(uint16 num,uint16 min,uint16 max){  //�����޷�
	if(num > max || num < min )return 0;
	return   1;
}



