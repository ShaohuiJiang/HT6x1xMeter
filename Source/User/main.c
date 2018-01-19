/*
*********************************************************************************************************
*                                 			   HT6XXX
*                            			      HT6XXX程序
*
*                       			Copyright 2013, Hi-Trend Tech, Corp.
*                             			 All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : main.c
* By           : SocTeam
* Version      : CurrentVersion
* Description  : 
*********************************************************************************************************
*/

#define  __MAIN_C

#include <stdio.h>
//#include "ht6xxx_lib.h"

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            本地变量
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                              MAIN
*
*********************************************************************************************************
*/
int main(void)                                   
{     
    /*  SystemInit() has been called before enter main() */ 
    
    /*  Add your application code here  */ 
    
    /*  Infinite loop  */
  
//    HT_CMU->WPREG = 0xA55A;
    
//    HT_GPIOE->IOCFG =0;
//    HT_GPIOE->PTDIR =0xFFFF;
  
    while (1)
    {
	//HT_GPIOE->PTTOG = 0xFFFF;	
    }
}


 /*
*********************************************************************************************************
*                                             ASSERT
*
*********************************************************************************************************
*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


