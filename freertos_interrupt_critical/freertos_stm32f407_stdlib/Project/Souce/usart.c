#include "usart.h"
 
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 
 
/*uart2 接收缓存数组，256字节*/
uint8_t ar_uart1_buffer[MAX_UART_RX_BUFFER];
/*uart2 接收、处理数据定位指针*/
Uart_pos uart_tmp1 = {INIT_ZERO};
/*uart2 数据接收完后超时计数*/
volatile uint8_t g_uart1_tick = INIT_ZERO;
/*uart2 数据接收完成标志位*/
volatile uint8_t g_uart1_receive_finish = INIT_ZERO;
uint8_t ar_uart1_protocol[MAX_UART_RX_BUFFER];

void uart_init(uint32_t bound)
{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}


void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {  			 
     ar_uart1_buffer[uart_tmp1.writepos] = USART_ReceiveData(USART1); 	 
     uart_tmp1.writepos++;		
     g_uart1_tick = 10;
     g_uart1_receive_finish = 1;
	}
}


void uart1_receive_handle(void)
{ 
	  uint8_t m_length = INIT_ZERO;
	  uint8_t i;
    /*一帧数据接收完成*/
    if((g_uart1_receive_finish == 1) && (g_uart1_tick == 0))
    {
			  g_uart1_receive_finish = 0;
			
			   /*判断接收、处理数据的指针是否一致，并将接收到的数据保存到tmp数组中*/
				if(uart_tmp1.readpos <= uart_tmp1.writepos)
				{
						m_length = uart_tmp1.writepos - uart_tmp1.readpos;
						memcpy(ar_uart1_protocol, &ar_uart1_buffer[uart_tmp1.readpos], m_length);
				}
				else
				{
						m_length = uart_tmp1.writepos + MAX_UART_RX_BUFFER - uart_tmp1.readpos;
						memcpy(ar_uart1_protocol, &ar_uart1_buffer[uart_tmp1.readpos], MAX_UART_RX_BUFFER - uart_tmp1.readpos);
						memcpy(&ar_uart1_protocol[MAX_UART_RX_BUFFER - uart_tmp1.readpos], ar_uart1_buffer, uart_tmp1.writepos);
				}
				uart_tmp1.readpos = uart_tmp1.writepos;
				
		    for(i=0;i<m_length;i++)
        {
						USART_SendData(USART1, ar_uart1_protocol[i]);         //向串口1发送数据
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束			
				}
		}
}

