/*******************************************************************************
* @file      : GENERAL.c
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Jan 21 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/*头文件----------------------------------------------------------------------*/
///添加头文件
#include "GENERAL.h"



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
 * @brief  非精确延时函数，同样的输入值根据系统时钟不同，延时时间也不同
 * @note   
 * @param  i: 0<i≤65535
 * @retval None
 */
extern void Delay( unsigned int i )
{
    while( i > 0)
    {
        i--;
    }
}


/** 
 * @brief  MS级非精确延时函数，同样的输入值根据系统时钟不同，延时时间也不同
 * @note   
 * @param  mSec: 输入值
 * @retval None
 */
extern void Delay_mSec(unsigned char  mSec)
{
    unsigned int  i;
    while (mSec--)
    {
        for (i=0; i<916; i++)
        {
            Delay(1);
        }
    }
}

/*计算RAM中连续一段字节数据的累加和*/

/** 
 * @brief  计算RAM中连续一段字节数据的累加和，起始值是0xa5
 * @note   
 * @param  *Ptr: RAM中需要计算字节数据的起始地址
 * @param  Len:  字节数据的个数，0<Len≤65535
 * @retval 
 */
extern unsigned char ChkNum( unsigned char *Ptr, int Len )
{
    unsigned char Num = 0xA5;
   
    while( Len > 0 )
    {
        Num += *Ptr++;
        Len--;
    }
    return Num;
}


/** 
 * @brief  计算RAM中连续一段字节数据的累加和，起始值是0
 * @note   
 * @param  *Ptr: RAM中需要计算字节数据的起始地址
 * @param  Len:  字节数据的个数，0<Len≤65535
 * @retval 
 */
extern unsigned char ChkNumAdd( unsigned char *Ptr, int Len )
{
    unsigned char Num = 0x00;
   
    while( Len > 0 )
    {
        Num += *Ptr++;
        Len--;
    }
    return Num;
}


/*HEX码转BCD码算法*/

/** 
 * @brief  将一个字节数据转换成1字节BCD
 * @note   当0x64≤Data≤0xff 时，函数都输出99，无意义
 * @param  Data: 无符号字节数  0x00≤Data≤0x63
 * @retval 一个字节BCD码
 */
extern unsigned char Byte_BCD(unsigned char Data)
{
    if(Data>=99) return 0x99;

    return( Data/10*0x10 + Data%10 );
}


/** 
 * @brief  将一个2字节数据转换成2字节BCD
 * @note   当0x2710≤Data≤0xffff 时，函数都输出9999，无意义
 * @param  *Ptr: 指向存放结果的RAM的存储区的起始地址
 * @param  Data: 无符号2字节数据，0x00≤Data≤0x270F
 * @retval None
 */
extern void Word_BCD(unsigned char *Ptr, unsigned int Data)
{
    if(Data>=9999)
    {
        *(Ptr+1) = 0x99;
        *Ptr = 0x99;
        return;
    }

    *(Ptr+1) = Byte_BCD( (unsigned char)(Data/100) );
    *Ptr = Byte_BCD( (unsigned char)(Data%100) );
}


/** 
 * @brief  将一个4字节数据转换成3字节BCD
 * @note   当0x000f4240≤Data≤0xffffffff 时，函数都输出999999，无意义
 * @param  *Ptr: 指向存放结果的RAM的存储区的起始地址
 * @param  Data: 无符号4字节数据，0≤Data≤0x000F423F
 * @retval None
 */
extern void Long_BCD3(unsigned char *Ptr, unsigned long Data)
{
    if(Data>=999999)
    {
        *(Ptr+2) = 0x99;
        *(Ptr+1) = 0x99;
        *Ptr = 0x99;
        return;
    }

    *(Ptr+2) = Data/100000*0x10 + Data/10000;
    Word_BCD(Ptr, (unsigned int)(Data%10000));
}


/** 
 * @brief  将一个4字节数据转换成4字节BCD
 * @note   当0x5F5E0FF≤Data≤0xffffff 时，函数都输出99999999，无意义
 * @param  *Ptr: 指向存放结果的RAM的存储区的起始地址
 * @param  Data: 无符号4字节数据，0≤Data≤0x05F5E0FF
 * @retval None
 */
