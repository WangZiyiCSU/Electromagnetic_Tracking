/*
 * icm.c
 *
 *  Created on: 2021��6��1��
 *      Author: ������
 */

#include "icm.h"
#include "math.h"
//#include "C_H.h"
//#include "MOTOR.h"
//#include "System.h"
//#include "buzzer.h"
#include "headfile.h"
#include "SEEKFREE_IMU660RA.h"
//#include "car.h"
ICMDatatypedef icmdata;


/*һ�׻����˲�����*/
#define RAD2DEG 57.295779513
short aacx_0=0,aacy_0=0,aacz_0=0;			//���ٶȼ���ƫ
short gyrox_0=0,gyroy_0=0,gyroz_0=0;	//��������ƫ

float K1=0.15;		//���ż��ٶȼƵĳ̶�	���仯������DMPʱ���ʵ�����
float dt=0.005;		//���� 5ms

float pitch_aac,roll_aac,yaw_aac;			//�ɼ��ٶȼƵõ��ĽǶ�
float pitch_gyro,roll_gyro,yaw_gyro;	//�������ǵõ��ĽǶ�

float pitch_fuse,roll_fuse,yaw_fuse;	//һ�׻����˲��ںϺ�ĽǶ�


void ICM_OneOrderFilter(void)
{
//	mpu6050_get_accdata();
//	mpu6050_get_gyro();
//	icmdata.YawVelocity = 2.21+(float)mpu6050_gyro_z /16.23;
//	icmdata.Yaw = (icmdata.YawVelocity) * dt + icmdata.Yaw;
//	if(abs(icmdata.YawVelocity)<0.5)icmdata.YawVelocity=0;
	
	imu660ra_get_acc(); 		//��ȡ���ٶ�����
	imu660ra_get_gyro();    	//��ȡ����������
	/****************ƫ���Ǽ������**************************/
	icmdata.YawVelocity = (-1)+(float)imu660ra_gyro_z /16.23;
	icmdata.Yaw         = (icmdata.YawVelocity) * dt + icmdata.Yaw;
	if(abs(icmdata.YawVelocity)<0.5)icmdata.YawVelocity=0;
	/****************ƫ���Ǽ������**************************/

	/****************�����Ǽ������**************************/
	icmdata.PitchVelocity = (float)imu660ra_gyro_y/11;
	icmdata.Pitch         = (icmdata.PitchVelocity) *dt + icmdata.Pitch;
	if(abs(icmdata.PitchVelocity)<0.5)icmdata.PitchVelocity = 0;
	/****************�����Ǽ������**************************/
}


// float Pitch; ������
// float Yaw; ƫ����
// float Roll; ��ת��
//Pitch��һ�׻����˲�
	// static float Last_Data = 0;
	// static float angle_x = 0;
	// static float angle_y = 0;
	// static float angle_up = 0;
	// static float angle_up_last = 0;
	// static float K1 = 0.01;
	// static float K2 = 0.04;
//	imu660ra_get_acc(); 		//��ȡ���ٶ�����
//  imu660ra_get_gyro();    	//��ȡ����������
//	get_icm20602_accdata_simiic();
//	get_icm20602_gyro_simiic();

	// //���㸩���� 
	// pitch_aac=-atan((float)mpu_acc_x/(float)mpu_acc_z)*RAD2DEG;
	// //��2000��/s -->  32768/2000=16.384
	// mpu_gyro_y=mpu_gyro_y/16.384;
	// pitch_gyro=mpu_gyro_y*dt;
	// pitch_fuse=K1*pitch_aac+(1-K1)*(Last_Data+pitch_gyro);
	// //pitch_temp +=pitch_fuse;
	// icmdata.Pitch = pitch_fuse;
	// Last_Data=pitch_fuse;



	//����ƫ����
	//icmdata.YawVelocity =3.4+ (float)icm_gyro_z /16.384 ;
	//icmdata.YawVelocity = (float)mpu_gyro_z / 16.23;
	// icmdata.Yaw = (icmdata.YawVelocity) * dt + icmdata.Yaw ;


	// //?????
	// angle_x = atan2((float)mpu_acc_x,(float)mpu_acc_z)*57.295;
	// angle_y = atan2((float)mpu_acc_y,(float)mpu_acc_z)*57.295;

	// angle_up = (1-K1)*(angle_up_last-(icm_gyro_y*0.005)*K2)+K1*angle_x;
	// angle_up_last = angle_up;


	// icmdata.Pitch =  angle_up;
	// icmdata.ACCEL_X = angle_x;
