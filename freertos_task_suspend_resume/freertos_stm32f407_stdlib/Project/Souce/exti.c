#include "exti.h"
#include "FreeRTOS.h"
#include "task.h"

//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void exti_init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	key_init(); //������Ӧ��IO�ڳ�ʼ��
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 ���ӵ��ж���4
	
	/* ����EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	   
}

//������
extern TaskHandle_t Task2Task_Handler;
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	BaseType_t YieldRequired;
	if(0==GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))	
   {
		YieldRequired=xTaskResumeFromISR(Task2Task_Handler);//�ָ�����2
		printf("�ָ�����2������!\r\n");
		if(YieldRequired==pdTRUE)
		{
			/*�������xTaskResumeFromISR()����ֵΪpdTRUE����ô˵��Ҫ�ָ������
			������������ȼ����ڻ��߸����������е�����(���жϴ�ϵ�����),������
			�˳��жϵ�ʱ��һ��Ҫ�����������л���*/
			portYIELD_FROM_ISR(YieldRequired);
		} 
	 }
	 EXTI_ClearITPendingBit(EXTI_Line4);//���LINE4�ϵ��жϱ�־λ  
}

