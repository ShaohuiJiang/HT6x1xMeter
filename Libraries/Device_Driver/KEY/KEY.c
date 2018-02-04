/*******************************************************************************
* @file      : KEY.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Feb 04 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "KEY.h"
#include "TypeDef.h"
#include "MCUConfig.h"
#include "Message.h"


/*宏定义----------------------------------------------------------------------*/
///添加宏定义
/* 声明一个按键输出状态枚举KEYSTATE */
typedef enum 
{
    KEYINITSTATE,               /// 按键初始态
    KEYJITTERSTATE,             /// 按键抖动态
    KEYPRESSSTATE,              /// 按键确认按下态
    KEYKEEPPRESSSTATE           /// 按键按下保持态
}KEYSTATE;

/* 声明一个输出状态跟按键电平状态的结构体 */
typedef struct
{
    KEYSTATE        keystate;                       /// 按键输出状态
    LEVEL_FUN_V     Check_KeyPinLevelStatus         /// 按键电平状态
} KEYPara;


/*内部变量声明----------------------------------------------------------------*/
///添加内部变量

/*  显示按键参数 */
static volatile  KEYPara DisplayKeyPara =
{
    dispalykeyState,
    Check_DisplayKeyPinLevelStatus
}



/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数






/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数
/**
  * @brief  获取按键（查询方式）的状态函数
  *         
  * @param  none
  * @param  none
  * @retval None
  * @attention 当返回值为2（KEYPRESSSTATE）时，即有按键事件发生
  *            
  */
static KEYSTATE Get_KeyState(KEYPara key)
{
    switch (key.keystate)
    {
        /* 初始态 */
        case KEYINITSTATE:
                        {
                            if(HIGHLEVEL == key.Check_KeyPinLevelStatus)
                            {
                                /* 如果按键电平状态为高，即无按键按下或者抖动 */
                                key.keystate == KEYINITSTATE;
                            }
                            else
                            {
                                /* 如果按键电平状态为低，那么进入抖动态 */
                                key.keystate = KEYJITTERSTATE;
                            }
                        }
            break;

        /* 按键抖动状态 */
        case KEYJITTERSTATE:
                        {
                            if(HIGHLEVEL == key.Check_KeyPinLevelStatus)
                            {
                                /* 如果按键电平状态为高，那么是抖动，返回初始态 */
                                key.keystate = KEYINITSTATE;
                            }
                            else
                            {
                                /* 如果按键电平为低，那么进入按键确认按下状态 */
                                key.keystate = KEYPRESSSTATE;
                            }
                        }
            break;

        /* 确认按下状态 */
        case KEYPRESSSTATE:
                        {
                            if(HIGHLEVEL == key.Check_KeyPinLevelStatus)
                            {
                                /* 如果按键电平状态为高，那么说明这次按键按下事件已经结束，返回初始态 */
                                key.keystate = KEYINITSTATE;
                            }
                            else
                            {
                                /* 如果按键电平状态为低，那么按键一直被按着不放，那么为了避免持续发送按键
                                按下的消息，需要进入按键按下停留状态 */
                                key.keystate = KEYKEEPPRESSSTATE;
                            }
                        }
            break;

        /* 按键按下停留状态 */
        case KEYKEEPPRESSSTATE:
                        {
                            if(HIGHLEVEL == key.Check_KeyPinLevelStatus)
                            {
                                /* 如果按键电平状态为高，那么说明这次按键按下事件已经结束，返回初始态 */
                                key.keystate = KEYINITSTATE;
                            }
                            else
                            {
                                /* 如果按键电平状态为低，维持状态不变 */
                                key.keystate = KEYKEEPPRESSSTATE;
                            }
                        }
            break;

        default:
                        {
                            /* 默认按键是初始态 */
                            key.keystate = KEYINITSTATE;
                        }
            break;
    }

    return key.keystate ;           //返回最新按键状态
}




/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/** 
 * @brief  定时检测显示按键
 * @note   这个要定时运行，间隔时间在10~50ms之间，这个才能既避免误判，又能及时响应
 * @retval None
 */
extern void Check_DisplayKey(void)
{
    if(KEYPRESSSTATE == Get_KeyState(DisplayKeyPara))       //如果有按键发生
    {
        g_Message[ID_TaskDisplay] |= MSG_DispKey;           //将按键消息存到TaskDisplay任务的消息缓存区
        //假如有其他任务也需要按键消息，也可以按上述方式直接将消息存到对应的任务消息缓存区
    }
}
















/*end-------------------------------------------------------------------------*/