/*********************************************************************************************************************
 * @file       		main
 * @author     		Wang
 * @version    		1.1.0
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @date       		2023-10-10
 * @brief           警告：绝对禁止在中断中使用串口发送数据，否则就死机了！
 ********************************************************************************************************************/

#include "headfile.h"
#include "key.h"
#include "menu.h"

#define HALL_PIN P26

 /*
  * 系统频率，可查看board.h中的 FOSC 宏定义修改。
  * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
  * 在board_init中,已经将P54引脚设置为复位
  * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
  */


void main()
{
    board_init();			// 初始化寄存器,勿删除此句代码。

    // 此处编写用户代码(例如：外设初始化代码等)
    car_System_Init();    //外设初始化
    car_Parameter_Init(); //参数初始化
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
