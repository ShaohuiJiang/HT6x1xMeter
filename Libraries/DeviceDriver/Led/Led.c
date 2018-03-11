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
*   2、在Led.h文件中增加LEDPara类型的声明
*   3、新增声明LED操作函数，用来封装Set_xxxLedPin(LEVEL)函数
*   4、定义初始化在Led.h文件中声明的结构体
*   5、定义LED操作函数
*   6、定义LED初始化函数
*   7、定义可以被外部调用的三个操作函数xxxled_Open()\xxxled_Closed()\xxxled_Blink
*   8、如果Blink没需求可以不写
*   9、定义获取LED状态的函数
*   10、在Led.h文件中声明以上这些外部函数
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "Led.h"
#include "TypeDef.h"
#include "MCUConfig.h"


/*宏定义----------------------------------------------------------------------*/

/* 
*灯状态翻转时间和LedGap函数的调用间隔共同决定了闪烁周期 默认LedGap函数
*15ms调用一次，那么FlipTimer为67，就是15ms*67=1.005s
*/
#define FlipTimer 67

/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数

//点亮或者熄灭跳闸灯：注意该函数不支持闪烁方法
static void RelayLed_Set(LED_SET set);
//增加新的LED操作函数声明...


/*内部变量声明----------------------------------------------------------------*/
///添加内部变量


/*内部变量初始化--------------------------------------------------------------*/
///添加内部变量


/*外部变量初始化--------------------------------------------------------------*/
///添加外部变量
/* 初始化一个跳闸灯结构体变量 */
extern LEDPara RelayLed = 
{
    RelayLed_Set,
    TURN_OFF_SET,
    0,
    0
};
//初始化一个xxx灯结构体变量（注意必须先在Led.h文件中声明）...


/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数


/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数

/** 
 * @brief  点亮或熄灭跳闸灯
 * @note   注意这里默认是低电平点亮，高电平熄灭，实际要根据硬件电路来
 * @retval None
 */
static void RelayLed_Set(LED_SET set)     //点亮或者熄灭跳闸灯
{
    switch(set)
    {
        case TURN_OFF_SET:                    //熄灭
                Set_RelayLedPin(HIGH_LEVEL);
        break;

        case TURN_ON_SET:                      //点亮
                Set_RelayLedPin(LOW_LEVEL);
        break;

        default:                        //熄灭
                Set_RelayLedPin(LOW_LEVEL);
    }
}
//增加新的LED操作函数定义(注意必须先声明)...



/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/** 
 * @brief  跳闸灯初始化函数
 * @note   每次上电未进入循环前调用，根据需求进行初始化
 * @retval None
 */
extern void Init_RelayLed(void)
{
    RelayLed.LedSetFun(TURN_OFF_SET);
    RelayLed.LedStatus = TURN_OFF_SET;
    RelayLed.LedStatusFlipTimer = FlipTimer;
    RelayLed.Ledcount = 0;
}

/** 
 * @brief  点亮跳闸灯
 * @note   供外部文件调用
 * @retval None
 */
extern void RelayLed_TurnOn(void)
{
    RelayLed.LedSetFun(TURN_ON_SET);    //点亮跳闸灯
    RelayLed.LedStatus = TURN_ON_SET;     //改变灯状态
}
/** 
 * @brief  熄灭跳闸灯
 * @note   供外部文件调用
 * @retval None
 */
extern void RelayLed_TurnOff(void)
{
    RelayLed.LedSetFun(TURN_OFF_SET);         //熄灭跳闸灯
    RelayLed.LedStatus = TURN_OFF_SET;          //改变灯状态

}


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

    RelayLed.LedStatus = BLINK_SET;                   //改变灯状态,具体IO口操作在LedGap函数中
    RelayLed.LedStatusFlipTimer = seconds*FlipTimer;  //灯的一个状态维持时间
    RelayLed.Ledcount = 0;                            //计数器初始化
}

/** 
 * @brief  返回跳闸灯的当前状态
 * @note   
 * @retval 跳闸灯的状态
 */
extern LED_SET Get_RelayLedStatus(void)
{
    return(RelayLed.LedStatus);
}



/** 
 * @brief  LED定时驱动函数
 * @note   该函数必须定时运行，周期建议在15ms左右；
 * @retval None
 */
extern void LedGap(LEDPara Led)
{
       switch(Led.LedStatus)
       {
            case TURN_ON_SET:          //点亮
            {
                Led.LedSetFun(TURN_ON_SET);
            }
            break;

            case TURN_OFF_SET:          //熄灭
            {
                Led.LedSetFun(TURN_OFF_SET);
            }
            break;

            case BLINK_STATUS:          //闪烁
            {
                if(Led.Ledcount < Led.LedStatusFlipTimer)  //点亮时间
                {
                    Led.LedSetFun(TURN_ON_SET);
                }
                else if((Led.Ledcount >= Led.LedStatusFlipTimer)&&(Led.Ledcount <= (2*Led.LedStatusFlipTimer-FlipTimer)))  //熄灭时间
                {
                    Led.LedSetFun(TURN_OFF_SET);

                    if(Led.Ledcount <= (Led.LedStatusFlipTimer-FlipTimer))      //临界值，需要转换状态
                    {
                        Led.Ledcount = 0;
                    }
                }
                else        //非法值，清零
                {
                    Led.Ledcount = 0;
                }

                Led.Ledcount++;        //计数器累加
            }
            break;

            default:                    //默认熄灭灯
            {
                Led.LedSetFun(TURN_OFF_SET);
                Led.LedStatus = TURN_OFF_SET;
            }
            break;
       } 
}
/*end-------------------------------------------------------------------------*/