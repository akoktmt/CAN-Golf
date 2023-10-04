#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "canframe.h"
#include "stm32f1xx_hal_can.h"
#include "CRC.h"
#include "CAN_Flag.h"
extern CAN_HandleTypeDef hcan;
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
uint16_t CAN_Send_Response(uint8_t ID,uint8_t Opcode,uint8_t FrameType){
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t Txmailbox;
	uint8_t OpcodeData[8] = { Opcode, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
	uint8_t StID=0x00;
	StID|=ID;
	StID=(StID<<3)|FrameType;
	CAN_TXHeaderConfig(&TxHeader, StID);
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, OpcodeData, &Txmailbox)
			!= HAL_OK) {
		Error_Handler();
	}
	while (HAL_CAN_IsTxMessagePending(&hcan, Txmailbox))
		;
	return HAL_OK;
}
void processFrame(FlagsDataHandle *FlagHandle,uint8_t ID,CANBufferHandleStruct *RxBuffer,uint8_t FrameType,uint8_t* Data) {
    if (RxBuffer->NodeHandle[ID].FrameType == FrameType && FlagHandle->FlagFrameFull[FrameType] == 0) {
    	memcpy(RxBuffer->NodeHandle[ID].NodeBuffer[RxBuffer->NodeHandle[ID].FrameType], Data, CAN_MAX_DATA);
        FlagHandle->FlagFrameFull[FrameType]= 1;
        FlagHandle->SumOfFlag+=FlagHandle->FlagFrameFull[FrameType];
    }
}

uint8_t CAN_Send_Network_Packet(CANBufferHandleStruct *TxBuffer, uint8_t *Data,
		uint8_t DataLength) {
	TxBuffer->PacketDataLength = DataLength + 2;
	TxBuffer->CRCValue = crc_8(Data, DataLength);
	TxBuffer->Buffer_Index = DataLength;
	if (TxBuffer->PacketDataLength % 8 == 0) {
		TxBuffer->NumberOfFrame = (TxBuffer->PacketDataLength / 8);
	} else {
		TxBuffer->NumberOfFrame = (TxBuffer->PacketDataLength / 8) + 1;
	}
	memcpy(TxBuffer->NetworkBuffer, Data, DataLength);
	TxBuffer->NetworkBuffer[TxBuffer->Buffer_Index] =
			TxBuffer->PacketDataLength;
	TxBuffer->NetworkBuffer[TxBuffer->Buffer_Index + 1] = TxBuffer->CRCValue;
	TxBuffer->Buffer_Index = 0;
	return HAL_OK;
}
uint8_t CAN_Send_DataLink_Separate(CANBufferHandleStruct *TxBuffer,
		uint8_t *Data) {
	CAN_Send_Network_Packet(TxBuffer, Data, DATA_TEST);
	uint8_t PacketLength = TxBuffer->PacketDataLength;
	uint8_t NumberOfFrame = TxBuffer->NumberOfFrame;
	TxBuffer->Buffer[NumberOfFrame - 1][6] = PacketLength;
	TxBuffer->Buffer[NumberOfFrame - 1][7] = TxBuffer->CRCValue;
	for (int i = 0; i < NumberOfFrame; i++) {
		for (TxBuffer->Buffer_Index = 0; TxBuffer->Buffer_Index < 8;
				TxBuffer->Buffer_Index++) {
			TxBuffer->Buffer[i][TxBuffer->Buffer_Index] =
					TxBuffer->NetworkBuffer[i * 8 + TxBuffer->Buffer_Index];
			PacketLength--;
			if (PacketLength == 2) {
				break;
			}
		}
		if (PacketLength == 2) {
			break;
		}
	}
	TxBuffer->Buffer_Index = 0;
	return HAL_OK;
}
uint8_t CAN_Send_Physical_Send(CANBufferHandleStruct *TxBuffer, uint8_t *Data,
		uint8_t DataLength, CANConfigIDTxtypedef *pIDtype, uint32_t Txmailbox) {
	CAN_Send_DataLink_Separate(TxBuffer, Data);
	CAN_TxHeaderTypeDef Txheader;
	uint8_t Message_ID = pIDtype->MessageType;
	uint8_t Sender_ID = pIDtype->SenderID;
	uint8_t FrameType = TxBuffer->FrameType_Index;
	uint8_t NumberOfFrame = TxBuffer->NumberOfFrame;
	uint16_t StdId = 0x00;

	StdId |= Message_ID;
	StdId = (StdId << 4) | Sender_ID;
	StdId = (StdId << 3) | TxBuffer->FrameType_Index;

	Txheader.DLC = 8;
	Txheader.RTR = CAN_RTR_DATA;
	Txheader.IDE = CAN_ID_STD;
	CAN_Store_Data(TxBuffer, pIDtype);
	for (int8_t i = NumberOfFrame - 1; i >= 0; i--) {
		Txheader.StdId = StdId;
		if (HAL_CAN_AddTxMessage(&hcan, &Txheader, TxBuffer->Buffer[i],
				&Txmailbox) != HAL_OK) {
			Error_Handler();
		}
		while (HAL_CAN_IsTxMessagePending(&hcan, Txmailbox))
			;
		StdId = StdId >> 3;
		FrameType++;
		StdId = (StdId << 3) | FrameType;
	}
	return HAL_OK;
}

