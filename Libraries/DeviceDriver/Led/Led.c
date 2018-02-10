/*******************************************************************************
* @file      : Led.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Feb 04 2018
* @brief     : 
********************************************************************************
* @attention :
*   本文件规定了系统所有CPU直接驱动的LED函数，后续要新增led的步骤是
*   1、确认在MCUConfig.c文件中已经有新led管脚的电平驱动函数Set_xxxLedPin(LEVEL)
*   2、在LED枚举中增加Led名称
*   3、在LEDARRAY数组中新增对应led的参数，即初始化
*   4、然后新增可以被外部调用的三个操作函数xxxled_Open()\xxxled_Closed()\xxxled_Blink
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "Led.h"
#include "TypeDef.h"
#include "MCUConfig.h"



/*宏定义----------------------------------------------------------------------*/
///添加宏定义

typedef enum
{
    RelayLed,        //跳闸灯
    BackLed,         //背光灯
    //增加LED名称...
    LedNumber        //要放在最后，这样才能等于灯的数量
}LED;

typedef enum
{
    Closed,         //关闭
    Open,           //打开
    Blink           //闪烁
}LEDSet;            //LED灯的操作方法

typedef void    (*V_FUN_LEVEL)(LEVEL);      //指向LED管脚的电平驱动函数

typedef struct
{
    V_FUN_LEVEL LedSetPin;              //操作LED对应的IO口高电平或者低电平；操作被封装，或者被LedGap函数根据LedStatus而执行相应操作
    LEDSet   LedStatus;              //LED的几种状态，当状态被改变时，LedGap函数会根据状态执行相应的操作
    u8       LedStatusFlipTimer;     //灯状态翻转时间；在闪烁状态，这个变量就是用来维持亮或者灭的持续时间参数之一；
}LEDPara;
/* 
*灯状态翻转时间和LedGap函数的调用间隔共同决定了闪烁周期 默认LedGap函数
*15ms调用一次，那么FlipTimer为67，就是15ms*67=1.005s
*/
#define FlipTimer 67

/*内部变量声明----------------------------------------------------------------*/
///添加内部变量


/*内部变量初始化--------------------------------------------------------------*/
///添加内部变量
/* LED数组的初始化，注意，行的排列顺序，必须跟枚举LED一一对应 */
static LEDPara LEDARRAY[LedNumber]=
{
    Set_RelayLedPin,Closed,FlipTimer,           //跳闸灯参数初始化
    Set_BackLedPin,Closed,FlipTimer             //背光灯参数初始化
    //增加新led的初始化
};




/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数

/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数

/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/** 
 * @brief  LED定时驱动函数
 * @note   该函数必须定时运行，周期建议在15ms左右；
 * @retval None
 */
extern void LedGap(void)
{
    u8 led;
    for(led=0;led<LedNumber;led++)
    {
       switch(LEDARRAY[led].LedStatus)
       {
            case Open:          //点亮
                    {
                        LEDARRAY[led].LedSetPin(LOWLEVEL);
                    }
            break;

            case Closed:        //熄灭
                    {
                        LEDARRAY[led].LedSetPin(HIGHLEVEL);
                    }
            break;

            case Blink:         //闪烁
                    {
                        LEDARRAY[led].LedStatusFlipTimer--;
                        if(LEDARRAY[led].LedStatusFlipTimer > FlipTimer)
                        {
                            LEDARRAY[led].LedSetPin(LOWLEVEL);
                        }
                        else if(LEDARRAY[led].LedStatusFlipTimer <= FlipTimer)
                        {
                            LEDARRAY[led].LedSetPin(HIGHLEVEL);
                        }
                        else if(LEDARRAY[led].LedStatusFlipTimer == 0)
                        {
                            LEDARRAY[led].LedStatusFlipTimer = 2*FlipTimer+1;
                            LEDARRAY[led].LedSetPin(LOWLEVEL);
                        }
                    }
            break;

            default:                //默认熄灭灯
                        LEDARRAY[led].LedStatus = Closed;
                        LEDARRAY[led].LedStatusFlipTimer = FlipTimer;
                        LEDARRAY[led].LedSetPin(HIGHLEVEL);
            break;
       } 
    }
}

/** 
 * @brief  点亮跳闸灯
 * @note   注意，默认管脚输出低电平时，点亮跳闸灯，实际要跟硬件确认过，有可能是反过来
 * @retval None
 */
extern void RelayLed_Open(void)
{
    LEDARRAY[RelayLed].LedStatus = Open;            //改变灯状态
    LEDARRAY[RelayLed].LedSetPin(LOWLEVEL);   //操作对应IO口
}
/** 
 * @brief  熄灭跳闸灯
 * @note   注意，默认管脚输出高电平时，熄灭跳闸灯，实际要跟硬件确认过，有可能是反过来
 * @retval None
 */
extern void RelayLed_Closed(void)
{
    LEDARRAY[RelayLed].LedStatus = Closed;            //改变灯状态
    LEDARRAY[RelayLed].LedSetPin(HIGHLEVEL);   //操作对应IO口
}

/** 
 * @brief  闪烁跳闸灯
 * @note   注意，这个函数只是改变了灯的状态变量，实际改变IO口的操作是在LedGap函数中
 * @retval None
 */
extern void RelayLed_Blink(void)
{
    LEDARRAY[RelayLed].LedStatus = Blink;            //改变灯状态,具体IO口操作在LedGap函数中
}

/** 
 * @brief  点亮背光灯
 * @note   注意，默认管脚输出低电平时，点亮背光灯，实际要跟硬件确认过，有可能是反过来
 * @retval None
 */
extern void BackLed_Open(void)
{
    LEDARRAY[BackLed].LedStatus = Open;            //改变灯状态
    LEDARRAY[BackLed].LedSetPin(LOWLEVEL);   //操作对应IO口
}
/** 
 * @brief  熄灭背光灯
 * @note   注意，默认管脚输出高电平时，熄灭背光灯，实际要跟硬件确认过，有可能是反过来
 * @retval None
 */
extern void BackLed_Closed(void)
{
    LEDARRAY[BackLed].LedStatus = Closed;            //改变灯状态
    LEDARRAY[BackLed].LedSetPin(HIGHLEVEL);   //操作对应IO口
}

/** 
 * @brief  闪烁背光灯
 * @note   注意，这个函数只是改变了灯的状态变量，实际改变IO口的操作是在LedGap函数中
 * @retval None
 */
extern void BackLed_Blink(void)
{
    LEDARRAY[BackLed].LedStatus = Blink;            //改变灯状态,具体IO口操作在LedGap函数中
}
/*end-------------------------------------------------------------------------*/