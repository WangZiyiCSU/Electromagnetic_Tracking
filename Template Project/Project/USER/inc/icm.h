/*
 * icm.h
 *
 *  Created on: 2021年6月1日
 *      Author: 周文奇
 */

#ifndef CODE_ICM_H_
#define CODE_ICM_H_
//#include "zf_stm_systick.h"
void ICM_OneOrderFilter(void);
void Angle_Calculate(void)   ;


//ICM数据
typedef struct{

    float ACCEL_X;
    float ACCEL_Y;
    float ACCEL_Z;
    float GYRO_X;
    float GYRO_Y;
    float GYRO_Z;
    //角度
    float Pitch;
    float Yaw;
    float Roll;
    //角速度
    float PitchVelocity;      //俯仰角
    float RollVelocity;       //翻滚角
    float YawVelocity;        //偏航角
    float YawVelocity_offset; //偏航角速度偏置
}ICMDatatypedef;

extern ICMDatatypedef icmdata;
// void First_order_complementary_filtering_Pitch();
// void First_order_complementary_filtering_Yaw();
#endif /* CODE_ICM_H_ */
