#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

void TIM3_Init(uint16_t arr,uint16_t psc);
void TIM5_Init(uint16_t arr,uint16_t psc);

#endif

