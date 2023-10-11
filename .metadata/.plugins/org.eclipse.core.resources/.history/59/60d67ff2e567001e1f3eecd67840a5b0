/*
 * CAN_Flag.h
 *
 *  Created on: Oct 4, 2023
 *      Author: win 10
 */

#ifndef CAN_FLAG_H_
#define CAN_FLAG_H_

#define ACK 0b10000001
#define FRAME_ERROR 0b10000011
#define PACKET_ERROR 0b10000111

typedef enum {
		REC_DATA,
		REC_FRAMEDATA_ERROR,
		REC_FRAMEDATA_SUCCESS,
		REC_PACKET_ERROR,
		REC_PACKET_SUCCESS,
		REC_SUCCESS
}FlagRecNotification;

typedef enum{
	SEND_DATA,
	SEND_FRAME_ERROR,
	SEND_PACKET_ERROR,
	SEND_SUCCESS,
	NOT_SEND
}FlagSendDataEnum;

typedef struct
{
	uint8_t FlagFrameFull[8];
	uint8_t SumOfFlag;
	uint8_t FrameError[8];
}FlagIdHandle;

typedef struct{
	FlagIdHandle FlagID[16];
	uint8_t ID;
	uint8_t NumberOfFrame;
	union {
     struct {
    	 	uint8_t Flag_Frame_0:1;
			uint8_t Flag_Frame_1:1;
			uint8_t Flag_Frame_2:1;
			uint8_t Flag_Frame_3:1;
			uint8_t Flag_Frame_4:1;
			uint8_t Flag_Frame_5:1;
			uint8_t Flag_Frame_6:1;
			uint8_t Flag_Frame_7:1;
     	 }FrameBits;
        uint8_t flags;
	};
}FlagFrameHandle;

#endif /* CAN_FLAG_H_ */
