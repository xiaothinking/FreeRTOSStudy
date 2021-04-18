#include "led.h"
 
void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  /* Configure PG6 and PG8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void led_ctrl(uint8_t mode)
{
	switch(mode)
    {
		case 0x00:
			    GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
	       	GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);
			   break;
	  case 0x01:
			    GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
	       	GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
			   break;
		case 0x02:
			    GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
	       	GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);
			   break;
	  case 0x03:
			    GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
	       	GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
			   break;
		default:
			   break;

		} 
}

