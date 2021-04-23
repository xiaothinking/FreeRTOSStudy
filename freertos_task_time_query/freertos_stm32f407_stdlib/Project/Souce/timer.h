#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

extern volatile unsigned long long FreeRTOSRunTimeTicks;

//��ʼ��TIM3ʹ��ΪFreeRTOS��ʱ��ͳ���ṩʱ��
void ConfigureTimeForRunTimeStats(void);
void TIM3_Init(uint16_t arr,uint16_t psc);
void TIM5_Init(uint16_t arr,uint16_t psc);

#endif

