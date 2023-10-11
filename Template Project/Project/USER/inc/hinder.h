#ifndef _HINDER_H_
#define _HINDER_H_
/* Includes ------------------------------------------------------------------*/
#include "car.h"
/* Includes ------------------------------------------------------------------*/


/*----------------函数声明-------------------------*/
void hinder_parameter_init();
void hinder_judge();
void turn_angle(float expect_angle,float current_angle,float speed,float R_circle);
void hinder_control();
/*----------------函数声明-------------------------*/

#endif
