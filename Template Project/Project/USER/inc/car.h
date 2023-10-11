#ifndef _CAR_H_
#define _CAR_H_
#include "motor.h"
#include "headfile.h"
#include "pid.h"
//������
#define BEEP_PIN P67
// #define BEEP_OFF  pwm_duty(PWMB_CH4_P77,0);
// #define BEEP_ON   pwm_duty(PWMB_CH4_P77,5000);
#define ELEMENT_NUM_MAX 40
#define BEEP_OFF BEEP_PIN = 0;
#define BEEP_ON  BEEP_PIN = 1;

//#define PIN_1 P36 //����Թ�1
//#define PIN_2 P37 //����Թ�2
//#define PIN_3 P50 //����Թ�3
//#define PIN_4 P51 //����Թ�4

/**********����*********/
typedef enum{
	LEFT   = 0, //��ת
	RIGHT  = 1, //��ת
}Dir;//����
/**********����*********/

typedef enum{
	STOP  = 0,
	START = 1,
}Start_Flag;//������־λ

typedef enum{
	NORMAL = 0,   //����Ѱ��ģʽ
	SCAN   = 1,   //ɨ��ģʽ
	REMOTE = 2,   //ң��ģʽ
	TEST   = 3,   //���Ե��ģʽ
}Work_Mode;       //����ģʽ

typedef struct{
	int fps;
	int ready;
	int count;
}SCREEN;

/*************Բ���ṹ��************************/
#define MAX_CIRCLE_NUM 10
typedef struct{
	Dir dir;       		    //��or�һ�
	int enter_point;   	    //���뻷��ʱ��������λ��
	int out_point;		    //��������λ��
	int distance_passed;    //�߹��ľ���
	int enter_angle;  	    //���뻷��ʱ�ĽǶ�
	int angle_turned;		//ת���ĽǶ�
	int state ;          	//��ǰ���ڵ�״̬
	int cur_num; 			//��ǰ�ڼ�������


	//Ԥ��������� �ֶ���ֵ
	int   NUM;              //��������
	//ÿ�������ĵ�������
//����
	//Ԥ�뻷
	int16 PRE_ENTER_H_STRENGTH[MAX_CIRCLE_NUM]; //Ԥ�뻷���������к�ֵ  //100
	int16 PRE_ENTER_MIDDLE[MAX_CIRCLE_NUM]; 	//Ԥ�뻷���м����ֵ	  //80
	//�뻷
	int16 Enter_DIST[MAX_CIRCLE_NUM];           //�뻷����������
	int16 Enter_MID[MAX_CIRCLE_NUM];            //�뻷�м�������
	int16 enter_angle_plan_B[MAX_CIRCLE_NUM];	//�뻷ת�ǽǶ�
	int16 enter_R_circle[MAX_CIRCLE_NUM];       //�뻷�뾶
	int16 enable[MAX_CIRCLE_NUM];			    //Ԫ��ʹ��
	float kp[MAX_CIRCLE_NUM];
	float kd[MAX_CIRCLE_NUM];
	int16 speed[MAX_CIRCLE_NUM];
	int16 BIAS_ADJUST[MAX_CIRCLE_NUM];         //��������
	int16 Enter_ANGLE[MAX_CIRCLE_NUM];         //�뻷�Ƕ�
	int16 Circlejudge[MAX_CIRCLE_NUM];
    int16 angle_1[MAX_CIRCLE_NUM];             //ת��1
	int16 angle_2[MAX_CIRCLE_NUM];             //ת��2
  //float motor_k;	
}Circle;
/*************Բ���ṹ��************************/

typedef struct{
	int16 state;                           //״̬

	float angle;                           //��¼��ʼ������
	float speed;                           //�µ��ٶ�
	float speed2;                          //�ٶ�
	int   count ;                          //����
	float in_point;
	float out_point;
	float distance_passed;
	float THRESHOLD;                      //�ж���ֵ
	int enable ;
  	float kp;
  	float kd;  
	float adc_middle;
	float adc_left;
	float adc_right;
}Ramp;
typedef struct{
	int ELEMENT_NUM;
	int excel[ELEMENT_NUM_MAX];
}ELEMENT;

