/*******************************************************************************
* @file      : Message.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Thu Mar 01 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "Message.h"
#include "Led.h"        //需要定时驱动led
#include "MCUConfig.h"  //需要定时器的频率值


/*宏定义----------------------------------------------------------------------*/
///添加宏定义




/*内部变量声明----------------------------------------------------------------*/
///添加内部变量
static volatile MSG MessageQueue[ID_TaskNum];               //任务控制块（任务消息）
static volatile u8 TaskTick;                             //任务级节拍定时器

/*内部变量初始化--------------------------------------------------------------*/
///添加内部变量

/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数







/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数





/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/** 
 * @brief  初始化消息系统函数
 * @note   清空消息队列
 * @retval None
 */
extern void Init_Message(void)
{
    u8 i;

    for(i=0; i<ID_TaskNum; i++)     //清空所有任务的消息队列
    {
        MessageQueue[i] = MSG_Null;
    }
    TaskTick = 0;                   //任务级节拍
}

/** 
 * @brief  发送消息函数
 * @note   
 * @param  idTask: 任务 
 * @param  msg:   消息类型
 * @retval None
 */
extern void Post_Message(ID_TASK idTask, MSG msg)
{
    if(idTask < ID_TaskNum)
    {
        MessageQueue[idTask] |= msg;
    }
}

/** 
 * @brief  获取消息函数
 * @note   获取消息后，消息队列中这个消息清除
 * @param  idTask: 任务
 * @retval 返回对应任务队列中的消息
 */
extern MSG Accept_Message(ID_TASK idTask)
{
    MSG	msg;

    if (idTask < ID_TaskNum)
    {
        msg = MessageQueue[idTask];
        MessageQueue[idTask] -= msg;       //清除该消息队列
        return msg;
    }
    else
    {
        return MSG_Null;
    }
}

/** 
 * @brief  消息系统定时节拍函数
 * @note   该函数要放在定时器中断中，定时器中断默认频率是64Hz（15ms）
 * @retval None
 */
extern void MessageSystemTick(void)
{
    if (++TaskTick >= C_TaskTick)                     //任务级节拍定时器
    {
        TaskTick = 0;
    }
    //消息系统秒节拍信号释放，理论上64HZ的情况下，极限情况是可以放64个任务，但这样每个任务的执行时间是1/64，下面这里定义了6个点，并比较均匀，理论上每个任务的最多执行时间可以达到150ms；
    switch (TaskTick)
    {
        case 10:
            MessageQueue[ID_TaskDisplay] |= MSG_SecStep;    //显示任务
            break;
        case 20:
            //添加任务
            break;
        case 30:
            //添加任务
            break;
        case 40:
            //添加任务
            break;
        case 50:
            //添加任务
            break;
        case 60:
            //添加任务
        break;
        
        default:
        break;
    }

    //普通IO口定时驱动或者检测；
    LedGap();       //Led每15ms定时驱动
}












/*end-------------------------------------------------------------------------*/