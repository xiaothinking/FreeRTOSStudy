/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "gpio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//����һ�������õ��б��3���б���
List_t TestList;		//�������б�
ListItem_t ListItem1;	//�������б���1
ListItem_t ListItem2;	//�������б���2
ListItem_t ListItem3;	//�������б���3
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */

  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);		
    osDelay(500);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
	uint8_t task1_num=0;
  /* Infinite loop */
	//��һ������ʼ���б���б���
	vListInitialise(&TestList);
	vListInitialiseItem(&ListItem1);
	vListInitialiseItem(&ListItem2);
	vListInitialiseItem(&ListItem3);
	
	ListItem1.xItemValue=40;			//ListItem1�б���ֵΪ40
	ListItem2.xItemValue=60;			//ListItem2�б���ֵΪ60
	ListItem3.xItemValue=50;			//ListItem3�б���ֵΪ50
	
	//�ڶ�������ӡ�б�������б���ĵ�ַ
	printf("/*******************�б���б����ַ*******************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList                          %#x					\r\n",(int)&TestList);
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd                %#x					\r\n",(int)(&TestList.xListEnd));
	printf("ListItem1                         %#x					\r\n",(int)&ListItem1);
	printf("ListItem2                         %#x					\r\n",(int)&ListItem2);
	printf("ListItem3                         %#x					\r\n",(int)&ListItem3);
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_sacn(0)!=4) HAL_Delay(10);					//�ȴ�KEY_UP������
	
	//�����������б�TestList����б���ListItem1����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem1);		//�����б���ListItem1
	printf("/******************����б���ListItem1*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_sacn(0)!=4) HAL_Delay(10);					//�ȴ�KEY_UP������
	
	//���Ĳ������б�TestList����б���ListItem2����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem2);	//�����б���ListItem2
	printf("/******************����б���ListItem2*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_sacn(0)!=4) HAL_Delay(10);					//�ȴ�KEY_UP������
	
	//���岽�����б�TestList����б���ListItem3����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem3);	//�����б���ListItem3
	printf("/******************����б���ListItem3*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_sacn(0)!=4) HAL_Delay(10);					//�ȴ�KEY_UP������
	
	//��������ɾ��ListItem2����ͨ�����ڴ�ӡ�����б����г�Ա����pxNext��
	//pxPrevious��ֵ��ͨ��������ֵ�۲��б������б��е����������
	uxListRemove(&ListItem2);						//ɾ��ListItem2
	printf("/******************ɾ���б���ListItem2*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_sacn(0)!=4) HAL_Delay(10);					//�ȴ�KEY_UP������
	
	//���߲���ɾ��ListItem2����ͨ�����ڴ�ӡ�����б����г�Ա����pxNext��
	//pxPrevious��ֵ��ͨ��������ֵ�۲��б������б��е����������
	TestList.pxIndex=TestList.pxIndex->pxNext;			//pxIndex�����һ�����pxIndex�ͻ�ָ��ListItem1��
	vListInsertEnd(&TestList,&ListItem2);				//�б�ĩβ����б���ListItem2
	printf("/***************��ĩβ����б���ListItem2***************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************����**************************/\r\n\r\n\r\n");
	while(1)
	{
		 HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
     vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
