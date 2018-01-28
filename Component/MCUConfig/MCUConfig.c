/*******************************************************************************
* @file      : MCUConfig.c
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
#include <stdio.h>
#include "ht6xxx.h"



/*宏定义----------------------------------------------------------------------*/
///添加宏定义
#define	C_TaskTick      64u                             //任务定时节拍频率（64Hz）
#define	C_SysTickLoad   ((11010048 / C_TaskTick) - 1)   //系统节拍周期



/*内部变量声明----------------------------------------------------------------*/
///添加内部变量




/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数






/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数





/*定义全局函数----------------------------------------------------------------*/
///定义可用于外部文件的函数

/** 
 * @brief   空指令函数
 * @note   
 * @retval None
 */
extern void NOP(void)
{
    __NOP();      //IAR是在core_cmInstr.h文件中有定义
}

/** 
 * @brief  使能总中断函数
 * @note   
 * @retval None
 */
extern void Enable_Int(void)
{
    __enable_irq();      //IAR是在core_cmFunc.h文件中有定义
}

/** 
 * @brief  禁止总中断函数
 * @note   
 * @retval None
 */
extern void Disable_Int(void)
{
    __disable_irq();      //IAR是在core_cmFunc.h文件中有定义
}

/** 
 * @brief  关闭写保护寄存器函数
 * @note   关闭写保护功能，用户可以写操作被保护的寄存器
 * @retval None
 */
extern void EnWr_WPREG(void)
{
    HT_CMU->WPREG = 0xA55A;      //在ht6xxx.h文件中有定义
}

/** 
 * @brief  开启写保护寄存器函数
 * @note   开启写保护功能，用户禁止写操作被保护的寄存器
 * @retval None
 */
extern void DisWr_WPREG(void)
{
    HT_CMU->WPREG = 0xA55A;      //在ht6xxx.h文件中有定义
}

/** 
 * @brief  进入sleep模式函数
 * @note   
 * @retval None
 */
extern void DisWr_WPREG(void)
{
    SCB->SCR = 0x0004;  
    __WFI();            //IAR是在core_cmInstr.h文件中有定义
    __NOP();            //IAR是在core_cmInstr.h文件中有定义
}

/** 
 * @brief  清硬件看门狗函数
 * @note   
 * @retval None
 */
extern void DisWr_WPREG(void)
{
    HT_WDT->WDTSET = 0xAA3F;        //在ht6xxx.h文件中有定义
}
/** 
 * @brief   初始化系统定时器
 * @note    定时周期跟C_SysTickLoad，和系统时钟频率相关
 * @retval None
 */
void Init_SysTick(void)
{ 
    SysTick->CTRL = 0x00000000;
    SysTick->LOAD = C_SysTickLoad;
    SysTick->VAL  = 0x00000000;
    NVIC_SetPriority (SysTick_IRQn, 3);
    SysTick->CTRL = 0x00000007;
}


/** 
 * @brief  运行系统定时器
 * @note   
 * @retval None
 */
void Run_SysTick(void)
{ 
    if (((SysTick->CTRL & 0x00000007) != 0x00000007)
    || ((SysTick->LOAD & 0x00FFFFFF) != C_SysTickLoad))
    {
        SysTick->CTRL = 0x00000000;
        SysTick->LOAD = C_SysTickLoad;
        SysTick->VAL  = 0x00000000;
        NVIC_SetPriority(SysTick_IRQn, 3);
        SysTick->CTRL = 0x00000007;
    }
}


/** 
 * @brief  停止系统定时器
 * @note   
 * @retval None
 */
void Stop_SysTick(void)
{ 
    SysTick->CTRL = 0x00000000;
}

/** 
 * @brief  切换到内部低速RC时钟函数
 * @note   32768Hz
 * @retval None
 */
void SwitchTo_Flrc(void)
{
    if ((HT_CMU->SYSCLKCFG != 0x0000)
    || (HT_CMU->LRCADJ    != 0x0009)
    || (HT_CMU->SYSCLKDIV != 0x0001))
    {
        EnWr_WPREG();
        HT_CMU->LRCADJ    = 0x0009;             //输出32768Hz
        HT_CMU->SYSCLKDIV = 0x0001;             //Fcpu = Fsys /2
        
        HT_CMU->SYSCLKCFG = 0x0080;             //Fsys = Flrc
        NOP();
        HT_CMU->SYSCLKCFG = 0x0000;             //Fsys = Flrc

        HT_CMU->CLKCTRL0 &= ~0x0030;            //LCD
        DisWr_WPREG();
    }
}