extern void Long_BCD4(unsigned char *Ptr, unsigned long Data)
{
    if(Data>=99999999)
    {
        *(Ptr+3) = 0x99;
        *(Ptr+2) = 0x99;
        *(Ptr+1) = 0x99;
        *Ptr = 0x99;
        return;
    }
    Word_BCD(Ptr+2, Data/10000);
    Word_BCD(Ptr, Data%10000);
}



/*判断数据是否为BCD码算法*/

/** 
 * @brief  判断字节是否符合BCD码格式
 * @note   
 * @param  Data: 被判断的字节
 * @retval 0：不符合
 * @retval 1：符合
 */
extern int IsBCD(unsigned char Data)
{
    if(((( Data >> 4) & 0x0f) > 9 )||( ( Data & 0x0f ) > 9 )) return 1;
    
    return 0;
}



/*BCD码转换成HEX码算法*/

/** 
 * @brief  将一个字节的BCD码转换成hex格式
 * @note   Data必须是BCD码
 * @param  Data: BCD码
 * @retval hex码 
 */
extern unsigned char BCD_Byte(unsigned char Data)
{
    return(((Data>>4)&0x0f)*10+(Data&0x0f));
}

/** 
 * @brief  将2个字节的BCD码转换成int型hex码
 * @note   数组里面的字节必须符合BCD码 
 * @param  *Ptr: 指向数组的起始地址
 * @retval int型的hex码 
 */
extern unsigned int BCD2_Word(unsigned char *Ptr)
{
   return(  (int)BCD_Byte(*Ptr)+
            (int)BCD_Byte(*(Ptr+1))*100 );
}


/** 
 * @brief  将3个字节的BCD码转换成long型hex码
 * @note   数组里面的字节必须符合BCD码 
 * @param  *Ptr: 指向数组的起始地址
 * @retval long型的hex码 
 */
extern unsigned long BCD3_Long(unsigned char *Ptr)
{
   return(  (long)BCD_Byte(*Ptr)+
            (long)BCD_Byte(*(Ptr+1))*100+
            (long)BCD_Byte(*(Ptr+2))*10000 );
}


/** 
 * @brief  将4个字节的BCD码转换成long型hex码
 * @note   数组里面的字节必须符合BCD码
 * @param  *Ptr: 指向数组的起始地址
 * @retval long型的hex码
 */
extern unsigned long BCD4_Long(unsigned char *Ptr)
{
   return(  (long)BCD_Byte(*Ptr)+
            (long)BCD_Byte(*(Ptr+1))*100+
            (long)BCD_Byte(*(Ptr+2))*10000+
            (long)BCD_Byte(*(Ptr+3))*1000000 );
}



/*BCD码加减算法*/

/** 
 * @brief  单字节的BCD码加法
 * @note   
 * @param  n: 加数
 * @param  m: 被加数
 * @param  *carry: 向上一高字节进位标志放在了carry中  
 * @retval 返回结果
 */
extern unsigned char Add_Bcd_1(unsigned char n,unsigned char m,unsigned char *carry)	
{
    unsigned char i,j,w;
    m=m+(*carry);
    *carry=0;
    j=n&0x0f;
    i=m&0x0f;
    i+=j;
    w=0;
    if(i>9)
    {
        w=0x01;
        i-=10;
    }
    j=n;
    j>>=4;		
    m>>=4;
    j=j+m+w;
    if(j>0x09)
    {
        j-=10;
        j<<=4;
        n=j+i;
        *carry=1;
        return(n);
    }
    j<<=4;
    n=j+i;
    *carry=0;
    return(n);
}

/** 
 * @brief  Len字节BCD码加法
 * @note   
 * @param  *BufA: 被加数
 * @param  *BufB: 加数
 * @param  Len: 长度
 * @retval 0:没有溢出，结果可信
 * @retval 1:有进位，已经溢出，结果不可信
 */
extern unsigned char Add_Bcd( unsigned char *BufA, unsigned char *BufB, unsigned char Len )
{
    unsigned char i;
    unsigned char bf_carry;
    bf_carry=0;
    for(i=(Len-1);i<Len;i--)
    {
        BufA[i]=Add_Bcd_1(BufA[i],BufB[i],&bf_carry);
    }
    
    return bf_carry;
}


