#include "zf_uart.h"
#include "up_pc.h"
#include "headfile.h"
//void Uart1_Process(){
//	static uint16 rlen;//接收到数据长度
//	static uint8 cmd[4];
//	static float d1;
//	static float d2;
//	static float d3;
//	
//	static int speed;
//  //串口1
//  if(USART_RX_STA&0X8000)//串口1接收到数据
//	{
//		//printf("接收内容：%s\r\n",uart1_rx_buff);
//		memset(uart1_rx_buff,0,sizeof(uart1_rx_buff));
//		USART_RX_STA=0; 
//	}
//}
//void Uart4_Process()
//{  //串口通信
//	static uint16 rlen;//接收到数据长度
//	static uint8 cmd[4];
//	static float d1;
//	static float d2;
//	static float d3;
//	static float d4;
//	static int index;
//	static int speed;
//	
//	if(USART4_RX_STA == RECEIVING_COMPLETED)//串口4接收到数据
//  	{	
//    	rlen=USART4_RX_LEN-2;	//得到本次接收到的数据长度 
//		printf("rec:%s\n",uart4_rx_buff);
//		if(strncmp(uart4_rx_buff,"START",5) == 0){ //发车和强制停车
//			Start_Up();   	//发车
//			printf("armed car\n");
//		}
//		else if(strncmp(uart4_rx_buff,"STOP",4) == 0){ //发车和强制停车
//			Force_Stop() ;     //强制停车
//			printf("disarmed car\n");
//		}
//		else if(strncmp(uart4_rx_buff,"Motor_PID",9) == 0){
//			if(sscanf(uart4_rx_buff,"%9s:%f,%f,%f",&cmd,&d1,&d2,&d3) == 4){
//				car.motor_average.kp = d1;
//				car.motor_average.ki = d2;
//				car.motor_average.kd = d3;
//				printf("Kp:%f,Ki:%f,Kd:%f\n",car.motor_average.kp,car.motor_average.ki,car.motor_average.kd);
//			}
//		}
//		else if(strncmp(uart4_rx_buff,"Motor_L",7) == 0){
//			if(sscanf(uart4_rx_buff,"%7s:%f,%f,%f",&cmd,&d1,&d2,&d3) == 4){
//				car.motor_left.kp = d1;
//				car.motor_left.ki = d2;
//				car.motor_left.kd = d3;
//				printf("Kp:%f,Ki:%f,Kd:%f\n",car.motor_left.kp,car.motor_left.ki,car.motor_left.kd);
//			}
//		}
//		else if(strncmp(uart4_rx_buff,"Motor_R",7) == 0){
//			if(sscanf(uart4_rx_buff,"%7s:%f,%f,%f",&cmd,&d1,&d2,&d3) == 4){
//				car.motor_right.kp = d1;
//				car.motor_right.ki = d2;
//				car.motor_right.kd = d3;
//				printf("Kp:%f,Ki:%f,Kd:%f\n",car.motor_right.kp,car.motor_right.ki,car.motor_right.kd);
//			}
//		}
//		//转向控制参数
//		else if(strncmp(uart4_rx_buff,"Steering",8) == 0)
//		{
//			if(sscanf(uart4_rx_buff,"%8s:%f,%f",&cmd,&d1,&d2) == 3)
//			{
//				car.straight.kp = d1;
//				car.straight.kd = d2;
//				printf("kp:%f,kd:%f\n",car.straight.kp,car.straight.kd);
//			}
//		}

//		else if(strncmp(uart4_rx_buff,"DIFF_PID",8) == 0){
//			if(sscanf(uart4_rx_buff,"%8s:%f,%f",&cmd,&d1,&d2) == 3){
//				car.straight.kp  = d1;
//				car.straight.kd  = d2;
//				printf("Kp:%f,Kd:%f\n",car.straight.kp ,car.straight.kd);
//			}
//		}
//		else if(strncmp(uart4_rx_buff,"PD_LOOP",7) == 0){
//			if(sscanf(uart4_rx_buff,"%7s:%f,%f,%f",&cmd,&d1,&d2,&d3) == 4){
//				car.pd_loop.kp  = d1;
//				car.pd_loop.ki  = d2;
//				car.pd_loop.kd  = d3;		
//				printf("Kp:%f,Ki:%f,Kd:%f\n",car.pd_loop.kp ,car.pd_loop.ki,car.pd_loop.kd);
//			}
//		}
////		else if(strncmp(uart4_rx_buff,"DIFF",4) == 0){
////			if(sscanf(uart4_rx_buff,"%4s:%f,%f",&cmd,&d1,&d2) == 3){
////				car.differential.diff = d1;
////				printf("DIFF:%f\n",car.differential.diff);
////			}
////		}
//		else if(strncmp(uart4_rx_buff,"UP_LOAD",7) == 0){ 
//			if(sscanf(uart4_rx_buff,"%7s:%f",&cmd,&d1) == 2){
//			  if((int)d1)
//                  { 
//					car.upload_enable = 1 ;   	
//					printf("start upload\n");
//				}
//				else{
//					car.upload_enable = 0 ;   	
//					printf("stop upload\n");
//				}
//			}
//		}
//		else if(strncmp(uart4_rx_buff,"EXP_PD_LOOP",11) == 0){ 
//			if(sscanf(uart4_rx_buff,"%11s:%f",&cmd,&d1) == 2){
//			 	car.pd_loop.expected = d1;
//				printf("EXP_PD_LOOP:%f\n",car.pd_loop.expected);
//			}
//		}	
//		else if(strncmp(uart4_rx_buff,"SPD_AVE",7) == 0){
//			if(sscanf(uart4_rx_buff,"%7s:%f",&cmd,&d1) == 2){
//				car.straight.base_speed  = d1;
//				printf("expected_spd:%f\n",car.straight.base_speed );
//			}
//		}
//		memset(uart4_rx_buff,0,sizeof(uart4_rx_buff));
//		USART4_RX_LEN  = 0; 	
//		USART4_RX_STA  = EMPTY;	
//	}
//}

