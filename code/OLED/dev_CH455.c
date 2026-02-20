/*************************************************
Copyright (C), 2016-2024, TYUT JBD  .
File name: CH455.c
Author: TYUT JBD
Version:1.0               Date: 2016.11.12
Description:    CH455
Others:      无
Function List:    1.CH450_Write
                  2.CH450_Read
History:
<author>  <time>      <version > <desc>
JBD       2016.10.21  0.0      初始
AmaZzzing 2016.11.12  1.0        初步完成构架
**************************************************/
#include "dev_CH455.h"
#include "dev_ssd1306.h"

void CH455_Init(void)
{
    JBD_simiic_Start();
    JBD_simiic_Write_OneByte(0x48);
    JBD_simiic_Wait_Ask();
    JBD_simiic_Write_OneByte(0x01);
    JBD_simiic_Wait_Ask();
    JBD_simiic_Stop();
}

unsigned char CH455_Read(void)
{
    unsigned char keycode;
    JBD_simiic_Start();
    JBD_simiic_Write_OneByte(0x4f);
    JBD_simiic_Wait_Ask();
    keycode = JBD_simiic_Read_OneByte(1);
    JBD_simiic_Stop();
    return(keycode);
}

/*************************************************
Function: CH455_GetOneKey
Description:从CH455获取一个按键抬起事件    （等待式）
Details：进入函数后一直获取CH455寄存器的值，直到检测到一个有效的按键抬起事件
*************************************************/
KeyValue_enum CH455_GetOneKey(void)
{
    uint8  KeyCodeOld = KEY_BLANK;
    uint8  KeyCode = CH455_Read();
    uint16 KeyValue = KEY_BLANK;  //短按时记录在低4位 长按记录在高4位
    uint16 timeout = 15000;

    CH455_Init();

    do{
        KeyCodeOld = CH455_Read();
      }while(KeyCodeOld < 0x40); /*等待按键按下*/

    while(KeyValue == KEY_BLANK && timeout > 0)
    {
        KeyCode = CH455_Read();

        if(KeyCodeOld - KeyCode == 0x40)    //检测到按键抬起
        {
            switch(KeyCode)
            {
                case 0x17 :     KeyValue = KEY_1_;       break;
                case 0x0f :     KeyValue = KEY_2_;       break;
                case 0x07 :     KeyValue = KEY_3_;       break;
                case 0x16 :     KeyValue = KEY_4_;       break;
                case 0x0e :     KeyValue = KEY_5;       break;
                case 0x06 :     KeyValue = KEY_6;       break;
                case 0x15 :     KeyValue = KEY_7;       break;
                case 0x0d :     KeyValue = KEY_8;       break;
                case 0x05 :     KeyValue = KEY_9;       break;
                case 0x0c :     KeyValue = KEY_0;       break;
                case 0x14 :     KeyValue = KEY_BACK;    break;   //退格键
                case 0x04 :     KeyValue = KEY_ENTER;   break;   //确认键
                default:                                break;
            }
        }

        timeout--;
    }

    if(KeyValue != KEY_BLANK && timeout <= 100)
    {
        KeyValue <<= 4;
        KeyValue |= 0x0F;
    }

    if(KeyValue == KEY_BLANK && timeout == 0)  //由于超时跳出循环
    {
        /*根据当前按下的键返回数值*/
        switch(KeyCode)
        {
            case 0x57 :     KeyValue = KEY_1_Long;       break;
            case 0x4f :     KeyValue = KEY_2_Long;       break;
            case 0x47 :     KeyValue = KEY_3_Long;       break;
            case 0x56 :     KeyValue = KEY_4_Long;       break;
            case 0x4e :     KeyValue = KEY_5_Long;       break;
            case 0x46 :     KeyValue = KEY_6_Long;       break;
            case 0x55 :     KeyValue = KEY_7_Long;       break;
            case 0x4d :     KeyValue = KEY_8_Long;       break;
            case 0x45 :     KeyValue = KEY_9_Long;       break;
            case 0x4c :     KeyValue = KEY_0_Long;       break;
            case 0x54 :     KeyValue = KEY_BACK_Long;    break;   //退格键
            case 0x44 :     KeyValue = KEY_ENTER_Long;   break;   //确认键
            default:                                     break;
        }
    }

    return KeyValue;
}

