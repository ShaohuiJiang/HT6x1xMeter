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
*   3、声明和定义led操作函数LedSet_xxx(LEDSet set)
*   4、在LEDARRAY数组中新增对应led的参数，即初始化
*   5、然后新增可以被外部调用的三个操作函数xxxled_Open()\xxxled_Closed()\xxxled_Blink
*   6、如果Blink没需求可以不写
*   7、在Led.h文件中增加被外部调用的操作函数
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "Led.h"
#include "TypeDef.h"
#include "MCUConfig.h"

/*宏定义----------------------------------------------------------------------*/
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

typedef void    (*V_FUN_SET)(LEDSet);      //指向LED驱动函数

/* 
*灯状态翻转时间和LedGap函数的调用间隔共同决定了闪烁周期 默认LedGap函数
*15ms调用一次，那么FlipTimer为67，就是15ms*67=1.005s
*/
#define FlipTimer 67

/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数
static void LedSet_Relay(LEDSet set);     //点亮或者熄灭跳闸灯
static void LedSet_Back(LEDSet set);      //点亮或者熄灭背光灯
//增加新的LED操作函数声明...

/*led参数结构体----------------------------------------------------------------*/
typedef struct
{
    V_FUN_SET LedSet_Name;              //操作LED对应的IO口；操作被封装，或者被LedGap函数根据LedStatus而执行相应操作
    LEDSet   LedStatus;                 //LED的几种状态，当状态被改变时，LedGap函数会根据状态执行相应的操作
    u16      LedStatusFlipTimer;        //灯状态翻转时间；在闪烁状态，这个变量就是用来维持亮或者灭的持续时间参数之一；
    u16      Ledcount;                  //灯计数器
}LEDPara;


/*内部变量声明----------------------------------------------------------------*/
///添加内部变量


/*内部变量初始化--------------------------------------------------------------*/
///添加内部变量
/* LED数组的初始化，注意，行的排列顺序，必须跟枚举LED一一对应 */
static LEDPara LEDARRAY[LedNumber]=
{
    LedSet_Relay,Closed,FlipTimer,FlipTimer,            //跳闸灯参数初始化
    LedSet_Back,Closed,FlipTimer,FlipTimer,             //背光灯参数初始化
    //增加新led的初始化...
};




/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数

/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数

/** 
 * @brief  点亮或熄灭跳闸灯
 * @note   注意这里默认是低电平点亮，高电平熄灭，实际要根据硬件电路来
 * @retval None
 */
static void LedSet_Relay(LEDSet set)     //点亮或者熄灭跳闸灯
{
    switch(set)
    {
        case Closed:                    //熄灭
                Set_RelayLedPin(HIGHLEVEL);
        break;

        case Open:                      //点亮
                Set_RelayLedPin(LOWLEVEL);
        break;

        default:                        //熄灭
                Set_RelayLedPin(LOWLEVEL);
    }
}

/** 
 * @brief  点亮或熄灭跳闸灯
 * @note   注意这里默认是低电平点亮，高电平熄灭，实际要根据硬件电路来
 * @retval None
 */
static void LedSet_Back(LEDSet set)     //点亮或者熄灭背光灯
{
    switch(set)
    {
        case Closed:                    //熄灭
                Set_BackLedPin(HIGHLEVEL);
        break;

        case Open:                      //点亮
                Set_BackLedPin(LOWLEVEL);
        break;

        default:                        //熄灭
                Set_BackLedPin(LOWLEVEL);
    }
}
//增加新的LED操作函数定义...
/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数



/** 
 * @brief  点亮跳闸灯
 * @note   供外部文件调用
 * @retval None
 */
extern void RelayLed_Open(void)
{
    LEDARRAY[RelayLed].LedStatus = Open;        //改变灯状态
    LEDARRAY[RelayLed].LedSet_Name(Open);       //点亮跳闸灯
}
/** 
 * @brief  熄灭跳闸灯
 * @note   供外部文件调用
 * @retval None
 */
extern void RelayLed_Closed(void)
{
    LEDARRAY[RelayLed].LedStatus = Closed;      //改变灯状态
    LEDARRAY[RelayLed].LedSet_Name(Closed);     //熄灭跳闸灯
}

/** 
 * @brief  闪烁跳闸灯
 * @note   供外部文件调用
 * @retval None
 */
/** 
 * @brief  闪烁跳闸灯
 * @note   供外部文件调用
 * @param  seconds: 点亮时间=熄灭时间 seconds范围：1~60，小于1默认1，大于60，默认60
 * @retval None
 */
extern void RelayLed_Blink(SEC seconds)
{
    if(seconds < 1)
    {
        seconds = 1;
    }
    else if(seconds > 60)
    {
        seconds = 60;
    }

    LEDARRAY[RelayLed].LedStatus = Blink;                       //改变灯状态,具体IO口操作在LedGap函数中
    LEDARRAY[RelayLed].LedStatusFlipTimer = seconds*FlipTimer;  //灯的一个状态维持时间
    LEDARRAY[RelayLed].Ledcount = seconds*FlipTimer;            //计数器初始化
}

/** 
 * @brief  点亮背光灯
 * @note   供外部文件调用
 * @retval None
 */
extern void BackLed_Open(void)
{
    LEDARRAY[BackLed].LedStatus = Open;            //改变灯状态
    LEDARRAY[BackLed].LedSet_Name(Open);           //点亮背光灯
}
/** 
 * @brief  熄灭背光灯
 * @note   供外部文件调用
 * @retval None
 */
extern void BackLed_Closed(void)
{
    LEDARRAY[BackLed].LedStatus = Closed;            //改变灯状态
    LEDARRAY[BackLed].LedSet_Name(Closed);           //熄灭背光灯
}
//背光灯无闪烁功能需求，所以没有相应函数

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
                        LEDARRAY[led].LedSet_Name(Open);
                    }
            break;

            case Closed:        //熄灭
                    {
                        LEDARRAY[led].LedSet_Name(Closed);
                    }
            break;

            case Blink:         //闪烁
                    {
                        
                       if((LEDARRAY[RelayLed].Ledcount>0)&&(LEDARRAY[RelayLed].Ledcount<=LEDARRAY[led].LedStatusFlipTimer ))      //计数器的值是0~LedStatusFlipTimer
                       {
                           LEDARRAY[led].LedSet_Name(Open);
                       }
                       else if((LEDARRAY[RelayLed].Ledcount>LEDARRAY[led].LedStatusFlipTimer)&&(LEDARRAY[RelayLed].Ledcount<=2*LEDARRAY[led].LedStatusFlipTimer ))  //计数器的值是LedStatusFlipTimer~2*LedStatusFlipTimer
                       {
                           LEDARRAY[led].LedSet_Name(Closed);
                       }
                       if(LEDARRAY[RelayLed].Ledcount != 0)
                       {
                           LEDARRAY[RelayLed].Ledcount--;
                       }
                       else
                       {
                           LEDARRAY[RelayLed].Ledcount=2*LEDARRAY[led].LedStatusFlipTimer;
                       }
                    }
            break;

            default:                //默认熄灭灯
                        LEDARRAY[led].LedStatus = Closed;
                        LEDARRAY[led].LedSet_Name(Closed);
            break;
       } 
    }
}
/*end-------------------------------------------------------------------------*/