/** 
 * @brief   切换到内部高速RC时钟函数
 * @note   3.5MHz
 * @retval None
 */
void SwitchTo_Fhrc(void)
{
    if ((HT_CMU->SYSCLKCFG != 0x0002)
    || (HT_CMU->HRCADJ    != 0x003D)
    || (HT_CMU->HRCDIV    != 0x0001)
    || !(HT_CMU->CLKCTRL0 & 0x0020)
    || (HT_CMU->SYSCLKDIV != 0x0001))
    {
        EnWr_WPREG();
        HT_CMU->HRCADJ    = 0x003D;             //输出8MHz
        HT_CMU->HRCDIV    = 0x0001;             //Fhrc' = Fhrc /2 = 7MHz
        HT_CMU->CLKCTRL0 |= 0x0020;             //使能HRC
        HT_CMU->SYSCLKDIV = 0x0001;             //Fcpu  = Fsys /2 = 3.5MHz
        while (HT_CMU->CLKSTA & 0x0008)
        {
            ;
        }

        HT_CMU->SYSCLKCFG = 0x0082;             //Fsys = Fhrc'
        NOP();
        HT_CMU->SYSCLKCFG = 0x0002;             //Fsys = Fhrc'
        DisWr_WPREG();
    }
}

/** 
 * @brief  切换到内部pll函数
 * @note   
 * @retval Fsys=22MHz，Fcpu=11.010048MHz
 */
void SwitchTo_Fpll(void)
{
    if ((HT_CMU->SYSCLKCFG != 0x0003)
    || !(HT_CMU->CLKCTRL0 & 0x0010)             //使能PLL
    || (HT_CMU->SYSCLKDIV != 0x0001))
    {
        EnWr_WPREG();
        HT_CMU->CLKCTRL0 |= 0x0010;             //使能PLL
        HT_CMU->SYSCLKDIV = 0x0001;             //Fcpu = Fsys/2 = Fpll/2 = 11.010048MHz
        while (HT_CMU->CLKSTA & 0x0010)
        {
            ;
        }
        HT_CMU->SYSCLKCFG = 0x0083;             //Fsys = Fpll
        NOP();
        HT_CMU->SYSCLKCFG = 0x0003;             //Fsys = Fpll
        DisWr_WPREG();
    }
}


/** 
 * @brief  开启UART0接收端
 * @note   
 * @param  baud:    0： 600bps
                    1：1200bps
                    2：2400bps
                    3：4800bps
                    4：9600bps
 * @retval None
 */
void OpenRx_UART0(unsigned char baud)
{
    NVIC_DisableIRQ(UART0_IRQn);                //禁止串口中断
    HT_UART0->MODESEL = 0x0000;                 //UART功能
    HT_UART0->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART0->UARTCON = 0x005A;                 //正逻辑,8位数据,偶校验,1停止位,接收使能
    HT_UART0->IRCON   = 0x0000;                 //非调制
    HT_UART0->SREL    = (18350>>baud)-1;        //600;1200;2400;4800;9600;19200;
    NVIC_ClearPendingIRQ(UART0_IRQn);           //清除挂起状态
    NVIC_SetPriority(UART0_IRQn, 3);            //设置优先级
    NVIC_EnableIRQ(UART0_IRQn);                 //使能串口中断
}


/** 
 * @brief  开启UART0发送端
 * @note   
 * @param  head: 首字节
 * @retval None
 */
void OpenTx_UART0(unsigned char head)
{
    NVIC_DisableIRQ(UART0_IRQn);                //禁止串口中断
    HT_UART0->MODESEL = 0x0000;                 //UART功能
    HT_UART0->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART0->UARTCON = 0x0055;                 //正逻辑,8位数据,偶校验,1停止位,发送使能
    NVIC_ClearPendingIRQ(UART0_IRQn);           //清除挂起状态
    NVIC_SetPriority(UART0_IRQn, 3);            //设置优先级
    NVIC_EnableIRQ(UART0_IRQn);                 //使能串口中断
    HT_UART0->SBUF    = head;
}