/** 
 * @brief  Len字节BCD码减法
 * @note   
 * @param  *BufA: 被减数
 * @param  *BufB: 减数
 * @param  Len: 长度
 * @retval 0：代表被减数大于等于减数，结果值可信
 * @retval 1：代表被减数小于减数，结果值是通过向更高位借来的。	结果不可信
 */
extern unsigned char Sub_Bcd( unsigned char *BufA, unsigned char *BufB, unsigned char Len )
{
    unsigned char i,j,k;
    unsigned char bf_carry;
    bf_carry=0;
    for(i=(Len-1);i<Len;i--)
    {
        j=BufB[i]+bf_carry;
        k=0;
        if(j<BufB[i])
        k=1;
        bf_carry=0;
        if((BufA[i]&0x0f)<(j&0x0f))
        {
            if(BufA[i]<j)
            {
                BufA[i]=0x9a-j+BufA[i];         //0xa0-6;
                bf_carry=1;
            }
            else
                BufA[i]=BufA[i]-j-6;
        }
        else
        {
            if(BufA[i]<j)
            {
                BufA[i]=0xa0-j+BufA[i];
                bf_carry=1;
            }
            else
                BufA[i]=BufA[i]-j;
        }
        if(k)
            bf_carry=1;		//由于k=1时，不会产生借位
    }
    
    return bf_carry;
}



/*数组判断及处理算法*/

/** 
 * @brief  检查字节数组数据是否为全零
 * @note   
 * @param  *Buf: 数组起始地址
 * @param  Len: 数组长度
 * @retval 0:代表数组元素为全零
 * @retval 1:代表数组元素存在非零
 */
extern unsigned char ChkBufZero( unsigned char *Buf,unsigned char Len )
{
    unsigned char i;

    for( i=0; i<Len; i++ )
    {
        if( Buf[i] )    return(1);  //非0
    }

    return(0);  //全0
}


/** 
 * @brief  默认比较的数组是高位在前然后两者数组比较的函数
 * @note   *BufA是最高位，*BufB是最高位
 * @param  *BufA: 被比较数
 * @param  *BufB: 被比较数
 * @param  Len: 数组长度
 * @retval 1:A>B
 * @retval 2:A<B
 * @retval 3:A=B
 */
extern unsigned char Cmp_Data( unsigned char *BufA, unsigned char *BufB, unsigned char Len )
{
    unsigned char i;

    for( i=0; i<Len; i++ )
    {
        if( BufA[i] > BufB[i] )
        {
            return 1; //A>B
        }
        else
        {
            if( BufA[i] < BufB[i] )
            {
                return 2; //A<B
            }
        }
    }

    return 3; //A=B
}


/** 
 * @brief  默认比较的数组是低位在前，然后两者数组比较的函数
 * @note   *BcdA是最低位，*BcdB是最低位
 * @param  *BcdA: 
 * @param  *BcdB: 
 * @param  Len: 比较的长度
 * @retval 0：A=B
 * @retval 1：A>B
 * @retval 2：A<B
 */
extern unsigned char Cmp_Bcd_Inv( unsigned char *BcdA, unsigned char *BcdB, unsigned char Len )
{
    unsigned char i;

    for( i=0; i<Len; i++ )
    {
        if( BcdA[Len-1-i] > BcdB[Len-1-i] )
        {
            return 1; //A > B
        }
        else
        {
            if( BcdA[Len-1-i] < BcdB[Len-1-i] )
            {
                return 2; //A < B
            }
        }
    }

    return 0; //A==B
}



/** 
 * @brief  多字节数组高低位字节交换
 * @note    举个例子：0x1234====》0x3412，是以一个字节为最小单位进行交换
 * @param  Dest: 目标数据
 * @param  Len: 数组长度
 * @retval None
 */
extern void SwapData( unsigned char* Dest, int Len )
{
	unsigned char Temp;
	int i;
	
	for( i=0;i<Len/2;i++ )
	{
		Temp = *(Dest+Len-1-i);	
		*(Dest+Len-1-i) = *(Dest+i);
		*(Dest+i) = Temp; 
	}	
}
/*end-------------------------------------------------------------------------*/