#ifndef INC_RX_H_
#define INC_RX_H_

#include <stdint.h>
#include <stdbool.h>
//
#define MAX_RX_SIZE		1024
//
#define DATA_HEADER_SIZE	10
#define MAX_SAMP_SIZE		300
#define MAX_DATA_SIZE		(DATA_HEADER_SIZE + MAX_SAMP_SIZE)
//
typedef struct _sRX_TMP
{
	uint8_t tmp;
	uint8_t rxbuf[MAX_RX_SIZE];
	//
	uint16_t pop_cnt;
	uint16_t push_cnt;
} _sRX_TMP;

typedef struct _sRX_MSG
{
	bool start_flag;
	uint8_t tmp;
	uint8_t rxbuf[MAX_DATA_SIZE];
	uint16_t cnt;
} _sRX_MSG;

typedef struct _sData
{
	struct _sData_Header
	{
		uint16_t ph;
		uint8_t ct;
		uint8_t lsn;
		uint16_t fsa;
		uint16_t lsa;
		uint16_t cs;
	} sHeader;

	struct _sData_Samp
	{
		uint16_t buf[MAX_SAMP_SIZE];
	} sSamp;
} _sData;

typedef union _uData
{
	uint8_t rxbuf[MAX_DATA_SIZE];
	_sData sData;
} _uData;

void RX_Struct_Init();
void RX_Init();
void RX_Doing();
void RX_Debug();


#endif /* INC_RX_H_ */