uint8_t CAN_Store_Data(CANBufferHandleStruct *Store, CANConfigIDTxtypedef *ID) {
	uint8_t FrameType = Store->FrameType_Index;
	uint8_t BufferIndex = Store->Buffer_Index;
	uint8_t NumberFrame = Store->NumberOfFrame;
	for (int i = 0; i < NumberFrame; i++) {
		Store->StoreData[i][BufferIndex] = ID->SenderID;
		BufferIndex++;
		Store->StoreData[i][BufferIndex] = FrameType;
		BufferIndex++;
		for (; BufferIndex < 10; BufferIndex++) {
			Store->StoreData[i][BufferIndex] =
					Store->Buffer[i][BufferIndex - 2];
		}
		FrameType++;
		BufferIndex = 0;
	}
	return HAL_OK;
}

uint8_t CAN_Recieve_Physical(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data) {

	while (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0)
		;
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, RxHeader, Data) != HAL_OK) {
		Error_Handler();
	}
	return HAL_OK;
}

uint8_t CAN_Receive_DataLink(CAN_RxHeaderTypeDef *RxHeader,
		FlagsDataHandle *FlagHandle, CANBufferHandleStruct *RxBuffer,FlagRecDataEnum *FlagRecHandle) {
	uint8_t Data[8] = { 0 };
	uint16_t StdID = 0;
	uint8_t ID = 0;
	uint8_t FrameType = 0;
	CAN_Recieve_Physical(RxHeader, Data);

	StdID = RxHeader->StdId;
	ID = (StdID >> 3) & 15;
	RxBuffer->NodeHandle[ID].FrameType = StdID & 7;
	if (RxBuffer->NodeHandle[ID].FrameType == SET_UP_FRAME&&RxBuffer->NodeHandle[ID].DuplicateFrame!=1) {
		RxBuffer->NodeHandle[ID].DuplicateFrame=1;
		RxBuffer->NodeHandle[ID].PacketLength = Data[6];
		RxBuffer->NodeHandle[ID].CRCValue = Data[7];
		if (RxBuffer->NodeHandle[ID].PacketLength % 8 == 0) {
			RxBuffer->NodeHandle[ID].NumberOfFrame = (RxBuffer->NodeHandle[ID].PacketLength / 8);
		} else {
			RxBuffer->NodeHandle[ID].NumberOfFrame = (RxBuffer->NodeHandle[ID].PacketLength / 8) + 1;
		}
	}
	else
	{
		if(RxBuffer->NodeHandle[ID].FrameType == SET_UP_FRAME)
		{
			//FlagFrameError
		}
	}
	switch (ID) {
	case OBSTALCE8:
		RxBuffer->NodeHandle[ID].NodeIndex++;
		for(FrameType=0; FrameType<RxBuffer->NodeHandle[ID].NumberOfFrame;FrameType++)
		{
			processFrame(FlagHandle, ID, RxBuffer,FrameType, Data);
		}
		if(RxBuffer->NodeHandle[ID].NodeIndex==RxBuffer->NodeHandle[ID].NumberOfFrame)
		{
			if(FlagHandle->SumOfFlag=RxBuffer->NodeHandle[ID].NumberOfFrame)
			{
				*FlagRecHandle=REC_SUCCESS;
			}
			else
			{
				for(FrameType=0;FrameType<=RxBuffer->NodeHandle[ID].NumberOfFrame;FrameType++)
				{
					if(FlagHandle->FlagFrameFull[FrameType]==0)
					{
						CAN_Send_Response(OBSTALCE8, FRAME_ERROR, FrameType);
					}
				}
			}
		}
	break;
	case OBSTALCE7:
		for(FrameType=0; FrameType<SIZE_FRAME_DATA;FrameType++)
				{
					processFrame(FlagHandle, ID, RxBuffer,FrameType, Data);
				}
		break;
	case OBSTALCE6:

		break;
	case OBSTALCE5:

		break;
	case OBSTALCE4:

		break;
	case OBSTALCE3:

		break;
	case OBSTALCE2:

		break;
	case OBSTALCE1:

		break;
	case POWER:

		break;
	case STEERING:

		break;
	case MASTER:

		break;
	case LIGHT_GPS:

		break;
	case ENGINE_CONTROL:

		break;
	case ALL_NODE:

		break;

}
	return HAL_OK;
}
uint8_t CAN_Receive_App(uint8_t *Data) {
FlagsDataHandle Flag;
//CAN_Receive_NetWork(Data,&Flag);

return HAL_OK;
}

