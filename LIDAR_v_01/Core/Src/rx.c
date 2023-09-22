 #include "all.h"
//
extern UART_HandleTypeDef huart5; // STM과 LIDAR의 통신을 위한 포트
extern UART_HandleTypeDef huart3; // STM과 PC의 통신을 위한 포트
extern UART_HandleTypeDef huart4; // BLE사용을 위한 포트
//
_sRX_TMP sRX_Debug;
_sRX_MSG sRX_Debug_Save;
//
_sRX_TMP sRX_BLE;
//
_sRX_TMP sRX_TMP;
_sRX_MSG sRX_MSG;
_uData uData;

//
void RX_Struct_Init() // 구조체들 전부 초기화
{
	memset(&sRX_Debug, 0, sizeof(sRX_Debug));
	memset(&sRX_BLE, 0, sizeof(sRX_BLE));
	//
	memset(&sRX_TMP, 0, sizeof(sRX_TMP));
	memset(&sRX_MSG, 0, sizeof(sRX_MSG));
}

void RX_Init() // 인터럽트 받기시작
{
	setvbuf(stdout, NULL, _IONBF, 0);
	HAL_UART_Receive_IT(&huart3, &sRX_Debug.tmp, 1);// UART3 = for Debug
	HAL_UART_Receive_IT(&huart4, &sRX_BLE.tmp, 1);// UART4 = for BLE
	HAL_UART_Receive_IT(&huart5, &sRX_TMP.tmp, 1); //UART5 = for LIDAR
}

//void Cal_angle(uint8_t FSA_LSB,uint8_t FSA_MSB,uint8_t LSA_LSB,uint8_t LSA_MSB)
//{//
//	uint16_t FSA = (FSA_MSB << 8)+FSA_LSB;
//	uint16_t LSA = (LSA_MSB << 8)+LSA_LSB;
//
//	uint16_t Angle_FSA = FSA >> 7;
//	uint16_t Angle_LSA = LSA >> 7;
//	uint16_t diff_Angle;
//
////	printf("FSA = %04X\n\r", FSA);
////	printf("LSA = %04X\n\r", LSA);
//	printf("Angle_FSA = %03d\n\r", Angle_FSA);
//	printf("Angle_LSA = %03d\n\r", Angle_LSA);
//	if(Angle_LSA-Angle_FSA >= 0)
//	{
//		diff_Angle = Angle_LSA-Angle_FSA;
//	}
//	else
//	{
//		diff_Angle = Angle_LSA-Angle_FSA+360;
//	}
//	printf("diff(Angle) = %03d\n\r",diff_Angle);
//}

