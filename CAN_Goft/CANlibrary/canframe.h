#include "main.h"
#ifndef CAN_MUTIFRAME_H
#define CAN_MUTIFRAME_H
#define TIMEOUT_MS 5000
#define CAN_MAX_DATA_LENGTH 				(0x08)
#define FILL_VALUE 							(0x55)
#define DATA_TEST							9


#define ALL_NODE 							0b0000
#define COMMAND_FRAME 						0b0001
#define ACK_COMMAND_FRAME 					0b0010
#define REMOTE_NOTICE_FRAME 				0b0011
#define TEST_FRAME 							0b0100
#define ACK_TEST_FRAME 						0b0101

#define ALL_NODE 							0b0000
#define ENGINE_CONTROL						0b0001
#define LIGHT_GPS							0b0010
#define MASTER								0b0011
#define STEERING							0b0100
#define POWER								0b0110
#define OBSTALCE1							0b1000
#define OBSTALCE2							0b1001
#define OBSTALCE3							0b1010
#define OBSTALCE4							0b1011
#define OBSTALCE5							0b1100
#define OBSTALCE6							0b1101
#define OBSTALCE7							0b1110
#define OBSTALCE8							0b1111

#define END_FRAME 						0b000
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

typedef struct {
	uint8_t NetworkBuffer[64];
	uint8_t NumberOfFrame;
	uint8_t Buffer_Index;
	uint8_t FrameType_Index;
	uint16_t SenderID;
	uint16_t RecvID;
	uint8_t Buffer[8][8];
	uint8_t StoreData[10][10];
} CANBufferHandleStruct;

typedef struct {
	uint16_t MessageType;
	uint16_t TargetNode;
	uint16_t Frametype;
	uint8_t ReceivedBuffer[8];
	uint8_t Index;
	uint8_t SenderID;
	uint8_t ExpectedLength;
	uint8_t frameIndex;
	uint8_t isLastFrame;
} CANConfigIDRxtypedef;
typedef struct {
	uint16_t MessageType;
	uint16_t SenderID;
} CANConfigIDTxtypedef;
//uint8_t CAN_Receive_Dataframe(CAN_HandlerStruct* Canhandle,CANConfigIDRxtypedef* pIDtype,uint8_t *ReceiveData, uint32_t *ReceiveLength);
//uint32_t CAN_Config_filtering(CAN_HandlerStruct *Can, uint16_t NodeID);
//uint16_t CAN_RECEIVE_ACK(CAN_HandlerStruct* canhandler);
//uint8_t CAN_Init(CAN_HandlerStruct* canhandler, CAN_HandleTypeDef* hcan, uint16_t ID);
//uint32_t CAN_Config_filtering(CAN_HandlerStruct *Can, uint16_t NodeID);
uint32_t CAN_TXHeaderConfig(CAN_TxHeaderTypeDef *Txheader, uint32_t StdId);
uint8_t CAN_Send_Dataframe(CANConfigIDTxtypedef *pIDtype, uint8_t *Data,uint32_t Txmailbox, CANBufferHandleStruct *TxBuffer);
uint16_t CAN_Send_Opcode(uint8_t Opcode, uint8_t StdId, uint32_t Txmailbox,
		CAN_TxHeaderTypeDef Txheader);
uint8_t CAN_Network_Packet(CANBufferHandleStruct *TxBuffer,uint8_t *Data,uint8_t DataLength);
uint8_t CAN_DataLink_Separate(CANBufferHandleStruct *TxBuffer,uint8_t *Data,uint8_t DataLength);
uint8_t CAN_Physical_Send(CANBufferHandleStruct *TxBuffer,uint8_t *Data,uint8_t DataLength,CANConfigIDTxtypedef *pIDtype,uint32_t Txmailbox);
uint8_t CAN_Store_Data(CANBufferHandleStruct *Store,CANConfigIDTxtypedef *ID);
void CANBufferHandleStruct_Init(CANBufferHandleStruct *TxBuffer);
#endif

