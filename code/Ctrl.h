/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
<<<<<<< HEAD
File name: Ctrl.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Ctrl.h
Others:      Êó†
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      ÂàùÁâà
Cross_Z   2026.3.25      1.0      ËßÑÂàôÁ™ÅÂèòÔºåÈáçÊûÑ‰ª£Á†Å
=======
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Ctrl.h
Others:      Œﬁ
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30      0.0      ≥ı º
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
**************************************************/
#ifndef __CTRL_H
#define __CTRL_H

#include "zf_common_headfile.h"
#include "headfiles.h"

<<<<<<< HEAD
/***********************************ÂÆèÂÆö‰πâ***********************************/
#define NODE_NUM_MAX    35   // ËäÇÁÇπÊï∞ÈáèÊúÄÂ§ßÂÄº
#define ELEMENT_NUM_MAX 30   // ÂÖÉÂô®‰ª∂Êï∞ÈáèÊúÄÂ§ßÂÄº
#define MAX_SPEED       80   // ÊúÄÈ´òÈÄüÂ∫¶

// ËøêË°åÊ®°ÂºèÂÆö‰πâ
#define SLOW_MODE       0   // ÊÖ¢ÈÄüËÆ∞ÈáåÁ®ãÊ®°Âºè
#define FAST_MODE       1   // Âø´ÈÄüË∑ëÊàêÁª©Ê®°Âºè
=======
/***********************************∫Í∂®“Â***********************************/
#define NODE_NUM_MAX    20   // Ω⁄µ„ ˝¡ø◊Ó¥Û÷µ
#define ELEMENT_NUM_MAX 5   // ‘™∆˜º˛ ˝¡ø◊Ó¥Û÷µ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

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

<<<<<<< HEAD
/**********************************Êûö‰∏æ&ÁªìÊûÑ‰Ωì*********************************/
=======
/**********************************√∂æŸ&Ω·ππÃÂ*********************************/
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
typedef struct
{
    int Left;
    int Right;
    int Stop;
    float Mileage;
<<<<<<< HEAD
    float Spd_Mileage;
    int Straight;
}Count_Typedef;/*ÊúâÂÖ≥ËÆ°Êï∞ÁöÑ*/

typedef enum
{
    Turn_Node_Left = 0,   // Â∑¶ËΩ¨ËäÇÁÇπ
    Turn_Node_Right = 1,  // Âè≥ËΩ¨ËäÇÁÇπ
    Straight_Node = 2,    // Áõ¥Ëµ∞ËäÇÁÇπ
    Normal_Elem = 3,      // ÊôÆÈÄöÂÖÉÂô®‰ª∂
    Special_Elem = 4,     // ÁâπÊÆäÂÖÉÂô®‰ª∂
    Source_Elem_Left = 5,    // ÊúâÊ∫êÂÖÉÂô®‰ª∂Â∑¶
    Source_Elem_Right = 6,    // ÊúâÊ∫êÂÖÉÂô®‰ª∂Âè≥
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
    uint8_t Node_Num;     // ËäÇÁÇπÊï∞Èáè
    uint8_t Exception_Num[NODE_NUM_MAX];
    uint8_t Exception_Type[ELEMENT_NUM_MAX];
    int16_t Node_Mileage[NODE_NUM_MAX][2];     // ËäÇÁÇπÈáåÁ®ã
    uint8_t Node_Turn_Dir[NODE_NUM_MAX];    // ËäÇÁÇπËΩ¨ÂºØÊñπÂêë
    Speed_Adjust_Typedef Speed_Adjust;
}Run_Track_Typedef;

/*********************************ÂÖ®Â±ÄÂèòÈáèÂ£∞Êòé*********************************/
=======
    int Straight;
}Count_Typedef;/*”–πÿº∆ ˝µƒ*/

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
}Racing_track_Typedef;/*¥Ê»¸µ¿–≈œ¢£¨±„”⁄«–ªª»¸µ¿*/

typedef enum
{
    Normal_Mode,
    Turn_Left,
    Turn_Right,
    Mileage_Mode,
    Straight_Mode,
}Run_Mode_Enum;/*‘À––ƒ£ Ω*/

typedef enum
{
    Normal_Stage,
    Straight_Stage,
}Mileage_Stage_Enum;/*º«¿Ô≥Ã∑÷¡Ω∂Œ≈‹*/

