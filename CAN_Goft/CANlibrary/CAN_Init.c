/*
 * CAN_Init.c
 *
 *  Created on: Oct 4, 2023
 *      Author: win 10
 */
#include <CAN_OSI.h>
#include <string.h>
void CANBufferHandleStruct_Init(CANBufferHandleStruct *Buffer) {
	NodeBufferHandle defaultNodeHandle = {0};
	Buffer->Buffer_Index = 0;
	Buffer->SenderID = 0;
	Buffer->RecvID = 0;
	Buffer->FrameType_Index = SET_UP_FRAME;
	Buffer->PacketDataLength = 0;
	Buffer->CRCValue = 0;
	Buffer->NumberOfFrame=0;
	memset(Buffer->NetworkBuffer, 0, sizeof(Buffer->NetworkBuffer));
	memset(Buffer->Buffer, 0x55, sizeof(Buffer->Buffer));
	memset(Buffer->StoreData, 0, sizeof(Buffer->StoreData));
	for (int i = 0; i < 16; i++) {
		Buffer->NodeHandle[i] = defaultNodeHandle;
	}
}
void FlagsDataHandle_Init(FlagsDataHandle *FlagInit) {
	FlagInit->Bits.Flag_bit_0 = 0;
	FlagInit->Bits.Flag_bit_1 = 0;
	FlagInit->Bits.Flag_bit_2 = 0;
	FlagInit->Bits.Flag_bit_3 = 0;
	FlagInit->Bits.Flag_bit_4 = 0;
	FlagInit->Bits.Flag_bit_5 = 0;
	FlagInit->Bits.Flag_bit_6 = 0;
	FlagInit->Bits.Flag_bit_7 = 0;
	FlagInit->FlagDuplicate=0;
	FlagInit->SumOfFlag=0;
	memset(FlagInit->FlagFrameFull,0,sizeof(FlagInit->FlagFrameFull));
}
void CAN_TXHeaderConfig(CAN_TxHeaderTypeDef *Txheader, uint32_t StdId) {
	Txheader->DLC = 8;
	Txheader->RTR = CAN_RTR_DATA;
	Txheader->IDE = CAN_ID_STD;
	Txheader->StdId = StdId;
}
void NodeBufferHandle_Init(NodeBufferHandle *NodeBuffer) {
	NodeBuffer->NodeIndex = 0;
	NodeBuffer->CRCValue = 0;
	NodeBuffer->FrameType = 0;
	NodeBuffer->NumberOfFlags = 0;
	memset(NodeBuffer->NodeBuffer, 0, sizeof(NodeBuffer->NodeBuffer));
}
