/*
 * CAN_Flag.c
 *
 *  Created on: Oct 4, 2023
 *      Author: win 10
 */

typedef enum {
		REC_DATA,
		REC_FRAMEDATA_ERROR,
		REC_PACKET_ERROR,
		REC_SUCCESS
}FlagRecDataEnum;

typedef enum{
	SEND_DATA,
	SEND_FRAME_ERROR,
	SEND_PACKET_ERROR,
	SEND_SUCCESS,
	NOT_SEND
}FlagSendDataEnum;
