/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
<<<<<<< HEAD
File name: Fun.c
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Fun.c
Others:      Êó†
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30   0.0        ÂàùÂßã
**************************************************/

#include "Fun.h"
/*********************************ÂèòÈáèÂÆö‰πâ*********************************/
uint8 icm20602_Check = 0;
uint16 Light_ADC[15] = {0};
float Current_Check = 0;
float Voltage_Check[2] = {0};

/* --------------- ÈòàÂÄº --------------- */
=======
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  Fun.c
Others:      Œﬁ
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30   0.0        ≥ı º
**************************************************/

#include "Fun.h"
/*********************************±‰¡ø∂®“Â*********************************/
uint8 icm20602_Check = 0;
uint16 Light_ADC[15] = {0};

/* --------------- –£◊º --------------- */
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
int Light_Raw_Min[15] = {4096, 4096, 4096, 4096, 4096, 4096, 4096,
                        4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
int Light_Raw_Max[15] = {0};
float  Light_Thr[15][2];

<<<<<<< HEAD
/*********************************ÂáΩÊï∞ÂÆö‰πâ*********************************/

/*************************************
** Function: Vofa_Send_Data
** Description: VofaÂèëÈÄÅ
=======
/*********************************∫Ø ˝∂®“Â*********************************/

/*************************************
** Function: Vofa_Send_Data
** Description: Vofaµ˜ ‘
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
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
<<<<<<< HEAD
    VOFA_data[2].floatdata = Next_Exception;
    VOFA_data[3].floatdata = Execute_Times;
    VOFA_data[4].floatdata = Error;
    VOFA_data[5].floatdata = Line_Num_Count;
    VOFA_data[6].floatdata = imu_Check;
    VOFA_data[7].floatdata = imu660rb_gyro_x;
    VOFA_data[8].floatdata = Gyro_Integral;
    VOFA_data[9].floatdata = imu660rb_gyro_z;
=======
    VOFA_data[2].floatdata = Count.Mileage;
    VOFA_data[3].floatdata = Run_Mode;
    VOFA_data[4].floatdata = Mileage_Times;
    VOFA_data[5].floatdata = Line_Num_Count;
    VOFA_data[6].floatdata = Mileage_Stage;
    VOFA_data[7].floatdata = Execute_Times;
    VOFA_data[8].floatdata = Finish_Flag;
    VOFA_data[9].floatdata = Finish_Count;
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

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
<<<<<<< HEAD
** Description: Â≠òÂÇ®Ê®°ÂùóÂèëÈÄÅ
=======
** Description: ¥Ê¥¢ƒ£øÈ∑¢ÀÕ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
** Others:
*************************************/
void Wit_Send_Data(void)
{
    uint8 data[4];
    floatu8data VOFA_data[20];
    memset(VOFA_data, 0, sizeof(VOFA_data));

    int i = 0;

<<<<<<< HEAD
    // ËµõÈÅìÂõæ
=======
    // ¥´ÕºœÒ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
    for (int i = 0; i < 15; i++)
    {
        VOFA_data[i].floatdata = Light_Convert[i];
    }

<<<<<<< HEAD
    // Ê®°Âºè‰ø°ÊÅØ
    VOFA_data[15].floatdata = Run_Mode;
    VOFA_data[16].floatdata = Execute_Times;
    VOFA_data[17].floatdata = Count.Mileage;

    // Â∏ßÂ§¥
    uart_write_byte(UART_0, 0x00);
    uart_write_byte(UART_0, 0x00);
    uart_write_byte(UART_0, 0x7f);
    uart_write_byte(UART_0, 0x80);

    // Êï∞ÊçÆÊÆµ
=======
    // ø…±‰≤Œ ˝
    VOFA_data[15].floatdata = Run_Mode;
    VOFA_data[16].floatdata = Error;
    VOFA_data[17].floatdata = 0;

    // ÷°Õ∑
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x7f);
    uart_write_byte(UART_3, 0x80);

    //  ˝æ›∂Œ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
    for(i = 0; i < 18; i++)
    {
        data[0] = VOFA_data[i].u8data[0];
        data[1] = VOFA_data[i].u8data[1];
        data[2] = VOFA_data[i].u8data[2];
        data[3] = VOFA_data[i].u8data[3];

<<<<<<< HEAD
        uart_write_byte(UART_0, data[0]);
        uart_write_byte(UART_0, data[1]);
        uart_write_byte(UART_0, data[2]);
        uart_write_byte(UART_0, data[3]);
    }

    // Â∏ßÂ∞æ
    uart_write_byte(UART_0, 0x00);
    uart_write_byte(UART_0, 0x00);
    uart_write_byte(UART_0, 0x80);
    uart_write_byte(UART_0, 0x7f);
=======
        uart_write_byte(UART_3, data[0]);
        uart_write_byte(UART_3, data[1]);
        uart_write_byte(UART_3, data[2]);
        uart_write_byte(UART_3, data[3]);
    }

    // ÷°Œ≤
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x00);
    uart_write_byte(UART_3, 0x80);
    uart_write_byte(UART_3, 0x7f);
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c

    return;
}

