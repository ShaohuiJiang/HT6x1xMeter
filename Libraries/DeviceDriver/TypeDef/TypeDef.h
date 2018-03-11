/*******************************************************************************
* @file      : TypeDef.h
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
#ifndef __TYPEDEF_H
#define __TYPEDEF_H



/*头文件----------------------------------------------------------------------*/
///添加头文件




/*宏定义----------------------------------------------------------------------*/
///添加宏定义
typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned long           u32;
typedef unsigned long long      u64;
typedef signed char             s8;
typedef signed short            s16;
typedef signed long             s32;
typedef signed long long        s64;


typedef	enum
{
    LOW_LEVEL   = (u8)0x55,    //低电平
    HIGH_LEVEL  = (u8)0xAA     //高电平
}LEVEL;                        //电平

typedef void    (*V_FUN_V)(void);
typedef void    (*V_FUN_U8)(u8);

#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000
/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数


#endif
/*end------------------------------------------------------------------------*/