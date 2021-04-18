#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"

void led_init(void);
void tim_init(uint16_t arr,uint16_t psc);

#endif

