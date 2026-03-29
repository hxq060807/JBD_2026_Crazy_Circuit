/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_driver_flash
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
* 2023-07-14       pudding            �޸������ע��
********************************************************************************************************************/

#ifndef _zf_driver_flash_h_
#define _zf_driver_flash_h_

#include "ifxFlash_cfg.h"
#include "zf_common_typedef.h"

#define EEPROM_BASE_ADDR             (IFXFLASH_DFLASH_START)
#define EEPROM_SIZE                  (IFXFLASH_DFLASH_SIZE)                     // һ����96KB
#define EEPROM_PAGE_SIZE             (EEPROM_SIZE / IFXFLASH_DFLASH_NUM_LOG_SECTORS)
#define EEPROM_PAGE_NUM              (IFXFLASH_DFLASH_NUM_LOG_SECTORS)          // 96KB��Ϊ��12ҳ

#define FLASH_DATA_SIZE              (IFXFLASH_DFLASH_PAGE_LENGTH)
#define EEPROM_PAGE_LENGTH           (EEPROM_PAGE_SIZE/FLASH_DATA_SIZE)         // ÿҳ���Դ�1024��uint32���͵�����


typedef union                                                                   // �̶������ݻ��嵥Ԫ��ʽ
{
    float   float_type;                                                         // float  ����
    uint32  uint32_type;                                                        // uint32 ����
    int32   int32_type;                                                         // int32  ����
    uint16  uint16_type;                                                        // uint16 ����
    int16   int16_type;                                                         // int16  ����
    uint8   uint8_type;                                                         // uint8  ����
    int8    int8_type;                                                          // int8   ����
}flash_data_union;                                                              // �����������ݹ���ͬһ�� 32bit ��ַ

// Flash ��Ӧ���ṹ��
typedef struct
{
    uint32 sector_num;       // ����编号（仅可填写0）
    uint32 page_num;         // ҳ编号（0-11）
    uint16 data_length;      // ���ݳ���
    uint32 *data_buffer;     // ���ݻ���
    uint8 erase_flag;        // 擦除标志
}flash_handle_t;

extern flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];

//====================================================FLASH ��������====================================================
uint8   flash_check                     (uint32 sector_num, uint32 page_num);                                   // У��FLASHҳ�Ƿ�������
void    flash_erase_page                (uint32 sector_num, uint32 page_num);                                   // ����ҳ
void    flash_read_page                 (uint32 sector_num, uint32 page_num, uint32 *buf, uint16 len);          // ��ȡһҳ
void    flash_write_page                (uint32 sector_num, uint32 page_num, const uint32 *buf, uint16 len);    // ���һҳ
void    flash_read_page_to_buffer       (uint32 sector_num, uint32 page_num);                                   // ��ָ�� FLASH ��ָ��ҳ���ȡ���ݵ�������
uint8   flash_write_page_from_buffer    (uint32 sector_num, uint32 page_num);                                   // ��ָ�� FLASH ��������ָ��ҳ��д�뻺����������
void    flash_buffer_clear              (void);                                                                 // ������ݻ�����

//====================================================FLASH ���ṹ�� ��������====================================================
void    flash_handle_init               (flash_handle_t *handle, uint32 sector_num, uint32 page_num, uint32 *buf, uint16 len); // ��ʼ��Flash���ṹ��
void    flash_handle_read               (flash_handle_t *handle);                                               // ��ȡFlash���
void    flash_handle_write              (flash_handle_t *handle);                                              // д��Flash���
void    flash_handle_erase              (flash_handle_t *handle);                                              // 擦除Flashҳ
uint8   flash_handle_check              (flash_handle_t *handle);                                              // У��Flashҳ�Ƿ�������
//====================================================FLASH ���ṹ�� ��������====================================================


#endif