/************��ѹ����************/
typedef struct{
	int16  left_duty;
	int16 right_duty;
}FAN;
/************��ѹ����************/


/************�����ṹ��**********/
typedef struct{
	int16 out_garage_enable;//����ʹ��
	int   count;
	float in_point;
	float base_angle;
	int   enter_flag;
	float out_circle_point;       //��¼��������λ������ͣ��
	float enter_distance;         //ͣ��������
	float back_distance;          //�������벹��
	float back_angle;             //���Ƕ�
	float in_garage_back_distance;//���ڵ�������
	int16 state;                  //���״̬

/*-------------���ⷽ��1����----------------------*/
	int16 out_garage_speed;      //�����ٶ�
	int16 out_garage_angle;      //����ת��
	int16 out_garage_distance;   //������ʻ����
	int16 out_garage_diff;       //�������
/*-------------���ⷽ��1����----------------------*/

/*-------------���ⷽ��2����----------------------*/
	int16 out_garage_R_circle;   //ת��뾶
	int16 out_garage_curve_speed;//ת���ٶ�
	int16 out_garage_curve_angle;//ת��Ƕ�
/*-------------���ⷽ��2����----------------------*/

/*------------------������----------------------*/

	int16 enter_garage_distance_1_left;   //��һ�ε�������(��)
	int16 enter_garage_distance_1_right;  //��һ�ε�������(��)

	int16 enter_garage_angle_left;        //����ת��(��)
	int16 enter_garage_angle_right;       //����ת��(��)

	int16 enter_garage_speed;             //�����ٶ�
	int16 enter_garage_diff;              //��������

	int16 enter_garage_distance_2_left;   //�����ڵ�������(��)
	int16 enter_garage_distance_2_right;  //�����ڵ�������(��)

	int16 enter_garage_R_left;            //ת��뾶(��)

    int16 last_elemnent_point;            //���һ��Ԫ��ʶ���
	int16 distance_passed;                //���һ��Ԫ�ص����ľ���
	int16 element_flag;                   //���һ��Ԫ��ʶ����λ��
/*------------------������----------------------*/

	//����
	int16 DIR;//���ⷽ��
	//���ģʽ
	int16 MODE ; //0 �������  1 �������

	// //����������	
	 int16 Element_Select;    //ѡ����һ��Ԫ����Ϊ�������
	 int16 Distance_to_Garage;//���һ��Ԫ�ص�����ľ���
	 int16 Element_Count;     //�ڼ���Ԫ�ظ���

	//�����ٱ�־λ
	int16 slow_down_flag;

	int16 zebra_distance;  //�����߾���
	int16 zebra_flag;      //�����߱��λ

	int16 distance_to_zebra;  //���һ��Ԫ�ص������߾���
	int16 forward_state ;     //�����״̬��

	int16 angle_forward;

	int16 turn_point;

	int16 forward_R;
	int16 forward_speed;
	int16 forward_distance;

}GARAGE;
/************�����ṹ��**********/


typedef struct{
	float kp;
	float kd;
	float base_speed;
	float speed_up;
	float SPEED_DOWN_DIST;//�ͼ��پ���
	float speed_down1;    //�����ͼ���
	float speed_down2;    //����ڼ���
	float SLOW_K;         //�������ϵ��
}STRAIGHT_Para;


typedef enum{
	OUT_GARAGE    = 0,		 //����
	STRAIGHT      = 1, 		 //ֱ��
	CIRCLE        = 2,       //����
	ENTER_GARAGE  = 3,	     //���
	Bend          = 4 ,      //���
	RAMP          = 5,	   	 //�µ�
	OUT_TRACK     = 6,       //������
	HINDER        = 7,       //�ϰ���
	ENTER_GARAGE_forward = 8,//�������
}Road_type;


