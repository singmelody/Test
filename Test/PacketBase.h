#pragma once

#define PACKET_BASE_SIZE					(2*sizeof(int32) + sizeof(uint8))

#define PACKET_EX_SIZE						(sizeof(uint16) + PACKET_BASE_SIZE)

#define PACKET_EX_BUFF_SIZE					512

#define PACKET_EX_BUFFER_MAX_SIZE			2048

#define PACKET_MAX_SIZE						(PACKET_EX_SIZE + PACKET_EX_BUFFER_MAX_SIZE) * 2

#define PACKET_SEND_LIST_BUFFER_SIZE		512

#define SOCKET_MY_MAX_DATA_BLOCK_SIZE		(PACKET_MAX_SIZE * 2)