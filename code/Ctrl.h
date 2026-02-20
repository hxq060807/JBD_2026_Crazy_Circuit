/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Ctrl.h
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      初始
**************************************************/
#ifndef __CTRL_H
#define __CTRL_H

#include "zf_common_headfile.h"
#include "headfiles.h"

/***********************************宏定义***********************************/
#define NODE_NUM_MAX    20   // 节点数量最大值
#define ELEMENT_NUM_MAX 5   // 元器件数量最大值

#define GYRO_PID { \
    .kp         = 0.008, \
    .ki         = 0, \
    .kd         = 0, \
    .iOutMax    = 0, \
    .outMax     = 500, \
    .mode = PID_MODE_POSITION}

#define LEFT_PID { \
    .kp         = 0, \
    .ki         = 0, \
    .kd         = 0, \
    .iOutMax    = 100, \
    .outMax     = 9500,\
    .mode = PID_MODE_ADD}

#define RIGHT_PID { \
    .kp         = 250, \
    .ki         = 65, \
    .kd         = 0, \
    .iOutMax    = 5000, \
    .outMax     = 9500,\
    .mode = PID_MODE_ADD}

#define TURN_PID { \
    .kp         = 80, \
    .ki         = 0, \
    .kd         = 0, \
    .iOutMax    = 0, \
    .outMax     = 10000, \
    .mode = 1}

/**********************************枚举&结构体*********************************/
typedef struct
{
    int Left;
    int Right;
    int Stop;
    float Mileage;
    int Straight;
}Count_Typedef;/*有关计数的*/

typedef struct
{
    uint8_t Node_Arr_Dir[NODE_NUM_MAX];
    uint8_t Node_Arr_Mileage_Num[NODE_NUM_MAX];
    uint8_t Node_Arr_Mileage_Dir[NODE_NUM_MAX][ELEMENT_NUM_MAX];
    int Node_Arr_Mileage_Normal[NODE_NUM_MAX][ELEMENT_NUM_MAX];
    int Node_Arr_Mileage_Element[NODE_NUM_MAX][ELEMENT_NUM_MAX];
    uint8_t Node_Num;
    uint8_t Element_Num;
    uint8_t Stop_Mode;
}Racing_track_Typedef;/*存赛道信息，便于切换赛道*/

typedef enum
{
    Normal_Mode,
    Turn_Left,
    Turn_Right,
    Mileage_Mode,
    Straight_Mode,
}Run_Mode_Enum;/*运行模式*/

typedef enum
{
    Normal_Stage,
    Straight_Stage,
}Mileage_Stage_Enum;/*记里程分两段跑*/

/*********************************全局变量声明*********************************/
extern int Basic_Speed;
extern int Left_Exp_Spd;
extern int Right_Exp_Spd;
extern float Average_Speed;
extern int Error;
extern uint16 Light_ADC[15];
extern uint8 Light_Convert[15];
extern int Left_Real_Spd;      // 左实际
extern int Right_Real_Spd;     // 右实际
extern float Turn_PID_Out;
extern float Gyro_PID_Out;
extern float Left_PID_Out;
extern float Right_PID_Out;
extern int Stop_Flag;
extern int8_t Execute_Times;
extern int8_t Mileage_Times;
extern uint8_t Line_Num_Count;       // 有元器件的线的数量
extern uint8_t In_Line_Ele_Count;    // 一段连线中元器件计数
extern int Track_Num;
extern float Gyro_Integral;
extern int Finish_Flag;
extern int Finish_Count;

extern Count_Typedef Count;

extern Run_Mode_Enum Run_Mode;
extern Racing_track_Typedef Run_Track;
extern Mileage_Stage_Enum Mileage_Stage;

extern Racing_track_Typedef Pre_Contest_1;
extern Racing_track_Typedef Pre_Contest_2;
extern Racing_track_Typedef Pre_Contest_3;

extern Racing_track_Typedef Final_Contest_1;
extern Racing_track_Typedef Final_Contest_2;
extern Racing_track_Typedef Final_Contest_3;

extern PID_HandleTypeDef Gyro_PID;
extern PID_HandleTypeDef Left_PID;
extern PID_HandleTypeDef Right_PID;
extern PID_HandleTypeDef Turn_PID;

/***********************************函数声明***********************************/
void Car_Go(void);
void Set_Speed(void);
void Get_Speed(void);
uint8 Check_Edge();
void Get_IMU(void);
void Get_Error(void);
void Soft_Start(void);
void Set_Out(void);
void Mileage_Mode_Run(void);
void Turn_Right_Run(void);
void Turn_Left_Run(void);
void Normal_Run(void);
void Straight_Run(void);
void Mileage_Run_Stage_1(void);
void Mileage_Run_Stage_2(void);

#endif
