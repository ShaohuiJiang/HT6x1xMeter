/*******************************************************************************
* @file      : TaskSystem.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Jan 28 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "MCUConfig.h"



/*宏定义----------------------------------------------------------------------*/
///添加宏定义




/*内部变量声明----------------------------------------------------------------*/
///添加内部变量




/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数






/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数





/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/** 
 * @brief  任务系统初始化
 * @note   
 * @retval None
 */
extern void Init_TaskSystem(void)
{

}


/** 
 * @brief   任务系统运行函数
 * @note   用于根据外部电源判断，而进行相应的操作；
 *         判断到外部掉电：停止各个任务，然后break跳出上电循环；
 *         判断到外部有电：判断系统定时器是否正常，判断MCU的各模块状态是否正常，
 *         根据表计各种状态判断对应的IO口等操作是否正常
 * @retval None
 */
extern void Run_TaskSystem(void)
{
    if(1==Check_PowerOn(1))         //外部有电
    {
        if(TRUE != Get_MeterWorkState())    //检测表计工作状态,如果非上电状态，就设置成上电状态
        {
            Set_MeterWorkState(TRUE);
        }
        Maintain_MCU();                     //MCU寄存器值检查
        Run_SysTick();                      //系统定时器状态检查
        //可以添加各种任务状态的检查，看状态是否正常...
    }
    else                            //外部没电
    {
        Disable_Int();              //关闭总中断
        Stop_SysTick();             //停止系统定时器
        //接下来停止各种任务...

        WDTReset_MCU();             //程序进入空循环，等待硬件看门狗复位
    }
}













/*end-------------------------------------------------------------------------*/