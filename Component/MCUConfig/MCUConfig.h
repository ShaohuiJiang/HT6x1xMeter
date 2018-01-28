/*******************************************************************************
* @file      : MCUConfig.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Jan 28 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __MCUCONFIG_H
#define __MCUCONFIG_H

/*头文件----------------------------------------------------------------------*/
///添加头文件





/*宏定义----------------------------------------------------------------------*/
///添加宏定义



/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void NOP(void);                              //空指令函数
extern void Enable_Int(void);                       //开全局中断
extern void Disable_Int(void);                      //关闭全局中断
extern void EnWr_WPREG(void);                       //HT6XXX芯片中关闭写保护寄存器
extern void DisWr_WPREG(void);                      //HT6XXX芯片中开启写保护寄存器
extern void Goto_Sleep(void);                       //关闭休眠模式
extern void Feed_WDT(void);                         //清看门狗
extern void Init_SysTick(void);                     //初始化系统定时器
extern void Run_SysTick(void);                      //运行系统定时器运行
extern void Stop_SysTick(void);                     //停止系统定时器
extern void SwitchTo_Flrc(void);                    //切换到内部低速RC时钟
extern void SwitchTo_Fhrc(void);                    //切换到外部高速RC时钟
extern void SwitchTo_Fpll(void);                    //切换到内部PLL时钟
extern void OpenRx_UART0(unsigned char baud);       //打开UART0接收端
extern void OpenTx_UART0(unsigned char head);       //打开UART0发送端
extern void Close_UART0(void);                      //关闭UART0
extern Bool IsRxing_UART0(void);                    //检测UART0接收端
extern void OpenRx_UART1(unsigned char baud);       //打开UART1接收端
extern void OpenTx_UART1(unsigned char head);       //打开UART1发送端
extern void Close_UART1(void);                      //关闭UART1
extern Bool IsRxing_UART1(void);                    //检测UART1接收端
extern void OpenRx_UART2(unsigned char baud);       //打开UART2接收端
extern void OpenTx_UART2(unsigned char  head);      //打开UART2发送端
extern void Close_UART2(void);                      //关闭UART2
extern Bool IsRxing_UART2(void);                    //检测UART2接收端
extern void OpenRx_UART3(void);                     //打开UART3接收端
extern void OpenTx_UART3(unsigned char head);       //打开UART3发送端
extern void Close_UART3(void);                      //关闭UART3
extern void OpenRx_UART4(void);                     //打开UART4接收端
extern void OpenTx_UART4(unsigned char head);       //打开UART4发送端
extern void Close_UART4(void);                      //关闭UART4
extern void Open_UART5(unsigned char head);         //打开UART5
extern void Close_UART5(void);                      //关闭UART5
extern signed short ADC_BattVolt(void);             //读取电池检测AD值
extern signed short ADC_TempVolt(void);             //读取芯片温度检测AD值
extern Bool Check_PowerOn(unsigned long times);     //检测外部电源状态
extern void Open_LCD(void);                         //打开LCD外设
extern Bool Load_InfoData(void);                    //读日计时补偿参数
extern void Write_InfoData(signed short adj);       //写日计时补偿参数


#endif
/*end------------------------------------------------------------------------*/