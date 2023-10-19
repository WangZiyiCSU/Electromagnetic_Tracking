/*********************************************************************************************************************
 * @file       		main
 * @author     		Wang
 * @version    		1.1.0
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @date       		2023-10-10
 * @brief           ���棺���Խ�ֹ���ж���ʹ�ô��ڷ������ݣ�����������ˣ�
 ********************************************************************************************************************/

#include "headfile.h"
#include "key.h"
#include "menu.h"

#define HALL_PIN P26

 /*
  * ϵͳƵ�ʣ��ɲ鿴board.h�е� FOSC �궨���޸ġ�
  * board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�����ϵͳƵ��Ϊ33.1776MHZ
  * ��board_init��,�Ѿ���P54��������Ϊ��λ
  * �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
  */


void main()
{
    board_init();			// ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û�����(���磺�����ʼ�������)
    car_System_Init();    //�����ʼ��
    car_Parameter_Init(); //������ʼ��
//    Read_Para();
//    pit_timer_ms(TIM_4, 5);
    ips114_clear(WHITE);

    while(1)
	{
        if(KEY1_PIN == KEY_DOWN)
            Start_Up();
//        int next_menu = MainMenu(); 
//        if(next_menu == 1)
//        {
//            Start_Up();
//        }
//        else if(next_menu == 3)
//        {
//            
//            Menu_ADC_test();
//        }
//        else if(next_menu == 2)
//        {
//            //ParameterMenu();
//        }  
    }
}
