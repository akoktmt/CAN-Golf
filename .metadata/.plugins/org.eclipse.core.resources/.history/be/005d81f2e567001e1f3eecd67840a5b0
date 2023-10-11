/*
 * CAN_Init.c
 *
 *  Created on: Oct 4, 2023
 *      Author: win 10
 */
#include <CAN_OSI.h>
#include <string.h>
#include "CAN_Flag.h"
void CANBufferHandleStruct_Init(CANBufferHandleStruct *Buffer) {
	NodeBufferHandle defaultNodeHandle = {0};
	Buffer->StdID=0;
	Buffer->Buffer_Index = 0;
	Buffer->SenderID = 0;
	Buffer->RecvID = 0;
	Buffer->FrameType_Index = SET_UP_FRAME;
	Buffer->PacketDataLength = 0;
	Buffer->CRCValue = 0;
	Buffer->NumberOfFrame=0;
	memset(Buffer->Data,0,sizeof(Buffer->Data));
	memset(Buffer->NetworkBuffer, 0, sizeof(Buffer->NetworkBuffer));
	memset(Buffer->Buffer, 0x55, sizeof(Buffer->Buffer));
	memset(Buffer->StoreData, 0, sizeof(Buffer->StoreData));
	for (int i = 0; i < 16; i++) {
		Buffer->NodeHandle[i] = defaultNodeHandle;
	}
}
void FlagsFrameHandle_Init(FlagFrameHandle *FlagInit) {
	FlagIdHandle DefaultFlagID= {0};
	FlagInit->FrameBits.Flag_Frame_0 = 0;
	FlagInit->FrameBits.Flag_Frame_1 = 0;
	FlagInit->FrameBits.Flag_Frame_2 = 0;
	FlagInit->FrameBits.Flag_Frame_3 = 0;
	FlagInit->FrameBits.Flag_Frame_4 = 0;
	FlagInit->FrameBits.Flag_Frame_5 = 0;
	FlagInit->FrameBits.Flag_Frame_6 = 0;
	FlagInit->FrameBits.Flag_Frame_7 = 0;
	for(int i=0; i<16;i++)
	{
		FlagInit->FlagID[i]=DefaultFlagID;
	}
}
void CAN_TXHeaderConfig(CAN_TxHeaderTypeDef *Txheader, uint32_t StdId) {
	Txheader->DLC = 8;
	Txheader->RTR = CAN_RTR_DATA;
	Txheader->IDE = CAN_ID_STD;
	Txheader->StdId = StdId;
}