/*************************************
** Function: Light_Init
<<<<<<< HEAD
** Description: ÂÖâÁîµÁÆ°ÂàùÂßãÂåñ
=======
** Description: π‚µÁπ‹≥ı ºªØ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
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
<<<<<<< HEAD
** Description: ÁºñÁ†ÅÂô®ÂàùÂßãÂåñ
=======
** Description: ±‡¬Î∆˜≥ı ºªØ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
** Others:
*************************************/
void Encoder_Init()
{
    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P00_7, TIM2_ENCODER_CH2_P00_8);
    encoder_quad_init(TIM3_ENCODER, TIM3_ENCODER_CH1_P02_6, TIM3_ENCODER_CH2_P02_7);
}

/*************************************
** Function: Motor_Init
<<<<<<< HEAD
** Description: ÁîµÊú∫ÂàùÂßãÂåñ
=======
** Description: µÁª˙≥ı ºªØ
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
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
<<<<<<< HEAD
** Description: ÂÖ∂‰ªñÂàùÂßãÂåñ
** Others:ÂåÖÊã¨‰ΩøËÉΩÂºÄÂÖ≥,OLED
=======
** Description: ∆‰À˚≥ı ºªØ
** Others:∑‰√˘∆˜£¨Õ”¬›“«£¨ πƒ‹ø™πÿ,OLED
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
*************************************/
void Other_Init()
{
    OLED_Init();
<<<<<<< HEAD
    gpio_init(P33_4, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P00_2, GPI, 0, GPI_PULL_DOWN);
    gpio_init(P20_10, GPI, 0, GPI_FLOATING_IN);
    adc_init(ADC0_CH10_A10, ADC_12BIT);
    adc_init(ADC0_CH5_A5, ADC_12BIT);
=======
    icm20602_Check = icm20602_init();
    gpio_init(P33_4, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P00_2, GPI, 0, GPI_PULL_DOWN);
    gpio_init(P20_10, GPI, 0, GPI_FLOATING_IN);
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
}

/*************************************
** Function: Get_Light
<<<<<<< HEAD
** Description: Ëé∑ÂèñÂÖâÁîµÁÆ°ÂÄº
=======
** Description: ∂¡»°π‚µÁπ‹
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
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
<<<<<<< HEAD

    Current_Check = (adc_convert(ADC0_CH10_A10) * 3.3 / (4095 * 20 * 0.015));

    Voltage_Check[1] = Voltage_Check[0];
    Voltage_Check[0] = (adc_convert(ADC0_CH5_A5) * 3.3 * 11 / 4095.0) + 0.567;
    Voltage_Check[0] = 0.3 * Voltage_Check[1] + 0.7 * Voltage_Check[0];
=======
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
}

/*************************************
** Function: Get_Threshold
<<<<<<< HEAD
** Description: ÈòàÂÄºËÆ°ÁÆó
=======
** Description: „–÷µΩ√’˝
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
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
<<<<<<< HEAD
        float mid_Up   = (float)(Light_Raw_Max[ch] - Light_Raw_Min[ch]) * 3 / 5.0f + Light_Raw_Min[ch];
        float mid_Down = (float)(Light_Raw_Max[ch] - Light_Raw_Min[ch]) * 2 / 5.0f + Light_Raw_Min[ch];
=======
        float mid_Up   = (float)(Light_Raw_Max[ch] - Light_Raw_Min[ch]) * 7 / 9.0f + Light_Raw_Min[ch];
        float mid_Down = (float)(Light_Raw_Max[ch] - Light_Raw_Min[ch]) * 6 / 9.0f + Light_Raw_Min[ch];
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
        Light_Thr[ch][0] = mid_Up;
        Light_Thr[ch][1] = mid_Down;
    }
}
<<<<<<< HEAD

/*************************************
** Function: Map_Range
** Description: Êï∞ÂÄºÊò†Â∞ÑÂáΩÊï∞
** @param min1 ÁõÆÊ†áËåÉÂõ¥ÊúÄÂ∞èÂÄº
** @param max1 ÁõÆÊ†áËåÉÂõ¥ÊúÄÂ§ßÂÄº
** @param min2 ÂéüËåÉÂõ¥ÊúÄÂ∞èÂÄº
** @param max2 ÂéüËåÉÂõ¥ÊúÄÂ§ßÂÄº
** @param value Ë¶ÅÊò†Â∞ÑÁöÑÂÄº
*************************************/
int16_t Map_Range(int16_t min1, int16_t max1, int16_t min2, int16_t max2, int16_t value)
{
    // ÈôêÂπÖ
    value = Data_Limit(value, min2, max2);

    // Èò≤Ê≠¢Èô§Èõ∂
    if (max2 == min2) return min1;

    // Á∫øÊÄßÊò†Â∞ÑËÆ°ÁÆó
    float ratio = (float)(max2 - value - min2) / (float)(max2 - min2);
    int16_t mapped_value = min1 + (int16_t)(ratio * (max1 - min1));

    return mapped_value;
}

=======
>>>>>>> 447b0a910c21e5fa7d2029967042733f6456e64c
