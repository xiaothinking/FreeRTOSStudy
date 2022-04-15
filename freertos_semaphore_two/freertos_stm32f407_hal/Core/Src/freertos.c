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
#include "lcd.h"
#include "gpio.h"
#include "usart.h"
#include "malloc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//用于命令解析用的命令值
#define LED1ON	1
#define LED1OFF	2
#define BEEPON	3
#define BEEPOFF	4
#define COMMANDERR	0XFF
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//将字符串中的小写字母转换为大写
//str:要转换的字符串
//len：字符串长度
void LowerToCap(uint8_t *str,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		if((96<str[i])&&(str[i]<123))	//小写字母
		str[i]=str[i]-32;				//转换为大写
	}
}

//命令处理函数，将字符串命令转换成命令值
//str：命令
//返回值: 0XFF，命令错误；其他值，命令值
uint8_t CommandProcess(uint8_t *str)
{
	uint8_t CommandValue=COMMANDERR;
	if(strcmp((char*)str,"LED1ON")==0) CommandValue=LED1ON;
	else if(strcmp((char*)str,"LED1OFF")==0) CommandValue=LED1OFF;
	else if(strcmp((char*)str,"BEEPON")==0) CommandValue=BEEPON;
	else if(strcmp((char*)str,"BEEPOFF")==0) CommandValue=BEEPOFF;
	return CommandValue;
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//LCD刷屏时使用的颜色
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void check_msg_queue(void);
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

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

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
	uint8_t len=0;
	uint8_t CommandValue=COMMANDERR;
	BaseType_t err=pdFALSE;
	
	uint8_t *CommandStr;
	POINT_COLOR=BLUE;
  /* Infinite loop */
  for(;;)
  {
		if(myBinarySem01Handle!=NULL)
		{
			err=xSemaphoreTake(myBinarySem01Handle,portMAX_DELAY);	//获取信号量
			if(err==pdTRUE)										//获取信号量成功
			{
				len=USART_RX_STA&0x3fff;						//得到此次接收到的数据长度
				CommandStr=mymalloc(SRAMIN,len+1);				//申请内存
				sprintf((char*)CommandStr,"%s",USART_RX_BUF);
				CommandStr[len]='\0';							//加上字符串结尾符号
				LowerToCap(CommandStr,len);						//将字符串转换为大写		
				CommandValue=CommandProcess(CommandStr);		//命令解析
				if(CommandValue!=COMMANDERR)
				{
					LCD_Fill(10,90,210,110,WHITE);				//清除显示区域
					LCD_ShowString(10,90,200,16,16,CommandStr);	//在LCD上显示命令
					printf("命令为:%s\r\n",CommandStr);
					switch(CommandValue)						//处理命令
					{
						case LED1ON: 
							HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
							break;
						case LED1OFF:
							HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
							break;
						case BEEPON:
							HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
							break;
						case BEEPOFF:
							HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
							break;
					}
				}
				else
				{
					printf("无效的命令，请重新输入!!\r\n");
				}
				USART_RX_STA=0;
				memset(USART_RX_BUF,0,USART_REC_LEN);			//串口接收缓冲区清零
				myfree(SRAMIN,CommandStr);						//释放内存
			}
		}
   else if(err==pdFALSE)
    {
			osDelay(10);			
		}
    
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
