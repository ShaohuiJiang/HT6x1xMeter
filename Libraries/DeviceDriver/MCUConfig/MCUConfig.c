/*******************************************************************************
* @file      : MCUConfig.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Jan 28 2018
* @brief     : 
********************************************************************************
* @attention :
*     该文件主要是封装MCU的寄存器，让Device_Driver文件夹下除Message组件外的其他驱动
* 组件不需要再去直接调用寄存器，便于以后移植平台时减少工作量。
* 
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "MCUConfig.h"
#include "General.h"
#include "TypeDef.h"

/*宏定义----------------------------------------------------------------------*/
///添加宏定义




/*内部变量声明----------------------------------------------------------------*/
///添加内部变量
static Bool b_RTCCalibrationState;   //RTC补偿校准状态标志位，TRUE：已补偿校准状态；FALSE：未补偿校准状态


/*声明内部函数----------------------------------------------------------------*/
///声明只在本文件使用的函数
static void SwitchTo_Flrc(void);                    //切换到内部低速RC时钟
static void SwitchTo_Fhrc(void);                    //切换到外部高速RC时钟
static void SwitchTo_Fpll(void);                    //切换到内部PLL时钟

/*定义内部函数----------------------------------------------------------------*/
///定义只能在本文件使用的函数
/** 
 * @brief  切换到内部低速RC时钟函数
 * @note   32768Hz
 * @retval None
 */
static void SwitchTo_Flrc(void)
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

        HT_CMU->CLKCTRL0 &= ~0x0030;            //确保PLL和HRC时钟关闭
        DisWr_WPREG();
    }
}

/** 
 * @brief   切换到内部高速RC时钟函数
 * @note   3.5MHz
 * @retval None
 */
static void SwitchTo_Fhrc(void)
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
static void SwitchTo_Fpll(void)
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
extern void Goto_Sleep(void)
{
    SCB->SCR = 0x0004;  
    __WFI();            //IAR是在core_cmInstr.h文件中有定义
    __NOP();            //IAR是在core_cmInstr.h文件中有定义
}

/** 
 * @brief  进入Hold模式函数
 * @note   
 * @retval None
 */
extern void Goto_Hold(void)
{
    SCB->SCR = 0;
    __WFI();            //IAR是在core_cmInstr.h文件中有定义
    __NOP();            //IAR是在core_cmInstr.h文件中有定义
}
/** 
 * @brief  清硬件看门狗函数
 * @note   
 * @retval None
 */
extern void Feed_WDT(void)
{
    HT_WDT->WDTSET = 0xAA3F;        //在ht6xxx.h文件中有定义
}

/** 
 * @brief  等待硬件看门狗复位
 * @note   
 * @retval None
 */
extern void WDTReset_MCU(void)
{
    Disable_Int();  //关闭全局中断
    HT_WDT->WDTSET = 0xAA00;
    while(1)
    {
       NOP(); 
    }
}

/** 
 * @brief   初始化系统定时器
 * @note    定时周期跟C_SysTickLoad，和系统时钟频率相关
 * @retval None
 */
