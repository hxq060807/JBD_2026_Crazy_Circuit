/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Fun.h
<<<<<<< HEAD
Others:      Êó†
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      ÂàùÂßã
=======
Others:      Œﬁ
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      ≥ı º
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
**************************************************/
#ifndef __FUN_H
#define __FUN_H

#include "zf_common_headfile.h"
#include "headfiles.h"

<<<<<<< HEAD
/***********************************ÂÆö‰πâ***********************************/
=======
/***********************************√∂æŸ***********************************/
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
typedef union floatu8data
{
    float floatdata;
    uint8 u8data[4];
}floatu8data;

<<<<<<< HEAD
/***********************************ÂÆèÂÆö‰πâ***********************************/
#define Left_Motor_IN1      ATOM0_CH7_P20_8
#define Left_Motor_IN2      ATOM2_CH4_P11_9
=======
/***********************************∫Í∂®“Â***********************************/
#define Left_Motor_IN1      ATOM0_CH7_P20_8
#define Left_Motor_IN2      ATOM2_CH6_P13_1
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
#define Right_Motor_IN1     ATOM2_CH0_P00_9
#define Right_Motor_IN2     ATOM2_CH3_P00_12
#define Suction_Motor_IN1   ATOM1_CH5_P00_6
#define Suction_Motor_IN2   ATOM1_CH3_P00_4

#define EnableSwitch_ON gpio_get_level(P00_2) == 1

<<<<<<< HEAD
/*********************************ÂÖ®Â±ÄÂèòÈáèÂ£∞Êòé*********************************/
extern uint16 Light_ADC[15];
extern float Light_Thr[15][2];
extern int imu_Check;
extern float Gyro_Z[2];

/***********************************ÂáΩÊï∞Â£∞Êòés***********************************/
=======
/*********************************»´æ÷±‰¡ø…˘√˜*********************************/
extern uint16 Light_ADC[15];
extern float Light_Thr[15][2];
extern int icm_Check;
extern int16 Gyro_Z;

/***********************************∫Ø ˝…˘√˜***********************************/
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
void Vofa_Send_Data(void);
void Light_Init(void);
void Encoder_Init(void);
void Motor_Init(void);
void Other_Init(void);
void Get_Light(void);
extern void Get_Light(void);
extern void Get_Threshold(void);
<<<<<<< HEAD
extern int16_t Map_Range(int16_t min1, int16_t max1, int16_t min2, int16_t max2, int16_t value);
=======
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

#endif
