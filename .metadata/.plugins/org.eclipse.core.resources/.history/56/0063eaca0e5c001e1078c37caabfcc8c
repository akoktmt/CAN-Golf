#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "canframe.h"
#include "stm32f1xx_hal_can.h"
//extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart1;
uint8_t CAN_Init(CAN_HandlerStruct* canhandler, CAN_HandleTypeDef* hcan, uint16_t ID)
{
	canhandler->hcan = hcan;
	canhandler->SenderID = ID;
	canhandler->fillterbank = 0;
	return HAL_OK;
}
uint8_t CAN_Send_Dataframe(CAN_HandlerStruct* canhandler,CANConfigIDTxtypedef* pIDtype, uint8_t *Data, uint32_t Datalength)
{
	CAN_TxHeaderTypeDef Txheader;
	/*Config Frame----------------------------------------------------------------*/
	Txheader.DLC=8;
	Txheader.RTR=CAN_RTR_DATA;
	Txheader.IDE=CAN_ID_STD;
	/*Config ID-------------------------------------------------------------------*/
	uint32_t Txmailbox;
	uint16_t ID_NUM = 0x00;
	uint8_t Message_ID = pIDtype->MessageType;
	uint8_t Target_ID=pIDtype->TargetNode;
	uint8_t Frame_type=FIRST_FRAME;
	ID_NUM |= Message_ID;
	ID_NUM =(ID_NUM <<4)|Target_ID;
	/*Implement send data----------------------------------------------------------*/
	uint8_t* frame = (uint8_t*)malloc(CAN_MAX_DATA_LENGTH);
	uint32_t frameIndex = 0;
	uint32_t length = Datalength;
	int isFirstFrame = 1;
	int isLastFrame=0;
	//uint32_t startTime = HAL_GetTick();
	/*Add SenderID vs Data length at first frame-----------------------------------*/
	for (int i = 0; i < Datalength; i++)
	{
		uint8_t byte = Data[i];
		if (isFirstFrame)
		{
			frame[frameIndex] = pIDtype->SenderID;
			frameIndex++;
			frame[frameIndex] = length;
			frameIndex++;
			isFirstFrame = 0;
		}
		/*Add byte into frame data----------------------------------------------------*/
		frame[frameIndex] = byte;
		frameIndex++;
		/*Check if frame data is not fill, add FILL byte until frame full 8bytes------*/
		if (frameIndex == CAN_MAX_DATA_LENGTH || i == Datalength - 1)
		{
			while (frameIndex < CAN_MAX_DATA_LENGTH)
			{
				frame[frameIndex] = FILL_VALUE;
				frameIndex++;
			}
			/*Check last frame------------------------------------------------------*/
			if(i == Datalength - 1)
			{
				isLastFrame=1;
				ID_NUM=(ID_NUM <<3)|END_FRAME;
			}
			if(isLastFrame==0){
				ID_NUM =(ID_NUM <<3)|Frame_type;
			}
			Txheader.StdId=ID_NUM;
			/*send data--------------------------------------------------------------------*/
			if(HAL_CAN_AddTxMessage(canhandler->hcan,&Txheader,frame,&Txmailbox)!=HAL_OK)
			{
				Error_Handler();
			}
			/*Check if a transmission request is pending on the selected TxMailboxes------*/
			while(HAL_CAN_IsTxMessagePending(canhandler->hcan,Txmailbox))
				/*Decrease data frame and set frame turn 0 again------------------------------*/
				memset(frame, 0, CAN_MAX_DATA_LENGTH);
			frameIndex = 0;
			/*add SenderID for every 1st next frame---------------------------------------*/
			frame[0] = pIDtype->SenderID;
			frameIndex++;
			ID_NUM=ID_NUM>>3;
			Frame_type++;
		}
	}
	free(frame);
	return HAL_OK;
}
/*uint8_t CAN_Receive_Dataframe(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data)
{
		while(! HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0))

    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, RxHeader, Data) != HAL_OK)
    {
        Error_Handler();
        return 0;
    }
    return 1;
}*/
/*CAN Receive Data---------------------------------------------------------------------------------------------------------------------------------*/
//uint32_t addArrayToBuffer(uint8_t* buffer, uint8_t* array, int size, int* bufferIndex) {
//    for (int i = 0; i < size; i++) {
//        buffer[*bufferIndex] = array[i];
//        (*bufferIndex)++;
//    }
//}
uint8_t CAN_Receive_Dataframe(CAN_HandlerStruct* canhandler,CANConfigIDRxtypedef* pIDtype,uint8_t *ReceiveData, uint32_t *ReceiveLength)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t frame[CAN_MAX_DATA_LENGTH] = {0};
	uint8_t isLastFrame = 0;
	uint8_t SenderId=0;
	uint8_t LengthRecData=0;
	CANConfigIDRxtypedef *Rec=(CANConfigIDRxtypedef*)malloc(CAN_MAX_DATA_LENGTH);
	for (int i=0; i< 13; i++)
	{
		Rec[i].Index=0;
		Rec[i].ExpectedLength=0;
		Rec[i].frameIndex=0;
	}
		while(!isLastFrame){
			while (HAL_CAN_GetRxFifoFillLevel(canhandler->hcan, CAN_RX_FIFO0) == 0);
			if (HAL_CAN_GetRxMessage(canhandler->hcan, CAN_RX_FIFO0, &RxHeader, frame) != HAL_OK)
			{
				Error_Handler();
			}
			SenderId=frame[0];
			uint16_t ID_NUM = RxHeader.StdId;
			pIDtype -> Frametype = ID_NUM & 0x07;
			pIDtype -> TargetNode = (ID_NUM >> 3) & 0x0F;
			pIDtype ->  MessageType= (ID_NUM >> 7) & 0x0F;
			switch (pIDtype->Frametype) {
			  case FIRST_FRAME:
			    Rec[SenderId].ExpectedLength = frame[1];
			    Rec[SenderId].frameIndex = 2;
			    break;
			  case END_FRAME:
			    Rec[SenderId].frameIndex = 1;
			    isLastFrame = 1;
			    break;
			  default:
			    Rec[SenderId].frameIndex = 1;
			    break;
			}
			for(;Rec[SenderId].frameIndex<CAN_MAX_DATA_LENGTH;Rec[SenderId].frameIndex++){
				Rec[SenderId].ReceivedBuffer[Rec[SenderId].Index]=frame[Rec[SenderId].frameIndex];
				Rec[SenderId].Index++;
			}
		}
	*ReceiveLength=Rec[SenderId].ExpectedLength;
	memcpy(ReceiveData,Rec[SenderId].ReceivedBuffer,Rec[SenderId].ExpectedLength);
	memset(Rec[SenderId].ReceivedBuffer,0,Rec[SenderId].ExpectedLength);
	Rec[SenderId].Index=0;
	free(Rec);
	return HAL_OK;
}
uint8_t CAN_Send_Request(CAN_HandlerStruct* canhandler,uint8_t Data)
{
	uint32_t Txmailbox;
	CAN_TxHeaderTypeDef Txheader;
	Txheader.DLC=1;
	Txheader.RTR=CAN_RTR_DATA;
	Txheader.IDE=CAN_ID_STD;
	Txheader.StdId=TEST_FRAME;
	if(HAL_CAN_AddTxMessage(canhandler->hcan,&Txheader,&Data,&Txmailbox)!=HAL_OK)
	{
		Error_Handler();
	}
	while(HAL_CAN_IsTxMessagePending(canhandler->hcan,Txmailbox));
	return 0;
}
uint16_t CAN_RECEIVE_ACK(CAN_HandlerStruct* canhandler)
{
	CAN_RxHeaderTypeDef Rxheader;
	uint8_t frame=0;
	uint8_t ack=0;
	while (HAL_CAN_GetRxFifoFillLevel(canhandler->hcan, CAN_RX_FIFO0) == 0)
		if (HAL_CAN_GetRxMessage(canhandler->hcan, CAN_RX_FIFO0, &Rxheader, frame) != HAL_OK)
		{
			Error_Handler();
		}
	if(Rxheader.StdId==TEST_FRAME&&frame==1){
		return HAL_ERROR;
	}else if(Rxheader.StdId==TEST_FRAME&&frame==0)
	{
		return HAL_OK;
	}
}
uint32_t CAN_Config_filtering(CAN_HandlerStruct *Can, uint16_t NodeID)
{
	CAN_FilterTypeDef Can_filter_init;
	Can_filter_init.FilterActivation=ENABLE;
	Can_filter_init.FilterBank=Can->fillterbank++;
	if(Can->fillterbank > 14){
		Error_Handler();
	}
	Can_filter_init.FilterFIFOAssignment=CAN_RX_FIFO0;
	Can_filter_init.FilterIdHigh=NodeID<<8;
	Can_filter_init.FilterIdLow= 0x0000;
	Can_filter_init.FilterMaskIdHigh= 0x0F00;
	Can_filter_init.FilterMaskIdLow= 0x0000;
	Can_filter_init.FilterMode=CAN_FILTERMODE_IDMASK;
	Can_filter_init.FilterScale=CAN_FILTERSCALE_32BIT;
	if(HAL_CAN_ConfigFilter(Can->hcan,&Can_filter_init)!=HAL_OK)
	{
		Error_Handler();
	}
	return HAL_OK;
}
