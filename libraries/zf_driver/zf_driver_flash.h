/*********************************************************************************************************************
* TC264 Opensourec Library (TC264 开源库)是一套基于飞腾 SDK 接口的底层驱动源文件
* Copyright (c) 2022 SEEKFREE SeekFree
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 版权所有
* 该源码受 GNU General Public License (GNU 通用公共许可证)的保护
* 采用 GPL 协议的第3版（即 GPL3.0）的授权方式，任何个人/公司都可以自由使用/修改
*
* 本开源库的发布目的是希望能够有用，但不提供任何形式的保证
* 也不保证适用于任何特定用途；不提供任何担保
* 详见 GPL
*
* 您使用本开源库的同时也接受了 GPL 的条款
* 更多详情，请访问 <https://www.gnu.org/licenses/>
*
* 特别说明
* 本开源库使用 GPL3.0 开源协议，仅授权用于非商业版本
* 商业授权请查看 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件
* 开源协议详情请查看 libraries 文件夹下的 LICENSE 文件
* 欢迎各位使用并分享，但修改时请保留 SeekFree 的版权信息和此声明
*
* 文件名称          zf_driver_flash
* 公司名称          山东有人物联网股份有限公司
* 版本信息          查看 libraries/doc 文件夹下的 version 文件 版本说明
* 开发环境          ADS v1.10.2
* 目标平台          TC264D
* 销售网址          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
* 2023-07-14       pudding            修改函数注释
********************************************************************************************************************/

#ifndef _zf_driver_flash_h_
#define _zf_driver_flash_h_

#include "ifxFlash_cfg.h"
#include "zf_common_typedef.h"

#define EEPROM_BASE_ADDR             (IFXFLASH_DFLASH_START)
#define EEPROM_SIZE                  (IFXFLASH_DFLASH_SIZE)                     // 总容量96KB
#define EEPROM_PAGE_SIZE             (EEPROM_SIZE / IFXFLASH_DFLASH_NUM_LOG_SECTORS)
#define EEPROM_PAGE_NUM              (IFXFLASH_DFLASH_NUM_LOG_SECTORS)          // 96KB划分为12页

#define FLASH_DATA_SIZE              (IFXFLASH_DFLASH_PAGE_LENGTH)
#define EEPROM_PAGE_LENGTH           (EEPROM_PAGE_SIZE/FLASH_DATA_SIZE)         // 每页可以存1024个uint32类型的数据


typedef union                                                                   // 定义共用体数据转换元素格式
{
    float   float_type;                                                         // float  类型
    uint32  uint32_type;                                                        // uint32 类型
    int32   int32_type;                                                         // int32  类型
    uint16  uint16_type;                                                        // uint16 类型
    int16   int16_type;                                                         // int16  类型
    uint8   uint8_type;                                                         // uint8  类型
    int8    int8_type;                                                          // int8   类型
}flash_data_union;                                                              // 使不同类型数据占用同一 32bit 地址

extern flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];

//====================================================FLASH 操作函数====================================================
uint8   flash_check                     (uint32 sector_num, uint32 page_num);                                   // 检查FLASH页是否可操作
void    flash_erase_page                (uint32 sector_num, uint32 page_num);                                   // 擦除页
void    flash_read_page                 (uint32 sector_num, uint32 page_num, uint32 *buf, uint16 len);          // 读取一页
void    flash_write_page                (uint32 sector_num, uint32 page_num, const uint32 *buf, uint16 len);    // 写入一页
void    flash_read_page_to_buffer       (uint32 sector_num, uint32 page_num);                                   // 从指定 FLASH 的指定页读取数据到缓冲区
uint8   flash_write_page_from_buffer    (uint32 sector_num, uint32 page_num);                                   // 将指定 FLASH 缓冲区数据写入指定页，擦除缓冲区内数据
void    flash_buffer_clear              (void);                                                                 // 清空数据缓冲区
//====================================================FALSH 操作函数====================================================


#endif
