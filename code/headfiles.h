/*************************************************
Copyright (C), 2016-2026, TYUT JBD TEAM C.
File name: Fun.h
Author: Cross_Z
Version:0.0               Date: 2026.1.30
Description:  headfiles.h
Others:      无
Function List:
History:
<author>  <time>      <version > <desc>
Cross_Z   2026.1.30   0.0        初始
**************************************************/
#ifndef __HEADFILES_H
#define __HEADFILES_H

/***********************************功能宏定义***********************************/
#define Amplitude_Limit(Value,Dowm,Up)  ((Value) < (Dowm) ? (Dowm) : ((Value) > (Up) ? (Up) : (Value)));
#define Step_Limit(New,Old,Range)       (New > (Old + Range) ? (Old + Range) : (New < (Old - Range) ? (Old - Range) : (New)))
#define ABS(x)                          (((x) >= (0.0f)) ? (x) : (-(x)))
#define Data_Limit(Value,Dowm,Up)       ((Value) < (Dowm) ? (Dowm) : ((Value) > (Up) ? (Up) : (Value)))
#define Limited_Add(Value,MAX)          ((++Value) > MAX ? MAX : Value)
#define SignOf(Value)                   ((Value < 0.0) ? (-1.0) : (1.0))
#define Increase_Limit(var, limit)      (var = (++var) > limit ? (limit+1) : var)

#define PI                              (3.1415926535897932384626433832795f)

/***********************************头文件包含***********************************/
#include "zf_common_headfile.h"
#include "pid.h"
#include "Fun.h"
#include "TCA9555.h"
#include "OLEDKeyboard.h"
#include "Ctrl.h"



#endif
