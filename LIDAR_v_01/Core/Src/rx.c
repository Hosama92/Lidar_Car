#include "rx.h"
#include "main.h"
//
#include <stdio.h>
#include <stdint.h>
#include <string.h>
//
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart3;
//
_sRX sRX;

void RX_Struct_Init()
{
	memset(&sRX, 0, sizeof(struct _sRX));
}

void RX_Init()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	HAL_UART_Receive_IT(&huart5, &sRX.tmp, 1);
}

void RX_Doing()
{
	if (sRX.pop_cnt != sRX.push_cnt)
	{
		uint8_t tmp = sRX.rxbuf[sRX.pop_cnt++];
		sRX.pop_cnt %= RX_SIZE;

		// debug check
		printf(" %02X", tmp);
		if (!(sRX.pop_cnt % 10))
		{
			printf("\n\r");
		}
	}
}

int __io_putchar(int ch)
{
	while (HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 1) != HAL_OK);
	return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart5.Instance)
	{
		sRX.rxbuf[sRX.push_cnt++] = sRX.tmp;
		sRX.push_cnt %= RX_SIZE;
		HAL_UART_Receive_IT(&huart5, &sRX.tmp, 1);
	}
}

