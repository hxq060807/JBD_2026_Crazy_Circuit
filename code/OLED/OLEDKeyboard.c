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
#include "zf_driver_flash.h"

// 运行模式定义
#define SLOW_MODE       0   // 慢速记里程模式
#define FAST_MODE       1   // 快速跑成绩模式

/* =========== 变量定义 =========== */
uint32 input = 0;
uint32 Speed_OKb[1] = {0};
uint32 PID_OKb[8] = {0};
uint32 DBG_OKb[3] = {0};
uint32 Current_Track_Page = 3; // 默认从页3加载

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
    OLED_Show_Str(0, 6, "Let's do it!!!", TextSize_F6x8);
    KeyboardInput(88, 6, TextSize_F6x8, 1.0);
    OLED_CLS();

    // 选择赛道
    OLED_Show_Str(0, 0, "Choose Your Map!!!:", TextSize_F6x8);
    OLED_Show_Str(0, 2, "1: Pre 1", TextSize_F6x8);
    OLED_Show_Str(0, 3, "2: Pre 2", TextSize_F6x8);
    OLED_Show_Str(0, 4, "3: Pre 3", TextSize_F6x8);
    OLED_Show_Str(0, 5, "4: Final 1", TextSize_F6x8);
    OLED_Show_Str(0, 6, "5: Final 2", TextSize_F6x8);
    OLED_Show_Str(60, 2, "6: Final 3", TextSize_F6x8);
    uint8_t track_index = KeyboardInput(88, 0, TextSize_F6x8, 1.0);
    
    if (track_index < 1 || track_index > 6)
    {
        OLED_Show_Str(20, 2, "Invalid Track", TextSize_F6x8);
        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
        OLED_CLS();
    }
    
    // 计算Flash页号（从3开始）
    uint32 flash_page = 2 + track_index;
    // 更新当前赛道页面
    Current_Track_Page = flash_page;
    
    // 询问操作类型
    OLED_CLS();
    OLED_Show_Str(0, 0, "Track Operation:", TextSize_F6x8);
    OLED_Show_Str(0, 2, "1: New Input", TextSize_F6x8);
    OLED_Show_Str(0, 4, "2: Read Only", TextSize_F6x8);
    OLED_Show_Str(0, 6, "3: Modify", TextSize_F6x8);
    uint8_t operation = KeyboardInput(88, 0, TextSize_F6x8, 1.0);
    OLED_CLS();
    
    if (operation == 1)
    {
        // 重新输入参数
        // 初始化节点计数
        uint8_t node_count = 0;
        uint8_t quit_flag = 0;
        uint16_t input = 0;
        uint8_t current_node = 0;
        
        // 清空结构体
        memset(&Flash_Track, 0, sizeof(Run_Track_Typedef));
        
        while (quit_flag == 0)
        {
            // 节点管理界面
            OLED_Show_Str(0, 0, "Node Manager", TextSize_F6x8);
            OLED_Show_Str(0, 2, "Nodes:", TextSize_F6x8);
            OLED_Show_Numbers(40, 2, node_count, TextSize_F6x8);
            OLED_Show_Str(0, 4, "Current:", TextSize_F6x8);
            if (node_count > 0)
            {
                OLED_Show_Numbers(50, 4, current_node, TextSize_F6x8);
            }
            OLED_Show_Str(0, 6, "1Add 2Edit 3 <- 4 ->", TextSize_F6x8);
            
            input = KeyboardInput(88, 0, TextSize_F8x16, 1.0);
            OLED_CLS();
            
            switch (input)
            {
                case 1: // 添加新节点
                    if (node_count < NODE_NUM_MAX)
                    {
                        // 输入节点类型
                        OLED_Show_Str(0, 0, "Input The NodeType", TextSize_F6x8);
                        OLED_Show_Str(0, 2, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 2, node_count, TextSize_F6x8);
                        OLED_Show_Str(0, 4, "NodeType:", TextSize_F6x8);
                        input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                        
                        // 存储节点类型
                        Flash_Track.Exception_Type[node_count] = input;
                        
                        OLED_CLS();
                        OLED_Show_Str(0, 0, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 0, node_count, TextSize_F6x8);
                        
                        // 根据节点类型输入相应参数
                        switch (Flash_Track.Exception_Type[node_count])
                        {
                            case Turn_Node_Left:
                                OLED_Show_Str(0, 2, "Turn_Node_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[node_count] = Left;
                                break;
                                
                            case Turn_Node_Right:
                                OLED_Show_Str(0, 2, "Turn_Node_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[node_count] = Right;
                                break;
                                
                            case Straight_Node:
                                OLED_Show_Str(0, 2, "Straight_Node", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
//                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
//                                if (input != 0)
//                                {
//                                    Flash_Track.Node_Mileage[current_node][0] = input;
//                                }
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                Flash_Track.Node_Mileage[current_node][0] = 250;
                                OLED_CLS();
                                break;
                                
                            case Normal_Elem:
                                OLED_Show_Str(0, 2, "Normal_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Special_Elem:
                                OLED_Show_Str(0, 2, "Special_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }

                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Left:
                                OLED_Show_Str(0, 2, "Source_Elem_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }
                                
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Right:
                                OLED_Show_Str(0, 2, "Source_Elem_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }
                                
                                OLED_Show_Str(0, 4, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            default:
                                OLED_Show_Str(0, 0, "Invalid Type", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                break;
                        }
                        
                        // 节点计数加一
                        node_count++;
                        // 更新当前节点为新添加的节点
                        current_node = node_count - 1;
                    }
                    else
                    {
                        OLED_Show_Str(20, 2, "Node Limit Reached", TextSize_F6x8);
                        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
                        OLED_CLS();
                    }
                    break;
                    
                case 2: // 修改当前节点
                    if (node_count > 0)
                    {
                        // 输入节点类型
                        OLED_Show_Str(0, 0, "Edit Node", TextSize_F6x8);
                        OLED_Show_Str(0, 2, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 2, current_node, TextSize_F6x8);
                        OLED_Show_Str(0, 4, "NodeType:", TextSize_F6x8);
                        input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                        
                        // 存储节点类型
                        Flash_Track.Exception_Type[current_node] = input;
                        
                        OLED_CLS();
                        OLED_Show_Str(0, 0, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 0, current_node, TextSize_F6x8);
                        
                        // 根据节点类型输入相应参数
                        switch (Flash_Track.Exception_Type[current_node])
                        {
                            case Turn_Node_Left:
                                OLED_Show_Str(0, 2, "Turn_Node_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[current_node] = Left;
                                break;
                                
                            case Turn_Node_Right:
                                OLED_Show_Str(0, 2, "Turn_Node_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[current_node] = Right;
                                break;
                                
                            case Straight_Node:
                                OLED_Show_Str(0, 2, "Straight_Node", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
//                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
//                                if (input != 0)
//                                {
//                                    Flash_Track.Node_Mileage[current_node][0] = input;
//                                }
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                Flash_Track.Node_Mileage[current_node][0] = 250;
                                OLED_CLS();
                                break;
                                
                            case Normal_Elem:
                                OLED_Show_Str(0, 2, "Normal_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Special_Elem:
                                OLED_Show_Str(0, 2, "Special_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }

                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Left:
                                OLED_Show_Str(0, 2, "Source_Elem_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }
                                
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Right:
                                OLED_Show_Str(0, 2, "Source_Elem_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }
                                
                                OLED_Show_Str(0, 4, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            default:
                                OLED_Show_Str(0, 0, "Invalid Type", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                break;
                        }
                    }
                    else
                    {
                        OLED_Show_Str(20, 2, "No Nodes Yet", TextSize_F6x8);
                        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
                        OLED_CLS();
                    }
                    break;
                    
                case 3: // 上一个节点
                    if (node_count > 0)
                    {
                        if (current_node > 0)
                        {
                            current_node--;
                        }
                        else
                        {
                            current_node = node_count - 1;
                        }
                    }
                    break;
                    
                case 4: // 下一个节点
                    if (node_count > 0)
                    {
                        if (current_node < node_count - 1)
                        {
                            current_node++;
                        }
                        else
                        {
                            current_node = 0;
                        }
                    }
                    break;
                    
                case 9: // 退出
                    quit_flag = 1;
                    OLED_CLS();
                    break;
                    
                default:
                    break;
            }
        }

        // 存储节点数量
        Flash_Track.Node_Num = node_count;

        // 显示存储成功信息
        OLED_Show_Str(20, 2, "Save Success!", TextSize_F6x8);
        OLED_Show_Str(0, 4, "Nodes:", TextSize_F6x8);
        OLED_Show_Numbers(40, 4, node_count, TextSize_F6x8);
        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
        OLED_CLS();

        // 使用Flash句柄存储整个赛道参数结构体到一页Flash
        flash_handle_t flash_handle;
        
        // 计算结构体大小（以字节为单位）
        uint16 struct_size_bytes = sizeof(Run_Track_Typedef);
        // 计算需要的uint32数量
        uint16 struct_size_words = (struct_size_bytes + sizeof(uint32) - 1) / sizeof(uint32);
        
        // 创建临时缓冲区，避免结构体对齐问题
        uint32 temp_buffer[struct_size_words];
        
        // 将结构体复制到临时缓冲区
        memcpy(temp_buffer, &Flash_Track, struct_size_bytes);
        
        // 存储临时缓冲区到对应页
        flash_handle_init(&flash_handle, 0, flash_page, temp_buffer, struct_size_words);
        flash_handle_erase(&flash_handle);
        flash_handle_write(&flash_handle);
    }
    else if (operation == 2)
    {
        // 直接从Flash读取参数
        flash_handle_t flash_handle;
        
        // 计算结构体大小（以字节为单位）
        uint16 struct_size_bytes = sizeof(Run_Track_Typedef);
        // 计算需要的uint32数量
        uint16 struct_size_words = (struct_size_bytes + sizeof(uint32) - 1) / sizeof(uint32);
        
        // 创建临时缓冲区，避免结构体对齐问题
        uint32 temp_buffer[struct_size_words];
        
        // 从对应页加载数据到临时缓冲区
        flash_handle_init(&flash_handle, 0, flash_page, temp_buffer, struct_size_words);
        flash_handle_read(&flash_handle);
        
        // 将临时缓冲区复制到结构体
        memcpy(&Flash_Track, temp_buffer, struct_size_bytes);
        
        // 显示加载成功信息
        OLED_CLS();
        OLED_Show_Str(20, 2, "Load Success!", TextSize_F6x8);
        OLED_Show_Str(0, 4, "Nodes:", TextSize_F6x8);
        OLED_Show_Numbers(40, 4, Flash_Track.Node_Num, TextSize_F6x8);
        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
        OLED_CLS();
    }
    else if (operation == 3)
    {
        // 修改现有参数
        // 从Flash加载现有配置
        flash_handle_t flash_handle;
        
        // 计算结构体大小（以字节为单位）
        uint16 struct_size_bytes = sizeof(Run_Track_Typedef);
        // 计算需要的uint32数量
        uint16 struct_size_words = (struct_size_bytes + sizeof(uint32) - 1) / sizeof(uint32);
        
        // 创建临时缓冲区，避免结构体对齐问题
        uint32 temp_buffer[struct_size_words];
        
        // 从对应页加载数据到临时缓冲区
        flash_handle_init(&flash_handle, 0, flash_page, temp_buffer, struct_size_words);
        flash_handle_read(&flash_handle);
        
        // 将临时缓冲区复制到结构体
        memcpy(&Flash_Track, temp_buffer, struct_size_bytes);
        
        // 初始化节点计数和当前节点
        uint8_t node_count = Flash_Track.Node_Num;
        uint8_t quit_flag = 0;
        uint16_t input = 0;
        uint8_t current_node = 0;
        
        while (quit_flag == 0)
        {
            // 节点管理界面
            OLED_Show_Str(0, 0, "Node Manager", TextSize_F6x8);
            OLED_Show_Str(0, 2, "Nodes:", TextSize_F6x8);
            OLED_Show_Numbers(40, 2, node_count, TextSize_F6x8);
            OLED_Show_Str(0, 4, "Current:", TextSize_F6x8);
            if (node_count > 0)
            {
                OLED_Show_Numbers(50, 4, current_node, TextSize_F6x8);
            }
            OLED_Show_Str(0, 6, "1Add 2Edit 3 <- 4 ->", TextSize_F6x8);
            
            input = KeyboardInput(88, 0, TextSize_F8x16, 1.0);
            OLED_CLS();
            
            switch (input)
            {
                case 1: // 添加新节点
                    if (node_count < NODE_NUM_MAX)
                    {
                        // 输入节点类型
                        OLED_Show_Str(0, 0, "Input The NodeType", TextSize_F6x8);
                        OLED_Show_Str(0, 2, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 2, node_count, TextSize_F6x8);
                        OLED_Show_Str(0, 4, "NodeType:", TextSize_F6x8);
                        input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                        
                        // 存储节点类型
                        Flash_Track.Exception_Type[node_count] = input;
                        
                        OLED_CLS();
                        OLED_Show_Str(0, 0, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 0, node_count, TextSize_F6x8);
                        
                        // 根据节点类型输入相应参数
                        switch (Flash_Track.Exception_Type[node_count])
                        {
                            case Turn_Node_Left:
                                OLED_Show_Str(0, 2, "Turn_Node_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[node_count] = Left;
                                break;
                                
                            case Turn_Node_Right:
                                OLED_Show_Str(0, 2, "Turn_Node_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[node_count] = Right;
                                break;
                                
                            case Straight_Node:
                                OLED_Show_Str(0, 2, "Straight_Node", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
//                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
//                                if (input != 0)
//                                {
//                                    Flash_Track.Node_Mileage[current_node][0] = input;
//                                }
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                Flash_Track.Node_Mileage[current_node][0] = 250;
                                OLED_CLS();
                                break;
                                
                            case Normal_Elem:
                                OLED_Show_Str(0, 2, "Normal_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Special_Elem:
                                OLED_Show_Str(0, 2, "Special_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }

                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Left:
                                OLED_Show_Str(0, 2, "Source_Elem_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }
                                
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Right:
                                OLED_Show_Str(0, 2, "Source_Elem_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][0] = input;
                                }
                                
                                OLED_Show_Str(0, 4, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[node_count][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            default:
                                OLED_Show_Str(0, 0, "Invalid Type", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                break;
                        }
                        
                        // 节点计数加一
                        node_count++;
                        // 更新当前节点为新添加的节点
                        current_node = node_count - 1;
                    }
                    else
                    {
                        OLED_Show_Str(20, 2, "Node Limit Reached", TextSize_F6x8);
                        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
                        OLED_CLS();
                    }
                    break;
                    
                case 2: // 修改当前节点
                    if (node_count > 0)
                    {
                        // 输入节点类型
                        OLED_Show_Str(0, 0, "Edit Node", TextSize_F6x8);
                        OLED_Show_Str(0, 2, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 2, current_node, TextSize_F6x8);
                        OLED_Show_Str(0, 4, "NodeType:", TextSize_F6x8);
                        input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                        
                        // 存储节点类型
                        Flash_Track.Exception_Type[current_node] = input;
                        
                        OLED_CLS();
                        OLED_Show_Str(0, 0, "Node:", TextSize_F6x8);
                        OLED_Show_Numbers(30, 0, current_node, TextSize_F6x8);
                        
                        // 根据节点类型输入相应参数
                        switch (Flash_Track.Exception_Type[current_node])
                        {
                            case Turn_Node_Left:
                                OLED_Show_Str(0, 2, "Turn_Node_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[current_node] = Left;
                                break;
                                
                            case Turn_Node_Right:
                                OLED_Show_Str(0, 2, "Turn_Node_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                Flash_Track.Node_Turn_Dir[current_node] = Right;
                                break;
                                
                            case Straight_Node:
                                OLED_Show_Str(0, 2, "Straight_Node", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "No params needed", TextSize_F6x8);
//                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
//                                if (input != 0)
//                                {
//                                    Flash_Track.Node_Mileage[current_node][0] = input;
//                                }
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                Flash_Track.Node_Mileage[current_node][0] = 250;
                                OLED_CLS();
                                break;
                                
                            case Normal_Elem:
                                OLED_Show_Str(0, 2, "Normal_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Special_Elem:
                                OLED_Show_Str(0, 2, "Special_Elem", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }

                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Left:
                                OLED_Show_Str(0, 2, "Source_Elem_Left", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }
                                
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            case Source_Elem_Right:
                                OLED_Show_Str(0, 2, "Source_Elem_Right", TextSize_F6x8);
                                OLED_Show_Str(0, 4, "Mileage 1:", TextSize_F6x8);
                                OLED_Show_Str(0, 6, "Mileage 2", TextSize_F6x8);
                                input = KeyboardInput(90, 4, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][0] = input;
                                }
                                
                                OLED_Show_Str(0, 4, "Mileage 2:", TextSize_F6x8);
                                input = KeyboardInput(90, 6, TextSize_F6x8, 1.0);
                                if (input != 0)
                                {
                                    Flash_Track.Node_Mileage[current_node][1] = input;
                                }
                                OLED_CLS();
                                break;
                                
                            default:
                                OLED_Show_Str(0, 0, "Invalid Type", TextSize_F6x8);
                                KeyboardInput(88, 6, TextSize_F8x16, 1.0); // 按任意键继续
                                OLED_CLS();
                                break;
                        }
                    }
                    else
                    {
                        OLED_Show_Str(20, 2, "No Nodes Yet", TextSize_F6x8);
                        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
                        OLED_CLS();
                    }
                    break;
                    
                case 3: // 上一个节点
                    if (node_count > 0)
                    {
                        if (current_node > 0)
                        {
                            current_node--;
                        }
                        else
                        {
                            current_node = node_count - 1;
                        }
                    }
                    break;
                    
                case 4: // 下一个节点
                    if (node_count > 0)
                    {
                        if (current_node < node_count - 1)
                        {
                            current_node++;
                        }
                        else
                        {
                            current_node = 0;
                        }
                    }
                    break;
                    
                case 9: // 退出
                    quit_flag = 1;
                    OLED_CLS();
                    break;
                    
                default:
                    break;
            }
        }

        // 存储节点数量
        Flash_Track.Node_Num = node_count;

        // 显示存储成功信息
        OLED_Show_Str(20, 2, "Save Success!", TextSize_F6x8);
        OLED_Show_Str(0, 4, "Nodes:", TextSize_F6x8);
        OLED_Show_Numbers(40, 4, node_count, TextSize_F6x8);
        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
        OLED_CLS();

        // 使用Flash句柄存储整个赛道参数结构体到一页Flash
        flash_handle_t flash_handle_modify;
        
        // 创建临时缓冲区，避免结构体对齐问题
        uint32 temp_buffer_modify[struct_size_words];
        
        // 将结构体复制到临时缓冲区
        memcpy(temp_buffer_modify, &Flash_Track, struct_size_bytes);
        
        // 存储临时缓冲区到对应页
        flash_handle_init(&flash_handle_modify, 0, flash_page, temp_buffer_modify, struct_size_words);
        flash_handle_erase(&flash_handle_modify);
        flash_handle_write(&flash_handle_modify);
    }
    else
    {
        OLED_Show_Str(20, 2, "Invalid Choice", TextSize_F6x8);
        KeyboardInput(88, 6, TextSize_F8x16, 1.0);
        OLED_CLS();
    }

//    //======选择运行模式======
//    OLED_Show_Str(0, 0, "Run Mode:", TextSize_F6x8);
//    OLED_Show_Str(0, 2, "1: Slow Mode", TextSize_F6x8);
//    OLED_Show_Str(0, 4, "2: Fast Mode", TextSize_F6x8);
//    OLED_Show_Str(0, 6, "(Slow: Record Mileage)", TextSize_F6x8);
//    uint8_t run_mode = KeyboardInput(88, 0, TextSize_F6x8, 1.0);
//
//    if (run_mode == 1)
//    {
//        Run_Mode_Choice = SLOW_MODE;
//    }
//    else if (run_mode == 2)
//    {
//        Run_Mode_Choice = FAST_MODE;
//    }
//    else
//    {
//        Run_Mode_Choice = SLOW_MODE; // 默认为慢速记里程模式
//    }
//    OLED_CLS();
    
    Run_Mode_Choice = SLOW_MODE;

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
                    DBG_OKb[i] = input;
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
    flash_read_page(0, 2, &DBG_OKb, 3);

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
    
    // 加载里程数据
    Load_Mileage_From_Flash();
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
