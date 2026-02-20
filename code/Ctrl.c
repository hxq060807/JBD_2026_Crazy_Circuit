/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Ctrl.c
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30    0.0        初始
**************************************************/

#include "Ctrl.h"
/*********************************变量定义*********************************/
/*---------------速度-----------------*/
int16 giSpeed_Right[3] = {0}; // 右测速
int16 giSpeed_Left[3] = {0};  // 左测速
int Left_Real_Spd = 0;      // 左实际
int Right_Real_Spd = 0;     // 右实际
int Left_Exp_Spd = 0;       // 左期望
int Right_Exp_Spd = 0;      // 右期望
int Basic_Speed = 0;
float Average_Speed = 0;
float Last_Average_Speed = 0;
int Speed_Get_Count = 1;
uint8 Soft_Start_Flag = 0;
int Soft_Start_Count = 0;
uint8 First_Mode = 0;

/*---------------光电管-----------------*/
uint8 Light_Convert[15] = {0};
uint8 Last_Light_Convert[15] = {0};

/*-----------------PID----------------*/
int16 Gyro_Z = 0;
PID_HandleTypeDef Gyro_PID = GYRO_PID;
PID_HandleTypeDef Left_PID = LEFT_PID;
PID_HandleTypeDef Right_PID = RIGHT_PID;
PID_HandleTypeDef Turn_PID = TURN_PID;

/*----------------循迹-----------------*/
int Left_Scan_Point = 0;        // 扫描点模式下的左扫描点
int Right_Scan_Point = 0;       // 扫描点模式下的右扫描点
int Error = 0;
int Last_Error = 0;
int Track_Arr[15] = {0};
int Last_Track_Arr[15] = {0};
int Track_Num = 0;
int Last_Track_Num = 0;
int Stop_Flag = 0;
int Finish_Count = 0;
int Finish_Flag = 0;
uint8_t Left_Num = 0;       // 左侧点数量
uint8_t Right_Num = 0;      // 右侧点数量
uint8_t Left_Flag = 0;       // 左侧点数量
uint8_t Right_Flag = 0;      // 右侧点数量
int Middle = 0;
float Gyro_Integral = 0;
int8_t Dir_Arr[15] = {-27, -20, -14, -10, -6, -4, -2, 0, 2, 4, 6, 10, 14, 20, 27};

/*-------------赛道结构体--------------*/
Racing_track_Typedef Run_Track;     // 跑赛道结构体，键显选择
int8_t Execute_Times = 0;
int8_t Mileage_Times = 0;
uint8_t Line_Num_Count = 0;       // 有元器件的线的数量
uint8_t In_Line_Ele_Count = 0;    // 一段连线中元器件计数

/*----------------输出-----------------*/
float Turn_PID_Out = 0.0;
float Gyro_PID_Out = 0.0;
float Left_PID_Out = 0.0;
float Right_PID_Out = 0.0;
Run_Mode_Enum Run_Mode = Normal_Mode;
Run_Mode_Enum Last_Run_Mode = Normal_Mode;
Mileage_Stage_Enum Mileage_Stage = Normal_Stage;

// 计数的都放一起了
Count_Typedef Count =
{
    .Left = 0,
    .Right = 0,
    .Stop = 0,
    .Mileage = 0,
    .Straight = 0,
};

/*********************************函数定义*********************************/

/*************************************
** Function: Car_Go
** Description: Go~Go~Go~
** Others:
*************************************/
void Car_Go()
{
    Speed_Get_Count *= -1;

    Get_Light();

    if (Speed_Get_Count == 1)
    {
        Get_Speed();
    }
    Get_IMU();
    Get_Error();
    Set_Out();
}

