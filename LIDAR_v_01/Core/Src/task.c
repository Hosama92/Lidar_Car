#include "all.h"

extern TIM_HandleTypeDef htim1;
_sTask sTask;
//
//바퀴 동작하는 클래스
void Task_Struct_Init()
{
	memset(&sTask, 0, sizeof(struct _sTask));
}

void Task_Init()
{
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);//좌측바퀴 전진
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);//좌측바퀴 후진
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);//우측바퀴 전진
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_4);//우측바퀴 후진
}

void Task_Doing()
{
	if (sTask.sFlag.lidar)
	{
		sTask.sFlag.lidar = false;
	}

	else if (sTask.sFlag.motor)
	{
		sTask.sFlag.motor = false;
	}
}

void motor_test()
{
	for (int pwm = 0; pwm < 1000; pwm += 100)// 전진
	{
		htim1.Instance->CCR1 = pwm;
		htim1.Instance->CCR2 = 0;
		htim1.Instance->CCR3 = pwm;
		htim1.Instance->CCR4 = 0;
		HAL_Delay(1000);
		printf(" %d, %d, %d, %d\n\r", (int)htim1.Instance->CCR1, (int)htim1.Instance->CCR2, (int)htim1.Instance->CCR3, (int)htim1.Instance->CCR4);
	}

	HAL_Delay(3000);

	for (int pwm = 0; pwm < 1000; pwm += 100) // 후진
	{
		htim1.Instance->CCR1 = 0;
		htim1.Instance->CCR2 = pwm;
		htim1.Instance->CCR3 = 0;
		htim1.Instance->CCR4 = pwm;
		printf(" %d, %d, %d, %d\n\r", (int)htim1.Instance->CCR1, (int)htim1.Instance->CCR2, (int)htim1.Instance->CCR3, (int)htim1.Instance->CCR4);
		HAL_Delay(1000);
	}
}
void motor_move(int n)
{
	if(n == 0)
	{
		htim1.Instance->CCR1 = 0;
		htim1.Instance->CCR2 = 0;
		htim1.Instance->CCR3 = 0;
		htim1.Instance->CCR4 = 0;
		printf("stop\n\r");
	}
	else if(n == 1)
	{
		htim1.Instance->CCR1 = 800;
		htim1.Instance->CCR2 = 0;
		htim1.Instance->CCR3 = 800;
		htim1.Instance->CCR4 = 0;
		printf("go\n\r");
	}
	else if(n== 2)
	{
		htim1.Instance->CCR1 = 0;
		htim1.Instance->CCR2 = 800;
		htim1.Instance->CCR3 = 0;
		htim1.Instance->CCR4 = 800;
		printf("back\n\r");
	}
	else if(n == 3)
	{
		htim1.Instance->CCR1 = 0;
		htim1.Instance->CCR2 = 800;
		htim1.Instance->CCR3 = 800;
		htim1.Instance->CCR4 = 0;
		HAL_Delay(500);
		printf("turn left\n\r");
	}
	else if(n == 4)
	{
		htim1.Instance->CCR1 = 800;
		htim1.Instance->CCR2 = 0;
		htim1.Instance->CCR3 = 0;
		htim1.Instance->CCR4 = 800;
		printf("turn right\n\r");
	}
	else // 에러테스트용
	{
		printf("error!!\n\r");
		printf("n = %d\n\r",n);
	}
}


