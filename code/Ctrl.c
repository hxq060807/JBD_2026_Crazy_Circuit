/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Ctrl.c
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description: Ctrl.c
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30    0.0        初始
Cross_Z   2026.3.25    1.0        规则突变，更换思路
**************************************************/

#include "Ctrl.h"
#include "zf_driver_flash.h"

/*********************************变量定义*********************************/
/*---------------速度-----------------*/
int16 giSpeed_Right[3] = {0}; // 右测速
int16 giSpeed_Left[3] = {0};  // 左测速
int Left_Real_Spd = 0;      // 左实际
int Right_Real_Spd = 0;     // 右实际
int Left_Exp_Spd = 0;       // 左期望
int Right_Exp_Spd = 0;      // 右期望
int Basic_Speed = 0;
int Run_Speed = 0;
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
float Gyro_Z[2] = {0};
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
int8_t Dir_Arr[15] = {-30, -20, -16, -12, -10, -7, -3, 0, 3, 7, 10, 12, 16, 20, 30};

/*-------------赛道结构体--------------*/
Run_Track_Typedef Run_Track;     // 跑赛道结构体，键显选择
Run_Track_Typedef Flash_Track;     // 修改赛道结构体，用于修改赛道参数
Next_Exception_Enum Next_Exception;
int8_t Execute_Times = 0;
int8_t Mileage_Times = 0;
uint8_t Line_Num_Count = 0;       // 有元器件的线的数量
uint8_t Straight_Num_Count = 0;   // 直道计数
uint8_t In_Line_Ele_Count = 0;    // 一段连线中元器件计数
int Lap_Count = 0;           // 圈数计数
uint8_t Mileage_Record_Flag = 1; // 里程记录标志，初始为1，开始记录
uint8_t Run_Mode_Choice = SLOW_MODE; // 运行模式选择，默认为慢速记里程模式

/*----------------输出-----------------*/
float Turn_PID_Out = 0.0;
float Gyro_PID_Out = 0.0;
float Left_PID_Out = 0.0;
float Right_PID_Out = 0.0;
Run_Mode_Enum Run_Mode = Normal_Mode;
Run_Mode_Enum Last_Run_Mode = Normal_Mode;

uint8_t Turn_Flag = 0;
uint8_t Straight_Flag = 0;

// 计数的都放一起了
Count_Typedef Count =
{
    .Left = 0,
    .Right = 0,
    .Stop = 0,
    .Mileage = 0,
    .Straight = 0,
    .Spd_Mileage = 0,
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
    Light_Process();

    if (Speed_Get_Count == 1)
    {
        Get_Speed();
    }
    Get_IMU();

    Exception_Judge();
    Mode_Update();
    Mode_Choose();

    Set_Speed();
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
    giSpeed_Right[0] = -1 * encoder_get_count(TIM2_ENCODER) / 3;

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
    imu660rb_get_gyro();

    if (fabs(imu660rb_gyro_z) < 20)
    {
        imu660rb_gyro_z = 0;
    }

    Gyro_Z[1] = Gyro_Z[0];
    Gyro_Z[0] = imu660rb_gyro_z / 1000.0;
    Gyro_Z[0] = 0.7 * Gyro_Z[0] + 0.3 * Gyro_Z[1];

    // 积分∫
    if (Run_Mode == Turn_Left || Run_Mode == Turn_Right)
    {
        Gyro_Integral += imu660rb_gyro_z * 1.0 / 100.0;
    }
}

/*************************************
** Function: Check_Edge
** Description: 检查边界
** Others:
*************************************/
uint8_t Check_Edge()
{
    for (int i = 0; i < 15; i++)
    {
        if (Track_Arr[i] == 0 || Track_Arr[i] == 14)
        {
            return 1;
        }
    }
    if (Track_Num > 10)
    {
        return 1;
    }

    return 0;
}

/*************************************
** Function: Light_Process
** Description: 处理光电管读值
** Others: 得到Track_Arr和Track_Num
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
}

/*************************************
** Function: Exception_Judge
** Description: 异常预判
** Others: 
*************************************/
void Exception_Judge()
{
    Next_Exception = Run_Track.Exception_Type[Execute_Times];
}

