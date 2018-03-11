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
#include "TypeDef.h"
/*宏定义----------------------------------------------------------------------*/
///添加宏定义

/* 灯状态枚举 */
typedef enum
{
    ON_STATUS    = (u8)0x55,      //点亮状态
    OFF_STATUS   = (u8)0x66,      //熄灭状态
    BLINK_STATUS = (u8)0x77       //闪烁状态
}LED_STATUS;                        

/* 灯操作方法枚举 */
typedef enum
{
    TURN_ON_SET  = (u8)0x55,     //打开
    TURN_OFF_SET = (u8)0x66,     //关闭
    BLINK_SET    = (u8)0x77      //闪烁
}LED_SET;

/* 秒数据类型定义为2字节 */
typedef u16 SEC;

/* 指向LED驱动函数的指针 */
typedef void    (*V_FUN_SET)(LED_SET);

/* 灯参数结构体 */
typedef struct
{
    V_FUN_SET  LedSetFun;                  //操作LED对应的IO口；操作被封装，或者被LedGap函数根据LedStatus而执行相应操作
    LED_SET    LedStatus;                  //LED的几种状态，当状态被改变时，LedGap函数会根据状态执行相应的操作
    u16        LedStatusFlipTimer;         //灯状态翻转时间；在闪烁状态，这个变量就是用来维持亮或者灭的持续时间参数之一；
    u16        Ledcount;                   //灯状态计数器
}LEDPara;

/*声明外部变量----------------------------------------------------------------*/
extern LEDPara RelayLed;           //声明一个跳闸灯结构体

/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数


/* 跳闸灯操作函数 */
extern void Init_RelayLed(void);                //每次上电未进入循环前调用，根据按键的实际状态进行初始化
extern void RelayLed_TurnOn(void);              //点亮跳闸灯
extern void RelayLed_TurnOff(void);             //熄灭跳闸灯
extern void RelayLed_Blink(SEC seconds);        //seconds是点亮时间和熄灭时间，点亮时间=熄灭时间 seconds范围：1~60
extern LED_SET Get_RelayLedStatus(void);        //获取跳闸灯状态


/* 定时运行的LED驱动函数，建议定时周期15ms */
extern void LedGap(LEDPara Led);

#endif
/*end------------------------------------------------------------------------*/