/*******************************************************************************
* @file      : KEY.h
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
#ifndef __KEY_H
#define __KEY_H



/*头文件----------------------------------------------------------------------*/
///添加头文件




/*宏定义----------------------------------------------------------------------*/
///添加宏定义



/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void Check_DisplayKey(void);                 //定时检测显示按键,这个要定时运行，间隔时间在10~50ms之间，这个才能既避免误判，又能及时响应

#endif
/*end------------------------------------------------------------------------*/