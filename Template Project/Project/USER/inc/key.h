#ifndef _KEY_H
#define _KEY_H

#include "common.h"
//��������
#define KEY1_PIN    P73
#define KEY2_PIN    P72
#define KEY3_PIN    P70
#define KEY4_PIN    P71
#define KEY5_PIN    P75
//���뿪������
// #define SW1_PIN     P75
// #define SW2_PIN     P76

//����ģ���
typedef enum
{
    KEY_1 = 1,
    KEY_2 = 2,
    KEY_3 = 3,
    KEY_4 = 4,
    KEY_5 = 5,
    KEY_NULL = 0,
} KEYn_e;
typedef enum
{
    KEY_DOWN    = 0,  //����
    KEY_UP      = 1,   //����   
    FAIL        =0xff,
}KEYs_e;

KEYn_e KEY_Scan();//����ɨ��
#endif