void RX_Doing()
{
	if (sRX_TMP.pop_cnt != sRX_TMP.push_cnt)
	{
		sRX_MSG.tmp = sRX_TMP.rxbuf[sRX_TMP.pop_cnt++];
		sRX_TMP.pop_cnt %= MAX_RX_SIZE;

		// 1.check start
		if (!sRX_MSG.start_flag)
		{
			if ((sRX_MSG.cnt == 0) && (sRX_MSG.tmp == 0xAA))
			{
				memset(&sRX_MSG, 0, sizeof(sRX_MSG));
				sRX_MSG.rxbuf[sRX_MSG.cnt++] = 0xAA;
			}

			else if ((sRX_MSG.cnt == 1) && (sRX_MSG.tmp == 0x55))
			{
				sRX_MSG.rxbuf[sRX_MSG.cnt++] = 0x55;
				sRX_MSG.start_flag = true;
			}
		}

		// 2.put else data
		else if (sRX_MSG.start_flag)
		{
			sRX_MSG.rxbuf[sRX_MSG.cnt++] = sRX_MSG.tmp;

			// check size
			uint16_t size = (sRX_MSG.rxbuf[3] * 2) + DATA_HEADER_SIZE;
			if (sRX_MSG.cnt >= size)
			{
				// copy
				memcpy(&uData.rxbuf[0], &sRX_MSG.rxbuf[0], size);

				// debug
				// 1.header
//				printf(" ==============================\n\r");
//				printf(" [Header] PH : 0x%04X\n\r", (int)__REV16(uData.sData.sHeader.ph));
//				printf(" [Header] CT : 0x%02X\n\r", uData.sData.sHeader.ct);
//				printf(" [Header] LSN: 0x%02X\n\r", uData.sData.sHeader.lsn);
//				printf(" [Header] FSA: 0x%04X\n\r", (int)__REV16(uData.sData.sHeader.fsa));
//				printf(" [Header] LSA: 0x%04X\n\r", (int)__REV16(uData.sData.sHeader.lsa));
//				printf(" [Header] CS : 0x%04X\n\n\r", (int)__REV16(uData.sData.sHeader.cs));

				// 2.data
//				for (int num = 0; num < sRX_MSG.rxbuf[3]; num++)
//				{
//					printf(" [Data] S[%03d]: 0x%04X\n\r", num + 1, (int)__REV16(uData.sData.sSamp.buf[num]));
//				}
//				printf("\n\r");

				// 3.Value
				double fsa = (double)__REV16(uData.sData.sHeader.fsa) / 128;
				double lsa = (double)__REV16(uData.sData.sHeader.lsa) / 128;
				if (fsa > 360) fsa -= 360;
				if (lsa > 360) lsa -= 360;
				double dif = lsa - fsa;
				if (dif < 0) dif += 360;

				// angle
				printf(" [Angle] FSA: %.2f[']\n\r", fsa);
				printf(" [Angle] LSA: %.2f[']\n\r", lsa);
				printf(" [Angle] Dif: %.2f[']\n\n\r", dif);

				// distance
				for (int num = 0; num < sRX_MSG.rxbuf[3]; num++)
				{
					double distance = (double)__REV16(uData.sData.sSamp.buf[num]);
					printf(" [Distance] S[%03d]: %.2f[m]\n\r", num + 1, distance / 4000);
				}
				printf("\n\r");

				// init
				sRX_MSG.start_flag = false;
				sRX_MSG.cnt = 0;
			}
		}

	}
}
void RX_Doing2()
{
	if (sRX_TMP.pop_cnt != sRX_TMP.push_cnt)
	{
		sRX_MSG.tmp = sRX_TMP.rxbuf[sRX_TMP.pop_cnt++];
		sRX_TMP.pop_cnt %= MAX_RX_SIZE;

		// 1.check start
		if (!sRX_MSG.start_flag)
		{
			if ((sRX_MSG.cnt == 0) && (sRX_MSG.tmp == 0xAA))
			{
				memset(&sRX_MSG, 0, sizeof(sRX_MSG));
				sRX_MSG.rxbuf[sRX_MSG.cnt++] = 0xAA;
			}

			else if ((sRX_MSG.cnt == 1) && (sRX_MSG.tmp == 0x55))
			{
				sRX_MSG.rxbuf[sRX_MSG.cnt++] = 0x55;
				sRX_MSG.start_flag = true;
			}
		}

		// 2.put else data
		else if (sRX_MSG.start_flag)
		{
			sRX_MSG.rxbuf[sRX_MSG.cnt++] = sRX_MSG.tmp;

			// check size
			uint16_t size = (sRX_MSG.rxbuf[3] * 2) + DATA_HEADER_SIZE;
			if (sRX_MSG.cnt >= size)
			{
				// copy
				memcpy(&uData.rxbuf[0], &sRX_MSG.rxbuf[0], size);

				double fsa = (double)__REV16(uData.sData.sHeader.fsa) / 128;
				double lsa = (double)__REV16(uData.sData.sHeader.lsa) / 128;
				double ang_diff;
				double sampled_data;
				double angle;
				double distance;
				double angle_correct;
				double angle_final;
				uint16_t distance_sum = 0;

				uint8_t lsn;
				if (fsa > lsa) ang_diff = fabs(fsa-(360+lsa));
				else ang_diff = fabs(fsa - lsa);

				lsn = uData.sData.sHeader.lsn;
				double angle_buff[(int)lsn];
				double distance_buff[2*(int)lsn];
				double angle_correct_buff[(int)lsn];
				double angle_final_buff[(int)lsn];

				for(int i = 0;i <2*(int)lsn;i+=2)
				{
					distance_sum = uData.sData.sSamp.buf[i]+(uData.sData.sSamp.buf[i+1]*256);
					distance = ((double)distance_sum)/4;
					distance_buff[i] = distance;
				}//이부분을 개선해야됨******************
				for(int i = 0;i <(int)lsn;i++)
				{
					angle = (((ang_diff/(((int)lsn)-1))*(i+1))+fsa);

					if(distance_buff[i] ==0 )
						angle_correct = 0;
					else
						angle_correct = atan2(21.8 *(155.3 - distance),(155.3 * distance))*180/3.14;

					angle_final_buff[i] = angle+angle_correct;

					if (angle_final_buff[i] >=360)
						angle_final_buff[i]-= 360;
					else
						angle_final_buff[i]=angle_final_buff[i];

					printf(" [debug] angle_final : %.3f[']\n\r", angle_final_buff[i]);
					//HAL_UART_Transmit(&huart4, &angle_final_buff[i], 4, 1);//for debug
				}



//				printf(" [debug] (int)lsn : %.3d[']\n\r", (int)lsn);//for debug


				//printf(" [debug] angle_correct : %.6f[']\n\r", angle_correct);//for debug
				////////////////////////////////////////////////

//
//				if (fsa > 360) fsa -= 360;
//				if (lsa > 360) lsa -= 360;

				// angle
//				printf(" [Angle] FSA: %.2f[']\n\r", fsa);
//				printf(" [Angle] LSA: %.2f[']\n\r", lsa);
				//printf(" [Angle] Dif: %.2f[']\n\n\r", dif);

				// distance
//				for (int num = 0; num < sRX_MSG.rxbuf[3]; num++)
//				{
//					double distance = (double)__REV16(uData.sData.sSamp.buf[num]);
//					printf(" [Distance] S[%03d]: %.2f[m]\n\r", num + 1, distance / 4000);
//				}
				printf("\n\r");

				// init
				sRX_MSG.start_flag = false;
				sRX_MSG.cnt = 0;
			}
		}

	}
}
void RX_Debug()
{
	if (sRX_Debug.pop_cnt != sRX_Debug.push_cnt)
	{
		sRX_Debug_Save.tmp = sRX_Debug.rxbuf[sRX_Debug.pop_cnt++];
		sRX_Debug.pop_cnt %= MAX_RX_SIZE;
		//
		printf("DEBUG = %c", sRX_Debug_Save.tmp);
		sRX_Debug_Save.rxbuf[sRX_Debug_Save.cnt++] = sRX_Debug_Save.tmp;
		sRX_Debug_Save.cnt %= MAX_RX_SIZE;

		// 응답값 코드(어플에서 확인되는값)
		if ((sRX_Debug_Save.tmp == '\n') || (sRX_Debug_Save.tmp == '\r'))
		{
			HAL_UART_Transmit(&huart4, &sRX_Debug_Save.rxbuf[0], (sRX_Debug_Save.cnt - 1), 20);

			// init
			memset(&sRX_Debug_Save, 0, sizeof(sRX_Debug_Save));
		}
	}
}
void RX_Debug2() //for BLE test(test용)
{
	HAL_UART_Transmit(&huart4, &sRX_BLE.tmp, 1, 1); // BLE로 보낸값을 그대로 다시 보냄
	printf("%c\n\r",sRX_BLE.tmp);
}
void RX_Debug3() //LIDAR값을 BLE모듈로 보내기(test용)
{
	if (sRX_TMP.pop_cnt != sRX_TMP.push_cnt)
	{
		sRX_MSG.tmp = sRX_TMP.rxbuf[sRX_TMP.pop_cnt++];
		sRX_TMP.pop_cnt %= MAX_RX_SIZE;
		///////////////
		printf("%02X ",sRX_MSG.tmp);
		HAL_UART_Transmit(&huart4, &sRX_MSG.tmp, 1, 1); // BLE로 보낸값을 그대로 다시 보냄
		if(sRX_TMP.pop_cnt%10==0)
			printf("\n\r");
	}
}
int RX_BLE2()
{
	printf("BLE.tmp = %c\n\r",sRX_BLE.tmp);
	int x =sRX_BLE.tmp-'0';
	return x;
}
int __io_putchar(int ch)
{
	while (HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 1) != HAL_OK);
	return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // 인터럽트로 값을 저장
{
	if (huart->Instance == huart3.Instance)//IDE의 시리얼 모니터상에 보이는 값
	{
		sRX_Debug.rxbuf[sRX_Debug.push_cnt++] = sRX_Debug.tmp;
		sRX_Debug.push_cnt %= MAX_RX_SIZE;
		HAL_UART_Receive_IT(&huart3, &sRX_Debug.tmp, 1);
	}

	else if (huart->Instance == huart4.Instance)
	{
		HAL_UART_Transmit(&huart3, &sRX_BLE.tmp, 4, 1);
		HAL_UART_Receive_IT(&huart4, &sRX_BLE.tmp, 4);
	}

	else if (huart->Instance == huart5.Instance) // UART5에서 instance가 발생하면
	{
		sRX_TMP.rxbuf[sRX_TMP.push_cnt++] = sRX_TMP.tmp;
		sRX_TMP.push_cnt %= MAX_RX_SIZE;
		HAL_UART_Receive_IT(&huart5, &sRX_TMP.tmp, 1);
	}
}

