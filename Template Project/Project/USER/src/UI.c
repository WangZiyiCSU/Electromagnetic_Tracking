#include "UI.h"
#include "car.h"

int edit_mode = 0;
float step_size = 2;//����
struct MenuItem* MenuPoint = NULL;//��ǰ�˵��ĵ�ַ
short selectItem;//��ǰ�ڵڼ���

float Get_Float(uint8 addr);
int16 Get_Int16(uint8 addr);
uint16 Get_uInt16(uint8 addr);

/**************************�˵����**************************/
#define MAINMENU_LEN 9
struct MenuItem MainMenu[MAINMENU_LEN] = {
{ MAINMENU_LEN,"START",     NULL         , NULL_TYPE, Start_Up      ,NULL,NULL },
{ MAINMENU_LEN,"AD",        NULL         , NULL_TYPE, NULL          ,NULL,NULL },
{ MAINMENU_LEN,"Element",   NULL         , NULL_TYPE, NULL          ,NULL,NULL },
{ MAINMENU_LEN,"TEST",      NULL         , NULL_TYPE, NULL          ,NULL,NULL },
{ MAINMENU_LEN,"MPU6050",   NULL         , NULL_TYPE, NULL          ,NULL,NULL },
{ MAINMENU_LEN,"data",      NULL         , NULL_TYPE, Store_Para    ,NULL,NULL },
{ MAINMENU_LEN,"data280",   NULL         , NULL_TYPE, Store_280     ,NULL,NULL },
{ MAINMENU_LEN,"mode"   ,   &car.road_type , MY_INT16  , NULL       ,NULL,NULL },
{ MAINMENU_LEN,"dis"    ,   &car.meter_distance , MY_INT16  , NULL  ,NULL,NULL },
};

/**************������*************************/

//��й�һ��ֵ
#define AD_MENU_LEN  3
struct MenuItem Ad_Menu[AD_MENU_LEN]={
{ AD_MENU_LEN,(u8 *)"Normalized",NULL ,NULL_TYPE,NULL,NULL,MainMenu },
{ AD_MENU_LEN,(u8 *)"Max"       ,NULL ,NULL_TYPE,NULL,NULL,MainMenu },
{ AD_MENU_LEN,(u8 *)"Original"  ,NULL ,NULL_TYPE,NULL,NULL,MainMenu },
};

//��й�һ��ֵ
#define AD_MENU1_LEN  3
struct MenuItem Ad_Menu1[AD_MENU1_LEN]={
	{ AD_MENU1_LEN,(u8 *)"Left_H", &car.adc_parameter.adc_normalized[LEFT_H] ,MY_FLOAT,NULL,NULL,Ad_Menu },
	{ AD_MENU1_LEN,(u8 *)"Middle", &car.adc_parameter.adc_normalized[MIDDLE] ,MY_FLOAT,NULL,NULL,Ad_Menu },
	{ AD_MENU1_LEN,(u8 *)"Right_H",&car.adc_parameter.adc_normalized[RIGHT_H],MY_FLOAT,NULL,NULL,Ad_Menu },
};



#define AD_MAX_LEN  5
struct MenuItem AD_MAX[AD_MAX_LEN]={
{ AD_MAX_LEN,(u8 *)"Left_H" ,&car.adc_parameter.adc_max[LEFT_H] , MY_UINT16,NULL,NULL,Ad_Menu },
{ AD_MAX_LEN,(u8 *)"Left_X" ,&car.adc_parameter.adc_max[LEFT_X] , MY_UINT16,NULL,NULL,Ad_Menu },
{ AD_MAX_LEN,(u8 *)"Middle" ,&car.adc_parameter.adc_max[MIDDLE] , MY_UINT16,NULL,NULL,Ad_Menu },
{ AD_MAX_LEN,(u8 *)"Right_X",&car.adc_parameter.adc_max[RIGHT_X], MY_UINT16,NULL,NULL,Ad_Menu },
{ AD_MAX_LEN,(u8 *)"Right_H",&car.adc_parameter.adc_max[RIGHT_H], MY_UINT16,NULL,NULL,Ad_Menu },
};

//���ԭʼֵ
#define AD_MENU2_LEN  3
struct MenuItem Ad_Menu2[AD_MENU2_LEN]={
{ AD_MENU2_LEN,(u8 *)"Left_H", &car.adc_parameter.adc_ave[LEFT_H]  ,MY_UINT16,NULL,NULL,Ad_Menu },
{ AD_MENU2_LEN,(u8 *)"Middle", &car.adc_parameter.adc_ave[MIDDLE]  ,MY_UINT16,NULL,NULL,Ad_Menu },
{ AD_MENU2_LEN,(u8 *)"Right_H", &car.adc_parameter.adc_ave[RIGHT_H],MY_UINT16,NULL,NULL,Ad_Menu },
};