//void Data_upload(int choice){ //数据上传
//	switch(choice){
//		
//	}
//}

//void get_Road_type(){
//	static int type_temp=0;
//	if(type_temp!=car.road_type){
//		type_temp=car.road_type;
//		switch(car.road_type){
//			case OUT_GARAGE:
//				//printf("出库\n");
//				break;	
//			case STRAIGHT : 		
//				//printf("直道\n");
//				break;	
//			case CIRCLE:
//				//printf("环岛\n");
//				break;	
////			case FORK :
////				//printf("cha路\n");
////				break;	
////			case STRAIGHT_FORK:
////				//printf("cha路内\n");
////				break;	
//			case ENTER_GARAGE :
//				//printf("入库");
//				break;	
////			case CROSS :
////				//printf("十字\n");
////				break;	
//			case RAMP:
//				//printf("坡道\n");
//				break;	

//		}
//		
//	}	
//}



////printf("%f,%f,%f\n",car.adc_parameter.adc_error,car.adc_parameter.H_bias,car.adc_parameter.V_bias);	
////printf("%d,%d,%d\n",mpu_acc_x,mpu_acc_y,mpu_acc_z);
////printf("%d,%f\n",mpu_gyro_z,car.adc_parameter.adc_error);
////printf("%f,%f\n",car.distance,car.garage.out_circle_point);

////printf("%f\n",car.distance);
//// printf("%f,%d,%f,%d\n",car.motor_left.speed_Current,car.motor_left.duty ,
//// 						car.motor_right.speed_Current,car.motor_right.duty);
//// if(type_temp!=car.road_type){
//// 	type_temp=car.road_type;
//// 	printf("%d,%f,%d\n",car.road_type,car.speed_set,car.cross.count);
//// }	
////printf("%d\n",car.cross.count);
////printf("%d,%d ,%d, %d, %d, %d\n",mpu_gyro_x,mpu_gyro_y,mpu_gyro_z,mpu_acc_x,mpu_acc_y,mpu_acc_z);
//// if(car.road_type == CIRCLE) {
//// 	printf("%f\n",icmdata.Yaw);
//// }
////printf("%f,%f,%f\n",car.speed_set,car.motor_left.speed_Current,car.motor_right.speed_Current);
////printf("%f,%f,%f\n",car.motor_diff,car.motor_left.speed_Expected,car.motor_right.speed_Expected);
////printf("%f\n",car.servo.history_error_ave);
////printf("%d,%d\n",car.servo.left_count,car.servo.right_count);
////printf("%f,%d,%d\n",car.servo.error_Now,car.servo.error_data[0],(int)car.servo.error_Now-car.servo.error_data[0]);
////printf("%d\n",car.servo.history_error_ave);
////printf("%f,%f\n",car.fork.in_angle,icmdata.Yaw);
////printf("%d,%d,%f\n",icm_gyro_y,icm_acc_z,car.angle_z);
////printf("%d,%d,%d,%d,%d,%d\n",icm_gyro_x,icm_gyro_y,icm_gyro_z,icm_acc_x,icm_acc_y,icm_acc_z);
////printf("%f,%f\n",car.garage.in_point,car.distance);
////printf("%d,%f,%f,%f\n",car.servo.duty,car.servo.error_Now,car.adc_parameter.H_bias,car.adc_parameter.V_bias);
////printf("%f,%f,%f\n",car.adc_parameter.H_bias,car.adc_parameter.V_bias,car.servo.error_Now);
////printf("%f,%f,%f\n",car.adc_parameter.H_bias,car.adc_parameter.V_bias,car.servo.error_Now);
////printf("%f,%f\n",icmdata.Yaw,icmdata.Pitch);
////printf("%f,%f,%f,%f,%f\n",car.motor_left.speed_Expected,car.motor_left.speed_Current,car.motor_right.speed_Expected,car.motor_right.speed_Current , car.motor_diff);
//// printf("%u,%u,%u,%u,%u\n",
//// 	car.adc_parameter.adc_left_h[0],
//// 	car.adc_parameter.adc_left_v[0],
//// 	car.adc_parameter.adc_middle[0],
//// 	car.adc_parameter.adc_right_v[0],
//// 	car.adc_parameter.adc_right_h[0]);
//// printf("%f,%f,%f,%f,%f\n",
//// 	car.adc_parameter.adc_normalized[LEFT_H],
//// 	car.adc_parameter.adc_normalized[LEFT_V],
//// 	car.adc_parameter.adc_normalized[MIDDLE],
//// 	car.adc_parameter.adc_normalized[RIGHT_V],
//// 	car.adc_parameter.adc_normalized[RIGHT_H]);
////printf("%f,%f\n",icmdata.Yaw,car.garage.base_angle);

