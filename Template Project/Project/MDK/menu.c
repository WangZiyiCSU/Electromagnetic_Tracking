#include "headfile.h"
#include "key.h"
#include "SEEKFREE_IPS114_SPI.h"

//#define ROW_SPACING 2;//行间距
int menu_length = 3;
int key = 0;

/*
* @name MainMenu
* @date 2023/10/15
* @breif 主菜单
* @return int
*/
int MainMenu()
{
    int position = 1;//第n行
    
    ips114_showstr(90, 0, "MAIN_MENU");
    ips114_showstr(10, 2, "Start");
    ips114_showstr(10, 4, "Modify");
    ips114_showstr(10, 6, "Test");

    while(1)
    {
        key = KEY_Scan();
        if(key == KEY_2)
        {
            position++;
            if(position > menu_length)
            {
                position = 1;
            }
        }
        
        if(key == KEY_3)
        {
            ips114_clear(WHITE);
            return position;
        }
        
        if(key == KEY_NULL)
        {
            return 0;
        }
        switch(position)
        {
            case 1:
            {
                ips114_showstr(0, 2, "*");
                ips114_showstr(0, 4, " ");
                ips114_showstr(0, 6, " ");
            }
            break;
            case 2:
            {
                ips114_showstr(0, 2, " ");
                ips114_showstr(0, 4, "*");
                ips114_showstr(0, 6, " ");
            }
            break;
            case 3:
            {
                ips114_showstr(0, 2, " ");
                ips114_showstr(0, 4, " ");
                ips114_showstr(0, 6, "*");
            }
            break;
        }
    }
}

void Menu_ADC_test()
{
    uint16 data1,data2,data3,data4;

    ips114_clear(WHITE);
//    while(1)
//    {
        data1 = adc_once(ADC_P00,ADC_12BIT);  	//采集ADC_P00电压，精度12位
        data2 = adc_once(ADC_P01,ADC_12BIT);    //采集ADC_P01电压，精度12位
        data3 = adc_once(ADC_P05,ADC_12BIT);	//采集ADC_P05电压，精度12位
        data4 = adc_once(ADC_P06,ADC_12BIT);	//采集ADC_P06电压，精度12位
    
        ips114_showstr(0, 0, "C 1:  ");	
        ips114_showuint16(6 * 8, 0, data1);
        ips114_showstr(0, 2, "C 2:  ");	
        ips114_showuint16(6 * 8, 2, data2);
        ips114_showstr(0, 4, "C 3:  ");	
        ips114_showuint16(6 * 8, 4, data3);
        ips114_showstr(0, 6, "C 4:  ");	
        ips114_showuint16(6 * 8, 6, data4);
        
//        key = KEY_Scan();
//        if(key == KEY_1)
//        {
//            break;
//        }
//    }
}
