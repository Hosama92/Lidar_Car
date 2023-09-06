#include "all.h"

extern TIM_HandleTypeDef htim1;
_sTask sTask;
//
//
void Task_Struct_Init()
{
	memset(&sTask, 0, sizeof(struct _sTask));
}

void Task_Init()
{
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_4);
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
	for (int pwm = 0; pwm < 1000; pwm += 100)
	{
		htim1.Instance->CCR1 = pwm;
		htim1.Instance->CCR2 = 0;
		htim1.Instance->CCR3 = pwm;
		htim1.Instance->CCR4 = 0;
		HAL_Delay(1000);
		printf(" %d, %d, %d, %d\n\r", (int)htim1.Instance->CCR1, (int)htim1.Instance->CCR2, (int)htim1.Instance->CCR3, (int)htim1.Instance->CCR4);
	}

	HAL_Delay(3000);

	for (int pwm = 0; pwm < 1000; pwm += 100)
	{
		htim1.Instance->CCR1 = 0;
		htim1.Instance->CCR2 = pwm;
		htim1.Instance->CCR3 = 0;
		htim1.Instance->CCR4 = pwm;
		printf(" %d, %d, %d, %d\n\r", (int)htim1.Instance->CCR1, (int)htim1.Instance->CCR2, (int)htim1.Instance->CCR3, (int)htim1.Instance->CCR4);
		HAL_Delay(1000);
	}
}




