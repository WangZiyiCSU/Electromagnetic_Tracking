/*
 * icm.h
 *
 *  Created on: 2021��6��1��
 *      Author: ������
 */

#ifndef CODE_ICM_H_
#define CODE_ICM_H_
//#include "zf_stm_systick.h"
void ICM_OneOrderFilter(void);
void Angle_Calculate(void)   ;


//ICM����
typedef struct{

    float ACCEL_X;
    float ACCEL_Y;
    float ACCEL_Z;
    float GYRO_X;
    float GYRO_Y;
    float GYRO_Z;
    //�Ƕ�
    float Pitch;
    float Yaw;
    float Roll;
    //���ٶ�
    float PitchVelocity;      //������
    float RollVelocity;       //������
    float YawVelocity;        //ƫ����
    float YawVelocity_offset; //ƫ�����ٶ�ƫ��
}ICMDatatypedef;

extern ICMDatatypedef icmdata;
// void First_order_complementary_filtering_Pitch();
// void First_order_complementary_filtering_Yaw();
#endif /* CODE_ICM_H_ */
