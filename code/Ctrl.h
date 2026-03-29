/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Ctrl.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Ctrl.h
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      初版
Cross_Z   2026.3.25      1.0      规则突变，重构代码
**************************************************/
#ifndef __CTRL_H
#define __CTRL_H

#include "zf_common_headfile.h"
#include "headfiles.h"

/***********************************宏定义***********************************/
#define NODE_NUM_MAX    35   // 节点数量最大值
#define ELEMENT_NUM_MAX 30   // 元器件数量最大值
#define MAX_SPEED       80   // 最高速度

// 运行模式定义
#define SLOW_MODE       0   // 慢速记里程模式
#define FAST_MODE       1   // 快速跑成绩模式

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
    float Spd_Mileage;
    int Straight;
}Count_Typedef;/*有关计数的*/

typedef enum
{
    Turn_Node_Left = 0,   // 左转节点
    Turn_Node_Right = 1,  // 右转节点
    Straight_Node = 2,    // 直走节点
    Normal_Elem = 3,      // 普通元器件
    Special_Elem = 4,     // 特殊元器件
    Source_Elem_Left = 5,    // 有源元器件左
    Source_Elem_Right = 6,    // 有源元器件右
}Next_Exception_Enum;

typedef enum
{
    Normal_Mode = 0,
    Turn_Left = 1,
    Turn_Right = 2,
    Straight_Mode = 3,
    Mileage_Mode = 4,
}Run_Mode_Enum;

typedef enum
{
    Left = 0,
    Right = 1,
}Node_Dir_Enum;

typedef struct
{
    int Range_Target[2];
    int Range_Source[2];
}Speed_Adjust_Typedef;

typedef struct 
{
    uint8_t Node_Num;     // 节点数量
    uint8_t Exception_Num[NODE_NUM_MAX];
    uint8_t Exception_Type[ELEMENT_NUM_MAX];
    int16_t Node_Mileage[NODE_NUM_MAX][2];     // 节点里程
    uint8_t Node_Turn_Dir[NODE_NUM_MAX];    // 节点转弯方向
    Speed_Adjust_Typedef Speed_Adjust;
}Run_Track_Typedef;

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
extern int Lap_Count;           // 圈数计数
extern uint8_t Mileage_Record_Flag; // 里程记录标志
extern uint32 Current_Track_Page;    // 当前赛道页面
extern uint8_t Run_Mode_Choice;     // 运行模式选择

extern Count_Typedef Count;

extern Run_Mode_Enum Run_Mode;
extern Run_Track_Typedef Run_Track;
extern Run_Track_Typedef Flash_Track; 
extern Next_Exception_Enum Next_Exception;

extern PID_HandleTypeDef Gyro_PID;
extern PID_HandleTypeDef Left_PID;
extern PID_HandleTypeDef Right_PID;
extern PID_HandleTypeDef Turn_PID;

extern Run_Track_Typedef Pre_Track_1;

/***********************************函数声明***********************************/
void Car_Go(void);
void Set_Speed(void);
void Get_Speed(void);
uint8 Check_Edge();
void Get_IMU(void);
void Get_Error(void);
void Soft_Start(void);
void Set_Out(void);
void Light_Process(void);
void Turn_Right_Run(void);
void Turn_Left_Run(void);
void Normal_Run(void);
void Straight_Run(void);
void Mileage_Run(void);
void Exception_Judge(void);
void Mode_Choose(void);
Run_Mode_Enum Mode_Update(void);
void Save_Mileage_To_Flash(void);
void Load_Mileage_From_Flash(void);

#endif
