/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Fun.c
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30   0.0        初始
**************************************************/

#include "Fun.h"
/*********************************变量定义*********************************/
uint8 icm20602_Check = 0;
uint16 Light_ADC[15] = {0};

/* --------------- 校准 --------------- */
int Light_Raw_Min[15] = {4096, 4096, 4096, 4096, 4096, 4096, 4096,
                        4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
int Light_Raw_Max[15] = {0};
float  Light_Thr[15][2];

/*********************************函数定义*********************************/

/*************************************
** Function: Vofa_Send_Data
** Description: Vofa调试
** Others:
*************************************/
void Vofa_Send_Data(void)
{
    uint8 data[4];
    floatu8data VOFA_data[20];
    memset(VOFA_data, 0, sizeof(VOFA_data));

    int i = 0;

//    for (int i = 0; i < 15; i++)
//    {
//        VOFA_data[i].floatdata = Light_ADC[i];
//    }
    VOFA_data[0].floatdata = Left_Real_Spd;
    VOFA_data[1].floatdata = Right_Real_Spd;
    VOFA_data[2].floatdata = Count.Mileage;
    VOFA_data[3].floatdata = Run_Mode;
    VOFA_data[4].floatdata = Mileage_Times;
    VOFA_data[5].floatdata = Line_Num_Count;
    VOFA_data[6].floatdata = Mileage_Stage;
    VOFA_data[7].floatdata = Execute_Times;
    VOFA_data[8].floatdata = Finish_Flag;
    VOFA_data[9].floatdata = Finish_Count;

    for(i = 0; i < 10; i++)
    {
        data[0] = VOFA_data[i].u8data[0];
        data[1] = VOFA_data[i].u8data[1];
        data[2] = VOFA_data[i].u8data[2];
        data[3] = VOFA_data[i].u8data[3];

        uart_write_byte(UART_0, data[0]);
        uart_write_byte(UART_0, data[1]);
        uart_write_byte(UART_0, data[2]);
        uart_write_byte(UART_0, data[3]);
    }

    uart_write_byte(UART_0, 0x00);
    uart_write_byte(UART_0, 0x00);
    uart_write_byte(UART_0, 0x80);
    uart_write_byte(UART_0, 0x7f);

    return;
}

/*************************************
** Function: Wit_Send_Data
** Description: 存储模块发送
** Others:
*************************************/
void Wit_Send_Data(void)
{
    uint8 data[4];
    floatu8data VOFA_data[20];
    memset(VOFA_data, 0, sizeof(VOFA_data));

    int i = 0;

    // 传图像
    for (int i = 0; i < 15; i++)
    {
        VOFA_data[i].floatdata = Light_Convert[i];
    }

    // 可变参数
    VOFA_data[15].floatdata = Run_Mode;
    VOFA_data[16].floatdata = Error;
    VOFA_data[17].floatdata = 0;

    // 帧头
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x7f);
    uart_write_byte(UART_3, 0x80);

    // 数据段
    for(i = 0; i < 18; i++)
    {
        data[0] = VOFA_data[i].u8data[0];
        data[1] = VOFA_data[i].u8data[1];
        data[2] = VOFA_data[i].u8data[2];
        data[3] = VOFA_data[i].u8data[3];

        uart_write_byte(UART_3, data[0]);
        uart_write_byte(UART_3, data[1]);
        uart_write_byte(UART_3, data[2]);
        uart_write_byte(UART_3, data[3]);
    }

    // 帧尾
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x80);
    uart_write_byte(UART_3, 0x7f);

    return;
}

/*************************************
** Function: Light_Init
** Description: 光电管初始化
** Others:
*************************************/
void Light_Init()
{
    // 1~15
    adc_init(ADC2_CH14_A48, ADC_12BIT);
    adc_init(ADC2_CH12_A46, ADC_12BIT);
    adc_init(ADC2_CH10_A44, ADC_12BIT);
    adc_init(ADC2_CH6_A38, ADC_12BIT);
    adc_init(ADC2_CH4_A36, ADC_12BIT);
    adc_init(ADC1_CH9_A25, ADC_12BIT);
    adc_init(ADC1_CH5_A21, ADC_12BIT);
    adc_init(ADC1_CH1_A17, ADC_12BIT);
    adc_init(ADC0_CH13_A13, ADC_12BIT);
    adc_init(ADC0_CH11_A11, ADC_12BIT);
    adc_init(ADC0_CH8_A8, ADC_12BIT);
    adc_init(ADC0_CH6_A6, ADC_12BIT);
    adc_init(ADC0_CH4_A4, ADC_12BIT);
    adc_init(ADC0_CH2_A2, ADC_12BIT);
    adc_init(ADC0_CH0_A0, ADC_12BIT);
}

