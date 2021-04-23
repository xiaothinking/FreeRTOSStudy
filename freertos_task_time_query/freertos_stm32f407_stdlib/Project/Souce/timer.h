#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

extern volatile unsigned long long FreeRTOSRunTimeTicks;

//初始化TIM3使其为FreeRTOS的时间统计提供时基
void ConfigureTimeForRunTimeStats(void);
void TIM3_Init(uint16_t arr,uint16_t psc);
void TIM5_Init(uint16_t arr,uint16_t psc);

#endif

