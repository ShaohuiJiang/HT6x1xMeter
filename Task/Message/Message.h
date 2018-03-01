/*******************************************************************************
* @file      : Message.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Thu Mar 01 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __MESSAGE_H
#define __MESSAGE_H



/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "TypeDef.h"



/*宏定义----------------------------------------------------------------------*/
///添加宏定义
//任务ID定义（无优先级概念）----------------------------------------------------
typedef enum
{
    //定义名称为ID_TaskXXXX的任务
    ID_TaskDisplay,     //显示任务  (消息	MSG_SecStep/MSG_DispKey)
    //增加任务名称...
    ID_TaskNum          //任务数量,必须放在最后，才能确保任务数量正确
}ID_TASK;

typedef u8           MSG;               //根据消息数量而定

//消息定义----------------------------------------------------------------------
#define MSG_Null        (MSG)0x00u      //空消息
#define MSG_SecStep     (MSG)0x01u      //秒节拍消息
#define MSG_MinStep     (MSG)0x02u      //分节拍消息

#define	MSG_DispKey     (MSG)0x04u      //显示按键消息    （显示任务用）


#define Is_Message(x,y)     ((x&y) != MSG_Null)     //用于判断x消息是否等于y消息

/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void Init_Message(void);                             //初始化消息系统
extern void Post_Message(ID_TASK idTask, MSG msg);          //发送消息
extern MSG Accept_Message(ID_TASK idTask);                  //获取消息
extern void MessageSystemTick(void);                        //消息系统定时节拍



#endif
/*end------------------------------------------------------------------------*/