/** 
 * @brief  关闭UART0串口
 * @note   
 * @retval None
 */
void Close_UART0(void)
{
    NVIC_DisableIRQ(UART0_IRQn);                //禁止串口中断
    HT_UART0->UARTCON = 0x0000;                 //禁止串口功能
}


/** 
 * @brief  检查UART0接收端
 * @note   
 * @retval TRUE:    接收端处于开启状态
 *         FALSE：  接收端处于关闭状态
 */
Bool IsRxing_UART0(void)
{
    NVIC_EnableIRQ(UART0_IRQn);                 //使能串口中断
    return (((HT_UART0->MODESEL==0x0000) && (HT_UART0->UARTCON==0x005A))? TRUE: FALSE);
}


/** 
 * @brief  开启UART1接收端
 * @note   红外
 * @param  baud:    0:  600bps
 *                  1：1200bps
                    2：2400bps
                    3：4800bps
                    4：9600bps
 * @retval None
 */
void OpenRx_UART1(unsigned char baud)
{
    baud    = baud;
    NVIC_DisableIRQ(UART1_IRQn);                //禁止串口中断
    HT_UART1->MODESEL = 0x0000;                 //UART功能
    HT_UART1->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART1->UARTCON = 0x005A;                 //正逻辑,8位数据,偶校验,1停止位,接收使能
    HT_UART1->IRCON   = 0x0001;                 //红外调制
    HT_UART1->IRDUTY  = 0x0000;                 //50%占空比
    HT_UART1->SREL    = 0x23D6;                 //1200bps
    NVIC_ClearPendingIRQ(UART1_IRQn);           //清除挂起状态
    NVIC_SetPriority(UART1_IRQn, 3);            //设置优先级
    NVIC_EnableIRQ(UART1_IRQn);                 //使能串口中断
}


/** 
 * @brief  开启UART1发送端
 * @note   红外
 * @param  head: 首字节
 * @retval None
 */
void OpenTx_UART1(unsigned char head)
{
    NVIC_DisableIRQ(UART1_IRQn);                //禁止串口中断
    HT_UART1->MODESEL = 0x0000;                 //UART功能
    HT_UART1->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART1->UARTCON = 0x0055;                 //正逻辑,8位数据,偶校验,1停止位,发送使能
    HT_UART1->IRCON   = 0x0001;                 //红外调制
    HT_UART1->IRDUTY  = 0x0000;                 //50%占空比
    NVIC_ClearPendingIRQ(UART1_IRQn);           //清除挂起状态
    NVIC_SetPriority(UART1_IRQn, 3);            //设置优先级
    NVIC_EnableIRQ(UART1_IRQn);                 //使能串口中断
    HT_UART1->SBUF    = head;
}

/** 
 * @brief  关闭UART1模块
 * @note   红外
 * @retval None
 */
void Close_UART1(void)
{
    NVIC_DisableIRQ(UART1_IRQn);                //禁止串口中断
    HT_UART1->UARTCON = 0x0000;                 //禁止串口功能
}


/** 
 * @brief  检查UART1接收端
 * @note   
 * @retval TRUE:    接收端处于开启状态
 *         FALSE：  接收端处于关闭状态
 */
Bool IsRxing_UART1(void)
{
    NVIC_EnableIRQ(UART1_IRQn);                 //使能串口中断
    return (((HT_UART1->MODESEL==0x0000) && (HT_UART1->UARTCON==0x005A))? TRUE: FALSE);
}


/** 
 * @brief  开启UART2接收端
 * @note   
 * @param  baud: 0:  600bps
 *               1: 1200bps
 *               2: 2400bps
 *               3: 4800bps
 *               4: 9600bps
 * @retval None
 */
void OpenRx_UART2(unsigned char baud)
{
    NVIC_DisableIRQ(UART2_IRQn);                //禁止串口中断
    HT_UART2->MODESEL = 0x0000;                 //UART功能
    HT_UART2->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART2->UARTCON = 0x005A;                 //正逻辑,8位数据,偶校验,1停止位,接收使能
    HT_UART2->IRCON   = 0x0000;                 //非调制
    HT_UART2->SREL    = (18350>>baud)-1;        //600;1200;2400;4800;9600;19200;
    NVIC_ClearPendingIRQ(UART2_IRQn);           //清除挂起状态
    NVIC_SetPriority(UART2_IRQn, 3);            //设置优先级
    NVIC_EnableIRQ(UART2_IRQn);                 //使能串口中断
}


