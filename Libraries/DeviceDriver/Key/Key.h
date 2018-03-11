/*******************************************************************************
* @file      : Key.h.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Thu Mar 08 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __KEY_H
#define __KEY_H



/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "TypeDef.h"



/*宏定义----------------------------------------------------------------------*/

/* 按键状态枚举 */
typedef enum
{
    UP_STATUS    = (u8)0x55,        //弹起状态
    DOWN_STATUS  = (u8)0xaa         //按下状态
}KEY_STATUS;                        //按键状态

/* 按键行为枚举 */
typedef enum
{
    UP_ACTION    = (u8)0x55,        //发生弹起
    Down_ACTION  = (u8)0xaa,        //发生按下
    NO_ACTION    = (u8)0x00         //无任何行为
}KEY_ACTION;                        //按键行为

/* 获取按键状态的函数指针 */
typedef KEY_STATUS (*KEY_FUN_GET)(void);

/* 按键结构体 */
typedef struct
{
    KEY_FUN_GET GetStatusFun;        //获取按键状态的函数
    KEY_STATUS  LastKeyStatus;       //上次按键状态
    KEY_STATUS  NowKeyStatus;        //当前按键状态
    u8          UpStatusCount;       //弹起状态计数器
    u8          DownStatusCount;     //按下状态计数器
    KEY_ACTION  KeyAction;           //按键行为
}KEY_PARA;


/*声明外部变量----------------------------------------------------------------*/

extern KEY_PARA DisplayKey;         //声明一个显示键结构体
///声明新的按键结构体...


/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数

/* 获取显示键状态：弹起状态或按下状态 （只针对瞬间，可能是抖动） */
extern KEY_STATUS GetStatus_DisplayKey(void);

/* 按键根据实际状态初始化函数 */
extern void Init_Key(KEY_PARA Key);

/* 按键定时查询并反馈行为函数，这个函数必须10~30ms左右运行一次 */
extern KEY_ACTION KeyGap(KEY_PARA Key);

#endif
/*end------------------------------------------------------------------------*/