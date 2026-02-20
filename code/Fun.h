/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Fun.h
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      初始
**************************************************/
#ifndef __FUN_H
#define __FUN_H

#include "zf_common_headfile.h"
#include "headfiles.h"

/***********************************枚举***********************************/
typedef union floatu8data
{
    float floatdata;
    uint8 u8data[4];
}floatu8data;

/***********************************宏定义***********************************/
#define Left_Motor_IN1      ATOM0_CH7_P20_8
#define Left_Motor_IN2      ATOM2_CH6_P13_1
#define Right_Motor_IN1     ATOM2_CH0_P00_9
#define Right_Motor_IN2     ATOM2_CH3_P00_12
#define Suction_Motor_IN1   ATOM1_CH5_P00_6
#define Suction_Motor_IN2   ATOM1_CH3_P00_4

#define EnableSwitch_ON gpio_get_level(P00_2) == 1

/*********************************全局变量声明*********************************/
extern uint16 Light_ADC[15];
extern float Light_Thr[15][2];
extern int icm_Check;
extern int16 Gyro_Z;

/***********************************函数声明***********************************/
void Vofa_Send_Data(void);
void Light_Init(void);
void Encoder_Init(void);
void Motor_Init(void);
void Other_Init(void);
void Get_Light(void);
extern void Get_Light(void);
extern void Get_Threshold(void);

#endif
