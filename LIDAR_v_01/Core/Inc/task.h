#ifndef INC_TASK_H_
#define INC_TASK_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct _sTask
{
	struct _sTask_flag
	{
		bool lidar;
		bool motor;
	} sFlag;
} _sTask;

void Task_Struct_Init();
void Task_Init();
void Task_Doing();
//
void motor_test();

#endif /* INC_TASK_H_ */