/** 
 * @brief  开启UART2发送端
 * @note   
 * @param  head: 首字节
 * @retval None
 */
void OpenTx_UART2(unsigned char  head)
{
    NVIC_DisableIRQ(UART2_IRQn);                //禁止串口中断
    HT_UART2->MODESEL = 0x0000;                 //UART功能
    HT_UART2->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART2->UARTCON = 0x0055;                 //正逻辑,8位数据,偶校验,1停止位,发送使能
    NVIC_ClearPendingIRQ(UART2_IRQn);           //清除挂起状态
    NVIC_SetPriority(UART2_IRQn, 3);            //设置优先级
    NVIC_EnableIRQ(UART2_IRQn);                 //使能串口中断
    HT_UART2->SBUF    = head;
}


/** 
 * @brief  关闭UART2模块
 * @note   
 * @retval None
 */
void Close_UART2(void)
{
    NVIC_DisableIRQ(UART2_IRQn);                //禁止串口中断
    HT_UART2->UARTCON = 0x0000;                 //禁止串口功能
}


/** 
 * @brief  检查UART2接收端
 * @note   
 * @retval TRUE:    接收端处于开启状态
 *         FALSE：  接收端处于关闭状态
 */
Bool IsRxing_UART2(void)
{
    NVIC_EnableIRQ(UART2_IRQn);                 //使能串口中断
    return (((HT_UART2->MODESEL==0x0000) && (HT_UART2->UARTCON==0x005A))? TRUE: FALSE);
}


/** 
 * @brief  开启UART3接收端
 * @note   7816功能
 * @retval None
 */
void OpenRx_UART3(void)
{
    NVIC_DisableIRQ(UART3_IRQn);                //禁止串口中断
    HT_UART3->MODESEL    = 0x0001;              //7816功能
    HT_UART3->ISO7816STA = 0x0000;              //清除中断标志位
    HT_UART3->ISO7816CON = 0x01C0;              //偶校验,重收重发,重复2次,响应1位,禁止中断
    HT_UART3->SREL       = 0x045B;              //9866bps
}


/** 
 * @brief  开启UART3发送端
 * @note   7816功能
 * @param  head: 首字节
 * @retval None
 */
void OpenTx_UART3(unsigned char head)
{
    NVIC_DisableIRQ(UART3_IRQn);                //禁止串口中断
    HT_UART3->MODESEL    = 0x0001;              //7816功能
    HT_UART3->ISO7816STA = 0x0000;              //清除中断标志位
    HT_UART3->ISO7816CON = 0x01C0;              //偶校验,重收重发,重复2次,响应1位,禁止中断
    HT_UART3->SREL       = 0x045B;              //9866bps
    HT_UART3->SBUF       = head;
}



/** 
 * @brief  关闭UART3模块
 * @note   7816功能
 * @retval None
 */
void Close_UART3(void)
{
    NVIC_DisableIRQ(UART3_IRQn);                //禁止串口中断
    HT_UART3->ISO7816CON = 0x0000;              //禁止串口功能
}



/** 
 * @brief  开启UART4接收端
 * @note   ESAM
 * @retval None
 */
void OpenRx_UART4(void)
{
    NVIC_DisableIRQ(UART4_IRQn);                //禁止串口中断
    HT_UART4->MODESEL	 = 0x0001;              //7816功能
    HT_UART4->ISO7816STA = 0x0000;              //清除中断标志位
    HT_UART4->ISO7816CON = 0x01C0;              //偶校验,重收重发,重复2次,响应1位,禁止中断
    HT_UART4->SREL       = 0x045B;              //9866bps
}


/** 
 * @brief  开启UART4发送端
 * @note   ESAM
 * @param  head: 首字节
 * @retval None
 */
