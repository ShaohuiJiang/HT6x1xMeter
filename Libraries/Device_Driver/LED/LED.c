/*******************************************************************************
* @file      : LED.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sat Feb 03 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "LED.h"
#include "TypeDef.h"
#include "MCUConfig.h"


/*宏定义----------------------------------------------------------------------*/
///添加宏定义

/* 
*拉闸灯状态翻转时间和RelayLedGap函数的调用间隔共同决定了闪烁周期 默认RelayLedGap函数
*15ms调用一次，那么RelayLedStatusFlipTimer为67，就是15ms*67=1.005s
*/
#define RelayLedStatusFlipTimer            67              //拉闸灯状态翻转时间
#define BackLedStatusFlipTimer             67              //拉闸灯状态翻转时间


/*内部变量声明----------------------------------------------------------------*/
///添加内部变量
static LEDSet RelayLedStatus;        //拉闸灯状态
static LEDSet BackLedStatus;         //背光灯状态

static u8 u8_RelayLedGapCnt;        //拉闸灯状态持续时间计数器，用于闪烁状态
static u8 u8_BackLedGapCnt;         //背光灯状态持续时间计数器，用于闪烁状态


/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数
/* 打开跳闸灯 */
static void Open_RelayLed(void);
/* 关闭跳闸灯 */
static void Close_RelayLed(void);
/* 打开背光灯 */
static void Open_BackLed(void);
/* 关闭背光灯 */
static void Close_BackLed(void);
/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数
/** 
 * @brief  打开跳闸灯
 * @note   
 * @retval None
 */
static void Open_RelayLed(void)
{
    Set_RelayLedPin(LOWLEVEL);
}
/** 
 * @brief  关闭跳闸灯
 * @note   
 * @retval None
 */
static void Close_RelayLed(void)
{
    Set_RelayLedPin(HIGHLEVEL);
}

/** 
 * @brief  打开背光灯
 * @note   
 * @retval None
 */
static void Open_BackLed(void)
{
    
}

/** 
 * @brief  关闭背光灯
 * @note   
 * @retval None
 */
static void Close_BackLed(void)
{

}

/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数


/** 
 * @brief   操作拉闸灯的函数
 * @note   
 * @param  method: Open 代表打开；Closed 代表关闭; Blink 代表闪烁
 * @retval None
 */
extern void Set_RelayLed(LEDSet method)
{
    switch(method)
    {
        case Open: 
                    RelayLedStatus = Open;
            break;
        
        case Closed:
                    RelayLedStatus = Closed;
            break;
        
        case Blink:
                    RelayLedStatus = Blink;
            break;
        
        default :
                    RelayLedStatus = Closed;
            break;
    }
}


/** 
 * @brief   操作背光灯的函数
 * @note   
 * @param  method:  Open 代表打开；Closed 代表关闭; Blink 代表闪烁
 * @retval None
 */
extern void Set_BackLed(LEDSet method)
{
    switch(method)
    {
        case Open: 
                    BackLedStatus = Open;
            break;
        
        case Closed:
                    BackLedStatus = Closed;
            break;
        
        case Blink:
                    BackLedStatus = Blink;
            break;
        
        default :
                    BackLedStatus = Closed;
            break;
    }
}

/** 
 * @brief  拉闸灯间隔驱动定时器函数
 * @note   该函数要放到定时器中断中，运行周期建议在15ms左右
 * @retval None
 */
extern void RelayLedGap(void)
{
    switch(RelayLedStatus)
    {
        case Open:
                    Open_RelayLed();
                    u8_RelayLedGapCnt = 0;
            break;

        case Closed:
                    Close_RelayLed();
                    u8_RelayLedGapCnt = 0;
            break;

        case Blink:
                {
                    if(u8_RelayLedGapCnt < RelayLedStatusFlipTimer)               //点亮时间：RelayLedStatusFlipTimer*函数运行周期
                    {
                        Open_RelayLed();
                        u8_RelayLedGapCnt++;
                    }
                    else if(u8_RelayLedGapCnt < (RelayLedStatusFlipTimer*2-1))    //熄灭时间：(RelayLedStatusFlipTimer-1)*函数运行周期
                    {
                        Close_RelayLed();
                        u8_RelayLedGapCnt++;
                    }
                    else                                                        //熄灭时间：1*函数运行周期
                    {
                        Close_RelayLed();
                        u8_RelayLedGapCnt = 0;
                    }
                }
            break;
        default:                                                                //非常值，就默认关闭拉闸灯
                    Close_RelayLed();
                    RelayLedStatus = Closed;
                    u8_RelayLedGapCnt = 0;
            break;
    }
}

/** 
 * @brief  背光灯间隔驱动定时器函数
 * @note   该函数要放到定时器中断中，运行周期建议在15ms左右
 * @retval None
 */
extern BackLedGap(void)
{
    switch(BackLedStatus)
    {
        case Open:
                    Open_BackLed();
                    u8_BackLedGapCnt = 0;
            break;

        case Closed:
                    Close_BackLed();
                    u8_BackLedGapCnt = 0;
            break;

        case Blink:
                {
                    if(u8_BackLedGapCnt < BackLedStatusFlipTimer)               //点亮时间：BackLedStatusFlipTimer*函数运行周期
                    {
                        Open_BackLed();
                        u8_BackLedGapCnt++;
                    }
                    else if(u8_BackLedGapCnt < (BackLedStatusFlipTimer*2-1))    //熄灭时间：(BackLedStatusFlipTimer-1)*函数运行周期
                    {
                        Close_BackLed();
                        u8_BackLedGapCnt++;
                    }
                    else                                                        //熄灭时间：1*函数运行周期
                    {
                        Close_BackLed();
                        u8_BackLedGapCnt = 0;
                    }
                }
            break;
        default:                                                               //非常值，就默认关闭背光灯
                    Close_BackLed();
                    BackLedStatus = Closed;
                    u8_BackLedGapCnt = 0;
            break;
    }
}










/*end-------------------------------------------------------------------------*/