/*************************************************
Function: CH455_QueryOneKey
Description:从CH455获取一个按键事件    （轮询式）
Details：进入函数获取一次CH455寄存器的值，计数检测到一个有效的按键事件
*************************************************/
KeyValue_enum CH455_QueryOneKey(uint32 timeout)
{
    // 实现思路 每次进此函数则与CH455通信一次，并记录通信得到的按键代码，得到按键代码后与历史按键代码做对比，检测抬起动作

    static uint8 ValuableKeyCode = KEY_BLANK;
    static uint8 LastKeyCode = KEY_BLANK;
    static uint16 Count = 0;

    uint8 ThisKeyCode = CH455_Read();


    if(KEY_BLANK == ValuableKeyCode)    // 当前没有有效按键
    {
        if(ThisKeyCode <= 0x40)
        {
            return KEY_BLANK;
        }
        else
        {
            switch(ThisKeyCode)
            {
//                case 0x17 :     ValuableKeyCode = KEY_1;       break;
//                case 0x0f :     ValuableKeyCode = KEY_2;       break;
//                case 0x07 :     ValuableKeyCode = KEY_3;       break;
//                case 0x16 :     ValuableKeyCode = KEY_4;       break;
//                case 0x0e :     ValuableKeyCode = KEY_5;       break;
//                case 0x06 :     ValuableKeyCode = KEY_6;       break;
//                case 0x15 :     ValuableKeyCode = KEY_7;       break;
//                case 0x0d :     ValuableKeyCode = KEY_8;       break;
//                case 0x05 :     ValuableKeyCode = KEY_9;       break;
//                case 0x0c :     ValuableKeyCode = KEY_0;       break;
//                case 0x14 :     ValuableKeyCode = KEY_BACK;    break;   //退格键
//                case 0x04 :     ValuableKeyCode = KEY_ENTER;   break;   //确认键
//                default:        ValuableKeyCode = KEY_BLANK;   break;

                case 0x57 :     ValuableKeyCode = KEY_1_;       break;
                case 0x4f :     ValuableKeyCode = KEY_2_;       break;
                case 0x47 :     ValuableKeyCode = KEY_3_;       break;
                case 0x56 :     ValuableKeyCode = KEY_4_;       break;
                case 0x4e :     ValuableKeyCode = KEY_5;       break;
                case 0x46 :     ValuableKeyCode = KEY_6;       break;
                case 0x55 :     ValuableKeyCode = KEY_7;       break;
                case 0x4d :     ValuableKeyCode = KEY_8;       break;
                case 0x45 :     ValuableKeyCode = KEY_9;       break;
                case 0x4c :     ValuableKeyCode = KEY_0;       break;
                case 0x54 :     ValuableKeyCode = KEY_BACK;    break;   //退格键
                case 0x44 :     ValuableKeyCode = KEY_ENTER;   break;   //确认键
                default:        ValuableKeyCode = KEY_BLANK;   break;
            }
            Count = 0;

            LastKeyCode = ThisKeyCode;
            return 0xFF - ValuableKeyCode;
        }
    }/* KEY_BLANK == ValuableKeyCode */
    else
    {
        if(ThisKeyCode < 0x40 && LastKeyCode - ThisKeyCode == 0x40)  // 本次查询按键抬起
        {
            uint8 temp;

//            if(Count < 3)         // 根据时长判断是否为误判
//            {
//                temp = KEY_BLANK;
//            }
//            else
            if(Count > timeout * 0.95)  // 根据时长判断是否为长按
            {
                temp = (ValuableKeyCode << 4) | 0x0F;     // 转换为长按
            }
            else
            {
                temp = ValuableKeyCode;
            }

            Count = 0;
            LastKeyCode = KEY_BLANK;
            ValuableKeyCode = KEY_BLANK;

            return temp;
        }
        else    // 本次查询按键还在按下状态
        {
            uint8 temp;
            Count++;
            LastKeyCode = ThisKeyCode;

            if(Count > timeout)
            {
                temp = (ValuableKeyCode << 4) | 0x0F;     // 转换为长按
//                Count = 0;
                LastKeyCode = ThisKeyCode;
            }
            else
            {
                temp = 0xFF - ValuableKeyCode;
            }

            return temp;

        }/* ThisKeyCode >= 0x40 */


    }/* KEY_BLANK != ValuableKeyCode */

}