void OpenTx_UART4(INT8U head)
{
    NVIC_DisableIRQ(UART4_IRQn);                //禁止串口中断
    HT_UART4->MODESEL	 = 0x0001;              //7816功能
    HT_UART4->ISO7816STA = 0x0000;              //清除中断标志位
    HT_UART4->ISO7816CON = 0x01C0;              //偶校验,重收重发,重复2次,响应1位,禁止中断
    HT_UART4->SREL       = 0x045B;              //9866bps
    HT_UART4->SBUF       = head;
}


/** 
 * @brief  关闭UART4模块
 * @note   ESAM
 * @retval None
 */
void Close_UART4(void)
{
    NVIC_DisableIRQ(UART4_IRQn);                //禁止串口中断
    HT_UART4->ISO7816CON = 0x0000;              //禁止串口功能
}


/** 
 * @brief  开启UART5模块
 * @note   
 * @param  head: 首字节
 * @retval None
 */
void Open_UART5(unsigned char head)
{
    NVIC_DisableIRQ(UART5_IRQn);                //禁止串口中断
    HT_UART5->MODESEL = 0x0000;                 //UART功能
    HT_UART5->UARTSTA = 0x0000;                 //清除中断标志位
    HT_UART5->UARTCON = 0x0053;                 //正逻辑,8位数据,偶校验,1停止位,发送,接收使能
    HT_UART5->IRCON   = 0x0000;                 //非调制
    HT_UART5->SREL    = 0x047A;                 //1200:0x23D6; 2400:0x11EB; 4800:0x08F5; 9600:0x047A; 19200:0x023C; 38400:0x011E
    HT_UART5->SBUF    = head;
}


/** 
 * @brief  关闭UART5模块
 * @note   EMU
 * @retval None
 */
void Close_UART5(void)
{
    NVIC_DisableIRQ(UART5_IRQn);                //禁止串口中断
    HT_UART5->UARTCON = 0x0000;                 //禁止串口功能
}

/** 
 * @brief  读取电池电压ADC采样值
 * @note   
 * @retval 电池电压AD值
 */
signed short ADC_BattVolt(void)
{
    return HT_TBS->VBATDAT;
}

/** 
 * @brief  读取芯片温度ADC采样值
 * @note   
 * @retval 芯片温度AD值
 */
signed short ADC_TempVolt(void)
{
    return HT_TBS->TMPDAT;
}


/** 
 * @brief  测试外部电源
 * @note   上电检测，比较VCC电压4.6V
 * @param  times: 比较次数
 * @retval TRUE： 电压正常
 *         FALSE: 电压异常
 */
Bool Check_PowerOn(unsigned long times)
{
    NVIC_DisableIRQ(PMU_IRQn);                  //禁止PMU中断
    HT_PMU->PMUIE   = 0x0000;                   //禁止PMU中断
    HT_PMU->VDETCFG = 0x006D;                   //比较VCC电压4.6V
    EnWr_WPREG();
    HT_PMU->PMUCON  = 0x0003;                   //关闭LVD_DET,开启BOR_DET,BOR复位模式
    DisWr_WPREG();
    times <<= 5;
    for (; times>0; times--)
    {
        Delay_mSec(1);                          //延时1ms
        if (0x0001 != (HT_PMU->PMUSTA & 0x0001))
        {
            break;
        }
    }
    return ((times==0)? TRUE:FALSE);
}

/** 
 * @brief  开启LCD显示模块
 * @note   
 * @retval None
 */
void Open_LCD(void)
{
    if ((HT_CMU->CLKCTRL0 & 0x0002) != 0x0002)	//使能LCD时钟
    {
        EnWr_WPREG();
        HT_CMU->CLKCTRL0 |= 0x0002;
        DisWr_WPREG();
    }
    HT_LCD->LCDCLK = 0x008C;                    //1/3偏压,6COM,85.3Hz
    HT_LCD->LCDCON = 0x0007;                    //对比度=71.37%VLCD
}

/** 
 * @brief  装载InfoData数据
 * @note   
 * @retval 
 */
