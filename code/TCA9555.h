/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: OLEDKeyboard.h
Author: TEAM  A B C
Version:0.0               Date: 2026.1.27
Description:  TCA9555.h
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.27   0.0        初始
**************************************************/
/*
* 芯片型号：TCA9555
* 核心功能：16位I²C至并行端口扩展器，用于为微控制器提供通用远程I/O扩展。
*/
#ifndef __TCA9555_H
#define __TCA9555_H

#include "zf_common_headfile.h"
#include "headfiles.h"

/***********************************宏定义***********************************/
#define TCA9555_BASE_ADDR      0x20  // 设备写地址（A0 A1 A2均为低）
#define TCA9555_REG_INPUT_P0   0x00  // 输入端口0
#define TCA9555_REG_INPUT_P1   0x01  // 输入端口1
#define TCA9555_REG_OUTPUT_P0  0x02  // 输出端口0
#define TCA9555_REG_OUTPUT_P1  0x03  // 输出端口1
#define TCA9555_REG_POLARITY_P0 0x04  // 极性反转端口0
#define TCA9555_REG_POLARITY_P1 0x05  // 极性反转端口1
#define TCA9555_REG_CONFIG_P0  0x06  // 配置端口0
#define TCA9555_REG_CONFIG_P1  0x07  // 配置端口1

typedef enum {
    // --- Port 0 (P00 - P07) ---
    LED_0,
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_6,
    LED_7,

    // --- Port 1 (P10 - P17) ---
    LED_8,
    LED_9,
    LED_10,
    LED_11,
    LED_12,
    LED_13,
    LED_14,
    LED_15,

    LED_ALL  // 特殊值，可用于全亮/全灭操作

} TCA9555_LED_t;

/*********************************全局变量声明*********************************/
extern TCA9555_LED_t LED[16];

/***********************************函数声明***********************************/
extern void TCA9555_Init();
extern void TCA9555_LED_Ctrl(TCA9555_LED_t pin, int state);
extern uint8_t TCA9555_Read_Input(uint8_t port);
extern void TCA9555_Set_Polarity(uint8_t port, uint8_t polarity_mask);
extern void TCA9555_All_LED_On(void);
extern void TCA9555_All_LED_Off(void);

#endif
