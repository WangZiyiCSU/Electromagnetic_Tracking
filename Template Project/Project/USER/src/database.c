#include "headfile.h"

#define ALL_DATA 22
//各种数据的基地址(这里define主要是为了说明)
#define ADC_MAX		0	//uint16 * 4	(第4位在前)
#define ADC_MIN		8	//uint16 * 4
#define Steer_Kp	16	//float
#define Steer_Ki	18	//float
#define Steer_Kd	20	//float



void data_save(){
//	uint8 temp[3];
////	temp[0]  = (uint8)10*car.servo.kp ;
////	temp[1]  = (uint8) 10*car.servo.ki ;
////	temp[2]  = (uint8)10*car.servo.kd ;
//	
//	
//	temp[0]  = 2 ;
//	temp[1]  = 3 ;
//	temp[2]  = 4;
////	at24c02_write_bytes(0, temp,3);
//  iap_write_bytes(0,temp,3);
}
void data_read(){
	
//	uint8 temp[3];
//	at24c02_read_bytes(0, temp,3);
////	car.servo.kp =  (float)(temp[0])/10.0f ;
////	car.servo.ki =  (float)(temp[1])/10.0f ;
////	car.servo.kd =  (float)(temp[2])/10.0f ;
//	
//	car.servo.kp =  temp[0] ;
//	car.servo.ki =  temp[1] ;
//	car.servo.kd =  temp[2] ;


}