//uint8_t CAN_Receive_NetWork(FlagsDataHandle Flag,CANBufferHandleStruct *NetBuffer)
//{
//	//check
//}

//uint8_t CAN_Receive_Dataframe(CAN_HandlerStruct* canhandler,CANConfigIDRxtypedef* pIDtype,uint8_t *ReceiveData, uint32_t *ReceiveLength)
//{
//	CAN_RxHeaderTypeDef RxHeader;
//	uint8_t frame[CAN_MAX_DATA_LENGTH] = {0};
//	uint8_t isLastFrame = 0;
//	uint8_t SenderId=0;
//	CANConfigIDRxtypedef *Rec=(CANConfigIDRxtypedef*)malloc(CAN_MAX_DATA_LENGTH);
//	for (int i=0; i< 13; i++)
//	{
//		Rec[i].Index=0;
//		Rec[i].ExpectedLength=0;
//		Rec[i].frameIndex=0;
//	}
//		while(!isLastFrame){
//			while (HAL_CAN_GetRxFifoFillLevel(canhandler->hcan, CAN_RX_FIFO0) == 0);
//			if (HAL_CAN_GetRxMessage(canhandler->hcan, CAN_RX_FIFO0, &RxHeader, frame) != HAL_OK)
//			{
//				Error_Handler();
//			}
//			SenderId=frame[0];
//			uint16_t ID_NUM = RxHeader.StdId;
//			pIDtype -> Frametype = ID_NUM & 0x07;
//			pIDtype -> TargetNode = (ID_NUM >> 3) & 0x0F;
//			pIDtype ->  MessageType= (ID_NUM >> 7) & 0x0F;
//			switch (pIDtype->Frametype) {
//			  case FIRST_FRAME:
//			    Rec[SenderId].ExpectedLength = frame[1];
//			    Rec[SenderId].frameIndex = 2;
//			    break;
//			  case END_FRAME:
//			    Rec[SenderId].frameIndex = 1;
//			    isLastFrame = 1;
//			    break;
//			  default:
//			    Rec[SenderId].frameIndex = 1;
//			    break;
//			}
//			for(;Rec[SenderId].frameIndex<CAN_MAX_DATA_LENGTH;Rec[SenderId].frameIndex++){
//				Rec[SenderId].ReceivedBuffer[Rec[SenderId].Index]=frame[Rec[SenderId].frameIndex];
//				Rec[SenderId].Index++;
//			}
//		}
//	*ReceiveLength=Rec[SenderId].ExpectedLength;
//	memcpy(ReceiveData,Rec[SenderId].ReceivedBuffer,Rec[SenderId].ExpectedLength);
//	memset(Rec[SenderId].ReceivedBuffer,0,Rec[SenderId].ExpectedLength);
//	Rec[SenderId].Index=0;
//	free(Rec);
//	return HAL_OK;
//}

uint32_t CAN_Config_filtering(void) {
CAN_FilterTypeDef Can_filter_init;
Can_filter_init.FilterActivation = ENABLE;
Can_filter_init.FilterBank = 0;
Can_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
Can_filter_init.FilterIdHigh = 0x0000;
Can_filter_init.FilterIdLow = 0x0000;
Can_filter_init.FilterMaskIdHigh = 0x0000;
Can_filter_init.FilterMaskIdLow = 0x0000;
Can_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
Can_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
if (HAL_CAN_ConfigFilter(&hcan, &Can_filter_init) != HAL_OK) {
	Error_Handler();
}
return HAL_OK;
}
//	void ProcessError(void) {
//		//print errormess
//	}