/*************************************
** Function: Get_Speed
** Description: 测速
** Others:
*************************************/
void Get_Speed()
{
    // 获取编码器值
    giSpeed_Left[2] = giSpeed_Left[1];
    giSpeed_Left[1] = giSpeed_Left[0];
    giSpeed_Right[2] = giSpeed_Right[1];
    giSpeed_Right[1] = giSpeed_Right[0];

    giSpeed_Left[0] =  1 * encoder_get_count(TIM3_ENCODER) / 3;
    giSpeed_Right[0] = 1 * encoder_get_count(TIM2_ENCODER) / 3;

    Left_Real_Spd  = 0.5 * giSpeed_Left[0] + 0.3 * giSpeed_Left[1] + 0.2 * giSpeed_Left[2];
    Right_Real_Spd = 0.5 * giSpeed_Right[0] + 0.3 * giSpeed_Right[1] + 0.2 * giSpeed_Right[2];

    // 清空编码器值
    encoder_clear_count(TIM3_ENCODER);
    encoder_clear_count(TIM2_ENCODER);
}

/*************************************
** Function: Get_IMU
** Description: 陀螺仪
** Others:
*************************************/
void Get_IMU()
{
    icm20602_get_gyro();

    if (fabs(icm20602_gyro_z) < 20)
    {
        icm20602_gyro_z = 0;
    }

    // 积分∫
    if (Run_Mode == Turn_Left || Run_Mode == Turn_Right)
    {
        Gyro_Integral += icm20602_gyro_z * 1.0 / 100.0;
    }
}

/*************************************
** Function: Check_Edge
** Description: 检查边界
** Others:
*************************************/
uint8 Check_Edge()
{
    for (int i = 0; i < 15; i++)
    {
        if (Track_Arr[i] == 0 || Track_Arr[i] == 14)
        {
            return 1;
        }
    }

    return 0;
}

/*************************************
** Function: Light_Process
** Description: 处理光电管读值
** Others:
*************************************/
void Light_Process()
{
    memcpy(Last_Light_Convert, Light_Convert, sizeof(Light_Convert));  // 存历史值

    for (int i = 0; i < 15; i++)
    {
        if (Light_ADC[i] > Light_Thr[i][0])
        {
            Light_Convert[i] = 1;
            TCA9555_LED_Ctrl(LED[14 - i], 1);    // 亮灯
        }
        if (Light_ADC[i] < Light_Thr[i][1])
        {
            Light_Convert[i] = 0;
            TCA9555_LED_Ctrl(LED[14 - i], 0);    // 灭灯
        }
    }

    // 初始化变量
    memcpy(Last_Track_Arr, Track_Arr, sizeof(Track_Arr));
    for (int i = 0; i < 15; i++)
    {
        Track_Arr[i] = 0;
    }
    Track_Num = 0;
    Left_Num = 0;
    Right_Num = 0;

    for (int i = 0; i < 15; i++)
    {
        if (Light_Convert[i] == 1)
        {
            Track_Arr[Track_Num++] = i;
        }
    }

    // 检查点是否连续,不连续保持上次结果
    for (int i = 0; i < Track_Num - 1; i++)
    {
        if (Track_Arr[i + 1] - Track_Arr[i] > 1)
        {
            Track_Num = Last_Track_Num;
            memcpy(Track_Arr, Last_Track_Arr, sizeof(Last_Track_Arr));
        }
    }

    // 出赛道判断
    if (Track_Num == 15 || Track_Num == 0)
    {
        Count.Stop++;
    }
    else
    {
        Count.Stop = 0;
    }

    if (Finish_Flag == 1)
    {
        Finish_Count++;
    }

    if (Finish_Count > 50)
    {
        Stop_Flag = 1;
    }
}

/*************************************
** Function: Get_Error
** Description: 误差计算
** Others:
*************************************/
void Get_Error()
{
    // 数据处理
    Light_Process();
    if (First_Mode == 0)
    {
        Run_Mode = Mileage_Mode;
        First_Mode = 1;
        Mileage_Times = Run_Track.Node_Arr_Mileage_Num[Execute_Times];
    }
    Run_Mode = Normal_Mode;
    // 模式选择
    switch (Run_Mode)
    {
        case Normal_Mode:
                Normal_Run();
                break;/*Normal_Mode*/
        case Turn_Left:
                Turn_Left_Run();
                break;/*Turn_Left*/
        case Turn_Right:
                Turn_Right_Run();
                break;/*Turn_Right*/
        case Straight_Mode:
                Straight_Run();
                break;/*Staright*/
        case Mileage_Mode:
                Mileage_Mode_Run();
                break;/*Mileage_Mode*/
    }

    // 差速计算
    Set_Speed();
}

