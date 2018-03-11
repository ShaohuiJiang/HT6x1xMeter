/*******************************************************************************
* @file      : Key.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Thu Mar 08 2018
* @brief     : 
********************************************************************************
* @attention :
*   本文件规定了系统所有CPU直接驱动的KEY函数，后续要新增KEY的步骤是
*   1、确认在MCUConfig.c文件中已经有新key管脚的电平检测函数Get_xxxKeyPinLevelStatus(LEVEL)
*   2、在Key.h文件中增加KeyPara类型的声明
*   3、Key.h文件中声明获取Key状态函数，用来封装Get_xxxKeyPinLevelStatus(LEVEL)函数
*   4、Key.h文件中声明Key初始化函数
*   5、定义初始化在Led.h文件中声明的结构体
*   6、定义定义获取Key状态函数
*   7、定义Key初始化函数
*
*/

/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "Key.h"
#include "MCUConfig.h"
#include "TypeDef.h"
#include "General.h"

/*宏定义----------------------------------------------------------------------*/
///添加宏定义
#define KEY_STATUS_COUNT    3                   //按键计数器累加到STATUS_COUNT时，认为当前状态有效

/*内部变量声明  --------------------------------------------------------------*/



/*内部变量初始化--------------------------------------------------------------*/


/*外部变量初始化--------------------------------------------------------------*/
//初始化一个KEYPara类型的显示键
KEY_PARA DisplayKey =
{
    GetStatus_DisplayKey,       //指向GetStatus_DispalyKey函数
    UP_STATUS,                  //弹起状态
    UP_STATUS,                  //弹起状态
    0,                          //清零
    0,                          //清零
    NO_ACTION                   //无行为
}; 
///定义新的按键结构体（注意，必须现在Key.h中先声明）...

/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数


/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数


/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/* 定时查询方式 */
/** 
 * @brief  检测显示键瞬间的状态函数
 * @note   
 * @retval UP_STATUS：弹起状态，DOWN_STATUS：按下状态
 */
extern KEY_STATUS GetStatus_DisplayKey(void)
{
    if(HIGH_LEVEL == Get_DisplayKeyPinLevelStatus())       //测得高电平
    {
            return UP_STATUS;      //返回状态：弹起状态
    }
    else                                                  //测得低电平
    {
            return DOWN_STATUS;    //返回状态：按下状态
    }
}

/** 
 * @brief  初始化按键状态函数
 * @note   每次上电未进入循环前调用，根据按键的实际状态进行初始化
 * @retval None
 */
extern void Init_Key(KEY_PARA Key)
{
    if(UP_STATUS != Key.GetStatusFun())          //如果按键状态是按下状态
    {
        Delay_mSec(10);                         //延时10ms

        if(UP_STATUS != Key.GetStatusFun())     //延时后按键状态还是按下状态
        {
            /* 按键上次状态和当前状态都初始化成按下状态 */
            Key.LastKeyStatus = DOWN_STATUS;
            Key.NowKeyStatus  = DOWN_STATUS;
        }
    }
    else                                        //如果显示键状态是弹起状态
    {
            /* 按键上次状态和当前状态都初始化成弹起状态 */
            Key.LastKeyStatus = UP_STATUS;
            Key.NowKeyStatus  = UP_STATUS;
    }

    /* 状态计数器初始化 */
    Key.UpStatusCount   = 0;
    Key.DownStatusCount = 0;

    /* 按键行为初始化无行为 */
    Key.KeyAction = NO_ACTION;
}


/** 
 * @brief  按键定时查询并反馈行为函数
 * @note   这个函数必须10~30ms左右运行一次
 * @param  Key: 某个按键
 * @retval UP_ACTION:弹起行为，Down_ACTION：按下行为，NO_ACTION：无行为
 */
extern KEY_ACTION KeyGap(KEY_PARA Key)
{


    Key.KeyAction = NO_ACTION;                             //初始化为无行为
    
    if(UP_STATUS == Key.GetStatusFun())                    //弹起状态
    {
        Key.DownStatusCount = 0;                           //按下状态计数器清零
        
        if(Key.UpStatusCount < KEY_STATUS_COUNT)       //弹起状态计数器小于阈值时
        {
            Key.UpStatusCount++;                           //弹起状态计数器累加
        }
        else if(Key.UpStatusCount >= KEY_STATUS_COUNT) //弹起状态计数器大于等于阈值时,现有状态为弹起状态
        {
            Key.NowKeyStatus = UP_STATUS;
        }

        if(Key.NowKeyStatus != Key.LastKeyStatus)         //如果现有按键状态跟上次按键状态不一致，那么就发生弹起行为
        {
            Key.LastKeyStatus = Key.NowKeyStatus;         //上次按键状态更新
            Key.KeyAction = UP_ACTION;                     //返回弹起行为
        }
    }
    else                                                    //按下状态
    {
        Key.UpStatusCount = 0;                                 //弹起状态计数器清零
        
        if(Key.DownStatusCount < KEY_STATUS_COUNT)         //按下状态计数器小于阈值时
        {
            Key.DownStatusCount++;                             //按下状态计数器累加
        }
        else if(Key.DownStatusCount >= KEY_STATUS_COUNT)   //按下状态计数器大于等于阈值时
        {
            Key.NowKeyStatus = DOWN_STATUS;                    //现有状态为按下状态
        }

        if(Key.NowKeyStatus != Key.LastKeyStatus)             //如果现有按键状态跟上次按键状态不一致，那么就发生按下行为
        {
            Key.LastKeyStatus = Key.NowKeyStatus;             //上次按键状态更新
            Key.KeyAction = Down_ACTION;                      //返回按下行为
        }
    }

    return Key.KeyAction;
}

/* 中断方式 */
//因为管脚不一定支持中断功能，所以有些按键不一定支持，注意在MCU初始化时需配置成中断方式

/** 
 * @brief  键按下中断函数
 * @note   该函数根据需求要放到对应的外部中断函数中
 * @param  Key: 按键
 * @retval None
 */
extern void IRQ_Key_Down(KEY_PARA Key)
{
   Key.KeyAction = Down_ACTION;                      //按下行为
}

/** 
 * @brief  键弹起中断函数
 * @note   该函数根据需求要放到对应的外部中断函数中
 * @param  Key: 按键
 * @retval None
 */
extern void IRQ_Key_Up(KEY_PARA Key)
{
   Key.KeyAction = UP_ACTION;                        //弹起行为
}







/*end-------------------------------------------------------------------------*/