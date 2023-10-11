#include "headfile.h"
int32 MotorPulse_L = 0;
int32 MotorPulse_R = 0;

int16 MotorSpeed_L = 0;
int16 MotorSpeed_R = 0;

void EncoderInit(void)
{
	ctimer_count_init(ENCODER_PULSE_L);	//初始化定时器0作为外部计数
	ctimer_count_init(ENCODER_PULSE_R);	//初始化定时器3作为外部计数
}


void EncoderPulseGet()
{
	  if(ENCODER_DIR_L == 1)
		{
			MotorPulse_L = (int32)ctimer_count_read(ENCODER_PULSE_L)*2;
		}
		else
		{
			MotorPulse_L = (int32)ctimer_count_read(ENCODER_PULSE_L) *-2 ;
		}
		
	  if(ENCODER_DIR_R == 1)
		{
			MotorPulse_R = (int32)ctimer_count_read(ENCODER_PULSE_R)*-2;
		}
		else
		{
			MotorPulse_R = (int32)ctimer_count_read(ENCODER_PULSE_R)*2 ;
		}
	

		ctimer_count_clean(ENCODER_PULSE_L);
		ctimer_count_clean(ENCODER_PULSE_R);

		//单位转换
		car.motor_left.speed_Current = ((float)MotorPulse_L / 116.6)  * 100; //单位cm/s
		car.motor_right.speed_Current = ((float)MotorPulse_R / 116.6) * 100; //单位cm/s
		car.speed_ave = (car.motor_left.speed_Current + car.motor_right.speed_Current)/2;//两轮平均速度
		car.motor_average.speed_Current = car.speed_ave;//两轮平均速度

		car.motor_left.distance += car.motor_left.speed_Current/200;
		car.motor_right.distance += car.motor_right.speed_Current/200;	
		car.distance += car.speed_ave/200;  //计量里程
}