Bool Load_InfoData(void)
{
    unsigned char	i;
    unsigned long	chksum = 0;

    for (i=0; i<14; i++)
    {
        chksum += HT_INFO->DataArry[i];
    }
    if (chksum != HT_INFO->DataArry[14])//未设置
    {
        return FALSE;
    }
    else//已设置
    {
        if (HT_RTC->DFAH    != HT_INFO->Muster.iDFAH)   HT_RTC->DFAH    = HT_INFO->Muster.iDFAH;
        if (HT_RTC->DFAL    != HT_INFO->Muster.iDFAL)   HT_RTC->DFAL    = HT_INFO->Muster.iDFAL;
        if (HT_RTC->DFBH    != HT_INFO->Muster.iDFBH)   HT_RTC->DFBH    = HT_INFO->Muster.iDFBH;
        if (HT_RTC->DFBL    != HT_INFO->Muster.iDFBL)   HT_RTC->DFBL    = HT_INFO->Muster.iDFBL;
        if (HT_RTC->DFCH    != HT_INFO->Muster.iDFCH)   HT_RTC->DFCH    = HT_INFO->Muster.iDFCH;
        if (HT_RTC->DFCL    != HT_INFO->Muster.iDFCL)   HT_RTC->DFCL    = HT_INFO->Muster.iDFCL;
        if (HT_RTC->DFDH    != HT_INFO->Muster.iDFDH)   HT_RTC->DFDH    = HT_INFO->Muster.iDFDH;
        if (HT_RTC->DFDL    != HT_INFO->Muster.iDFDL)   HT_RTC->DFDL    = HT_INFO->Muster.iDFDL;
        if (HT_RTC->DFEH    != HT_INFO->Muster.iDFEH)   HT_RTC->DFEH    = HT_INFO->Muster.iDFEH;
        if (HT_RTC->DFEL    != HT_INFO->Muster.iDFEL)   HT_RTC->DFEL    = HT_INFO->Muster.iDFEL;
        if (HT_RTC->Toff    != HT_INFO->Muster.iToff)   HT_RTC->Toff    = HT_INFO->Muster.iToff;
        if (HT_RTC->MCON01  != HT_INFO->Muster.iMCON01) HT_RTC->MCON01  = HT_INFO->Muster.iMCON01;
        if (HT_RTC->MCON23  != HT_INFO->Muster.iMCON23) HT_RTC->MCON23  = HT_INFO->Muster.iMCON23;
        if (HT_RTC->MCON45  != HT_INFO->Muster.iMCON45) HT_RTC->MCON45  = HT_INFO->Muster.iMCON45;
        return TRUE;
    }
}



/** 
 * @brief  写入InfoData数据
 * @note   
 * @param  adj: 
 * @retval None
 */
void Write_InfoData(signed short adj)
{
    unsigned char	i;
    unsigned long	info[15];
    signed long	tmp;

    //读出InfoData
    for (i=0; i<14; i++)
    {
        info[i] = HT_INFO->DataArry[i];
    }
    if (info[0] & 0x0040)
    {
        info[0] |= 0xFF80;
    }
    tmp  = info[0]<<16 | info[1];
    tmp += adj;
    info[0]  = tmp >> 16;
    info[0] &= 0x007F;
    info[1]  = tmp & 0xFFFF;
    info[14] = 0;
    for (i=0; i<14; i++)
    {
        info[14] += info[i];
    }
    //擦除InfoData
    EnWr_WPREG();
    HT_CMU->FLASHLOCK = 0x7A68;                             //Flash unlock
    HT_CMU->INFOLOCK  = 0xF998;
    HT_CMU->FLASHCON  = 0x02;                               //擦除
    *((unsigned long *)(HT_INFO_BASE-4)) = 0xFF;
    while (HT_CMU->FLASHCON & 0x04)                         //FLASH_BSY
    ;

    HT_CMU->FLASHCON  = 0x00;
    Delay_mSec(2);
    //更新InfoData
    HT_CMU->FLASHCON  = 0x01;
    for (i=0; i<15; i++)
    {
        *((unsigned long *)(HT_INFO_BASE+i*4)) = info[i];   //ProgramWord
        while (HT_CMU->FLASHCON  & 0x04)                    //FLASH_BSY
        ;
    }
    HT_CMU->FLASHLOCK = ~0x7A68;                            //FLASH锁定,禁止操作FALSH
    HT_CMU->INFOLOCK  = ~0xF998;                            //INFOFLASH锁定,禁止操作INFOFALSH
    HT_CMU->FLASHCON = 0x00;
    DisWr_WPREG();
}

/*end-------------------------------------------------------------------------*/