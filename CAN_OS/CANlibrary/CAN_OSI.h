#include "main.h"
#include "CAN_Flag.h"
#ifndef CAN_MUTIFRAME_H
#define CAN_MUTIFRAME_H

#define TIMEOUT_MS 5000
#define SIZE_FRAME_DATA						0X08
#define FILL_VALUE 							(0x55)
#define DATA_TEST							62
#define CAN_MAX_DATA						8

#define Node_Number							16

#define CAN_MAX_NODE						16

#define ALL_NODE 							0b0000
#define COMMAND_FRAME 						0b0001
#define ACK_COMMAND_FRAME 					0b0010
#define REMOTE_NOTICE_FRAME 				0b0011
#define TEST_FRAME 							0b0100
#define ACK_TEST_FRAME 						0b0101

#define ALL_NODE 							0b0000
#define ENGINE_CONTROL						0b0011
#define LIGHT_GPS							0b0100
#define MASTER								0b0111
#define STEERING							0b0101
#define POWER								0b0110
#define OBSTALCE1							0b1000
#define OBSTALCE2							0b1001
#define OBSTALCE3							0b1010
#define OBSTALCE4							0b1011
#define OBSTALCE5							0b1100
#define OBSTALCE6							0b1101
#define OBSTALCE7							0b1110
#define OBSTALCE8							0b1111

#define SET_UP_FRAME 							0b000
#define FIRST_FRAME 					0b001
#define SECOND_FRAME 					0b010
#define THIRD_FRAME 					0b011
#define FOURTH_FRAME 					0b100
#define FIFTH_FRAME 					0b101
#define SIX_FRAME 						0b110
#define SEVEN_FRAME 					0b111

//typedef struct {
//	unit8_t EndFrame;
//	unit8_t FirstFrame;
//	unit8_t SecondFrame;
//	unit8_t ThirdFrame;
//	unit8_t FourthFrame;
//	unit8_t FifthFrame;
//	unit8_t SixFrame;
//	unit8_t SevenFrame;
//}FrametypeHandleStruct;
typedef struct
{
	uint8_t ID;
	uint8_t NodeBuffer[8][8];
	uint8_t DuplicateFrame;
	uint8_t NodeIndex;
	uint8_t PacketLength;
	uint8_t NumberOfFrame;
	uint8_t CRCValue;
	uint8_t FrameType;
}NodeBufferHandle;

typedef struct {
	uint8_t NetworkBuffer[64];
	uint8_t NumberOfFrame;
	uint8_t Buffer_Index;
	uint8_t FrameType_Index;
	uint16_t SenderID;
	uint16_t RecvID;
	uint8_t Buffer[8][8];
	uint8_t StoreData[8][8];
	int8_t PacketDataLength;
	uint8_t CRCValue;
	NodeBufferHandle NodeHandle[16];
} CANBufferHandleStruct;


typedef struct {
	uint8_t	*StoreData;
	CANBufferHandleStruct StroreBuffer;
} CANTxStoreData;

typedef struct {
	uint16_t MessageType;
	uint16_t SenderID;
} CANConfigIDTxtypedef;

uint16_t CAN_Send_Response(uint8_t ID,uint8_t Opcode,uint8_t FrameType);
uint8_t CAN_Send_Application(CANBufferHandleStruct *AppBuffer,CANConfigIDTxtypedef *pStID,uint8_t *Data,uint8_t DataLength);
uint8_t CAN_Send_Network_Packet(CANBufferHandleStruct *TxBuffer, uint8_t *Data,
		uint8_t DataLength,CANConfigIDTxtypedef *pStID);
uint8_t CAN_Send_DataLink_Separate(CANBufferHandleStruct *TxBuffer,
		uint8_t *Data,CANConfigIDTxtypedef *pStID);
uint8_t CAN_Send_Physical_Send(CANBufferHandleStruct *TxBuffer, uint8_t *Data,CANConfigIDTxtypedef *pIDtype);
uint8_t CAN_Receive_DataLink(FlagFrameHandle *FlagHandle, CANBufferHandleStruct *RxBuffer,
		FlagRecNotification *FlagNotiHandle);
uint8_t CAN_Receive_Application(CANBufferHandleStruct *AppBuffer,uint8_t *Data,FlagFrameHandle *FlagFrame ,FlagRecNotification *FlagNotification);
uint8_t CAN_Receive_Network(CANBufferHandleStruct *NetBuffer,
		FlagFrameHandle *NetworkFlag,FlagRecNotification *FlagNotiHandle);
void CAN_Recieve_Physical_FIFO0(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data) ;
void CAN_ProcessRxBuffer(FlagFrameHandle *FlagHandle, uint8_t ID ,CANBufferHandleStruct *RxBuffer, uint8_t *DataPhysical, FlagRecNotification *FlagRecHandle);
void CAN_ProcessFrame(FlagFrameHandle *FlagHandle, uint8_t ID,
		CANBufferHandleStruct *RxBuffer, uint8_t FrameType, uint8_t *Data);
void CAN_Handle_Receive_Flow(CANBufferHandleStruct *Buffer ,CANConfigIDTxtypedef *pIDType,CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data);
void CAN_Receive_Response(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data);
void CAN_TXHeaderConfig(CAN_TxHeaderTypeDef *Txheader, uint32_t StdId);
void CANBufferHandleStruct_Init(CANBufferHandleStruct *TxBuffer);
void NodeBufferHandle_Init(NodeBufferHandle *NodeBuffer);
void FlagsFrameHandle_Init(FlagFrameHandle *FlagInit);
//void CAN_Config_filtering(uint8_t FillterBank , uint16_t NodeID, uint8_t FiFo);
uint32_t CAN_Config_filtering(uint8_t FIFO);
#endif

