#ifndef __UI_H
#define __UI_H
#include "headfile.h"

typedef unsigned char u8;

//��������
typedef enum{
	NULL_TYPE = 0,
	MY_FLOAT = 1,
	MY_UINT16 = 2,
	MY_INT16 = 3,

}Num_Type;



//�˵���ṹ��
struct MenuItem
{
	short MenuCount;               //��ǰ�˵���Ŀ����
	unsigned char *DisplayString;  //��ǰ��ĿҪ��ʾ���ַ�
    float* DiplayData;              //��ǰ��ĿҪ��ʾ������
	Num_Type num_type;             //��ǰ��ĿҪ��ʾ����������
	void(*Subs)();                 //ѡ��ĳһ�˵���ִ�еĹ��ܺ���
	struct MenuItem *ChildrenMenus;//��ǰ��Ŀ���Ӳ˵�
	struct MenuItem *ParentMenus;  //��ǰ��Ŀ�ĸ��˵�
};



void MainMenu_Display(KEYn_e key);
void menu_Display1(KEYn_e key);
void menu_Display2(KEYn_e key);
// void AD_Scan_Display(KEYn_e key);//������ֵ�ɼ���ʾ

extern void Update_Screen(KEYn_e key); //��Ļˢ��

extern void MenuInit(); //�˵���ʼ��
extern void display_str(struct MenuItem * MenuPoint,short selectItem);
extern void display_data(struct MenuItem * MenuPoint,short selectItem);


extern void Screen_update(KEYn_e key);

extern struct MenuItem* MenuPoint ;//��ǰ�˵��ĵ�ַ
extern short selectItem;//��ǰ�ڵڼ���
extern void Read_Para();
extern void Store_Para();

extern void  Store_Para280();
extern void  Store_Para320();
extern void  Store_Para350();

extern void Store_280();

//������Ļ��Ҫ�޸ĺ���
// OLED_Clear(); oled_fill_spi(BLACK)
//OLED_Display_GB2312_string   oled_p6x8str_spi




#endif