//��ͬ�������ٶ��趨ֵ
typedef struct{
	float straight;
	float small_circle;
	float ramp;
}Speed_Choice;


//ת�����

typedef struct{
	float linear_error_kp;    //һ�������ϵ��
	float quadratic_error_kp; //���������ϵ��
	float error_kd; 		  //���΢��ϵ��
	float gyro_kp;  	      //������ϵ��
	float error_now;		  //��ǰƫ��
	float error_last;		  //�ϴ�ƫ��
	float duty;     	      //���


}Steering;

typedef struct{
	float angle;           //������ת��
	int16 distance;        //ת��֮��ǰ������
}Point;

typedef struct{

	Point point_1;          //�յ�1
	Point point_2;          //�յ�2
	Point point_3;          //�յ�3
	Point point_4;          //�յ�4

    int  enable_flag;       //ʹ�ܱ��λ

	float distance;          //ʶ���������
	int   state;             //״̬
	float speed;             //ʶ���ϰ���״̬1�����ٶ�
	float diff;              //ƫ��
	float gyro_kp;           //������
    float enter_angle;       //�ϰ���״̬1����Ƕ�
    float forward_distance;  //ǰ������
	float point;             //ת���ǵ�
	float judge_distance;    //ʶ���жϾ���
	float diff_angle;        //�Ƕ�ƫ��ֵ
	float diff_angle_kp;     //�Ƕ�ƫ��kp
	float R_circle;          //��һ��ת��뾶
	float R_circle_2;        //�ڶ���ת��뾶
	float dir;               //���Ϸ���
	int   count;

	int16 adc;               //���������ֵ

	int16 initial_angle;      //������

}Hinder;



//С���ṹ��
typedef struct
{
	//Ԫ�ر�
	ELEMENT element;
	//���
	ADC_Parameter adc_parameter;	 
	//ת�����
	Steering steering;
	//���
	Motor motor_average;			            //ƽ�����
	Motor motor_left;                           //����
	Motor motor_right;          				//�ҵ��



	//���ٶȻ��ṹ��
	PD_LOOP pd_loop;                            //���ٶȻ��ṹ��   

	//��Ļ
	SCREEN screen;  			            	//��Ļ��ز���
	//��������
	Road_type road_type;		            	//��������
	Road_type road_type_last;
	//����Ԫ��
	STRAIGHT_Para straight;                      //ֱ��
	Ramp ramp;   					             //�µ�
	Circle circle;   				             //����
	GARAGE garage;					             //����
	Hinder hinder;                               //�ϰ���
	FAN  fan;                                    //��ѹ����
	//С��״̬
	Work_Mode work_mode;		           	     //С������ģʽ
	Start_Flag start_flag;     		             //������־λ
	int upload_enable;				             //���ߴ���ʹ��
	int uart_busy;                               //����æµ
	//�������
	float speed_set;                             //�����ٶ�
	float k_diff;                                //����ϵ��
	float motor_diff;                            //���ҵ������
	float speed_ave; 			                 //������ٶ�ƽ��ֵ

	int lose_flag ;                              //����

	//���
	float distance; //���������	
	float MAX_DISTANCE; //������о���

	int16 circle_distance;
	int16 ramp_distance;

	int16 meter_distance;
}CAR; 





//С�������ṩȫ�ֵ���
extern CAR car;

extern void	 			Beep_Init();			  //��������ʼ��
extern void	            element_parameter_init(); //Ԫ�ر��ʼ��
extern void 			car_Parameter_Init();     //������ʼ��
extern void 			car_System_Init();	      //�����ʼ��
extern void 			Normal_Tracing();	      //��ͨѰ��ģʽ
extern void 			Force_Stop() ;     		  //ǿ��ͣ��
extern void             Start_Up()  ;       	  //����
//extern void       test_hinder();
extern Start_Flag IsStarted();	  				//��ȡС����ǰʹ��״̬


#endif