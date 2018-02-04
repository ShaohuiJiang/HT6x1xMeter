/*******************************************************************************
* @file      : LED.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sat Feb 03 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __xxx_H
#define __xxx_H



/*头文件----------------------------------------------------------------------*/
///添加头文件




/*宏定义----------------------------------------------------------------------*/
///添加宏定义
typedef enum
{
    Closed,         //关闭
    Open,           //打开
    Blink           //闪烁
}LEDSet;            //LED灯的操作方法


/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void Set_RelayLed(LEDSet method);                 //操作拉闸灯的函数
extern void Set_BackLed(LEDSet method);                  //操作背光灯的函数

/* 间隔驱动定时器函数，必须放在定时器中断中，间隔周期为10~30ms */
extern void RelayLedGap(void);                          //跳闸灯间隔驱动定时器函数
extern void BackLedGap(void);                           //背光灯间隔驱动定时器函数


#endif
/*end------------------------------------------------------------------------*/