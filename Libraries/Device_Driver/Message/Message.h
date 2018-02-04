/*******************************************************************************
* @file      : Message.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Mon Jan 29 2018
* @brief     : 
********************************************************************************
* @attention : 该头文件要被专门放中断函数的C文件包含，因为节拍定时器函数要放到中断函数中
*              这个相当于底层调用了上层文件
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __MESSAGE_H
#define __MESSAGE_H



/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "ht6xxx.h"
#include "TypeDef.h"


/*宏定义----------------------------------------------------------------------*/
///添加宏定义
#define C_TaskTick      64             //任务定时节拍频率（64Hz）
//任务ID定义（无优先级概念）
typedef	enum
{
    ID_TaskDisplay,     //显示任务      (消息 MSG_SecStep/MSG_DispKey/MSG_DispWake/MSG_FrameIn/MSG_FrameOut/MSG_DispKey)
    //添加各种任务...
    ID_TaskNum          //任务数量      放在最后，就正好是所有任务数量和
}ID_TASK;

typedef unsigned char   MSG;            //将消息这种数据格式定为一个字节

/* 对于消息值的定义，是按bit位来定义的，所以不能出现0x03、0x05、0x07等这种多bit位置1的值 */
/* 只有一个bit位代表一种消息的好处，是可以让一个任务的多个消息同时存在，目前的缺点是由于是一个字节，除去定时节拍消息，一个任务只剩余6个消息 */
/*定时节拍消息*/
#define MSG_Null        (MSG)0x00      //空消息
#define MSG_SecStep     (MSG)0x01      //秒节拍消息 
#define MSG_MinStep     (MSG)0x02      //分节拍消息 


/*显示任务(TaskDispaly)消息（除了定时节拍消息）*/
#define MSG_DispKey     (MSG)0x04      //显示按键消息（显示任务用）


/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void Init_Message(void);                     //初始化消息系统
extern void Stop_Message(void);                     //停止消息系统
extern void Post_Message(ID_TASK idTask, MSG msg);  //发送消息
extern MSG Accept_Message(ID_TASK idTask);          //获取消息
extern BOOL Is_Message(MSG x,MSG y);                //确认消息类型
extern void Task_Systick(void);                     //任务系统节拍定时器中断
#endif
/*end------------------------------------------------------------------------*/