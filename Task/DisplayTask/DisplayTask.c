/*******************************************************************************
* @file      : DisplayTask.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Wed Mar 07 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "DisplayTask.h"
#include "Message.h"
#include "Led.h"




/*宏定义----------------------------------------------------------------------*/
///添加宏定义




/*内部变量声明----------------------------------------------------------------*/
///添加内部变量


/*内部变量初始化--------------------------------------------------------------*/
///添加内部变量

/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数






/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数





/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数
/** 
 * @brief  初始化显示任务
 * @note   
 * @retval None
 */
extern void Init_DisplayTask(void)
{
    Init_RelayLed();              //关闭跳闸灯
}

/** 
 * @brief  运行显示任务
 * @note   
 * @retval None
 */
extern void Run_DisplayTask(void)
{
    MSG msg;
    msg = Accept_Message(ID_TaskDisplay);       //接收显示任务消息

    if (Is_Message(msg, MSG_SecStep))       //收到秒节拍消息
    {

    }
    if (Is_Message(msg, MSG_DispKey))       //收到显示按键消息
    {
        LED_SET Ledstate;
        Ledstate=Get_RelayLedStatus();
        switch(Ledstate)
        {
            case OFF_STATUS:
                RelayLed_TurnOn();
            break;

            case TURN_ON_SET:
                RelayLed_Blink(1);
            break;

            case BLINK_STATUS:
                RelayLed_TurnOff();
            break;
            default:
            break;
        }
    }
    //收到XX消息...
}

/** 
 * @brief  停止显示任务
 * @note   
 * @retval None
 */
extern void Stop_DisplayTask(void)
{
    Init_RelayLed();              //跳闸灯初始化
}











/*end-------------------------------------------------------------------------*/