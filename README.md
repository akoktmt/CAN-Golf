# CAN-Golf
Overview

This project focuses on building the Controller Area Network (CAN) layers, including the physical layer, data link layer, network layer, and application layer. The codebase implements communication functionalities over the CAN protocol, allowing for seamless communication between nodes in a CAN network. The project supports the transmission of multiple frames consecutively.

Code Structure

The codebase is organized into several files, each serving a specific purpose:

1. CAN_OSI.c and CAN_OSI.h
These files contain the implementation of the CAN communication layers, including functions for sending and receiving data, error handling, and processing different layers of the OSI model. The codebase has been extended to support the transmission of multiple frames in a continuous manner.

2. CAN_Flag.h
This file defines the flags and structures used for tracking the status of sent and received frames. It includes enums for notification flags and structures for handling frame flags.

3. CRC.h
This file provides functionality for calculating cyclic redundancy check (CRC) values, which are essential for ensuring data integrity.

Code Initialization
The project includes an initialization section in CAN_OSI.c to set up default values for the CAN buffer and flag handling structures. These functions help initialize the state of the CAN communication components.

Usage
To use the provided functionalities, follow these steps:

Initialize CAN Buffer and Flag Handling Structures:

Use CANBufferHandleStruct_Init to initialize the CAN buffer structure.
Use FlagsFrameHandle_Init to initialize the flag handling structure.
Configure Filtering:

Use CAN_Config_filtering to configure CAN filtering. This function sets up the necessary CAN filter parameters.
Sending Data:

Use the CAN_Send_Application function to send data at the application layer.
The data will traverse the network and data link layers before reaching the physical layer for transmission.
The project now supports the transmission of multiple frames in succession.
Receiving Data:

Utilize the CAN_Receive_Application function to receive data at the application layer.
The data will be processed through the network, data link, and physical layers before reaching the application layer.
Error Handling
The code includes error-handling mechanisms for frame and packet errors. The CAN_Send_Error_Handle and CAN_Receive_Error_Handle functions handle error responses appropriately.

Note
Ensure that the CAN configuration parameters, such as filter settings, are appropriately adjusted based on the specific requirements of your CAN network.

Feel free to reach out for any further assistance or customization of the codebase. The extended functionality allows for the transmission of multiple frames consecutively, enhancing the versatility of the CAN communication in your project.
