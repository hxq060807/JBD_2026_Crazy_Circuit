/*************************************************
Copyright (C), 2016-2024, TYUT JBD .
File name: OLEDKeyboard.h
Author: TYUT JBD
Version:2.0               Date: 2024.12.07
Description:  OLEDKeyboard.c
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
JBD       2016.10.21  0.0        初始
AmaZzzing 2016.11.12  1.0        初步完成构架
SUV       2024.12.07  2.0        基于新库重构
**************************************************/

#include "OLEDKeyboard.h"
#include "Ctrl.h"

/* =========== 变量定义 =========== */
uint32 input = 0;
uint32 Speed_OKb[1] = {0};
uint32 PID_OKb[8] = {0};
uint32 DBG_OKb[3] = {0};

/* ========= 内部函数声明 ========== */

/* =========== 函数定义 ============ */
/*************************************
** Function: OLED_Input
** Description: 开机显示
** Others:无
*************************************/
void OLED_Input(void)
{
    CH455_Init(); //IIC通信

    int32 OLED_Choose;

    OLED_Show_Str(20, 3, "Nothing or Best.", TextSize_F6x8);
    OLED_Choose = KeyboardInput(88, 6, TextSize_F8x16, 1.0);

    //======先选赛道======
    switch (OLED_Choose)
    {
        case 1:   // 预赛---串
            Run_Track = Pre_Contest_1;
            break;
        case 2:   // 预赛---并
            Run_Track = Pre_Contest_2;
            break;
        case 3:   // 预赛---串+并
            Run_Track = Pre_Contest_3;
            break;
        case 4:   // 决赛---串
            Run_Track = Final_Contest_1;
            break;
        case 5:   // 决赛---并
            Run_Track = Final_Contest_2;
            break;
        case 6:   // 决赛---串+并
            Run_Track = Final_Contest_3;
            break;
        default:  // 默认预赛串
            Run_Track = Pre_Contest_1;
    }
    OLED_CLS();

    //======选择速度======
    flash_read_page(0, 0, Speed_OKb, 1);
    OLED_Show_Str(0, 0, "B_Spd", TextSize_F6x8);       OLED_Show_Numbers(47, 0, Speed_OKb[0], TextSize_F6x8);

    input = KeyboardInput(90, 0, TextSize_F6x8, 1.0);
    if (input != 0)
    {
        Speed_OKb[0] = input;
    }
    OLED_CLS();
    // 擦
    flash_erase_page(0, 0);
    // 存
    flash_write_page(0, 0, Speed_OKb, 1);

    // 其他参数
    OLED_Choose = KeyboardInput(88, 6, TextSize_F8x16, 1.0);
    OLED_CLS();

    //======调参======
    switch (OLED_Choose)
    {
        case 1: // PID
        {
            flash_read_page(0, 1, PID_OKb, 8);

            // 轮PID
            OLED_Show_Str(0, 0, "L_P", TextSize_F6x8);       OLED_Show_Numbers(47, 0, PID_OKb[0], TextSize_F6x8);
            OLED_Show_Str(0, 2, "L_I", TextSize_F6x8);       OLED_Show_Numbers(47, 2, PID_OKb[1], TextSize_F6x8);
            OLED_Show_Str(0, 4, "R_P", TextSize_F6x8);       OLED_Show_Numbers(47, 4, PID_OKb[2], TextSize_F6x8);
            OLED_Show_Str(0, 6, "R_I", TextSize_F6x8);       OLED_Show_Numbers(47, 6, PID_OKb[3], TextSize_F6x8);

            for(uint16 i = 0; i < 4; i++)
            {
                input = KeyboardInput(90, 2*i, TextSize_F6x8, 1.0);
                if (input != 0)
                {
                    PID_OKb[i] = input;
                }
            }
            OLED_CLS();

            // 方向PID
            OLED_Show_Str(0, 0, "T_P", TextSize_F6x8);       OLED_Show_Numbers(47, 0, PID_OKb[4], TextSize_F6x8);
            OLED_Show_Str(0, 2, "T_D", TextSize_F6x8);       OLED_Show_Numbers(47, 2, PID_OKb[5], TextSize_F6x8);
            OLED_Show_Str(0, 4, "G_P", TextSize_F6x8);       OLED_Show_Numbers(47, 4, PID_OKb[6], TextSize_F6x8);
            OLED_Show_Str(0, 6, "G_D", TextSize_F6x8);       OLED_Show_Numbers(47, 6, PID_OKb[7], TextSize_F6x8);

            for(uint16 i = 4; i < 6; i++)
            {
                input = KeyboardInput(90, 2*i, TextSize_F6x8, 1.0);
                if (input != 0)
                {
                    PID_OKb[i] = input;
                }
            }
            for(uint16 i = 6; i < 8; i++)
            {
                input = KeyboardInput(90, 2*i, TextSize_F6x8, 1.0);
                if (input != 0)
                {
                    PID_OKb[i] = input;
                }
            }
            OLED_CLS();
            // 擦
            flash_erase_page(0, 1);
            // 存
            flash_write_page(0, 1, PID_OKb, 8);
        }break;/*OLED_INPUT---PID*/

        case 2: // 调试
        {
            flash_read_page(0, 2, DBG_OKb, 3);

            // 轮PID
            OLED_Show_Str(0, 0, "Exp_G", TextSize_F6x8);       OLED_Show_Numbers(47, 0, DBG_OKb[0], TextSize_F6x8);
            OLED_Show_Str(0, 2, "Exp_L", TextSize_F6x8);       OLED_Show_Numbers(47, 2, DBG_OKb[1], TextSize_F6x8);
            OLED_Show_Str(0, 4, "Exp_R", TextSize_F6x8);       OLED_Show_Numbers(47, 4, DBG_OKb[2], TextSize_F6x8);

            for(uint16 i = 0; i < 3; i++)
            {
                input = KeyboardInput(90, 4*i, TextSize_F6x8, 1.0);
                if (input != 0)
                {
                    PID_OKb[i] = input;
                }
            }
            OLED_CLS();
            // 擦
            flash_erase_page(0, 2);
            // 存
            flash_write_page(0, 2, DBG_OKb, 3);
        }break;/*OLED_INPUT---DEBUG*/
    }
}

