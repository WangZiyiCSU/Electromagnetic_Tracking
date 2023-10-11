#ifndef _CAR_H_
#define _CAR_H_
#include "motor.h"
#include "headfile.h"
#include "pid.h"
//蜂鸣器
#define BEEP_PIN P67
// #define BEEP_OFF  pwm_duty(PWMB_CH4_P77,0);
// #define BEEP_ON   pwm_duty(PWMB_CH4_P77,5000);
#define ELEMENT_NUM_MAX 40
#define BEEP_OFF BEEP_PIN = 0;
#define BEEP_ON  BEEP_PIN = 1;

//#define PIN_1 P36 //红外对管1
//#define PIN_2 P37 //红外对管2
//#define PIN_3 P50 //红外对管3
//#define PIN_4 P51 //红外对管4

/**********方向*********/
typedef enum{
	LEFT   = 0, //左转
	RIGHT  = 1, //右转
}Dir;//方向
/**********方向*********/

typedef enum{
	STOP  = 0,
	START = 1,
}Start_Flag;//发车标志位

typedef enum{
	NORMAL = 0,   //正常寻迹模式
	SCAN   = 1,   //扫描模式
	REMOTE = 2,   //遥控模式
	TEST   = 3,   //测试电感模式
}Work_Mode;       //工作模式

typedef struct{
	int fps;
	int ready;
	int count;
}SCREEN;

/*************圆环结构体************************/
#define MAX_CIRCLE_NUM 10
typedef struct{
	Dir dir;       		    //左环or右环
	int enter_point;   	    //进入环岛时编码器的位置
	int out_point;		    //出环岛的位置
	int distance_passed;    //走过的距离
	int enter_angle;  	    //进入环岛时的角度
	int angle_turned;		//转过的角度
	int state ;          	//当前处于的状态
	int cur_num; 			//当前第几个环岛


	//预留多个环岛 手动赋值
	int   NUM;              //环岛个数
	//每个环岛的单独参数
//条件
	//预入环
	int16 PRE_ENTER_H_STRENGTH[MAX_CIRCLE_NUM]; //预入环点两个横电感和值  //100
	int16 PRE_ENTER_MIDDLE[MAX_CIRCLE_NUM]; 	//预入环点中间横电感值	  //80
	//入环
	int16 Enter_DIST[MAX_CIRCLE_NUM];           //入环编码器条件
	int16 Enter_MID[MAX_CIRCLE_NUM];            //入环中间电感条件
	int16 enter_angle_plan_B[MAX_CIRCLE_NUM];	//入环转角角度
	int16 enter_R_circle[MAX_CIRCLE_NUM];       //入环半径
	int16 enable[MAX_CIRCLE_NUM];			    //元素使能
	float kp[MAX_CIRCLE_NUM];
	float kd[MAX_CIRCLE_NUM];
	int16 speed[MAX_CIRCLE_NUM];
	int16 BIAS_ADJUST[MAX_CIRCLE_NUM];         //出环修正
	int16 Enter_ANGLE[MAX_CIRCLE_NUM];         //入环角度
	int16 Circlejudge[MAX_CIRCLE_NUM];
    int16 angle_1[MAX_CIRCLE_NUM];             //转角1
	int16 angle_2[MAX_CIRCLE_NUM];             //转角2
  //float motor_k;	
}Circle;
/*************圆环结构体************************/

