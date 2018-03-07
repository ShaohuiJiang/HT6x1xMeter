/*******************************************************************************
* @file      : Led.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Feb 04 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __LED_H
#define __LED_H



/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "MCUConfig.h"

/*宏定义----------------------------------------------------------------------*/
///添加宏定义
typedef u16 SEC;
/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数

/* 跳闸灯操作函数 */
extern void RelayLed_Open(void);
extern void RelayLed_Closed(void);
extern void RelayLed_Blink(SEC seconds);        //seconds是点亮时间和熄灭时间，点亮时间=熄灭时间 seconds范围：1~60
/* 背光灯操作函数 */
extern void BackLed_Open(void);
extern void BackLed_Closed(void);

/* 定时运行的LED驱动函数，建议定时周期15ms */
extern void LedGap(void);
#endif
/*end------------------------------------------------------------------------*/