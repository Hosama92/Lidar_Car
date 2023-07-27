#include "rx.h"
#include "main.h"
//
#include <stdio.h>
#include <stdint.h>
#include <string.h>
//
extern UART_HandleTypeDef huart5; // STM과 LIDAR의 통신을 위한 포트
extern UART_HandleTypeDef huart3; // STM과 PC의 통신을 위한 포트
//
_sRX sRX; // 구조체 선언

void RX_Struct_Init() // 구조체 sRX의 맴버 값을 전부 0으로 초기화
{
	memset(&sRX, 0, sizeof(struct _sRX));
}

void RX_Init() // 출력버퍼를 초기화한후 인터럽트 발생시 sRX.tmp에 그 값을 1바이트씩 저장
{
	setvbuf(stdout, NULL, _IONBF, 0);
	HAL_UART_Receive_IT(&huart5, &sRX.tmp, 1);
}

void RX_Doing()
{
	if (sRX.pop_cnt != sRX.push_cnt) // pop와 push가 다를때
	{
		sRX.pop_cnt++;
		uint8_t tmp = sRX.rxbuf[sRX.strpt_cnt]; // tmp에 rxbuf값을 넣어준다
		sRX.pop_cnt %= RX_SIZE; // pop카운트는 계속 증가하지만 1024를 넘으면 초기화된다

		// debug check
		if(tmp != 0xAA)
			printf("*******************************************************");
		printf(" %02X", tmp);// tmp에 값은 1바이트씩 저장되있고 그걸 2자리의 16진수로 프린트한다
		printf("\n\r");
		printf("strpt_cnt = %04d",sRX.strpt_cnt);
		printf("\n\r");
//		if (!(sRX.pop_cnt % 10)) // 값을 10개 출력할때마다 줄바꿈을 해준다
//		{
//			printf("\n\r");
//		}
	}
}


int __io_putchar(int ch)
{
	while (HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 1) != HAL_OK);
	return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // 인터럽트로 값을 저장
{
	if (huart->Instance == huart5.Instance) // UART5에서 instance가 발생하면
	{
		sRX.rxbuf[sRX.push_cnt++] = sRX.tmp;
		if (sRX.tmp == 0XAA)
			sRX.strpt_step = 1;
		if (sRX.tmp == 0X55 && sRX.strpt_step == 1)
		{
			sRX.strpt_cnt = sRX.push_cnt-2;
			sRX.strpt_step = 0;
		}

		// tmp에 저장된 값을 rxbuf로 수신
		sRX.push_cnt %= RX_SIZE; // 오버플로우 방지를 위해 1024넘어가면 자릿수 초기화
		HAL_UART_Receive_IT(&huart5, &sRX.tmp, 1);
		// 데이터 도착할때마다 계속 인터럽트 발생시킴
	}
}

