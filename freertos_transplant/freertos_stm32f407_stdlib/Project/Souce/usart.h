#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define MAX_UART_RX_BUFFER 256
#define INIT_ZERO  0
typedef struct
{
    uint8_t writepos;
    uint8_t readpos;
}Uart_pos;

/*uart2 接收缓存数组，256字节*/
extern uint8_t ar_uart1_buffer[MAX_UART_RX_BUFFER];
/*uart2 接收、处理数据定位指针*/
extern Uart_pos uart_tmp1;
/*uart2 数据接收完后超时计数*/
extern volatile uint8_t g_uart1_tick;
/*uart2 数据接收完成标志位*/
extern volatile uint8_t g_uart1_receive_finish;


void uart_init(uint32_t bound);
void uart1_receive_handle(void);

#endif

