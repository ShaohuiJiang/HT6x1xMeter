/*******************************************************************************
* @file      : Message.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Mon Jan 29 2018
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




/*宏定义----------------------------------------------------------------------*/
///添加宏定义
//任务ID定义（无优先级概念）
typedef	enum
{
    ID_TaskDisplay,     //显示任务      (消息 MSG_SecStep/MSG_DispKey/MSG_DispWake/MSG_FrameIn/MSG_FrameOut/MSG_DispKey)
    ID_TaskRecord,      //记录任务      (消息 MSG_SecStep/MSG_MinStep)
    ID_TaskTime,        //时钟任务      (消息 MSG_SecStep)
    ID_TaskMetering,    //计量任务      (消息 MSG_SecStep)
    ID_TaskComm,        //通信任务      (消息 MSG_SecStep)
    ID_TaskRate,        //费率任务      (消息 MSG_SecStep/MSG_MinStep)
    ID_TaskFreeze,      //冻结任务      (消息 MSG_MinStep)
    ID_TaskSystem,      //系统任务      (系统任务没有消息，放在主循环中，一调用就运行)
    ID_TaskRelay,       //拉合闸任务    (消息  MSG_SecStep/MSG_ChkRelay)
    ID_TaskRmtFee,      //远程费控任务  (消息 MSG_InitEsam/MSG_RenewKey)
    ID_TaskLclFee,      //本地费控任务  ()
    //添加各种任务...
    ID_TaskNum          //任务数量     放在最后，就正好是所有任务数量和
}ID_TASK;

typedef unsigned char   MSG;            //将消息这种数据格式定为一个字节

/*定时节拍消息*/
#define MSG_Null        (MSG)0x00      //空消息
#define MSG_SecStep     (MSG)0x01      //秒节拍消息 （显示,记录,实时时钟,计量,拉合闸任务用）
#define MSG_MinStep     (MSG)0x02      //分节拍消息 （通讯,费率,冻结任务用）


/*显示任务(TaskDispaly)消息（除了定时节拍消息）*/
#define MSG_DispKey     (MSG)0x04      //显示按键消息（显示任务用）
#define MSG_DispWake    (MSG)0x08      //红外唤醒消息（显示任务用）
#define MSG_FrameIn     (MSG)0x10      //通信帧识别消息（显示任务用）
#define MSG_FrameOut    (MSG)0x20      //通信帧响应消息（显示任务用）
#define MSG_FramePLC    (MSG)0x40      //载波通信帧消息（显示任务用）


/*记录任务(TaskRecord)消息(除了定时节拍消息)*/
#define MSG_InitTask    (MSG)0x04      //上电初始化 （记录任务用）
#define MSG_ChgTime     (MSG)0x08      //(校时)日期或者时间改变（记录任务用）

/* 时钟任务消息(除了定时节拍消息) */
//

/* 计量任务消息(除了定时节拍消息) */
//

/* 通信任务消息(除了定时节拍消息) */
//

/* 费率任务消息(除了定时节拍消息) */
//

/* 冻结任务消息(除了定时节拍消息) */
//

/* 冻结任务消息(除了定时节拍消息) */
//

/*拉合闸任务(TaskRelay)(除了定时节拍消息)*/
#define MSG_CtrlRelay   (MSG)0x40      //拉合闸消息（拉合闸任务用）

/*远程费控任务(TaskRmtFee)(除了定时节拍消息)*/
#define MSG_InitEsam    (MSG)0x40      //初始化ESAM消息（远程费控任务用）
#define MSG_RenewKey    (MSG)0x80      //密钥更新消息（远程费控任务用）


/*本地费控任务(TaskLclFee)(除了定时节拍消息)*/
#define MSG_CardInsert  (MSG)0x40      //卡插入消息（本地费控任务用）
#define MSG_CardOperate (MSG)0x80      //卡操作消息（本地费控任务用）

/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void Init_Message(void);                     //初始化消息系统
extern void Stop_Message(void);                     //停止消息系统
extern void Post_Message(ID_TASK idTask, MSG msg);  //发送消息
MSG Accept_Message(ID_TASK idTask);                 //获取消息
#define Is_Message(x,y) ((x&y) != MSG_Null)

#endif
/*end------------------------------------------------------------------------*/