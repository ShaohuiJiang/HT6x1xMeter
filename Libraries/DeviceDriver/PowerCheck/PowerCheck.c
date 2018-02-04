/*******************************************************************************
* @file      : PowerCheck.c
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
#include "PowerCheck.h"
#include "TypeDef.h"
#include "MCUConfig.h"
#include "General.h"

/*宏定义----------------------------------------------------------------------*/
///添加宏定义




/*内部变量声明----------------------------------------------------------------*/
///添加内部变量
static Power p_MeterWorkState;                                  //表计工作状态


/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数






/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数





/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数
/** 
 * @brief  外部电源检测
 * @note   
 * @param  times: 判断次数
 * @retval POWERON 上电状态 POWEROFF 掉电状态
 */
extern Power Check_Power(u8 times)
{
    u8 i;

    /* 判断times次检测引脚的电平，假如有一次是低电平，就认为外部电源没电，否则就是有电 */
    for(i=0;i<times;i++)
    {
        if(LOWLEVEL == Get_PowerOnPinLevelStatus())
        {
            p_MeterWorkState = POWEROFF;        //将表计工作状态标志位置成掉电状态
            return POWEROFF;                    //返回掉电状态
        }
        
        Delay_mSec(10);
    }

    p_MeterWorkState = POWERON;                 //将表计工作状态标志位置成上电状态
    return POWERON;                             //返回工作状态
}

/** 
 * @brief  获取表计工作状态标志位
 * @note   
 * @retval POWERON 上电状态 POWEROFF 掉电状态
 */
extern Power Get_MeterWorkState(void)
{
    return p_MeterWorkState;
}













/*end-------------------------------------------------------------------------*/