/*************************************
** Function: Encoder_Init
** Description: 编码器初始化
** Others:
*************************************/
void Encoder_Init()
{
    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P00_7, TIM2_ENCODER_CH2_P00_8);
    encoder_quad_init(TIM3_ENCODER, TIM3_ENCODER_CH1_P02_6, TIM3_ENCODER_CH2_P02_7);
}

/*************************************
** Function: Motor_Init
** Description: 电机初始化
** Others:
*************************************/
void Motor_Init()
{
    pwm_init(Left_Motor_IN1, 30000, 0);
    pwm_init(Left_Motor_IN2, 30000, 0);
    pwm_init(Right_Motor_IN1, 30000, 0);
    pwm_init(Right_Motor_IN2, 30000, 0);
    pwm_init(Suction_Motor_IN1, 70000, 0);
    pwm_init(Suction_Motor_IN2, 70000, 0);
}

/*************************************
** Function: Other_Init
** Description: 其他初始化
** Others:蜂鸣器，陀螺仪，使能开关,OLED
*************************************/
void Other_Init()
{
    OLED_Init();
    icm20602_Check = icm20602_init();
    gpio_init(P33_4, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P00_2, GPI, 0, GPI_PULL_DOWN);
    gpio_init(P20_10, GPI, 0, GPI_FLOATING_IN);
}

/*************************************
** Function: Get_Light
** Description: 读取光电管
** Others:
*************************************/
void Get_Light()
{
    Light_ADC[14]  = adc_convert(ADC2_CH14_A48);
    Light_ADC[13]  = adc_convert(ADC2_CH12_A46);
    Light_ADC[12]  = adc_convert(ADC2_CH10_A44);
    Light_ADC[11]  = adc_convert(ADC2_CH6_A38);
    Light_ADC[10]  = adc_convert(ADC2_CH4_A36);
    Light_ADC[9]  = adc_convert(ADC1_CH9_A25);
    Light_ADC[8]  = adc_convert(ADC1_CH5_A21);
    Light_ADC[7]  = adc_convert(ADC1_CH1_A17);
    Light_ADC[6]  = adc_convert(ADC0_CH13_A13);
    Light_ADC[5]  = adc_convert(ADC0_CH11_A11);
    Light_ADC[4] = adc_convert(ADC0_CH8_A8);
    Light_ADC[3] = adc_convert(ADC0_CH6_A6);
    Light_ADC[2] = adc_convert(ADC0_CH4_A4);
    Light_ADC[1] = adc_convert(ADC0_CH2_A2);
    Light_ADC[0] = adc_convert(ADC0_CH0_A0);
}

/*************************************
** Function: Get_Threshold
** Description: 阈值矫正
** Others:
*************************************/
void Get_Threshold()
{
    Get_Light();

    for (uint8_t ch = 0; ch < 15; ch++)
    {
        if (Light_ADC[ch] > Light_Raw_Max[ch] && Light_ADC[ch] < 5000) Light_Raw_Max[ch] = Light_ADC[ch];
        if (Light_ADC[ch] < Light_Raw_Min[ch] && Light_ADC[ch] > 0) Light_Raw_Min[ch] = Light_ADC[ch];
    }

    for (uint8_t ch = 0; ch < 15; ch++)
    {
        uint16_t span = Light_Raw_Max[ch] - Light_Raw_Min[ch];
        if (span == 0) span = 1;
        float mid_Up   = (float)(Light_Raw_Max[ch] - Light_Raw_Min[ch]) * 7 / 9.0f + Light_Raw_Min[ch];
        float mid_Down = (float)(Light_Raw_Max[ch] - Light_Raw_Min[ch]) * 6 / 9.0f + Light_Raw_Min[ch];
        Light_Thr[ch][0] = mid_Up;
        Light_Thr[ch][1] = mid_Down;
    }
}
