#include "math.h"
#include "headfile.h"
void ADC_Process(ADC_Parameter* adc) //均值滤波
{
	uint8 i;
	//采集十次电感
	for(i = 0; i < SAMPLING_TIMES; i++)
	{
		adc->adc_original[LEFT_H][i]  = adc_once(ADC_P00, ADC_12BIT);  //左横
		adc->adc_original[LEFT_X][i]  = 0;						            	   //左斜
		adc->adc_original[MIDDLE][i]  = adc_once(ADC_P06, ADC_12BIT);  //中间
		adc->adc_original[RIGHT_X][i] = 0;							               //右斜
		adc->adc_original[RIGHT_H][i] = adc_once(ADC_P05, ADC_12BIT);  //右横
	}	
	
	//采样值从小到大排列（冒泡法）
	Bubble_Sort(adc->adc_original[LEFT_H] ,SAMPLING_TIMES);	   //左横
	Bubble_Sort(adc->adc_original[MIDDLE] ,SAMPLING_TIMES);    //中间
	Bubble_Sort(adc->adc_original[RIGHT_H],SAMPLING_TIMES);    //右横

	//去除最大最小极值求和
	for(i = 1; i < SAMPLING_TIMES - 1; i++)
	{
		adc->adc_ave[LEFT_H]  += adc->adc_original[LEFT_H][i];
		adc->adc_ave[LEFT_X]  += adc->adc_original[LEFT_X][i];
		adc->adc_ave[MIDDLE]  = 0;
		adc->adc_ave[RIGHT_X] += adc->adc_original[RIGHT_X][i];
		adc->adc_ave[RIGHT_H] += adc->adc_original[RIGHT_H][i];
	}
	//求平均
	for(i = 0; i<INDUCTOR_NUM; i++)
	{
		adc->adc_ave[i] = adc->adc_ave[i]/(SAMPLING_TIMES-2);
	}
}

//冒泡排序
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

//归一化运算
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
		AD_Normalized[i]=100 * AD_Normalized[i];  //AD[i]归一化后的值，0-100之间
		adc->adc_normalized[i] = AD_Normalized[i];
	}
	adc->H_strength = adc->adc_normalized[LEFT_H]+adc->adc_normalized[RIGHT_H];
	adc->X_strength = adc->adc_normalized[LEFT_X]+adc->adc_normalized[RIGHT_X];
	adc->adc_strength =adc->H_strength+adc->adc_normalized[MIDDLE];
}

#define ERROR_MAX 100
//差比和
void  get_error_normal(ADC_Parameter* adc)
{
	//横电感计算偏差
	adc->adc_error = car.adc_parameter.error_caculate*(adc->adc_normalized[LEFT_H] - adc->adc_normalized[RIGHT_H])/
					     ((adc->adc_normalized[LEFT_H] +adc->adc_normalized[MIDDLE]+adc->adc_normalized[RIGHT_H])*adc->adc_normalized[MIDDLE]);

	// if(adc->adc_normalized[LEFT_H]>5 && adc->adc_normalized[RIGHT_H]<5)adc->adc_error = ERROR_MAX;
	// else if(adc->adc_normalized[LEFT_H]<5 && adc->adc_normalized[RIGHT_H]>5)adc->adc_error = -ERROR_MAX;
	//偏差限幅
	if(adc->adc_error>ERROR_MAX)adc->adc_error=ERROR_MAX;
	else if(adc->adc_error<-ERROR_MAX)adc->adc_error=-ERROR_MAX;
}

//去绝对值
float my_abs(float error){
	if(error<0)error = -error;
	return error;
}
//范围判定
int Limit_min_max(uint16 num,uint16 min,uint16 max){  //上下限幅
	if(num > max || num < min )return 0;
	return   1;
}