extern void Init_SysTick(void)
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
extern void Run_SysTick(void)
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
extern void Stop_SysTick(void)
{ 
    SysTick->CTRL = 0x00000000;
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
extern void OpenRx_UART0(unsigned char baud)
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
extern void OpenTx_UART0(unsigned char head)
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
extern void Close_UART0(void)
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
extern Bool IsRxing_UART0(void)
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
extern void OpenRx_UART1(unsigned char baud)
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
extern void OpenTx_UART1(unsigned char head)
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
extern void Close_UART1(void)
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
extern Bool IsRxing_UART1(void)
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
extern void OpenRx_UART2(unsigned char baud)
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
extern void OpenTx_UART2(unsigned char  head)
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
extern void Close_UART2(void)
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
extern Bool IsRxing_UART2(void)
{
    NVIC_EnableIRQ(UART2_IRQn);                 //使能串口中断
    return (((HT_UART2->MODESEL==0x0000) && (HT_UART2->UARTCON==0x005A))? TRUE: FALSE);
}


/** 
 * @brief  开启UART3接收端
 * @note   7816功能
 * @retval None
 */
extern void OpenRx_UART3(void)
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
extern void OpenTx_UART3(unsigned char head)
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
extern void Close_UART3(void)
{
    NVIC_DisableIRQ(UART3_IRQn);                //禁止串口中断
    HT_UART3->ISO7816CON = 0x0000;              //禁止串口功能
}



/** 
 * @brief  开启UART4接收端
 * @note   ESAM
 * @retval None
 */
extern void OpenRx_UART4(void)
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
extern void OpenTx_UART4(unsigned char  head)
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
extern void Close_UART4(void)
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
extern void Open_UART5(unsigned char head)
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
extern void Close_UART5(void)
{
    NVIC_DisableIRQ(UART5_IRQn);                //禁止串口中断
    HT_UART5->UARTCON = 0x0000;                 //禁止串口功能
}

/** 
 * @brief  读取电池电压ADC采样值
 * @note   
 * @retval 电池电压AD值
 */
extern signed short ADC_BattVolt(void)
{
    return HT_TBS->VBATDAT;
}

/** 
 * @brief  读取芯片温度ADC采样值
 * @note   
 * @retval 芯片温度AD值
 */
extern signed short ADC_TempVolt(void)
{
    return HT_TBS->TMPDAT;
}




/** 
 * @brief  开启LCD显示模块
 * @note   
 * @retval None
 */
extern void Open_LCD(void)
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
extern Bool Load_InfoData(void)
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
extern void Write_InfoData(signed short adj)
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


/** 
 * @brief   MCU电池工作状态初始化
 * @note   
 * @retval None
 */
extern void Init_MCU_BatteryState(void)
{
    Feed_WDT();                     //清看门狗
    SwitchTo_Fhrc();                //切换到3.5MHz

    /* MCU的存储器模块初始化 */
    EnWr_WPREG();                   //关闭写保护
    HT_CMU->FLASHLOCK = ~0x7A68;    //启用Flash写保护
    DisWr_WPREG();                  //开启写保护

    /* MCU的时钟模块初始化 */
    EnWr_WPREG();
    HT_CMU->CLKCTRL0 = 0X2C60;      //HRC时钟使能、OSC低功耗使能(手册上要求打开)、硬件看门狗时钟使能、LDO内部LBOR使能(手册上要求打开)
    HT_CMU->CLKCTRL1 = 0;           //UART0~5和TIME0~3都时钟关闭
    DisWr_WPREG();

    /* MCU的电源模块初始化 */
    NVIC_DisableIRQ(PMU_IRQn);      //禁止PMU中断
    EnWr_WPREG();
    HT_PMU->PMUCON  = 0x0013;       //开启BOR_DET,BOR复位模式，关闭LVDIN_DET模块，打开Hold模式下大功耗LDO（先试试打开，如果功耗不满意，再关闭）
    DisWr_WPREG();
    HT_PMU->VDETCFG = 0x006D;       //VCC检测阈值=4.6V;BOR检测阈值=2.2V
    HT_PMU->VDETPCFG= 0x0022;       //Hold和Sleep模式下VCC_DET,BOR_DET检测时间=300us,周期=67ms
    HT_PMU->PMUIF   = 0x0000;       //清中断标志
    HT_PMU->PMUIE   = 0x0001;       //使能VCC检测中断，关闭BOR检测中断，关闭LVDIN检测中断
    NVIC_ClearPendingIRQ(PMU_IRQn); //清除挂起状态
    NVIC_SetPriority(PMU_IRQn, 3);  //设置优先级
    NVIC_EnableIRQ(PMU_IRQn);       //使能PMU中断

    /* MCU的GPIO初始化 */
    EnWr_WPREG();
    HT_GPIOA->IOCFG = 0x0520;       //PA5->INT0;PA8->INT3;PA10->INT5
    HT_GPIOA->AFCFG = 0x0000;       //功能1
    DisWr_WPREG();
    HT_GPIOA->PTSET = 0x2017;
//  HT_GPIOA->PTCLR = 0x0000;
    HT_GPIOA->PTOD  = 0x1FE8;
    HT_GPIOA->PTUP  = 0x253F;
    HT_GPIOA->PTDIR = 0x2017;

    EnWr_WPREG();
    HT_GPIOB->IOCFG = 0xF7FF;      //PB[0:10]->SEG[0:10];PB[12:15]->SEG[12:15]
    HT_GPIOB->AFCFG = 0x0000;      //功能1
    DisWr_WPREG();
    HT_GPIOB->PTSET = 0x0800;
    HT_GPIOB->PTOD  = 0xF7FF;
    HT_GPIOB->PTUP  = 0xFFFF;
    HT_GPIOB->PTDIR = 0x0800;

    EnWr_WPREG();
    HT_GPIOC->IOCFG = 0x0000;       //
    HT_GPIOC->AFCFG = 0x0000;       //功能1
    DisWr_WPREG();
    HT_GPIOC->PTSET = 0x4929;
    HT_GPIOC->PTCLR = 0x0640;
    HT_GPIOC->PTOD  = 0x3696;
    HT_GPIOC->PTUP  = 0x7F7F;
    HT_GPIOC->PTDIR = 0x0F69;

    EnWr_WPREG();
    HT_GPIOD->IOCFG = 0x3FFF;       //PD[0-7]->SEG[16:23];PD[8-13]->COM[0:5]
    HT_GPIOD->AFCFG = 0x0000;       //功能1,2
    DisWr_WPREG();
    HT_GPIOD->PTOD = 0xBFFF;        //开漏
    HT_GPIOD->PTUP  = 0xFFFF;
    HT_GPIOD->PTDIR = 0x4000;
    
    EnWr_WPREG();
    HT_GPIOE->IOCFG = 0x0000;       //PE[7]->LVDIN
    HT_GPIOE->AFCFG = 0x0000;       //功能1
    DisWr_WPREG();
    HT_GPIOE->PTSET = 0x002E;
    HT_GPIOE->PTOD  = 0x01D1;       //不开漏
    HT_GPIOE->PTUP  = 0x006F;
    HT_GPIOE->PTDIR = 0x002E;

    /* MCU中断模块的初始化 */
    NVIC_DisableIRQ(EXTI0_IRQn);        //禁止INT0中断
    NVIC_DisableIRQ(EXTI3_IRQn);        //禁止INT3中断
    NVIC_DisableIRQ(EXTI5_IRQn);        //禁止INT5中断
    HT_INT->PINFLT  = 0x0000;           //INT[0,3,5]引脚数字滤波使能
    HT_INT->EXTIF   = 0x0000;           //清中断标志
    HT_INT->EXTIE   = 0x0928;           //INT0(Pulse)上;INT3(CoverKey)上下;INT5(DispKey)下,沿中断使能
    NVIC_ClearPendingIRQ(EXTI0_IRQn);   //清除挂起状态
    NVIC_ClearPendingIRQ(EXTI3_IRQn);   //清除挂起状态
    NVIC_ClearPendingIRQ(EXTI5_IRQn);   //清除挂起状态
    NVIC_SetPriority(EXTI0_IRQn, 3);    //设置优先级
    NVIC_SetPriority(EXTI3_IRQn, 3);    //设置优先级
    NVIC_SetPriority(EXTI5_IRQn, 3);    //设置优先级
    NVIC_EnableIRQ(EXTI0_IRQn);         //使能INT2中断
    NVIC_EnableIRQ(EXTI3_IRQn);         //使能INT3中断
    NVIC_EnableIRQ(EXTI5_IRQn);         //使能INT5中断
    NVIC->ICER[0]   = ~0x001000A5;      //中断清除使能寄存器（RTC,PMU,INT0,INT3,INT5除外）


    /* MCU的UART/7816模块初始化 */
    //暂时没写

    /* MCU的LCD模块初始化 */
    HT_LCD->LCDCLK	= 0x008C;           //1/3偏压,6COM,85.3Hz
    HT_LCD->LCDCON	= 0x00F1;           //短时大电流，快速充电模式,1/64个Flcd周期，对比度=93.63%VLCD

    /* MCU的硬件看门狗模块初始化 */
    HT_WDT->WDTCFG	= 0x00;             //产生复位


    /* MCU的定时器模块初始化 */
    //暂时没写

    /* MCU的硬件SPI模块初始化 */
    //暂时没写

    /* MCU的硬件I2C模块初始化 */
    //暂时没写

    /* MCU的RTC模块初始化 */
    NVIC_DisableIRQ(RTC_IRQn);                      //禁止RTC中断
    HT_RTC->RTCCON = 0x0000;                        //禁止TOUT输出
    HT_RTC->RTCIF  = 0x0000;                        //清中断标志
    HT_RTC->RTCIE = 0x0003;                         //使能RTC分,秒中断
    NVIC_ClearPendingIRQ(RTC_IRQn);                 //清除挂起状态
    NVIC_SetPriority(RTC_IRQn, 3);                  //设置优先级
    NVIC_EnableIRQ(RTC_IRQn);                       //使能RTC中断
    b_RTCCalibrationState = Load_InfoData();        //装载InfoData数据,并反馈到RTC补偿校准状态标志位

    /* MCU的TBS模块初始化 */
    HT_TBS->TBSCON  = 0x6541;                       //ADC2次平均;只使能温度测量
    HT_TBS->TBSIE   = 0x0000;                       //禁止TBS中断、禁止电池电压检测中断、禁止ADC通道0检测中断、禁止ADC通道1检测中断、禁止VBAT小于VDRCMP中断、禁止电源电压测量中断
    HT_TBS->TBSIF   = 0X0000;                       //清中断标志
    HT_TBS->TBSPRD  = 0x0004;                       //HT601x温度测量周期=8s

    /* Cotex-M0内核系统定时器初始化 */
    Stop_SysTick();                                 //关闭系统定时器
}


/** 
 * @brief  MCU外部电源工作状态初始化
 * @note   
 * @retval None
 */
extern void Init_MCU_ExternalPowerState(void)
{
    Feed_WDT();                     //清看门狗
    SwitchTo_Fpll();                //切换到Fsys=22MHz，Fcpu=11MHz

    /* MCU的存储器模块初始化 */
    EnWr_WPREG();                   //关闭写保护
    HT_CMU->FLASHLOCK = ~0x7A68;    //启用Flash写保护
    DisWr_WPREG();                  //开启写保护

    /* MCU的时钟模块初始化 */
    EnWr_WPREG();
    HT_CMU->CLKCTRL0 = 0x3C52;      //LCD时钟使能、PLL时钟使能、OSC低功耗使能(手册上要求打开)、硬件看门狗时钟使能、CLKOUT使能、LDO内部LBOR使能(手册上要求打开)
    HT_CMU->CLKCTRL1 = 0x03F0;      //使能UART0-5；关闭TMR0-3
    DisWr_WPREG();

    /* MCU的电源模块初始化 */
    NVIC_DisableIRQ(PMU_IRQn);      //禁止PMU中断
    EnWr_WPREG();
    HT_PMU->PMUCON  = 0x0013;       //开启BOR_DET,BOR复位模式，关闭LVDIN_DET模块，打开Hold模式下大功耗LDO（先试试打开，如果功耗不满意，再关闭）
    DisWr_WPREG();
    HT_PMU->VDETCFG = 0x006D;       //VCC检测阈值=4.6V;BOR检测阈值=2.2V
    HT_PMU->VDETPCFG= 0x0022;       //Hold和Sleep模式下VCC_DET,BOR_DET检测时间=300us,周期=67ms
    HT_PMU->PMUIF   = 0x0000;       //清中断标志
    HT_PMU->PMUIE   = 0x0001;       //使能VCC检测中断，关闭BOR检测中断，关闭LVDIN检测中断
    NVIC_ClearPendingIRQ(PMU_IRQn); //清除挂起状态
    NVIC_SetPriority(PMU_IRQn, 3);  //设置优先级
    NVIC_EnableIRQ(PMU_IRQn);       //使能PMU中断

    /* MCU的GPIO初始化 */
    EnWr_WPREG();
    HT_GPIOA->IOCFG = 0x0520;       //PA5->INT0;PA8->INT3;PA10->INT5
    HT_GPIOA->AFCFG = 0x0000;       //功能1
    DisWr_WPREG();
    HT_GPIOA->PTSET = 0x2017;

    HT_GPIOA->PTOD  = 0x1FE8;
    HT_GPIOA->PTUP  = 0x253F;
    HT_GPIOA->PTDIR = 0x2017;

    EnWr_WPREG();
    HT_GPIOB->IOCFG = 0xF7FF;      //PB[0:10]->SEG[0:10];PB[12:15]->SEG[12:15]
    HT_GPIOB->AFCFG = 0x0000;      //功能1
    DisWr_WPREG();
    HT_GPIOB->PTSET = 0x0800;
    HT_GPIOB->PTOD  = 0xF7FF;
    HT_GPIOB->PTUP  = 0xFFFF;
    HT_GPIOB->PTDIR = 0x0800;

    EnWr_WPREG();
    HT_GPIOC->IOCFG = 0x0000;       //
    HT_GPIOC->AFCFG = 0x0000;       //功能1
    DisWr_WPREG();
    HT_GPIOC->PTSET = 0x4929;
    HT_GPIOC->PTCLR = 0x0640;
    HT_GPIOC->PTOD  = 0x3696;
    HT_GPIOC->PTUP  = 0x7F7F;
    HT_GPIOC->PTDIR = 0x0F69;

    EnWr_WPREG();
    HT_GPIOD->IOCFG = 0x3FFF;       //PD[0-7]->SEG[16:23];PD[8-13]->COM[0:5]
    HT_GPIOD->AFCFG = 0x0000;       //功能1,2
    DisWr_WPREG();
    HT_GPIOD->PTOD = 0xBFFF;        //开漏
    HT_GPIOD->PTUP  = 0xFFFF;
    HT_GPIOD->PTDIR = 0x4000;
    
    EnWr_WPREG();
    HT_GPIOE->IOCFG = 0x0000;       //PE[7]->LVDIN
    HT_GPIOE->AFCFG = 0x0000;       //功能1
    DisWr_WPREG();
    HT_GPIOE->PTSET = 0x002E;
    HT_GPIOE->PTOD  = 0x01D1;       //不开漏
    HT_GPIOE->PTUP  = 0x006F;
    HT_GPIOE->PTDIR = 0x002E;

    /* MCU中断模块的初始化 */
    NVIC_DisableIRQ(EXTI0_IRQn);        //禁止INT0中断
    NVIC_DisableIRQ(EXTI3_IRQn);        //禁止INT3中断
    NVIC_DisableIRQ(EXTI5_IRQn);        //禁止INT5中断
    HT_INT->PINFLT  = 0x0000;           //INT[0,3,5]引脚数字滤波使能
    HT_INT->EXTIF   = 0x0000;           //清中断标志
    HT_INT->EXTIE   = 0x0928;           //INT0(Pulse)上;INT3(CoverKey)上下;INT5(DispKey)下,沿中断使能
    NVIC_ClearPendingIRQ(EXTI0_IRQn);   //清除挂起状态
    NVIC_ClearPendingIRQ(EXTI3_IRQn);   //清除挂起状态
    NVIC_ClearPendingIRQ(EXTI5_IRQn);   //清除挂起状态
    NVIC_SetPriority(EXTI0_IRQn, 3);    //设置优先级
    NVIC_SetPriority(EXTI3_IRQn, 3);    //设置优先级
    NVIC_SetPriority(EXTI5_IRQn, 3);    //设置优先级
    NVIC_EnableIRQ(EXTI0_IRQn);         //使能INT2中断
    NVIC_EnableIRQ(EXTI3_IRQn);         //使能INT3中断
    NVIC_EnableIRQ(EXTI5_IRQn);         //使能INT5中断
    NVIC->ICER[0]   = ~0x001000A5;      //中断清除使能寄存器（RTC,PMU,INT0,INT3,INT5除外）


    /* MCU的UART/7816模块初始化 */
    //暂时没写

    /* MCU的LCD模块初始化 */
    HT_LCD->LCDCLK	= 0x008C;           //1/3偏压,6COM,85.3Hz
    HT_LCD->LCDCON	= 0x00F1;           //短时大电流，快速充电模式,1/64个Flcd周期，对比度=93.63%VLCD

    /* MCU的硬件看门狗模块初始化 */
    HT_WDT->WDTCFG	= 0x00;             //产生复位


    /* MCU的定时器模块初始化 */
    //暂时没写

    /* MCU的硬件SPI模块初始化 */
    //暂时没写

    /* MCU的硬件I2C模块初始化 */
    //暂时没写

    /* MCU的RTC模块初始化 */
    NVIC_DisableIRQ(RTC_IRQn);                      //禁止RTC中断
    HT_RTC->RTCCON = 0x0000;                        //禁止TOUT输出
    HT_RTC->RTCIF  = 0x0000;                        //清中断标志
    HT_RTC->RTCIE = 0x0003;                         //使能RTC分,秒中断
    NVIC_ClearPendingIRQ(RTC_IRQn);                 //清除挂起状态
    NVIC_SetPriority(RTC_IRQn, 3);                  //设置优先级
    NVIC_EnableIRQ(RTC_IRQn);                       //使能RTC中断
    b_RTCCalibrationState = Load_InfoData();        //装载InfoData数据,并反馈到RTC补偿校准状态标志位

    /* MCU的TBS模块初始化 */
    HT_TBS->TBSCON  = 0x6541;                       //ADC2次平均;只使能温度测量
    HT_TBS->TBSIE   = 0x0000;                       //禁止TBS中断、禁止电池电压检测中断、禁止ADC通道0检测中断、禁止ADC通道1检测中断、禁止VBAT小于VDRCMP中断、禁止电源电压测量中断
    HT_TBS->TBSIF   = 0X0000;                       //清中断标志
    HT_TBS->TBSPRD  = 0x0004;                       //HT601x温度测量周期=8s

    /* Cotex-M0内核系统定时器初始化 */
    Init_SysTick();                                 //初始化系统定时器

}


/** 
 * @brief   MCU进入Hold模式状态前初始化
 * @note   
 * @retval None
 */
extern void Init_MCU_HoldState(void)
{
    Feed_WDT();                      //清看门狗
    EnWr_WPREG();
    HT_CMU->CLKCTRL0 &= ~0xD3BD;     //确保BIT0、BIT2、BIT3、BIT4、BIT5、BIT7、BIT8、BIT9、BIT12、BIT14、BIT15都关闭
    DisWr_WPREG();

    SwitchTo_Flrc();                 //切换到内部32k时钟
    NOP();
    Feed_WDT();                      //清看门狗
    NOP();
}

/** 
 * @brief  获取RTC补偿校准状态
 * @note   
 * @retval 
 */
extern Bool Get_RTCCalibrationState(void)
{
    return (b_RTCCalibrationState);
}
/** 
 * @brief   设置RTC补偿校准状态
 * @note   
 * @param  state: TRUE：已经校准状态；FLASE：未校准状态
 * @retval None
 */
extern void Set_RTCCalibrationState(Bool state)
{
    b_RTCCalibrationState = state;
}

/** 
 * @brief  上电状态检测MCU寄存器值函数
 * @note   定时运行，确保寄存器值正确
 * @retval None
 */
extern void Maintain_MCU(void)
{


}

/** 
 * @brief  检测外部电源管脚状态 
 * @note   
 * @retval LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_PowerOnPinLevelStatus(void)
{
    if(((HT_GPIOE->IOCFG & BIT7) == BIT7)         //PE7没有配置为GPIO
     ||((HT_GPIOE->PTUP  & BIT7) != BIT7)         //或者PE7没有配置浮空
     ||((HT_GPIOE->PTDIR & BIT7) == BIT7))        //或者PE7没有配置成输入
    {
        EnWr_WPREG();
        HT_GPIOE->IOCFG &= ~BIT7;             //配置为GPIO
        DisWr_WPREG();
        HT_GPIOE->PTUP  |=  BIT7;             //浮空
        HT_GPIOE->PTDIR &= ~BIT7;             //输入脚
    
    }

    if(BIT7 != (HT_GPIOE->PTDAT & BIT7) )        //检测到PE7为低
    {
        return LOW_LEVEL;
    } 
    return HIGH_LEVEL;
}


/** 
 * @brief  检测超级权限管脚状态 
 * @note   
 * @retval LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_SuperAuthorityPinLevelStatus(void)
{
    if(((HT_GPIOA->IOCFG & BIT11) == BIT11)         //PA11没有配置为GPIO
     ||((HT_GPIOA->PTUP  & BIT11) == BIT11)         //或者PA11没有配置上拉
     ||((HT_GPIOA->PTDIR & BIT11) == BIT11))        //或者PA11没有配置成输入
    {
        EnWr_WPREG();
        HT_GPIOA->IOCFG &= ~BIT11;       //配置为GPIO
        DisWr_WPREG();
        HT_GPIOA->PTUP  &= ~BIT11;       //上拉
        HT_GPIOA->PTDIR &= ~BIT11;       //输入脚
    }

    if(BIT11 == (HT_GPIOA->PTDAT & BIT11) )        //检测到PA11为高
    {
        return HIGH_LEVEL;
    } 
    return LOW_LEVEL;
}

/** 
 * @brief  检测按键管脚状态 
 * @note   
 * @retval LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_DisplayKeyPinLevelStatus(void)
{
    if(((HT_GPIOA->IOCFG & BIT10) == BIT10)         //PA10没有配置为GPIO
     ||((HT_GPIOA->PTUP  & BIT10) == BIT10)         //或者PA10没有配置上拉
     ||((HT_GPIOA->PTDIR & BIT10) == BIT10))        //或者PA10没有配置成输入
    {
        EnWr_WPREG();
        HT_GPIOA->IOCFG &= ~BIT10;       //配置为GPIO
        DisWr_WPREG();
        HT_GPIOA->PTUP  &= ~BIT10;       //上拉
        HT_GPIOA->PTDIR &= ~BIT10;       //输入脚
    }

    if(BIT10 == (HT_GPIOA->PTDAT & BIT10) )        //检测到PA11为高
    {
        return HIGH_LEVEL;
    }

    return LOW_LEVEL;
}

/** 
 * @brief  检测插卡管脚状态 
 * @note   
 * @retval LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_CPUCardPinLevelStatus(void)
{
        //具体代码暂缺
        return HIGH_LEVEL;
}

/** 
 * @brief  检测继电器状态检测管脚状态
 * @note   
 * @retval  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_RelayPinLevelStatus(void)
{
        //具体代码暂缺
        return HIGH_LEVEL;
}

/** 
 * @brief  检测PLC通信状态检测管脚状态
 * @note   
 * @retval LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_PLCStaPinLevelStatus(void)
{
        //具体代码暂缺
        return HIGH_LEVEL;
}

/** 
 * @brief  设置跳闸灯管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 * @retval None
 */

extern void Set_RelayLedPin(LEVEL level)
{
    switch(level)
    {
        case HIGH_LEVEL:
                        HT_GPIOC->PTSET |= BIT8;     //输出高
            break;
        
        case LOW_LEVEL:
                        HT_GPIOC->PTSET &= ~BIT8;     //输出低
            break;
        
        default:
                        HT_GPIOC->PTSET |= BIT8;     //输出高,即默认输出高
            break;
    }

    if(((HT_GPIOC->IOCFG & BIT8) == BIT8)         //PC8没有配置为GPIO
     ||((HT_GPIOC->PTOD  & BIT8) != BIT8)         //或者PC8没有配置成推挽模式
     ||((HT_GPIOC->PTDIR & BIT8) != BIT8))        //或者PC8没有配置成输出
    {
        EnWr_WPREG();
        HT_GPIOC->IOCFG &= ~ BIT8;                  //配置为GPIO
        DisWr_WPREG();
        HT_GPIOC->PTOD  |= BIT8;                    //推挽模式
        HT_GPIOA->PTDIR |= BIT8;                    //输出
    }
}

/** 
 * @brief  设置PLC事件报警管脚的输出电平
 * @note   
 * @param  level: LOW_LEVEL 低电平;HIGH_LEVEL 高电平(注意，PLC事件接口这里是要开漏输出高)
 * @retval None
 */

extern void Set_PLCEventPin(LEVEL level)
{
    //具体代码暂缺
}

/** 
 * @brief  设置PLC复位管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平(注意，PLC复位接口这里是要开漏输出高)
 * @retval None
 */
extern void Set_PLCResetPin(LEVEL level)
{
    //具体代码暂缺
}
/** 
 * @brief  设置继电器闭合控制管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 * @retval None
 */
extern void Set_RelayOffPin(LEVEL level)
{
    //具体代码暂缺
}

/** 
 * @brief  设置继电器断开控制管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 * @retval None
 */
extern void Set_RelayOnPin(LEVEL level)
{
    //具体代码暂缺
}

/** 
 * @brief  设置背光灯管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 * @retval None
 */
extern void Set_BackLedPin(LEVEL level)
{
    //具体代码暂缺
}
/** 
 * @brief  检测开盖管脚状态
 * @note   
 * @retval  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 */

extern LEVEL Get_CoverKeyPinLevelStatus(void)
{
    //具体代码暂缺
}

/** 
 * @brief  设置SDA管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平(注意，这是软件模拟的I2C)
 * @retval None
 */
extern void Set_SDAPin(LEVEL level)
{
    //具体代码暂缺
}

/** 
 * @brief  //设置SCL管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平(注意，这是软件模拟的I2C)
 * @retval None
 */
extern void Set_SCLPin(LEVEL level)
{
    //具体代码暂缺
}
/** 
 * @brief  设置485发送接收使能管脚的输出电平
 * @note   
 * @param  level:  LOW_LEVEL 低电平;HIGH_LEVEL 高电平
 * @retval None
 */
extern void Set_RS485CtrlPin(LEVEL level)
{
    //具体代码暂缺
}

/*end-------------------------------------------------------------------------*/