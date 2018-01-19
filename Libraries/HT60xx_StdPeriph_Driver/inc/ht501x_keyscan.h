/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht511x_keyscan.h
* By           : SocTeam
* Version      : CurrentVersion
* Description  : Only support HT501x
**********************************************************************************************************
*/

#ifndef __HT511X_KEYSCAN_H__
#define __HT511X_KEYSCAN_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
#if defined  HT501x                          /* This File Only support HT501x */
    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             全局变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           全局函数申明
*********************************************************************************************************
*/
uint32_t HT_KEY_ScanStatusGet(void);
void HT_KEY_ClearScanStatus(void);
ITStatus HT_KEY_ITFlagStatusGet(uint8_t ITFlag);
void HT_RTC_ClearITPendingBit(uint8_t ITFlag);








#endif                                               /* This File Only support HT501x */


#ifdef __cplusplus
}
#endif

#endif /* __HT511X_KEYSCAN_H__ */ 