/*************************************
** Function: Mode_Update
** Description: 模式更新
** Others: 
*************************************/
Run_Mode_Enum Mode_Update()
{
    switch (Next_Exception)
    {
        case Turn_Node_Left:
            Run_Mode = Turn_Left;
            break;
        case Turn_Node_Right:
            Run_Mode = Turn_Right;
            break;
        case Straight_Node:
        case Normal_Elem:
            Run_Mode = Straight_Mode;
            break;
        case Special_Elem:
        case Source_Elem_Left:
        case Source_Elem_Right:
            Run_Mode = Mileage_Mode;
            break;
    }
}

/*************************************
** Function: Mode_Choose
** Description: 模式选择
** Others: 
*************************************/
void Mode_Choose()
{
    switch (Run_Mode)
    {
        case Turn_Left:
            Turn_Left_Run();
            break;
        case Turn_Right:
            Turn_Right_Run();
            break;
        case Straight_Mode:
            Straight_Run();
            break;
        case Mileage_Mode:
            Mileage_Run();
            break;
    }
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

    if (Track_Num < 2)
    {
        Error = (Last_Error / fabs(Last_Error));
    }
    else if (Track_Num < 7 && Track_Num >= 2) // 赛道宽度小于6，正常循迹
    {
        Left_Scan_Point = Track_Arr[0];                 // 左扫描点
        Right_Scan_Point = Track_Arr[Track_Num - 1];    // 右扫描点
        Error = (Dir_Arr[Left_Scan_Point] + Dir_Arr[Right_Scan_Point]) / 2;
    }
    else
    {
         // 异常，按规划跑
        Error = (Dir_Arr[Left_Scan_Point] + Dir_Arr[Right_Scan_Point]) / 2;

        if (Check_Edge())
        {
            if (Next_Exception == Straight_Node || Next_Exception == Normal_Elem)
            {
                Straight_Flag = 1;
            }
            else if (Next_Exception == Turn_Node_Left || Next_Exception == Turn_Node_Right)
            {
                Turn_Flag = 1;
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
    if (Turn_Flag == 0)
    {
        Normal_Run();
    }
    else
    {
        Error = -35;
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

        if (Count.Left > 2 && Gyro_Integral > 2600)
        {
            Gyro_Integral = 0;
            Count.Left = 0;
            Turn_Flag = 0;
            
            Execute_Times = (Execute_Times + 1) % Run_Track.Node_Num;
        }
    }
}

/*************************************
** Function: Turn_Right_Run
** Description: 右直角
** Others:
*************************************/
void Turn_Right_Run()
{
    if (Turn_Flag == 0)
    {
        Normal_Run();
    }
    else
    {
        Error = 35;
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

        if (Count.Right > 2 && Gyro_Integral < -2600)
        {
            Gyro_Integral = 0;
            Count.Right = 0;
            Turn_Flag = 0;
            
            Execute_Times = (Execute_Times + 1) % Run_Track.Node_Num;
        }
    }
}

/*************************************
** Function: Mileage_Mode_Run
** Description: 里程计
** Others:
*************************************/
void Mileage_Run()
{
    // 里程计积分
    if (EnableSwitch_ON)
    {
        Last_Average_Speed = Average_Speed;
        Average_Speed = (Left_Real_Spd + Right_Real_Spd) / 2.0;             // 平均速度
        Count.Mileage += (0.7 * Average_Speed + 0.3 * Last_Average_Speed);  // 累加里程
    }
    
    if (Count.Mileage < Run_Track.Node_Mileage[Execute_Times][0])
    {
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
    else if (Count.Mileage < Run_Track.Node_Mileage[Execute_Times][0] + Run_Track.Node_Mileage[Execute_Times][1])
    {
        switch (Next_Exception)
        {
            case Special_Elem:
                Error = 0;
                break;
            case Source_Elem_Left:
                Error = -18;
                break;
            case Source_Elem_Right:
                Error = 18;
                break;
        }
    }
    else
    {
        Count.Mileage = 0;
        
        Execute_Times = (Execute_Times + 1) % Run_Track.Node_Num;
    }
}

/*************************************
** Function: Straight_Run
** Description: 直行
** Others:
*************************************/
void Straight_Run(void)
{
    if (Straight_Flag == 0)
    {
        gpio_set_level(P33_4, 1);
        Normal_Run();
    }
    else
    {
        // 里程计积分
        gpio_set_level(P33_4, 1);
        if (EnableSwitch_ON)
        {
            Last_Average_Speed = Average_Speed;
            Average_Speed = (Left_Real_Spd + Right_Real_Spd) / 2.0;             // 平均速度
            Count.Mileage += (0.7 * Average_Speed + 0.3 * Last_Average_Speed);  // 累加里程
        }

        if (Count.Mileage < Run_Track.Node_Mileage[Execute_Times][0])
        {
            Error = 0;
        }
        else
        {
            // 记录里程
            if (Mileage_Record_Flag)
            {
                Run_Track.Node_Mileage[Execute_Times][0] = Count.Mileage;
            }
            Count.Mileage = 0;
            Straight_Flag = 0;
            
            Execute_Times = (Execute_Times + 1) % Run_Track.Node_Num;
            
            // 检查是否完成一圈
            if (Execute_Times == 0)
            {
                Lap_Count++;
                if (Run_Mode_Choice == SLOW_MODE && Lap_Count == 1)
                {
                    Save_Mileage_To_Flash(); // 将里程数据存储到Flash
                }
            }
        }
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

    Turn_PID_Out  = PID_calc(&Turn_PID, 0, (float)(Error / 100.0));

    Gyro_PID_Out = PID_calc(&Gyro_PID, 0, (-Turn_PID_Out) + Gyro_Z[0]);

    // 动态速度调整
    if (EnableSwitch_ON)
    {
        Last_Average_Speed = Average_Speed;
        Average_Speed = (Left_Real_Spd + Right_Real_Spd) / 2.0;             // 平均速度
        Count.Spd_Mileage += (0.7 * Average_Speed + 0.3 * Last_Average_Speed);  // 累加里程
    }

//    Run_Track.Speed_Adjust.Range_Target[0] = Basic_Speed;
//    Run_Track.Speed_Adjust.Range_Target[1] = 50;
//    Run_Track.Speed_Adjust.Range_Source[0] = 0;
//    Run_Track.Speed_Adjust.Range_Source[1] = 1000;Run_Track.Node_Mileage_ALL[Straight_Num_Count];
//
//    Run_Speed = Map_Range(Run_Track.Speed_Adjust.Range_Target[0],
//                          Run_Track.Speed_Adjust.Range_Target[1],
//                          Run_Track.Speed_Adjust.Range_Source[0],
//                          Run_Track.Speed_Adjust.Range_Source[1],
//                          Count.Spd_Mileage);
    // 根据运行模式调整速度
    int current_speed = Basic_Speed;
    
    if (Run_Mode_Choice == SLOW_MODE) // 慢速记里程模式
    {
        // 始终使用Basic_Speed作为速度
        current_speed = Basic_Speed;
        // 确保Mileage_Record_Flag为1，开始记录里程
        Mileage_Record_Flag = 1;
    }
    else if (Run_Mode_Choice == FAST_MODE) // 快速跑成绩模式
    {
        // 正常行驶时使用最高速度
        current_speed = MAX_SPEED;
        // 确保Mileage_Record_Flag为0，使用之前存储的里程数据
        Mileage_Record_Flag = 0;
        
        // 提前减速逻辑
        // 检查是否接近转弯节点
        if ((Next_Exception == Turn_Node_Left || Next_Exception == Turn_Node_Right) && Count.Mileage > Run_Track.Node_Mileage[Execute_Times][0] * 0.7)
        {
            // 当接近转弯节点时，从最高速度逐渐减速到Basic_Speed
            float deceleration_ratio = 1.0 - (Count.Mileage - Run_Track.Node_Mileage[Execute_Times][0] * 0.7) / (Run_Track.Node_Mileage[Execute_Times][0] * 0.3);
            current_speed = MAX_SPEED * deceleration_ratio + Basic_Speed * (1 - deceleration_ratio);
            // 确保速度不会低于Basic_Speed
            if (current_speed < Basic_Speed)
            {
                current_speed = Basic_Speed;
            }
        }
    }
    
    Left_Exp_Spd = current_speed - Gyro_PID_Out;
    Right_Exp_Spd = current_speed + Gyro_PID_Out;

    if ((Run_Mode == Turn_Left || Run_Mode == Turn_Right) && Turn_Flag == 1)
    {
        Left_Exp_Spd = 25 - Gyro_PID_Out;
        Right_Exp_Spd = 25 + Gyro_PID_Out;
    }

    if (EnableSwitch_ON)
    {
        Left_PID_Out  = PID_calc(&Left_PID, (float)Left_Exp_Spd, (float)Left_Real_Spd);
        Right_PID_Out = PID_calc(&Right_PID, (float)Right_Exp_Spd, (float)Right_Real_Spd);
    }
}

/*************************************
** Function: Soft_Start
** Description: 缓启动(暂时没用)
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
    if (Stop_Flag == 0)
    {
        pwm_set_duty(Suction_Motor_IN1, 0);
        pwm_set_duty(Suction_Motor_IN2, 7000);
    }
    else
    {
        pwm_set_duty(Suction_Motor_IN1, 0);
        pwm_set_duty(Suction_Motor_IN2, 0);
    }

    if (EnableSwitch_ON && Stop_Flag == 0) // 使能开关
    {
        if (Left_PID_Out == 0)
        {
            pwm_set_duty(Left_Motor_IN1, 10000);
            pwm_set_duty(Left_Motor_IN2, 10000);
        }
        else if (Left_PID_Out > 0)  // 正转
        {
            pwm_set_duty(Left_Motor_IN1, (Left_PID_Out + 10000) / 2);
            pwm_set_duty(Left_Motor_IN2, 0);
        }
        else                    // 反转
        {
            pwm_set_duty(Left_Motor_IN1, 0);
            pwm_set_duty(Left_Motor_IN2, (fabs(Left_PID_Out) + 10000) / 2);
        }

        if (Right_PID_Out == 0)
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

//    pwm_set_duty(Left_Motor_IN1, 7000);
//    pwm_set_duty(Left_Motor_IN2, 8000);
}

/*************************************
** Function: Save_Mileage_To_Flash
** Description: 将里程数据存储到Flash
** Others:
*************************************/
void Save_Mileage_To_Flash(void)
{
    // 使用Flash句柄存储整个赛道参数结构体到一页Flash
    flash_handle_t flash_handle;
    
    // 计算结构体大小（以字节为单位）
    uint16 struct_size_bytes = sizeof(Run_Track_Typedef);
    // 计算需要的uint32数量
    uint16 struct_size_words = (struct_size_bytes + sizeof(uint32) - 1) / sizeof(uint32);
    
    // 创建临时缓冲区，避免结构体对齐问题
    uint32 temp_buffer[struct_size_words];
    
    // 将结构体复制到临时缓冲区
    memcpy(temp_buffer, &Run_Track, struct_size_bytes);
    
    // 存储临时缓冲区到当前赛道页面
    flash_handle_init(&flash_handle, 0, Current_Track_Page, temp_buffer, struct_size_words);
    flash_handle_erase(&flash_handle);
    flash_handle_write(&flash_handle);
}

/*************************************
** Function: Load_Mileage_From_Flash
** Description: 从Flash中读取里程数据
** Others:
*************************************/
void Load_Mileage_From_Flash(void)
{
    // 使用Flash句柄从一页Flash加载整个赛道参数结构体
    flash_handle_t flash_handle;
    
    // 计算结构体大小（以字节为单位）
    uint16 struct_size_bytes = sizeof(Run_Track_Typedef);
    // 计算需要的uint32数量
    uint16 struct_size_words = (struct_size_bytes + sizeof(uint32) - 1) / sizeof(uint32);
    
    // 创建临时缓冲区，避免结构体对齐问题
    uint32 temp_buffer[struct_size_words];
    
    // 从当前赛道页面加载数据到临时缓冲区
    flash_handle_init(&flash_handle, 0, Current_Track_Page, temp_buffer, struct_size_words);
    flash_handle_read(&flash_handle);
    
    // 将临时缓冲区复制到结构体
    memcpy(&Run_Track, temp_buffer, struct_size_bytes);
    
    // 检查数据有效性
    if (Run_Track.Node_Num > 0)
    {
        // 数据有效，设置Mileage_Record_Flag为0，避免重复记录
        Mileage_Record_Flag = 0;
    }
}
