#ifndef __ENCODER_H
#define __ENCODER_H
#include "common.h"
//定义脉冲引脚
#define ENCODER_PULSE_L   CTIM0_P34
#define ENCODER_PULSE_R   CTIM3_P04
//定义脉冲方向
#define ENCODER_DIR_L     P35
#define ENCODER_DIR_R     P53

extern int32 MotorPulse_L ;
extern int32 MotorPulse_R ;

extern int16 MotorSpeed_L ;
extern int16 MotorSpeed_R ;

void EncoderInit(void);
void EncoderPulseGet(void);




#endif