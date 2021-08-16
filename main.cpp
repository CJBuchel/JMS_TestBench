
// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>

#include "JMS/electronics/src/libs/Network/Messages/JMS_Messages/messages.h"

#include <pb.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>

// message packets
// TestMessage test_packet = TestMessage_init_zero;
jms_electronics_UpdateNode2Field n2f_packet = jms_electronics_UpdateNode2Field_init_zero;
jms_electronics_UpdateField2Node f2n_packet = jms_electronics_UpdateField2Node_init_zero;

/**
 * Encode the local send message.
 * Returns the enocded buffer
 */
uint8_t *encodeSendMessage(size_t bufferSize, int &bytesWritten) {
	uint8_t *returner = (uint8_t *)malloc(sizeof(uint8_t) * bufferSize);

	uint8_t buffer[bufferSize];
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);

	bool status = pb_encode(&stream, jms_electronics_UpdateField2Node_fields, &f2n_packet);

	printf("bytes written: %ld\n", stream.bytes_written);
	bytesWritten = stream.bytes_written;
	for (size_t i = 0; i < bufferSize; i++) {
		returner[i] = buffer[i];
	}

	if (!status) {
		printf("Encoding Failed: %s\n", PB_GET_ERROR(&stream));
	}

	return returner;
}

/**
 * Decode form the buffer
 * places decoded data into local receive message
 */
void decodeReceiveMessage(uint8_t buffer[128], size_t message_length) {
	jms_electronics_UpdateNode2Field tmpInputMessage = jms_electronics_UpdateNode2Field_init_zero;

	std::cout << "Message len: " << message_length << std::endl;
	pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
	bool status = pb_decode(&stream, jms_electronics_UpdateNode2Field_fields, &tmpInputMessage);

	if (!status) {
		printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
	}

	n2f_packet = tmpInputMessage;
}

#define PORT 5333
int main(int argc, char const *argv[]) {

	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	// uint8_t buffer[128];
	// const char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
			
	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	printf("Set Socket success\n");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
			
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Bind success\n");

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Listen success\n");
	
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	while (1) {

		// Receiving
		// printf("Accepted reading...\n");
		// valread = recv(new_socket, buffer, sizeof(buffer), 0);
		// if (valread < 0) {
		// 	std::cout << "Error in message received" << valread << std::endl;
		// } else {
		// 	jms_electronics_UpdateNode2Field tmpInputMessage = jms_electronics_UpdateNode2Field_init_zero;
		// 	pb_istream_t stream = pb_istream_from_buffer(buffer, valread);
		// 	printf("Recv: %d\n", valread);

		// 	bool status = pb_decode(&stream, jms_electronics_UpdateNode2Field_fields, &tmpInputMessage);

		// 	if (!status) {
		// 		printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
		// 	}

		// 	n2f_packet = tmpInputMessage;

		// 	// std::cout << "Abort Status: " << n2f_packet.data.scoringTable.abort << std::endl;
		// 	std::cout << "Role: " << n2f_packet.role << std::endl;
		// 	std::cout << "ip: " << n2f_packet.ipv4 << std::endl; 
		// }

		// Sending
		f2n_packet.role = jms_electronics_NodeRole_NODE_SCORING_TABLE;
		f2n_packet.which_data = jms_electronics_UpdateField2Node_scoringTable_tag;
		int bytesWritten = 0;
		uint8_t *buffer = encodeSendMessage(128, bytesWritten);

		int sendBytes = send(new_socket, buffer, bytesWritten, 0);
		if (sendBytes < 0) {
			std::cout << "Error sending" << std::endl;
		} else {
			std::cout << "Bytes sent: " << sendBytes << std::endl;
		}
		// pb_ostream_t stream_out = pb_ostream_from_buffer(buffer_out, buffSize);


		// f2n_packet.which_data = jms_electronics_UpdateField2Node_first_tag;
		// f2n_packet.role = jms_electronics_NodeRole_NODE_SCORING_TABLE;

		// bool status_out = pb_encode(&stream_out, jms_electronics_UpdateField2Node_fields, &f2n_packet);
		// if (!status_out) {
			// std::cout << "Encoding failed: " << PB_GET_ERROR(&stream_out) << std::endl;
		// }


		// valread = send(new_socket, buffer, stream_out.bytes_written, 0);
		// std::cout << "Valread: " << valread << std::endl;
		// std::cout << "Which data: " << f2n_packet.which_data << std::endl;
		// std::cout << "Sent data size: " << valread << std::endl;
		sleep(1);
	}
		
	return 0;
}