/*************************************
** Function: Normal_Run
** Description: 正常跑
** Others:
*************************************/
void Normal_Run()
{
    gpio_set_level(P33_4, 0);
    Last_Error = Error;

    if (Track_Num <= 2)
    {
        Error = 1.5 * Last_Error;
    }
    else if (Track_Num < 7 && Track_Num > 2) // 赛道宽度小于6，正常循迹
    {
        Left_Scan_Point = Track_Arr[0];                 // 左扫描点
        Right_Scan_Point = Track_Arr[Track_Num - 1];    // 右扫描点
        Error = (Dir_Arr[Left_Scan_Point] + Dir_Arr[Right_Scan_Point]) / 2;
    }
    else        // 异常，按规划跑
    {
        Error = (Dir_Arr[Left_Scan_Point] + Dir_Arr[Right_Scan_Point]) / 2;

        if (Check_Edge())
        {
            switch (Run_Track.Node_Arr_Dir[Execute_Times])
            {
                case 1:     // 左转
                    Run_Mode = Turn_Left;
                    /* code */
                    break;
                case 2:     // 右转
                    Run_Mode = Turn_Right;
                    /* code */
                    break;
                case 4:     // 直行
                    Run_Mode = Straight_Mode;
                    break;
            }
            Execute_Times = (Execute_Times + 1) % (Run_Track.Node_Num + 1);
            Mileage_Times = Run_Track.Node_Arr_Mileage_Num[Execute_Times];

            if (Run_Track.Stop_Mode == 0)
            {
                if (Execute_Times == Run_Track.Node_Num)
                {
                    Finish_Flag = 1;
                }
            }
        }
    }
}

/*************************************
** Function: Turn_Left_Run
** Description: 左直角
** Others:
*************************************/
void Turn_Left_Run()
{
    Error = -30;
    Middle = (Track_Arr[0] + Track_Arr[Track_Num - 1]) / 2;
    gpio_set_level(P33_4, 1);

    for (int i = 0; i < 15; i++)
    {
        if (Track_Arr[i] >= 7 && Track_Arr[i] != 0)
        {
            Right_Num++;
        }
    }

    if (Track_Num < 6 && Track_Num > 1 && Middle >= 0 && Middle <= 7 && Right_Num < 2/*&& Turn_Action == Straight_Stage*/)
    {
        Count.Left++;
    }
    else
    {
        Count.Left = 0;
    }

    if (Count.Left > 1 && Gyro_Integral < -2600)
    {
        Gyro_Integral = 0;
        Run_Mode = Mileage_Mode;
        Count.Left = 0;
    }
}

/*************************************
** Function: Turn_Right_Run
** Description: 右直角
** Others:
*************************************/
void Turn_Right_Run()
{
    Error = 30;
    Middle = (Track_Arr[0] + Track_Arr[Track_Num - 1]) / 2;
    gpio_set_level(P33_4, 1);
    for (int i = 0; i < 15; i++)
    {
        if (Track_Arr[i] <= 7 && Track_Arr[i] != 0)
        {
            Left_Num++;
        }
    }

    if (Track_Num < 6 && Track_Num > 1 && Middle >= 7 && Middle < 15 && Left_Num < 2/*&& Turn_Action == Straight_Stage*/)
    {
        Count.Right++;
    }
    else
    {
        Count.Right = 0;
    }

    if (Count.Right > 1 && Gyro_Integral > 2600)
    {
        Gyro_Integral = 0;
        Run_Mode = Mileage_Mode;
        Count.Right = 0;
    }
}