/**************************Ԫ�����*************************/
#define PID_LEN 8
struct MenuItem PID[PID_LEN] = {
{ PID_LEN,(u8 *)"Straight"      ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"Circle"        ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"Ramp"          ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"Hinder"        ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"out_garage"    ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"enter_garage"  ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"fan"           ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
{ PID_LEN,(u8 *)"excel"         ,NULL,NULL_TYPE,NULL,NULL,MainMenu },
};
/*********************Ԫ�ر�***********************/
#define EXCEL_LEN 8
struct MenuItem ELEMENT_EXCEL[EXCEL_LEN] = {
{EXCEL_LEN,(u8 *)"order1:" ,&car.element.excel[0] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order2:" ,&car.element.excel[1] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order3:" ,&car.element.excel[2] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order4:" ,&car.element.excel[3] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order5:" ,&car.element.excel[4] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order6:" ,&car.element.excel[5] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order7:" ,&car.element.excel[6] ,MY_INT16,NULL,NULL,PID},
{EXCEL_LEN,(u8 *)"order8:" ,&car.element.excel[7] ,MY_INT16,NULL,NULL,PID},
};
/*********************Ԫ�ر�***********************/

/**********************ֱ��************************/
#define STRAIGHT_LEN 3
struct MenuItem Straight_Pid[STRAIGHT_LEN]={
{ STRAIGHT_LEN,(u8 *)"Kp"   ,&car.straight.kp        ,MY_FLOAT,NULL,NULL,PID },
{ STRAIGHT_LEN,(u8 *)"kd"   ,&car.straight.kd        ,MY_FLOAT,NULL,NULL,PID },
{ STRAIGHT_LEN,(u8 *)"Speed",&car.straight.base_speed,MY_FLOAT,NULL,NULL,PID },
};
/**********************ֱ��************************/

/**********************����************************/
#define CIRCLE_Para_LEN 3
struct MenuItem Circle_Para[CIRCLE_Para_LEN]={
{ CIRCLE_Para_LEN,(u8 *)"Circle1",  NULL, NULL_TYPE,NULL,NULL,PID },
{ CIRCLE_Para_LEN,(u8 *)"Circle2",  NULL, NULL_TYPE,NULL,NULL,PID },
{ CIRCLE_Para_LEN,(u8 *)"Circle3",  NULL, NULL_TYPE,NULL,NULL,PID },
};

/***********����1(С��R50)����**********/
#define CIRCLE_LEN_1 12
struct MenuItem Circle_Para_1[CIRCLE_LEN_1]={
{ CIRCLE_LEN_1,(u8 *)"Spd",   & car.circle.speed[0]               ,MY_UINT16,NULL,NULL,Circle_Para},//�����ٶ�
{ CIRCLE_LEN_1,(u8 *)"L_R",   & car.circle.PRE_ENTER_H_STRENGTH[0],MY_UINT16,NULL,NULL,Circle_Para},//�����
{ CIRCLE_LEN_1,(u8 *)"Mid",   & car.circle.PRE_ENTER_MIDDLE[0]    ,MY_UINT16,NULL,NULL,Circle_Para},//�м���
{ CIRCLE_LEN_1,(u8 *)"kp",    & car.circle.kp[0]                  ,MY_FLOAT ,NULL,NULL,Circle_Para},//����kp
{ CIRCLE_LEN_1,(u8 *)"kd",    & car.circle.kd[0]                  ,MY_FLOAT ,NULL,NULL,Circle_Para},//����kd
{ CIRCLE_LEN_1,(u8 *)"dis",   & car.circle.Enter_DIST[0]          ,MY_UINT16,NULL,NULL,Circle_Para},//Ԥ�뻷���뻷��ľ���
{ CIRCLE_LEN_1,(u8 *)"ENMID", & car.circle.Enter_MID[0]           ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷����ֵ
{ CIRCLE_LEN_1,(u8 *)"angle", & car.circle.Enter_ANGLE[0]         ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷�㵽����ѭ��ת��
{ CIRCLE_LEN_1,(u8 *)"ang_1", & car.circle.angle_1[0]             ,MY_UINT16,NULL,NULL,Circle_Para},//����ѭ��ת��
{ CIRCLE_LEN_1,(u8 *)"ang_2", & car.circle.angle_2[0]             ,MY_UINT16,NULL,NULL,Circle_Para},//��ֵ���
{ CIRCLE_LEN_1,(u8 *)"R"    , & car.circle.enter_R_circle[0]      ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷�뾶
{ CIRCLE_LEN_1,(u8 *)"pz   ", & car.circle.BIAS_ADJUST[0]         ,MY_INT16 ,NULL,NULL,Circle_Para},//����ƫ��
};
/***********����1(С��R50)����**********/

/***********����2(��R60)����**********/
#define CIRCLE_LEN_2 12
struct MenuItem Circle_Para_2[CIRCLE_LEN_2]={
{ CIRCLE_LEN_2,(u8 *)"Spd",   & car.circle.speed[1]               ,MY_UINT16,NULL,NULL,Circle_Para},//�����ٶ�
{ CIRCLE_LEN_2,(u8 *)"L_R",   & car.circle.PRE_ENTER_H_STRENGTH[1],MY_UINT16,NULL,NULL,Circle_Para},//�����
{ CIRCLE_LEN_2,(u8 *)"Mid",   & car.circle.PRE_ENTER_MIDDLE[1]    ,MY_UINT16,NULL,NULL,Circle_Para},//�м���
{ CIRCLE_LEN_2,(u8 *)"kp",    & car.circle.kp[1]                  ,MY_FLOAT ,NULL,NULL,Circle_Para},//����kp
{ CIRCLE_LEN_2,(u8 *)"kd",    & car.circle.kd[1]                  ,MY_FLOAT ,NULL,NULL,Circle_Para},//����kd
{ CIRCLE_LEN_2,(u8 *)"dis",   & car.circle.Enter_DIST[1]          ,MY_UINT16,NULL,NULL,Circle_Para},//Ԥ�뻷���뻷��ľ���
{ CIRCLE_LEN_2,(u8 *)"ENMID", & car.circle.Enter_MID[1]           ,MY_UINT16 ,NULL,NULL,Circle_Para},//�뻷����ֵ
{ CIRCLE_LEN_2,(u8 *)"angle", & car.circle.Enter_ANGLE[1]         ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷�㵽����ѭ��ת��
{ CIRCLE_LEN_2,(u8 *)"ang_1", & car.circle.angle_1[1]             ,MY_UINT16,NULL,NULL,Circle_Para},//����ѭ��ת��
{ CIRCLE_LEN_2,(u8 *)"ang_2", & car.circle.angle_2[1]             ,MY_UINT16,NULL,NULL,Circle_Para},//��ֵ���
{ CIRCLE_LEN_2,(u8 *)"R"    , & car.circle.enter_R_circle[1]      ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷�뾶
{ CIRCLE_LEN_2,(u8 *)"pz   ", & car.circle.BIAS_ADJUST[1]         ,MY_INT16 ,NULL,NULL,Circle_Para},//����ƫ��
};
/***********����2(��R60)����**********/

/***********����3(��R90)����**********/
#define CIRCLE_LEN_3 3
struct MenuItem Circle_Para_3[CIRCLE_LEN_3]={
{ CIRCLE_LEN_3,(u8 *)"dis"  , & car.circle.Enter_DIST[2]          ,MY_UINT16,NULL,NULL,Circle_Para},//Ԥ�뻷���뻷��ľ���
{ CIRCLE_LEN_3,(u8 *)"ENMID", & car.circle.Enter_MID[2]           ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷����ֵ
{ CIRCLE_LEN_3,(u8 *)"R"    , & car.circle.enter_R_circle[2]      ,MY_UINT16,NULL,NULL,Circle_Para},//�뻷�뾶
};
/***********����3(��R90)����**********/

/**********************����************************/


/**********************�µ�************************/
#define RAMP_LEN 5
struct MenuItem Ramp_element[RAMP_LEN]={
{ RAMP_LEN,(u8 *)"spd1" ,&car.ramp.speed     ,MY_FLOAT,NULL,NULL,PID },//�����ٶ�
{ RAMP_LEN,(u8 *)"spd2" ,&car.ramp.speed2    ,MY_FLOAT,NULL,NULL,PID },//�����ٶ�
{ RAMP_LEN,(u8 *)"mid"  ,&car.ramp.adc_middle,MY_FLOAT,NULL,NULL,PID },//�м���ʶ��
{ RAMP_LEN,(u8 *)"left" ,&car.ramp.adc_left  ,MY_FLOAT,NULL,NULL,PID },//����ʶ��
{ RAMP_LEN,(u8 *)"right",&car.ramp.adc_right ,MY_FLOAT,NULL,NULL,PID },//�ҵ��ʶ��
};
/**********************�µ�************************/


/**********************�ϰ�************************/
#define HINDER_LEN 12
struct MenuItem HINDER_element[HINDER_LEN]={
{ HINDER_LEN,(u8 *)"spd"   ,&car.hinder.speed            ,MY_FLOAT,NULL,NULL,PID },//�ϰ��ٶ�
{ HINDER_LEN,(u8 *)"dif"   ,&car.hinder.diff             ,MY_FLOAT,NULL,NULL,PID },//����
{ HINDER_LEN,(u8 *)"dis"   ,&car.hinder.judge_distance   ,MY_FLOAT,NULL,NULL,PID },//ʶ�����
{ HINDER_LEN,(u8 *)"1_ang" ,&car.hinder.point_1.angle    ,MY_FLOAT,NULL,NULL,PID },//�յ�1�Ƕ�
{ HINDER_LEN,(u8 *)"1_dis" ,&car.hinder.point_1.distance ,MY_FLOAT,NULL,NULL,PID },//�յ�1��ʻ����
{ HINDER_LEN,(u8 *)"1_R"   ,&car.hinder.R_circle         ,MY_FLOAT,NULL,NULL,PID },//��һ��ת��뾶
{ HINDER_LEN,(u8 *)"2_ang" ,&car.hinder.point_2.angle    ,MY_FLOAT,NULL,NULL,PID },//�յ�2�Ƕ�
{ HINDER_LEN,(u8 *)"2_dis" ,&car.hinder.point_2.distance ,MY_UINT16,NULL,NULL,PID},//�յ�2��ʻ����
{ HINDER_LEN,(u8 *)"2_R"   ,&car.hinder.R_circle_2       ,MY_FLOAT,NULL,NULL,PID },//�ڶ���ת��뾶
{ HINDER_LEN,(u8 *)"dir"   ,&car.hinder.dir              ,MY_UINT16,NULL,NULL,PID },//���Ϸ���
{ HINDER_LEN,(u8 *)"circle",&car.circle_distance         ,MY_UINT16,NULL,NULL,PID },//���Ϸ���
{ HINDER_LEN,(u8 *)"ramp"  ,&car.ramp_distance           ,MY_UINT16,NULL,NULL,PID },//���Ϸ���
};
/**********************�ϰ�************************/

/**********************����************************/
#define Out_garage_Mode_Len 5
struct MenuItem Out_garage_Mode[Out_garage_Mode_Len] = {
{ Out_garage_Mode_Len,(u8 *) "spd" , &car.garage.out_garage_speed    ,MY_UINT16,NULL,NULL ,PID },//�����ٶ�
{ Out_garage_Mode_Len,(u8 *) "dis" , &car.garage.out_garage_distance ,MY_UINT16,NULL,NULL ,PID },//������ʻ����
{ Out_garage_Mode_Len,(u8 *) "ang" , &car.garage.out_garage_angle    ,MY_UINT16,NULL,NULL ,PID },//����ת��
{ Out_garage_Mode_Len,(u8 *) "diff", &car.garage.out_garage_diff     ,MY_UINT16,NULL,NULL ,PID },//������� 
{ Out_garage_Mode_Len,(u8 *) "dir",  &car.garage.DIR                 ,MY_UINT16,NULL,NULL ,PID },//���ⷽ��
};
/**********************����************************/

/**********************���(�˵�)******************/
#define Enter_garage_Mode_Len 8
struct MenuItem Enter_garage_Mode[Enter_garage_Mode_Len] = {
{ Enter_garage_Mode_Len,(u8 *) "left"   , NULL                          ,NULL_TYPE,NULL,NULL,PID },//�����
{ Enter_garage_Mode_Len,(u8 *) "right"  , NULL                          ,NULL_TYPE,NULL,NULL,PID },//�����
{ Enter_garage_Mode_Len,(u8 *) "dis"    , &car.garage.distance_passed   ,MY_INT16 ,NULL,NULL,PID },//���һ��Ԫ�ص���ľ���
{ Enter_garage_Mode_Len,(u8 *) "zebra"  , &car.garage.distance_to_zebra ,MY_INT16 ,NULL,NULL,PID },//���һ��Ԫ�ص���ʶ�����
{ Enter_garage_Mode_Len,(u8 *) "ang"    , &car.garage.turn_point        ,MY_INT16 ,NULL,NULL,PID },//����ת��
{ Enter_garage_Mode_Len,(u8 *) "diff"   , &car.garage.forward_R         ,MY_INT16 ,NULL,NULL,PID },//����ת��
{ Enter_garage_Mode_Len,(u8 *) "speed"  , &car.garage.forward_speed     ,MY_INT16 ,NULL,NULL,PID },//����ת��
{ Enter_garage_Mode_Len,(u8 *) "dis_2"  , &car.garage.forward_distance  ,MY_INT16 ,NULL,NULL,PID },//����ת��
};
/**********************���(�˵�)*****************/

/**********************���(��)*******************/
#define Enter_garage_Left_Len 4
struct  MenuItem Enter_garage_Left[Enter_garage_Left_Len]={
{ Enter_garage_Left_Len,(u8 *)"dis_1",&car.garage.enter_garage_distance_1_left,MY_INT16,NULL,NULL,Enter_garage_Mode},
{ Enter_garage_Left_Len,(u8 *)"dis_2",&car.garage.enter_garage_distance_2_left,MY_INT16,NULL,NULL,Enter_garage_Mode},
{ Enter_garage_Left_Len,(u8 *)"angle",&car.garage.enter_garage_angle_left     ,MY_INT16,NULL,NULL,Enter_garage_Mode},
{ Enter_garage_Left_Len,(u8 *)"diff" ,&car.garage.enter_garage_diff           ,MY_INT16,NULL,NULL,Enter_garage_Mode},
};
/**********************���(��)*******************/

/**********************���(��)*******************/
#define Enter_garage_Right_Len 4
struct MenuItem Enter_garage_Right[Enter_garage_Right_Len] = {
{ Enter_garage_Left_Len,(u8 *)"dis_1",&car.garage.enter_garage_distance_1_right,MY_INT16,NULL,NULL,Enter_garage_Mode},
{ Enter_garage_Left_Len,(u8 *)"dis_2",&car.garage.enter_garage_distance_2_right,MY_INT16,NULL,NULL,Enter_garage_Mode},
{ Enter_garage_Left_Len,(u8 *)"angle",&car.garage.enter_garage_angle_right     ,MY_INT16,NULL,NULL,Enter_garage_Mode},
{ Enter_garage_Left_Len,(u8 *)"diff ",&car.garage.enter_garage_diff            ,MY_INT16,NULL,NULL,Enter_garage_Mode},
};
/**********************���(��)*******************/

/************************����********************/
#define fan_LEN 2
struct MenuItem fan_Mode[fan_LEN] = {
{ fan_LEN,(u8*)"left" , &car.fan.left_duty  ,MY_UINT16,NULL,NULL,PID},//�����ת��
{ fan_LEN,(u8*)"right", &car.fan.right_duty ,MY_UINT16,NULL,NULL,PID},//�����ת��
};
/************************����********************/


/**************************���������*************************/
//������
struct MenuItem Mpu6050[2] = {
{2,(u8 *)"Pitch",&icmdata.Pitch,     MY_FLOAT,NULL,NULL,MainMenu },
{2,(u8 *)"Yaw",  &icmdata.Yaw,       MY_FLOAT,NULL,NULL,MainMenu },
};
/****************************************************
 * @name	    MenuInit  
 * @function    ���˵�����
 * @parameter   NULL
 * @return	    NULL
 * @date		2023/7/9
 ***************************************************/
void MenuInit(){
	MenuPoint   =  MainMenu;
	selectItem =1;

/*****************���˵�������ת�ڶ����˵�***************/
	MainMenu[1].ChildrenMenus = Ad_Menu;
	MainMenu[2].ChildrenMenus = PID;
	//MainMenu[3].ChildrenMenus = Servo_Test;
	MainMenu[4].ChildrenMenus = Mpu6050;
/*****************���˵�������ת�ڶ����˵�***************/


/*****************�ڶ���������ת�������˵�***************/
	Ad_Menu[0].ChildrenMenus = Ad_Menu1;
	Ad_Menu[1].ChildrenMenus = AD_MAX;
	Ad_Menu[2].ChildrenMenus = Ad_Menu2;
/*****************�ڶ���������ת�������˵�***************/


/*****************������������ת���ļ��˵�***************/
	PID[0].ChildrenMenus = Straight_Pid;      //ֱ��
	PID[1].ChildrenMenus = Circle_Para;       //Բ��
	PID[2].ChildrenMenus = Ramp_element;      //�µ�
	PID[3].ChildrenMenus = HINDER_element;    //�ϰ�
	PID[4].ChildrenMenus = Out_garage_Mode;   //����
	PID[5].ChildrenMenus = Enter_garage_Mode; //���
	PID[6].ChildrenMenus = fan_Mode;          //����
	PID[7].ChildrenMenus = ELEMENT_EXCEL;     //Ԫ�ر�
/*****************������������ת���ļ��˵�***************/



/**********************���һ���˵�*********************/
   Enter_garage_Mode[0].ChildrenMenus = Enter_garage_Left;  //�����
   Enter_garage_Mode[1].ChildrenMenus = Enter_garage_Right; //�����
/**********************���һ���˵�*********************/

/**********************����һ���˵�*********************/
	Circle_Para[0].ChildrenMenus =Circle_Para_1;
	Circle_Para[1].ChildrenMenus =Circle_Para_2;
	Circle_Para[2].ChildrenMenus =Circle_Para_3;
/**********************����һ���˵�*********************/


	
	display_str(MenuPoint,selectItem);
	
}



void Store_Para(){

	//�洢�ռ�ȫ��д0
	//  uint8 i ;
	//  for(i=0; i<511;i++)
	//  {
	// 	extern_iap_write_bytes(i, (uint8)0);
	//  }
	iap_erase_page(0);
/**********************ֱ��*************************/
	//car.straight.kp 
	store_float(0, 20); 

	//car.straight.kd
	store_float(4, 0.7); 

	//car.straight.base_speed
	store_float(8, 250); 
/**********************ֱ��*************************/

/**********************Բ��(R50)********************/

   //car.circle.speed[0]
   store_int16(16,250);

   //car.circle.PRE_ENTER_H_STRENGTH[0]
   store_int16(20,140);
   
   //car.circle.PRE_ENTER_MIDDLE[0]
   store_int16(24,70);

   //car.circle.Enter_DIST[0]
   store_int16(28,35);
   
   //car.circle.Enter_ANGLE[0]
   store_int16(32,30);

   //car.circle.angle_1[0]
   store_int16(36,260);

   //car.circle.angle_2[0] 
   store_int16(40,300);

   //car.circle.BIAS_ADJUST[0]
   store_int16(44,-10);

   //car.circle.Enter_MID[0]
   store_int16(216,90);

   //car.circle.enter_R_circle[0]
   store_int16(244,35);
/**********************Բ��(R50)***********************/

/**********************Բ��(R60)***********************/

   // car.circle.speed[1]
   store_int16(52,250);

   //car.circle.PRE_ENTER_H_STRENGTH[1]
   store_int16(56,140);
   
   //car.circle.PRE_ENTER_MIDDLE[1]
   store_int16(60,80);

   //car.circle.Enter_DIST[1]
   store_int16(64,44);
   
   //car.circle.Enter_ANGLE[1]
   store_int16(68,30);

   //car.circle.angle_1[1]
   store_int16(72,260);

   //car.circle.angle_2[1] 
   store_int16(76,300);

   //car.circle.BIAS_ADJUST[1]
   store_int16(80,-10);

	//car.circle.Enter_MID[1]
   store_int16(220,90);

    //car.circle.enter_R_circle[1]
    store_int16(246,45);
/**********************Բ��(R60)***********************/

/**********************Բ��(R90)***********************/

    //car.circle.Enter_DIST[2]
    store_int16(10,50);
   
    //car.circle.Enter_MID[2] 
    store_int16(14,70);

	//car.circle.enter_R_circle[2]
    store_int16(48,50);
/**********************Բ��(R90)***********************/

/************************�µ�**********************/

   //car.ramp.speed
   store_float(84,100);

   //car.ramp.speed2
   store_float(88,200);

   //car.ramp.adc_middle
   store_float(92,75);

   //car.ramp.adc_left
   store_float(96,50);

   //car.ramp.adc_right
   store_float(100,50);

/************************�µ�**********************/

/************************�ϰ�**********************/

   //car.hinder.speed 
   store_float(104,250);

   //car.hinder.diff
   store_float(108,100);

   //car.hinder.judge_distance
   store_float(112,935);

   //car.hinder.point_1.angle
   store_float(116,50);

   //car.hinder.point_1.distance
   store_float(120,10);

   //car.hinder.point_2.angle
   store_float(124,100);

   //car.hinder.point_2.distance
   store_float(128,40);

  

   //car.hinder.R_circle;
   store_float(136,35);

   //car.hinder.R_circle_2
	store_float(140,35); 

   //car.hinder.dir
   store_int16(204,0);

/************************�ϰ�**********************/


/**********************����************************/

   //car.garage.out_garage_speed
   store_int16(144,200);

   //car.garage.out_garage_distance
   store_int16(148,15);
   
   //car.garage.out_garage_angle
   store_int16(152,50);

   //car.garage.out_garage_diff
   store_int16(156,150);

/**********************����************************/



/**********************���***********************/

    /*********��������********/
	//car.garage.enter_garage_distance_1_left
	store_int16(158,2);
	
	//car.garage.enter_garage_angle_left
    store_int16(160,10);

	//car.garage.enter_garage_distance_2_left
	store_int16(162,35);

	//car.garage.enter_garage_R_left
	store_int16(164,30);
	/*********��������********/

	/*********��������********/
	//car.garage.enter_garage_distance_1_right
	store_int16(166,2);
	
	//car.garage.enter_garage_angle_right
    store_int16(168,8);

	//car.garage.enter_garage_distance_2_right
	store_int16(170,50);

	//car.garage.enter_garage_diff 
	store_int16(172,100);
	/*********��������********/

	//car.garage.distance_to_zebra
	store_int16(248,5000);

	//car.garage.turn_point
	store_int16(250,75);

	//car.garage.forward_R 
	store_int16(252,250);

	//car.garage.forward_speed 
	store_int16(254,0);

	//car.garage.forward_distance
	store_int16(200,40);


/**********************���**********************/

/**********************������ֵ******************/

   //car.adc_parameter.adc_max[LEFT_H]
   store_int16(180,4000);

   //car.adc_parameter.adc_max[LEFT_X]
   store_int16(184,4000);
   
   //car.adc_parameter.adc_max[MIDDLE]
   store_int16(188,4000);

   //car.adc_parameter.adc_max[RIGHT_X]
   store_int16(192,4000);

   //car.adc_parameter.adc_max[RIGHT_H]
   store_int16(196,4000);

/**********************������ֵ******************/

/*************************����********************/

   //car.fan.left_duty
	store_int16(224,850);

   //car.fan.right_duty
    store_int16(228,850);

/*************************����********************/

/************************Ԫ�ر�*******************/

	//car.element.excel[0] 
	 store_int16(230,0);

	//car.element.excel[1]
	 store_int16(232,3);

	//car.element.excel[2]
	 store_int16(234,8);

	//car.element.excel[3]
	 store_int16(236,4);

	//car.element.excel[4]
	 store_int16(238,11);

	//car.element.excel[5]
	 store_int16(240,6);

	//car.element.excel[6]
	 store_int16(242,8);

	//car.element.excel[7]
	 store_int16(202,9);
/************************Ԫ�ر�*******************/
	//car.circle_distance
	store_int16(132,50);

	//car.ramp_distance
	store_int16(134,50);

	//car.road.type
	store_int16(222,0);
	
	//car.meter_distance         
	store_int16(146,100);
}
void Store_280(){

	//�洢�ռ�ȫ��д0
	//  uint8 i ;
	//  for(i=0; i<511;i++)
	//  {
	// 	extern_iap_write_bytes(i, (uint8)0);
	//  }
	iap_erase_page(0);
/**********************ֱ��*************************/
	//car.straight.kp 
	store_float(0, 35); 

	//car.straight.kd
	store_float(4, 0.85); 

	//car.straight.base_speed
	store_float(8, 280); 
/**********************ֱ��*************************/

/**********************Բ��(С��)*******************/
  
   //car.circle.speed[0]
   store_int16(16,270);

   //car.circle.PRE_ENTER_H_STRENGTH[0]
   store_int16(20,140);
   
   //car.circle.PRE_ENTER_MIDDLE[0]
   store_int16(24,70);
   //28
   //car.circle.Enter_DIST[0]
   store_int16(28,42);
   
   //car.circle.Enter_ANGLE[0]
   store_int16(32,30);

   //car.circle.angle_1[0]
   store_int16(36,260);

   //car.circle.angle_2[0] 
   store_int16(40,300);

   //car.circle.BIAS_ADJUST[0]
   store_int16(44,20);
   //94
   //car.circle.Enter_MID[0]
   store_int16(216,94);

    //car.circle.enter_R_circle[0]
   store_int16(244,35);
/**********************Բ��(С��)*******************/

/**********************Բ��(��)*******************/

   // car.circle.speed[1]
   store_int16(52,260);

   //car.circle.PRE_ENTER_H_STRENGTH[1]
   store_int16(56,140);
   
   //car.circle.PRE_ENTER_MIDDLE[1]
   store_int16(60,70);

   //car.circle.Enter_DIST[1]
   store_int16(64,45);
   
   //car.circle.Enter_ANGLE[1]
   store_int16(68,30);

   //car.circle.angle_1[1]
   store_int16(72,260);

   //car.circle.angle_2[1] 
   store_int16(76,300);

   //car.circle.BIAS_ADJUST[1]
   store_int16(80,0);

   //car.circle.Enter_MID[1]
   store_int16(220,82);

    //car.circle.enter_R_circle[1]
    store_int16(246,45);
/**********************Բ��(��)*******************/

/**********************Բ��(R90)***********************/
    //55
    //car.circle.Enter_DIST[2]
    store_int16(10,50);
    //78
    //car.circle.Enter_MID[2] 
    store_int16(14,70);
    //55
	//car.circle.enter_R_circle[2]
    store_int16(48,50);
/**********************Բ��(R90)***********************/

/************************�µ�**********************/

   //car.ramp.speed
   store_float(84,100);

   //car.ramp.speed2
   store_float(88,200);

   //car.ramp.adc_middle
   store_float(92,75);

   //car.ramp.adc_left
   store_float(96,50);

   //car.ramp.adc_right
   store_float(100,50);

/************************�µ�**********************/

/************************�ϰ�**********************/

   //car.hinder.speed 
   store_float(104,280);

   //car.hinder.diff
   store_float(108,100);

   //car.hinder.judge_distance
   store_float(112,950);

   //car.hinder.point_1.angle
   store_float(116,50);

   //car.hinder.point_1.distance
   store_float(120,10);

   //car.hinder.point_2.angle
   store_float(124,100);

   //car.hinder.point_2.distance
   store_float(128,40);

   //car.hinder.R_circle;
   store_float(136,35);

   //car.hinder.R_circle_2
	store_float(140,35); 

   //car.hinder.dir
   store_int16(204,0);

/************************�ϰ�**********************/


/**********************����************************/

   //car.garage.out_garage_speed
   store_int16(144,200);

   //car.garage.out_garage_distance
   store_int16(148,15);
   
   //car.garage.out_garage_angle
   store_int16(152,50);

   //car.garage.out_garage_diff
   store_int16(156,150);


/**********************����************************/



/**********************���(����)******************/

/**********************���***********************/

    /*********��������********/
	//car.garage.enter_garage_distance_1_left
	store_int16(158,2);
	
	//car.garage.enter_garage_angle_left
    store_int16(160,8);

	//car.garage.enter_garage_distance_2_left
	store_int16(162,35);

	//car.garage.enter_garage_diff
	store_int16(164,100);
	/*********��������********/

	/*********��������********/
	//car.garage.enter_garage_distance_1_right
	store_int16(166,2);
	
	//car.garage.enter_garage_angle_right
    store_int16(168,8);

	//car.garage.enter_garage_distance_2_right
	store_int16(170,50);

	//car.garage.enter_garage_diff 
	store_int16(172,100);
	/*********��������********/

	//car.garage.distance_to_zebra
	store_int16(248,5000);

	//car.garage.turn_point
	store_int16(250,75);

	//car.garage.forward_R 
	store_int16(252,250);

	//car.garage.forward_speed 
	store_int16(254,0);

	//car.garage.forward_distance
	store_int16(200,40);


/**********************���**********************/

	

/**********************���(����)******************/

/**********************������ֵ******************/

   //car.adc_parameter.adc_max[LEFT_H]
   store_int16(180,4000);

   //car.adc_parameter.adc_max[LEFT_X]
   store_int16(184,4000);
   
   //car.adc_parameter.adc_max[MIDDLE]
   store_int16(188,4000);

   //car.adc_parameter.adc_max[RIGHT_X]
   store_int16(192,4000);

   //car.adc_parameter.adc_max[RIGHT_H]
   store_int16(196,4000);

/**********************������ֵ******************/

/*************************����********************/

   //car.fan.left_duty
	store_int16(224,920);

   //car.fan.right_duty
    store_int16(228,920);

/*************************����********************/

/************************Ԫ�ر�*******************/

	//car.element.excel[0] 
	 store_int16(230,0);

	//car.element.excel[1]
	 store_int16(232,3);

	//car.element.excel[2]
	 store_int16(234,8);

	//car.element.excel[3]
	 store_int16(236,4);

	//car.element.excel[4]
	 store_int16(238,6);

	//car.element.excel[5]
	 store_int16(240,8);

	//car.element.excel[6]
	 store_int16(242,6);

	//car.element.excel[7]
	 store_int16(202,9);
/************************Ԫ�ر�*******************/

	//car.circle_distance
	store_int16(132,50);

	//car.ramp_distance
	store_int16(134,50);

	//car.road.type
	store_int16(222,0);

    //car.meter_distance         
	store_int16(146,100);

}

void Read_Para(){
	/********************ֱ��****************************/
	car.straight.kp       	 	         = Get_Float(0);
	car.straight.kd                      = Get_Float(4);
    car.straight.base_speed              = Get_Float(8);
	/********************ֱ��****************************/

	/********************Բ��****************************/
	car.circle.speed[0]                  = Get_Int16(16);
	car.circle.PRE_ENTER_H_STRENGTH[0]   = Get_Int16(20);
	car.circle.PRE_ENTER_MIDDLE[0]       = Get_Int16(24);
	car.circle.Enter_DIST[0]             = Get_Int16(28);
	car.circle.Enter_ANGLE[0]            = Get_Int16(32);
	car.circle.angle_1[0]                = Get_Int16(36);
    car.circle.angle_2[0]                = Get_Int16(40);
	car.circle.BIAS_ADJUST[0]            = Get_Int16(44);

	car.circle.speed[1]                  = Get_Int16(52);
	car.circle.PRE_ENTER_H_STRENGTH[1]   = Get_Int16(56);
	car.circle.PRE_ENTER_MIDDLE[1]       = Get_Int16(60);
	car.circle.Enter_DIST[1]             = Get_Int16(64);
	car.circle.Enter_ANGLE[1]            = Get_Int16(68);
	car.circle.angle_1[1]                = Get_Int16(72);
    car.circle.angle_2[1]                = Get_Int16(76);
	car.circle.BIAS_ADJUST[1]            = Get_Int16(80);

	car.circle.enter_R_circle[0]         = Get_Int16(244);
	car.circle.enter_R_circle[1]         = Get_Int16(246);

	car.circle.Enter_DIST[2]             = Get_Int16(10);
	car.circle.Enter_MID[2]              = Get_Int16(14);
	car.circle.enter_R_circle[2]         = Get_Int16(48);

	/********************Բ��****************************/

	/********************�µ�****************************/
	car.ramp.speed                       = Get_Float(84);
	car.ramp.speed2                      = Get_Float(88);
	car.ramp.adc_middle                  = Get_Float(92);
	car.ramp.adc_left                    = Get_Float(96);
	car.ramp.adc_right                   = Get_Float(100);
	/********************�µ�****************************/

	/********************�ϰ�****************************/
	car.hinder.speed                     = Get_Float(104);
	car.hinder.diff                      = Get_Float(108);
	car.hinder.judge_distance            = Get_Float(112);
	car.hinder.point_1.angle             = Get_Float(116);
	car.hinder.point_1.distance          = Get_Float(120);
	car.hinder.point_2.angle             = Get_Float(124);
	car.hinder.point_2.distance          = Get_Float(128);
	car.hinder.R_circle                  = Get_Float(136);
	car.hinder.R_circle_2                = Get_Float(140);
	/********************�ϰ�****************************/

	/********************����****************************/
	car.garage.out_garage_speed          = Get_Int16(144);
	car.garage.out_garage_distance       = Get_Int16(148);
	car.garage.out_garage_angle          = Get_Int16(152);
	car.garage.out_garage_diff           = Get_Int16(156);
	/********************����****************************/
	
	/********************���****************************/
	car.garage.enter_garage_distance_1_left   = Get_Int16(158);
	car.garage.enter_garage_angle_left        = Get_Int16(160);
	car.garage.enter_garage_distance_2_left   = Get_Int16(162);
	car.garage.enter_garage_R_left            = Get_Int16(164);

	car.garage.enter_garage_distance_1_right  = Get_Int16(166);
	car.garage.enter_garage_angle_right       = Get_Int16(168);
	car.garage.enter_garage_distance_2_right  = Get_Int16(170);	
	car.garage.enter_garage_diff              = Get_Int16(172);

	car.garage.distance_to_zebra              = Get_Int16(248);
	car.garage.turn_point                     = Get_Int16(250);

	car.garage.forward_R                      = Get_Int16(252);
	car.garage.forward_speed                  = Get_Int16(254);

	car.garage.forward_distance               = Get_Int16(200);
	/********************���****************************/

	/********************���****************************/
	car.adc_parameter.adc_max[LEFT_H]    = Get_Int16(180);
	car.adc_parameter.adc_max[LEFT_X]    = Get_Int16(184);
	car.adc_parameter.adc_max[MIDDLE]    = Get_Int16(188);
	car.adc_parameter.adc_max[RIGHT_X]   = Get_Int16(192);
    car.adc_parameter.adc_max[RIGHT_H]   = Get_Int16(196);
	/********************���****************************/
	//car.garage.DIR                       = Get_Int16(200);
	car.hinder.dir                       = Get_Int16(204);
	car.garage.MODE                      = Get_Int16(208);
	car.garage.Distance_to_Garage        = Get_Int16(212);
	car.circle.Enter_MID[0]              = Get_Int16(216);
	car.circle.Enter_MID[1]              = Get_Int16(220);

	/********************����****************************/
	car.fan.left_duty                    = Get_Int16(224);
	car.fan.right_duty                   = Get_Int16(228);
	/********************����****************************/

	/*********************Ԫ�ر�*************************/
	car.element.excel[0]                 = Get_Int16(230);
	car.element.excel[1]                 = Get_Int16(232);
	car.element.excel[2]                 = Get_Int16(234);
	car.element.excel[3]                 = Get_Int16(236);
	car.element.excel[4]                 = Get_Int16(238);
	car.element.excel[5]                 = Get_Int16(240);
	car.element.excel[6]                 = Get_Int16(242);
	car.element.excel[7]                 = Get_Int16(202);
	/*********************Ԫ�ر�*************************/
	car.circle_distance                  = Get_Int16(132);
	car.ramp_distance                    = Get_Int16(134);

	car.road_type                        =Get_Int16(222);

	car.meter_distance                   =Get_Int16(146);
	

}
void Save_Parameter(){
	store_float(0  , car.straight.kp);
	store_float(4  , car.straight.kd);
	store_float(8  , car.straight.base_speed);

	store_int16(16 , car.circle.speed[0]);
	store_int16(20 , car.circle.PRE_ENTER_H_STRENGTH[0]);
	store_int16(24 , car.circle.PRE_ENTER_MIDDLE[0]);
	store_int16(28 , car.circle.Enter_DIST[0]);
	store_int16(32 , car.circle.Enter_ANGLE[0]);
	store_int16(36 , car.circle.angle_1[0]);
	store_int16(40 , car.circle.angle_2[0]);
	store_int16(44 , car.circle.BIAS_ADJUST[0]);

	store_int16(52 , car.circle.speed[1]);
	store_int16(56 , car.circle.PRE_ENTER_H_STRENGTH[1]);
	store_int16(60 , car.circle.PRE_ENTER_MIDDLE[1]);
	store_int16(64 , car.circle.Enter_DIST[1]);
	store_int16(68 , car.circle.Enter_ANGLE[1]);
	store_int16(72 , car.circle.angle_1[1]);
	store_int16(76 , car.circle.angle_2[1]);
	store_int16(80 , car.circle.BIAS_ADJUST[1]);
	store_float(84 , car.ramp.speed);
	store_float(88 , car.ramp.speed2);
	store_float(92 , car.ramp.adc_middle);
	store_float(96 , car.ramp.adc_left);
	store_float(100, car.ramp.adc_right);

	store_float(104, car.hinder.speed);
	store_float(108, car.hinder.diff);
	store_float(112, car.hinder.judge_distance);
	store_float(116, car.hinder.point_1.angle);
	store_float(120, car.hinder.point_1.distance);
	store_float(124, car.hinder.point_2.angle);
	store_float(128, car.hinder.point_2.distance);
	store_float(136, car.hinder.R_circle);
	store_float(140, car.hinder.R_circle_2);

	store_int16(144, car.garage.out_garage_speed);
	store_int16(148, car.garage.out_garage_distance);
	store_int16(152, car.garage.out_garage_angle);
	store_int16(156, car.garage.out_garage_diff);

	store_int16(158,car.garage.enter_garage_distance_1_left);
	store_int16(160,car.garage.enter_garage_angle_left);
	store_int16(162,car.garage.enter_garage_distance_2_left);
	store_int16(164,car.garage.enter_garage_R_left);
	store_int16(166,car.garage.enter_garage_distance_1_right);
	store_int16(168,car.garage.enter_garage_angle_right);
	store_int16(170,car.garage.enter_garage_distance_2_right );
	store_int16(172,car.garage.enter_garage_diff);

	store_int16(180, car.adc_parameter.adc_max[LEFT_H]);
	store_int16(184, car.adc_parameter.adc_max[LEFT_X]);
	store_int16(188, car.adc_parameter.adc_max[MIDDLE]);
	store_int16(192, car.adc_parameter.adc_max[RIGHT_X]);
	store_int16(196, car.adc_parameter.adc_max[RIGHT_H]);
	store_int16(200, car.garage.DIR);
	store_int16(204, car.hinder.dir);
	store_int16(208, car.garage.MODE);
	store_int16(212, car.garage.Distance_to_Garage);
	store_int16(216, car.circle.Enter_MID[0]);
	store_int16(220, car.circle.Enter_MID[1]);
	store_int16(224, car.fan.left_duty);
	store_int16(228, car.fan.right_duty);

	store_int16(230,car.element.excel[0]);
	store_int16(232,car.element.excel[1]);
	store_int16(234,car.element.excel[2]);
	store_int16(236,car.element.excel[3]);
	store_int16(238,car.element.excel[4]);
	store_int16(240,car.element.excel[5]);
	store_int16(242,car.element.excel[6]);
	store_int16(202,car.element.excel[7]);

	store_int16(244,car.circle.enter_R_circle[0]);
	store_int16(246,car.circle.enter_R_circle[1]);

	store_int16(248,car.garage.distance_to_zebra);
	store_int16(250,car.garage.turn_point);
	store_int16(252,car.garage.forward_R);
	store_int16(254,car.garage.forward_speed);
	store_int16(200,car.garage.forward_distance);

	store_int16(10,car.circle.Enter_DIST[2]);
	store_int16(14,car.circle.Enter_MID[2]);
	store_int16(48,car.circle.enter_R_circle[2]);

	store_int16(132,car.circle_distance);
	store_int16(134,car.ramp_distance);

	store_int16(222,car.road_type);
	     
	store_int16(146,car.meter_distance);
}

#define MAX_LINES 4  //��Ļ�����ʾ����������
#define LINE_STEP 2  //ÿһ��������Ҫռ�õ���Ļ���� Ӣ��Ϊ1 ����Ϊ2

void display_str(struct MenuItem * MenuPoint,short selectItem)
{
	u8 j;
	u8 index;
	
	//��ҳ����
	
	static int last_page = 0;
	int curr_page = (selectItem-1)/MAX_LINES;
  //ips114_clear(WHITE);
	//�Ǳ༭ģʽ
	if(edit_mode == 0){
		if(curr_page!=last_page){
			last_page = curr_page;
			//oled_fill_spi(BLACK);
			ips114_clear(WHITE);
		}

		//�����ͷ
		for ( j = 0; j < MAX_LINES; j++)
		{
			index = (uint8)((selectItem-1)%MAX_LINES);
			if(j!=index){
        ips114_showstr(0,(u8)(LINE_STEP*j),"  ");
			}
			else{
        ips114_showstr(0,(u8)(LINE_STEP*j),">>");
			}	
		}
		if(selectItem-1>=MAX_LINES*(MenuPoint->MenuCount/MAX_LINES)){
			for ( j = 0; j < (MenuPoint->MenuCount%MAX_LINES ) ; j++){
        ips114_showstr(17,(u8)(LINE_STEP*j),MenuPoint[j+MAX_LINES*((selectItem-1)/MAX_LINES)].DisplayString);
			}
		}
		else{
			for ( j = 0; j < (MenuPoint->MenuCount<MAX_LINES?MenuPoint->MenuCount:MAX_LINES ) ; j++){
        ips114_showstr(17,(u8)(LINE_STEP*j),MenuPoint[j+MAX_LINES*((selectItem-1)/MAX_LINES)].DisplayString);
			}
		}
	}

	//�༭ģʽ
	else{
		//��ͷ
		for ( j = 0; j < MAX_LINES; j++)
		{
		ips114_showstr(0,(u8)(LINE_STEP*j),"  ");
		}
		ips114_showstr(55,LINE_STEP,">>");
		//����
		ips114_showstr(20,0,"STEP");
		ips114_showstr(20,LINE_STEP,MenuPoint[selectItem-1].DisplayString);
	}

}

uint8 start_x = 100;
void display_data(struct MenuItem * MenuPoint,short selectItem)
{
	uint8 j;
	uint8 index;
	if(edit_mode == 0){
		if(selectItem-1>=MAX_LINES*(MenuPoint->MenuCount/MAX_LINES)){
			for ( j = 0; j < (MenuPoint->MenuCount%MAX_LINES ) ; j++){
				index = j+MAX_LINES*((selectItem-1)/MAX_LINES);
				if(MenuPoint[index].DiplayData != NULL){

					switch(MenuPoint[index].num_type){
						case MY_FLOAT:
              ips114_showfloat(start_x,(u8)(LINE_STEP*j),*MenuPoint[index].DiplayData,5,2);  
							break;
						case MY_INT16:
              ips114_showint16(start_x,(u8)(LINE_STEP*j),*((int16*)MenuPoint[index].DiplayData));  
							break;
						case MY_UINT16:
              ips114_showuint16(start_x,(u8)(LINE_STEP*j),*((uint16*)MenuPoint[index].DiplayData)); 
							break;
					}

						
				}
			}
		}
		else{
			for ( j = 0; j < (MenuPoint->MenuCount<MAX_LINES?MenuPoint->MenuCount:MAX_LINES ) ; j++){
				index = j+MAX_LINES*((selectItem-1)/MAX_LINES);
				if(MenuPoint[index].DiplayData != NULL){
						switch(MenuPoint[index].num_type){
						case MY_FLOAT:
                ips114_showfloat(start_x,(u8)(LINE_STEP*j),*MenuPoint[index].DiplayData,5,2); 
							break;
						case MY_INT16:
                ips114_showint16(start_x,(u8)(LINE_STEP*j),*((int16*)MenuPoint[index].DiplayData));  
							break;
						case MY_UINT16:
               ips114_showuint16(start_x,(u8)(LINE_STEP*j),*((uint16*)MenuPoint[index].DiplayData));
							break;
					}
				}
			}
		}
	}
	else{
		index = selectItem - 1;
		if(MenuPoint[index].DiplayData != NULL){
			switch(MenuPoint[index].num_type){
			case MY_FLOAT:
        ips114_showfloat(start_x,LINE_STEP,*MenuPoint[index].DiplayData,5,3); 
				break;
			case MY_INT16:
         ips114_showint16(start_x,LINE_STEP,*((int16*)MenuPoint[index].DiplayData)); 
				break;
			case MY_UINT16:
          ips114_showuint16(start_x,LINE_STEP,*((uint16*)MenuPoint[index].DiplayData));
				break;
			}
			if(step_size == 0){
       ips114_showfloat(start_x,0,0.01,5,2); 
			}
			else if(step_size == 1){
       ips114_showfloat(start_x,0,0.1,5,2);
			}
			else if(step_size == 2){

       ips114_showfloat(start_x,0,1,5,2);
			}
			else if(step_size == 3){
       ips114_showfloat(start_x,0,10,5,2);
			}
			else if(step_size == 4){
       ips114_showfloat(start_x,0,100,5,2);
			}
			else if(step_size == 5){
       ips114_showfloat(start_x,0,1000,5,2);
			}
			
		}
	}
	
}

//��Ļˢ��
void Screen_update(KEYn_e key){
	
	if(key!=KEY_NULL){
		//�Ǳ༭ģʽ
		if(edit_mode==0){
			switch(key){
				case KEY_1://��
					{
						//�����ǰ������Ϊ1����ǰ����Ϊ��ǰ�˵���������
						if (selectItem == 1) selectItem = MenuPoint->MenuCount;
						//����������1�������ƶ�һ��
						else selectItem--;
					};
					break;
				case KEY_2://��
					{			
						//�����ǰ������Ϊ��ǰ�˵�������������ǰ����Ϊ1
						if (selectItem == MenuPoint->MenuCount) selectItem = 1;
						//����������1�������ƶ�һ��
						else selectItem++;
					};
					break;
				case KEY_4:
					{//�� ����
						if (MenuPoint[selectItem - 1].ParentMenus != NULL)
						{
							MenuPoint = MenuPoint[selectItem - 1].ParentMenus;
							selectItem = 1;
               ips114_clear(WHITE);
						}
					};
					break;
				case KEY_3://�� ȷ�Ͻ������Ŀ
					{
						//����Ӳ˵���Ϊ��
						if (MenuPoint[selectItem - 1].ChildrenMenus != NULL)
						{
							MenuPoint = MenuPoint[selectItem - 1].ChildrenMenus;
							selectItem = 1;
               				ips114_clear(WHITE);
						}
						//�������ָ�벻�� ���øú���
						if(MenuPoint[selectItem - 1].Subs != NULL){
							MenuPoint[selectItem - 1].Subs();
						}


					};
					break;
				case KEY_5://�޸Ĳ���
					if (MenuPoint[selectItem - 1].DiplayData != NULL){
						//����༭ģʽ
						edit_mode = 1;
             ips114_clear(WHITE);
					}
					break;
			}
			
		}

		else{
			switch(key){
				case KEY_1://�� ����
					{
						if(MenuPoint[selectItem - 1].num_type == MY_FLOAT){
							if(step_size == 0){
								*MenuPoint[selectItem - 1].DiplayData  +=0.01; 
							}
							else if(step_size == 1){
								*MenuPoint[selectItem - 1].DiplayData  +=0.1; 
							}
							else if(step_size == 2){
								*MenuPoint[selectItem - 1].DiplayData  +=1; 
							}
							else if(step_size == 3){
								*MenuPoint[selectItem - 1].DiplayData  +=10; 
							}
							else if(step_size == 4){
								*MenuPoint[selectItem - 1].DiplayData  +=100; 
							}
						}
						else if(MenuPoint[selectItem - 1].num_type == MY_INT16 || MY_UINT16){
							if(step_size == 2){
								*((int16*)MenuPoint[selectItem - 1].DiplayData ) +=1; 
							}
							else if(step_size == 3){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )   +=10; 
							}
							else if(step_size == 4){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )   +=100; 
							}
							else if(step_size == 5){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )   +=1000; 
							}

						}
					};
					break;
				case KEY_2://�� ��С
					{		
						if(MenuPoint[selectItem - 1].num_type == MY_FLOAT){
							if(step_size == 0){
								*MenuPoint[selectItem - 1].DiplayData  -=0.01; 
							}
							else if(step_size == 1){
								*MenuPoint[selectItem - 1].DiplayData  -=0.1; 
							}
							else if(step_size == 2){
								*MenuPoint[selectItem - 1].DiplayData  -=1; 
							}
							else if(step_size == 3){
								*MenuPoint[selectItem - 1].DiplayData  -=10; 
							}
							else if(step_size == 4){
								*MenuPoint[selectItem - 1].DiplayData  -=100; 
							}

						}
						else if(MenuPoint[selectItem - 1].num_type == MY_INT16 || MY_UINT16){
							if(step_size == 2){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )   -=1; 
							}
							else if(step_size == 3){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )  -=10; 
							}
							else if(step_size == 4){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )  -=100; 
							}
							else if(step_size == 5){
								*((int16*)MenuPoint[selectItem - 1].DiplayData )   -=1000; 
							}
						}
					};
					break;
				case KEY_3://�� ���󲽳�
				{		
					step_size ++; 
					if(step_size>=5)
						step_size = 5;
				};
					break;
				case KEY_4://�� ��С����
				{		
					step_size --;	
					if(step_size<=0)
						step_size = 0;
				};
					break;
				case KEY_5://�޸Ĳ���
					if (MenuPoint[selectItem - 1].DiplayData != NULL){
						Save_Parameter();
						//�˳��༭ģʽ
						edit_mode = 0;
            ips114_clear(WHITE);
						//�������
						printf("���Ϳ�ʼ");
						
						printf("���ͽ���");
					}
					break;
			}
		}
		display_str(MenuPoint,selectItem);
		//ips114_showint32(0,6,(float)selectItem,2);
	}
	display_data(MenuPoint,selectItem);
}