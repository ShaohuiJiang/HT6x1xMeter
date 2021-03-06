/*******************************************************************************
* @file      : PowerCheck.h
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
#ifndef __POWERCHECK_H
#define __POWERCHECK_H



/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "TypeDef.h"



/*宏定义----------------------------------------------------------------------*/
///添加宏定义
typedef enum
{
    POWERON  = (u8)0x55,            //外部电源有电
    POWEROFF = (u8)0xaa             //外部电源无电
}Power;


/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern Power Check_Power(u8 times);             //外部电源检测：POWERON 上电状态 POWEROFF 掉电状态 ,判断次数times
extern Power Get_MeterWorkState(void);          //获取表计工作状态标志位，POWERON 上电状态 POWEROFF 掉电状态


#endif
/*end------------------------------------------------------------------------*/