typedef struct{
	int16 state;                           //状态

	float angle;                           //记录初始俯仰角
	float speed;                           //坡道速度
	float speed2;                          //速度
	int   count ;                          //次数
	float in_point;
	float out_point;
	float distance_passed;
	float THRESHOLD;                      //判断阈值
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

/************负压风扇************/
typedef struct{
	int16  left_duty;
	int16 right_duty;
}FAN;
/************负压风扇************/


/************出入库结构体**********/
typedef struct{
	int16 out_garage_enable;//出库使能
	int   count;
	float in_point;
	float base_angle;
	int   enter_flag;
	float out_circle_point;       //记录出环岛的位置用于停车
	float enter_distance;         //停车入库距离
	float back_distance;          //倒车距离补偿
	float back_angle;             //入库角度
	float in_garage_back_distance;//库内倒车距离
	int16 state;                  //入库状态

/*-------------出库方案1参数----------------------*/
	int16 out_garage_speed;      //出库速度
	int16 out_garage_angle;      //出库转角
	int16 out_garage_distance;   //出库行驶距离
	int16 out_garage_diff;       //出库差速
/*-------------出库方案1参数----------------------*/

/*-------------出库方案2参数----------------------*/
	int16 out_garage_R_circle;   //转弯半径
	int16 out_garage_curve_speed;//转弯速度
	int16 out_garage_curve_angle;//转弯角度
/*-------------出库方案2参数----------------------*/

/*------------------入库参数----------------------*/

	int16 enter_garage_distance_1_left;   //第一段倒车距离(左)
	int16 enter_garage_distance_1_right;  //第一段倒车距离(右)

	int16 enter_garage_angle_left;        //倒车转角(左)
	int16 enter_garage_angle_right;       //倒车转角(右)

	int16 enter_garage_speed;             //倒车速度
	int16 enter_garage_diff;              //倒车差速

	int16 enter_garage_distance_2_left;   //车库内倒车距离(左)
	int16 enter_garage_distance_2_right;  //车库内倒车距离(右)

	int16 enter_garage_R_left;            //转弯半径(左)

    int16 last_elemnent_point;            //最后一个元素识别点
	int16 distance_passed;                //最后一个元素到入库的距离
	int16 element_flag;                   //最后一个元素识别标记位置
/*------------------入库参数----------------------*/

	//方向
	int16 DIR;//车库方向
	//入库模式
	int16 MODE ; //0 倒车入库  1 正向入库

	// //正向入库参数	
	 int16 Element_Select;    //选择哪一个元素作为入库依据
	 int16 Distance_to_Garage;//最后一个元素到车库的距离
	 int16 Element_Count;     //第几个元素个数

	//入库减速标志位
	int16 slow_down_flag;

	int16 zebra_distance;  //斑马线距离
	int16 zebra_flag;      //斑马线标记位

	int16 distance_to_zebra;  //最后一个元素到斑马线距离
	int16 forward_state ;     //正入库状态机

	int16 angle_forward;

	int16 turn_point;

	int16 forward_R;
	int16 forward_speed;
	int16 forward_distance;

}GARAGE;
/************出入库结构体**********/


typedef struct{
	float kp;
	float kd;
	float base_speed;
	float speed_up;
	float SPEED_DOWN_DIST;//猛减速距离
	float speed_down1;    //入弯猛减速
	float speed_down2;    //弯道内减速
	float SLOW_K;         //弯道减速系数
}STRAIGHT_Para;


typedef enum{
	OUT_GARAGE    = 0,		 //出库
	STRAIGHT      = 1, 		 //直道
	CIRCLE        = 2,       //环岛
	ENTER_GARAGE  = 3,	     //入库
	Bend          = 4 ,      //弯道
	RAMP          = 5,	   	 //坡道
	OUT_TRACK     = 6,       //出赛道
	HINDER        = 7,       //障碍物
	ENTER_GARAGE_forward = 8,//正向入库
}Road_type;


//不同赛道的速度设定值
typedef struct{
	float straight;
	float small_circle;
	float ramp;
}Speed_Choice;


//转向控制

typedef struct{
	float linear_error_kp;    //一次项比例系数
	float quadratic_error_kp; //二次项比例系数
	float error_kd; 		  //误差微分系数
	float gyro_kp;  	      //陀螺仪系数
	float error_now;		  //当前偏差
	float error_last;		  //上次偏差
	float duty;     	      //输出


}Steering;

typedef struct{
	float angle;           //陀螺仪转角
	int16 distance;        //转角之后前进距离
}Point;

typedef struct{

	Point point_1;          //拐点1
	Point point_2;          //拐点2
	Point point_3;          //拐点3
	Point point_4;          //拐点4

    int  enable_flag;       //使能标记位

	float distance;          //识别距离条件
	int   state;             //状态
	float speed;             //识别到障碍物状态1——速度
	float diff;              //偏差
	float gyro_kp;           //陀螺仪
    float enter_angle;       //障碍物状态1进入角度
    float forward_distance;  //前进距离
	float point;             //转弯标记点
	float judge_distance;    //识别判断距离
	float diff_angle;        //角度偏差值
	float diff_angle_kp;     //角度偏差kp
	float R_circle;          //第一段转弯半径
	float R_circle_2;        //第二段转弯半径
	float dir;               //避障方向
	int   count;

	int16 adc;               //电感条件数值

	int16 initial_angle;      //最初打角

}Hinder;



//小车结构体
typedef struct
{
	//元素表
	ELEMENT element;
	//电感
	ADC_Parameter adc_parameter;	 
	//转向控制
	Steering steering;
	//电机
	Motor motor_average;			            //平均电机
	Motor motor_left;                           //左电机
	Motor motor_right;          				//右电机



	//角速度环结构体
	PD_LOOP pd_loop;                            //角速度环结构体   

	//屏幕
	SCREEN screen;  			            	//屏幕相关参数
	//赛道类型
	Road_type road_type;		            	//赛道类型
	Road_type road_type_last;
	//特殊元素
	STRAIGHT_Para straight;                      //直道
	Ramp ramp;   					             //坡道
	Circle circle;   				             //环岛
	GARAGE garage;					             //车库
	Hinder hinder;                               //障碍物
	FAN  fan;                                    //负压风扇
	//小车状态
	Work_Mode work_mode;		           	     //小车工作模式
	Start_Flag start_flag;     		             //发车标志位
	int upload_enable;				             //无线传输使能
	int uart_busy;                               //串口忙碌
	//电机参数
	float speed_set;                             //设置速度
	float k_diff;                                //差速系数
	float motor_diff;                            //左右电机差速
	float speed_ave; 			                 //两电机速度平均值

	int lose_flag ;                              //丢线

	//里程
	float distance; //编码器里程	
	float MAX_DISTANCE; //最大运行距离

	int16 circle_distance;
	int16 ramp_distance;

	int16 meter_distance;
}CAR; 





//小车变量提供全局调用
extern CAR car;

extern void	 			Beep_Init();			  //蜂鸣器初始化
extern void	            element_parameter_init(); //元素表初始化
extern void 			car_Parameter_Init();     //参数初始化
extern void 			car_System_Init();	      //外设初始化
extern void 			Normal_Tracing();	      //普通寻迹模式
extern void 			Force_Stop() ;     		  //强制停车
extern void             Start_Up()  ;       	  //发车
//extern void       test_hinder();
extern Start_Flag IsStarted();	  				//获取小车当前使能状态


#endif