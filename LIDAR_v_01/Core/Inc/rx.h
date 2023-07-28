#ifndef INC_RX_H_
#define INC_RX_H_

#include <stdint.h>

#define RX_SIZE		1024

typedef struct _sRX
{
	uint8_t tmp;
	uint8_t rxbuf[RX_SIZE];

	uint16_t pop_cnt;
	uint16_t push_cnt;
	uint16_t strpt_cnt; //데이터 프로토콜 시작하는 인덱스값
	uint8_t samp_num;



} _sRX;

void RX_Struct_Init();
void RX_Init();
void RX_Doing();


#endif /* INC_RX_H_ */