/*************************************
** Function: Mileage_Mode_Run
** Description: 里程计
** Others:
*************************************/
void Mileage_Mode_Run()
{
    if (Mileage_Times == 0)
    {
        Run_Mode = Normal_Mode;
        Mileage_Stage = Normal_Stage;
        In_Line_Ele_Count = 0;
    }
    else
    {
        // 里程计积分
        if (EnableSwitch_ON)
        {
            Last_Average_Speed = Average_Speed;
            Average_Speed = (Left_Real_Spd + Right_Real_Spd) / 2.0; // 平均速度
            Count.Mileage += (0.7 * Average_Speed + 0.3 * Last_Average_Speed); // 计算里程(32 mm)
        }
        if (Count.Mileage < Run_Track.Node_Arr_Mileage_Normal[Line_Num_Count][In_Line_Ele_Count])
        {
            // 正常循迹
            Mileage_Stage = Normal_Stage;
            Mileage_Run_Stage_1();
        }
        else if (Count.Mileage < (Run_Track.Node_Arr_Mileage_Normal[Line_Num_Count][In_Line_Ele_Count]
                                      + Run_Track.Node_Arr_Mileage_Element[Line_Num_Count][In_Line_Ele_Count]))
        {
            // 直走
            Mileage_Stage = Straight_Stage;
            Mileage_Run_Stage_2();
        }
        else
        {
            In_Line_Ele_Count++;
            Mileage_Stage = Normal_Stage;
            Count.Mileage = 0;
            if (In_Line_Ele_Count == Run_Track.Node_Arr_Mileage_Num[Execute_Times])
            {
                Run_Mode = Normal_Mode;
                Line_Num_Count++;
                In_Line_Ele_Count = 0;
            }

            if (Run_Track.Stop_Mode == 1)
            {
                if (Line_Num_Count == Run_Track.Element_Num - 1)
                {
                    Finish_Flag = 1;
                }
            }
        }
    }
}

/*************************************
** Function: Mileage_Run_Stage_1
** Description: 里程计(1)
** Others:
*************************************/
void Mileage_Run_Stage_1()
{
    gpio_set_level(P33_4, 0);
    if (Track_Num == 0)
    {
        Error = Last_Error;
    }
    else if (Track_Num < 6 && Track_Num > 0) // 赛道宽度小于6，正常循迹
    {
        Last_Error = Error;
        Left_Scan_Point = Track_Arr[0];                 // 左扫描点
        Right_Scan_Point = Track_Arr[Track_Num - 1];    // 右扫描点
        Error = (Dir_Arr[Left_Scan_Point] + Dir_Arr[Right_Scan_Point]) / 2;
    }
    else
    {
        Error = Last_Error;
    }
}

/*************************************
** Function: Mileage_Run_Stage_2
** Description: 里程计(2)
** Others:
*************************************/
void Mileage_Run_Stage_2()
{
    gpio_set_level(P33_4, 1);
    TCA9555_All_LED_On();

    switch (Run_Track.Node_Arr_Mileage_Dir[Line_Num_Count][In_Line_Ele_Count])
    {
        case 0:
            Error = 0;
            break;
        case 1:
            Error = -13;
            break;
        case 2:
            Error = 13;
            break;
    }
}

/*************************************
** Function: Set_Speed
** Description: 期望速度计算
** Others:
*************************************/
void Set_Speed()
{
    Left_PID_Out = 0;
    Right_PID_Out = 0;
    PID_cleardata(&Turn_PID);
    PID_cleardata(&Gyro_PID);

    Turn_PID_Out  = PID_calc(&Turn_PID, 0, (float)Error);

    Gyro_PID_Out = PID_calc(&Gyro_PID, 0, (-Turn_PID_Out) - icm20602_gyro_z);

    Left_Exp_Spd = Basic_Speed - Gyro_PID_Out;
    Right_Exp_Spd = Basic_Speed + Gyro_PID_Out;

    if (Run_Mode == Turn_Left || Run_Mode == Turn_Right)
    {
        Left_Exp_Spd = 20 - Gyro_PID_Out;
        Right_Exp_Spd = 20 + Gyro_PID_Out;
    }

    if (EnableSwitch_ON)
    {
        Left_PID_Out  = PID_calc(&Left_PID, (float)Left_Exp_Spd, (float)Left_Real_Spd);
        Right_PID_Out = PID_calc(&Right_PID, (float)Right_Exp_Spd, (float)Right_Real_Spd);
    }
}

