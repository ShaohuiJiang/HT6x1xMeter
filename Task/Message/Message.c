/*******************************************************************************
* @file      : Message.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Mon Jan 29 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "Message.h"



/*宏定义----------------------------------------------------------------------*/
///添加宏定义




/*内部变量声明----------------------------------------------------------------*/
///添加内部变量
static volatile MSG u8_Message[ID_TaskNum];              //任务控制块（任务消息）
static volatile unsigned char   u8_TaskTick;             //任务级节拍定时器



/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数






/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数





/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数


/** 
 * @brief   消息系统初始化
 * @note   
 * @retval None
 */
extern void Init_Message(void)
{
    unsigned i;
    for (i=0; i<ID_TaskNum; i++)            //将所有任务的消息队列清空
    {
        u8_Message[i] = MSG_Null;
    }
    u8_TaskTick = 0;                       //任务级节拍清零
}


/** 
 * @brief  发送消息到某个任务队列
 * @note   
 * @param  idTask: 任务
 * @param  msg: 消息
 * @retval None
 */
extern void Post_Message(ID_TASK idTask, MSG msg)
{
    if (idTask < ID_TaskNum)            //任务的值肯定小于任务数，不然就是这个任务是不存在的。
    {
        u8_Message[idTask] |= msg;
    }
}


/** 
 * @brief  
 * @note   
 * @param  idTask: 
 * @retval 
 */
extern MSG Accept_Message(ID_TASK idTask)
{
    MSG msg;
    
    if (idTask < ID_TaskNum)
    {
        msg = u8_Message[idTask];
        u8_Message[idTask] -= msg;               //清除该消息队列
        return msg;
    }
    else
    {
        return MSG_Null;
    }
}


/** 
 * @brief  确认消息
 * @note   判断队列中x是否为y消息
 * @param  x: 被测消息
 * @param  y: 比对用的消息
 * @retval 
 */
extern Bool Is_Message(MSG x,MSG y)
{
   if((x&y) != MSG_Null)
   {
       return FALSE;
   }
   else
   {
       return TRUE;
   }
}

/** 
 * @brief  任务系统节拍定时器中断
 * @note   将该函数放到定义为系统节拍定时器的中断中
 * @retval None
 */
extern void Task_Systick(void)
{
    if (++u8_TaskTick >= C_TaskTick)            //任务级节拍定时器
    {
        u8_TaskTick = 0;
    }

    /* 秒节拍信号释放 */
    switch (u8_TaskTick)
    {
        case 10:
            u8_Message[ID_TaskDisplay]  |= MSG_SecStep;         //显示任务
        break;
        
        case 20:
            u8_Message[ID_TaskRecord]   |= MSG_SecStep;         //记录任务
        break;
        
        case 30:
            u8_Message[ID_TaskTime]     |= MSG_SecStep;         //时钟任务
        break;
        
        case 40:
            u8_Message[ID_TaskMetering] |= MSG_SecStep;         //计量任务
        break;
        
        case 50:
            u8_Message[ID_TaskRelay]    |= MSG_SecStep;         //拉合闸任务
        break;
        
        case 60:
            u8_Message[ID_TaskRmtFee]   |= MSG_SecStep;         //远程费控任务
        break;
        
        default:
        break;
    }
    //中断定时任务检测----------------------------------------------------------
    //INT_DispKey();                  //显示按键检测
    //INT_ChkComGap();                //计量通讯间隔定时器
    //INT_ChkEmuGap();                //计量通讯间隔定时器
    //INT_ChkFeeGap();                //费控通讯间隔定时器
}





/*end-------------------------------------------------------------------------*/