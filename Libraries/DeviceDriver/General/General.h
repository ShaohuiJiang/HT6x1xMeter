/*******************************************************************************
* @file      : GENERAL.h
* @author    : Jiangshaohui
* @version   : V1.0.0
* @date      : Sun Jan 21 2018
* @brief     : 
********************************************************************************
* @attention :
*
*
*/
/* 条件编译-------------------------------------------------------------------*/
#ifndef __GENERAL_H
#define __GENERAL_H



/*头文件----------------------------------------------------------------------*/
///添加头文件




/*宏定义----------------------------------------------------------------------*/
///添加宏定义



/*声明外部函数----------------------------------------------------------------*/
///声明对应C文件中的全局函数
extern void Delay( unsigned int i );
extern void Delay_mSec(unsigned char  mSec);
extern unsigned char ChkNum( unsigned char *Ptr, int Len );
extern unsigned char ChkNumAdd( unsigned char *Ptr, int Len );
extern unsigned char Byte_BCD(unsigned char Data);
extern void Word_BCD(unsigned char *Ptr, unsigned int Data);
extern void Long_BCD3(unsigned char *Ptr, unsigned long Data);
extern void Long_BCD4(unsigned char *Ptr, unsigned long Data);
extern int IsBCD(unsigned char Data);
extern unsigned char BCD_Byte(unsigned char Data);
extern unsigned int BCD2_Word(unsigned char *Ptr);
extern unsigned long BCD3_Long(unsigned char *Ptr);
extern unsigned long BCD4_Long(unsigned char *Ptr);
extern unsigned char Add_Bcd_1(unsigned char n,unsigned char m,unsigned char *carry);
extern unsigned char Add_Bcd( unsigned char *BufA, unsigned char *BufB, unsigned char Len );
extern unsigned char Sub_Bcd( unsigned char *BufA, unsigned char *BufB, unsigned char Len );
extern unsigned char ChkBufZero( unsigned char *Buf,unsigned char Len );
extern unsigned char Cmp_Data( unsigned char *BufA, unsigned char *BufB, unsigned char Len );
extern unsigned char Cmp_Bcd_Inv( unsigned char *BcdA, unsigned char *BcdB, unsigned char Len );
extern void SwapData( unsigned char* Dest, int Len );


#endif
/*end------------------------------------------------------------------------*/