/*********************************»´æ÷±‰¡ø…˘√˜*********************************/
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
extern int Basic_Speed;
extern int Left_Exp_Spd;
extern int Right_Exp_Spd;
extern float Average_Speed;
extern int Error;
extern uint16 Light_ADC[15];
extern uint8 Light_Convert[15];
<<<<<<< HEAD
extern int Left_Real_Spd;      // Â∑¶ÂÆûÈôÖ
extern int Right_Real_Spd;     // Âè≥ÂÆûÈôÖ
=======
extern int Left_Real_Spd;      // ◊Û µº 
extern int Right_Real_Spd;     // ”“ µº 
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
extern float Turn_PID_Out;
extern float Gyro_PID_Out;
extern float Left_PID_Out;
extern float Right_PID_Out;
extern int Stop_Flag;
extern int8_t Execute_Times;
extern int8_t Mileage_Times;
<<<<<<< HEAD
extern uint8_t Line_Num_Count;       // ÊúâÂÖÉÂô®‰ª∂ÁöÑÁ∫øÁöÑÊï∞Èáè
extern uint8_t In_Line_Ele_Count;    // ‰∏ÄÊÆµËøûÁ∫ø‰∏≠ÂÖÉÂô®‰ª∂ËÆ°Êï∞
=======
extern uint8_t Line_Num_Count;       // ”–‘™∆˜º˛µƒœﬂµƒ ˝¡ø
extern uint8_t In_Line_Ele_Count;    // “ª∂Œ¡¨œﬂ÷–‘™∆˜º˛º∆ ˝
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
extern int Track_Num;
extern float Gyro_Integral;
extern int Finish_Flag;
extern int Finish_Count;
<<<<<<< HEAD
extern int Lap_Count;           // ÂúàÊï∞ËÆ°Êï∞
extern uint8_t Mileage_Record_Flag; // ÈáåÁ®ãËÆ∞ÂΩïÊ†áÂøó
extern uint32 Current_Track_Page;    // ÂΩìÂâçËµõÈÅìÈ°µÈù¢
extern uint8_t Run_Mode_Choice;     // ËøêË°åÊ®°ÂºèÈÄâÊã©
=======
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

extern Count_Typedef Count;

extern Run_Mode_Enum Run_Mode;
<<<<<<< HEAD
extern Run_Track_Typedef Run_Track;
extern Run_Track_Typedef Flash_Track; 
extern Next_Exception_Enum Next_Exception;
=======
extern Racing_track_Typedef Run_Track;
extern Mileage_Stage_Enum Mileage_Stage;

extern Racing_track_Typedef Pre_Contest_1;
extern Racing_track_Typedef Pre_Contest_2;
extern Racing_track_Typedef Pre_Contest_3;

extern Racing_track_Typedef Final_Contest_1;
extern Racing_track_Typedef Final_Contest_2;
extern Racing_track_Typedef Final_Contest_3;
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

extern PID_HandleTypeDef Gyro_PID;
extern PID_HandleTypeDef Left_PID;
extern PID_HandleTypeDef Right_PID;
extern PID_HandleTypeDef Turn_PID;

<<<<<<< HEAD
extern Run_Track_Typedef Pre_Track_1;

/***********************************ÂáΩÊï∞Â£∞Êòé***********************************/
=======
/***********************************∫Ø ˝…˘√˜***********************************/
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
void Car_Go(void);
void Set_Speed(void);
void Get_Speed(void);
uint8 Check_Edge();
void Get_IMU(void);
void Get_Error(void);
void Soft_Start(void);
void Set_Out(void);
<<<<<<< HEAD
void Light_Process(void);
=======
void Mileage_Mode_Run(void);
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
void Turn_Right_Run(void);
void Turn_Left_Run(void);
void Normal_Run(void);
void Straight_Run(void);
<<<<<<< HEAD
void Mileage_Run(void);
void Exception_Judge(void);
void Mode_Choose(void);
Run_Mode_Enum Mode_Update(void);
void Save_Mileage_To_Flash(void);
void Load_Mileage_From_Flash(void);
=======
void Mileage_Run_Stage_1(void);
void Mileage_Run_Stage_2(void);
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

#endif
