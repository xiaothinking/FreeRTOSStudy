/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "led.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		128  
//������
TaskHandle_t Led0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define QUERY_TASK_PRIO		3
//�����ջ��С	
#define QUERY_STK_SIZE 		256  
//������
TaskHandle_t QueryTask_Handler;
//������
void query_task(void *pvParameters);

char InfoBuffer[1000];				//������Ϣ������



int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);		//��ʼ����ʱ����
	uart_init(115200);     	//��ʼ������
	led_init();		        //��ʼ��LED�˿�
  key_init(); 
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}
 
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,             
                (const char*    )"led0_task",           
                (uint16_t       )LED0_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED0_TASK_PRIO,        
                (TaskHandle_t*  )&Led0Task_Handler);   
    //����QUERY����
    xTaskCreate((TaskFunction_t )query_task,     
                (const char*    )"query_task",   
                (uint16_t       )QUERY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )QUERY_TASK_PRIO,
                (TaskHandle_t*  )&QueryTask_Handler); 
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//led0������
void led0_task(void *pvParameters)
{

	while(1)
	{
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
    vTaskDelay(500);			//��ʱ500ms��Ҳ����500��ʱ�ӽ���	
	}
}

//query������
void query_task(void *pvParameters)
{
	u32 TotalRunTime;
	UBaseType_t ArraySize,x;
	TaskStatus_t *StatusArray;
	
	//��һ��������uxTaskGetSystemState()��ʹ��
	printf("/********��һ��������uxTaskGetSystemState()��ʹ��**********/\r\n");
	ArraySize=uxTaskGetNumberOfTasks();		//��ȡϵͳ��������
	StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//�����ڴ�
	if(StatusArray!=NULL)					//�ڴ�����ɹ�
	{
		ArraySize=uxTaskGetSystemState((TaskStatus_t* 	)StatusArray, 	//������Ϣ�洢����
									   (UBaseType_t		)ArraySize, 	//������Ϣ�洢�����С
								       (uint32_t*		)&TotalRunTime);//����ϵͳ�ܵ�����ʱ��
		printf("TaskName\t\tPriority\t\tTaskNumber\t\t\r\n");
		for(x=0;x<ArraySize;x++)
		{
			//ͨ�����ڴ�ӡ����ȡ����ϵͳ������й���Ϣ�������������ơ�
			//�������ȼ��������š�
			printf("%s\t\t%d\t\t\t%d\t\t\t\r\n",				
					StatusArray[x].pcTaskName,
					(int)StatusArray[x].uxCurrentPriority,
					(int)StatusArray[x].xTaskNumber);
			
		}
	}
	vPortFree(StatusArray);	//�ͷ��ڴ�
	printf("/**************************����***************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_scan(0)!=4) delay_ms(10);		//�ȴ�KEY_UP������
	
	//�ڶ���������vTaskGetInfo()��ʹ��
	TaskHandle_t TaskHandle;	
	TaskStatus_t TaskStatus;
	
	printf("/************�ڶ���������vTaskGetInfo()��ʹ��**************/\r\n");
	TaskHandle=xTaskGetHandle("led0_task");			//������������ȡ��������
	//��ȡLED0_Task��������Ϣ
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ��
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ���С
			     (eTaskState	)eInvalid);			//�����Լ���ȡ��������׳̬
	//ͨ�����ڴ�ӡ��ָ��������й���Ϣ��
	printf("������:                %s\r\n",TaskStatus.pcTaskName);
	printf("������:              %d\r\n",(int)TaskStatus.xTaskNumber);
	printf("����׳̬:              %d\r\n",TaskStatus.eCurrentState);
	printf("����ǰ���ȼ�:        %d\r\n",(int)TaskStatus.uxCurrentPriority);
	printf("��������ȼ�:          %d\r\n",(int)TaskStatus.uxBasePriority);
	printf("�����ջ����ַ:        %#x\r\n",(int)TaskStatus.pxStackBase);
	printf("�����ջ��ʷʣ����Сֵ:%d\r\n",TaskStatus.usStackHighWaterMark);
	printf("/**************************����***************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_scan(0)!=4) delay_ms(10);		//�ȴ�KEY_UP������
	
	//������������eTaskGetState()��ʹ��	
	eTaskState TaskState;
	char TaskInfo[10];
	printf("/***********������������eTaskGetState()��ʹ��*************/\r\n");
	TaskHandle=xTaskGetHandle("query_task");		//������������ȡ��������
	TaskState=eTaskGetState(TaskHandle);			//��ȡquery_task���������׳̬
	memset(TaskInfo,0,10);						
	switch((int)TaskState)
	{
		case 0:
			sprintf(TaskInfo,"Running");
			break;
		case 1:
			sprintf(TaskInfo,"Ready");
			break;
		case 2:
			sprintf(TaskInfo,"Suspend");
			break;
		case 3:
			sprintf(TaskInfo,"Delete");
			break;
		case 4:
			sprintf(TaskInfo,"Invalid");
			break;
	}
	printf("����׳ֵ̬:%d,��Ӧ��׳̬Ϊ:%s\r\n",TaskState,TaskInfo);
	printf("/**************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	while(key_scan(0)!=4) delay_ms(10);		//�ȴ�KEY_UP������
	
	//���Ĳ�������vTaskList()��ʹ��	
	printf("/*************������������vTaskList()��ʹ��*************/\r\n");
	vTaskList(InfoBuffer);							//��ȡ�����������Ϣ
	printf("%s\r\n",InfoBuffer);					//ͨ�����ڴ�ӡ�����������Ϣ
	printf("/**************************����**************************/\r\n");
	while(1)
	{
		GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
    vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