/*************************************
** Function: OLED_Data_Load
** Description: OLED参数加载
** Others:无
*************************************/
void OLED_Data_Load()
{
    // PID参数
    flash_read_page(0, 1, &PID_OKb, 8);
    Left_PID.kp = PID_OKb[0];
    Left_PID.ki = PID_OKb[1] * 0.01;
    Right_PID.kp = PID_OKb[2];
    Right_PID.ki = PID_OKb[3] * 0.01;
    Turn_PID.kp = PID_OKb[4] * 0.01;
    Turn_PID.kd = PID_OKb[5] * 0.01;
    Gyro_PID.kp = PID_OKb[6] * 0.001;
    Gyro_PID.kd = PID_OKb[7] * 0.001;

    // 基础速度
    flash_read_page(0, 0, &Speed_OKb, 1);
    Basic_Speed = Speed_OKb[0];

    // 调试参数
    flash_read_page(0, 3, &DBG_OKb, 1);

}

/*************************************
** Function: OLED_Display
** Description: OLED参数显示
** Others:无
*************************************/
void OLED_Display(void)
{
    OLED_Show_Str(20, 0, "Nothing or Best.", TextSize_F6x8);

    OLED_Show_Str(0, 2, "L_Spd", TextSize_F6x8);    OLED_Show_Numbers(77, 2, Left_Exp_Spd, TextSize_F6x8);
    OLED_Show_Str(0, 4, "R_Spd", TextSize_F6x8);    OLED_Show_Numbers(77, 4, Right_Exp_Spd, TextSize_F6x8);
    OLED_Show_Str(0, 6, "Err", TextSize_F6x8);    OLED_Show_Numbers(77, 6, Error, TextSize_F6x8);
}