/*************************************
** Function: Soft_Start
** Description: 缓启动
** Others:
*************************************/
void Soft_Start()
{
    // 缓启动
    if (Soft_Start_Flag == 0 && EnableSwitch_ON)
    {
        Soft_Start_Count++;
    }

    if (Soft_Start_Count % 10 == 0)
    {
        pwm_set_duty(Left_Motor_IN1, 0);
        pwm_set_duty(Left_Motor_IN2, 4 * Soft_Start_Count);

        pwm_set_duty(Right_Motor_IN1, 0);
        pwm_set_duty(Right_Motor_IN2, 4 * Soft_Start_Count);
    }

    if (Soft_Start_Count > 700)
    {
        Soft_Start_Flag = 1;
    }
}

/*************************************
** Function: Set_Out
** Description: 设置输出
** Others:
*************************************/
void Set_Out(void)
{
    if (Count.Stop > 100)
    {
        Stop_Flag = 1;
    }

    // 风扇
    pwm_set_duty(Suction_Motor_IN1, 0);
    pwm_set_duty(Suction_Motor_IN2, 10000);

    if (EnableSwitch_ON && Stop_Flag == 0) // 使能开关
    {
        if (Left_PID_Out == 0 || Stop_Flag == 1)
        {
            pwm_set_duty(Left_Motor_IN1, 10000);
            pwm_set_duty(Left_Motor_IN2, 10000);
        }
        else if (Left_PID_Out > 0)  // 正转
        {
            pwm_set_duty(Left_Motor_IN1, 0);
            pwm_set_duty(Left_Motor_IN2, (Left_PID_Out + 10000) / 2);
        }
        else                    // 反转
        {
            pwm_set_duty(Left_Motor_IN1, (fabs(Left_PID_Out) + 10000) / 2);
            pwm_set_duty(Left_Motor_IN2, 0);
        }

        if (Right_PID_Out == 0 || Stop_Flag == 1)
        {
            pwm_set_duty(Right_Motor_IN1, 10000);
            pwm_set_duty(Right_Motor_IN2, 10000);
        }
        else if (Right_PID_Out > 0) // 正转
        {
            pwm_set_duty(Right_Motor_IN1, 0);
            pwm_set_duty(Right_Motor_IN2, (Right_PID_Out + 10000) / 2);
        }
        else                    // 反转
        {
            pwm_set_duty(Right_Motor_IN1, (fabs(Right_PID_Out) + 10000) / 2);
            pwm_set_duty(Right_Motor_IN2, 0);
        }
    }
    else
    {
        pwm_set_duty(Left_Motor_IN1, 0);
        pwm_set_duty(Left_Motor_IN2, 0);
        pwm_set_duty(Right_Motor_IN1, 0);
        pwm_set_duty(Right_Motor_IN2, 0);

        PID_cleardata(&Left_PID);
        PID_cleardata(&Right_PID);
    }


}

/*************************************
** Function: Straight_Run
** Description: 直行
** Others:
*************************************/
void Straight_Run(void)
{
    Error = 0;
    Middle = (Track_Arr[0] + Track_Arr[Track_Num - 1]) / 2;

    if (Track_Num < 6 && Track_Num > 1 && Middle > 3 && Middle < 12)
    {
        Count.Straight++;
    }
    else
    {
        Count.Straight = 0;
    }

    if (Count.Straight > 5)
    {
        Run_Mode = Mileage_Mode;
        Count.Straight = 0;
    }
}
