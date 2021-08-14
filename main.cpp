
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
jms_electronics_UpdateNode2Field receive_packet = jms_electronics_UpdateNode2Field_init_zero;

#define PORT 5333
int main(int argc, char const *argv[]) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	uint8_t buffer[128];
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
		printf("Accepted reading...\n");
		valread = recv(new_socket, buffer, sizeof(buffer), 0);
		if (valread < 0) {
			std::cout << "Error in message received" << valread << std::endl;
		} else {
			jms_electronics_UpdateNode2Field tmpInputMessage = jms_electronics_UpdateNode2Field_init_zero;
			pb_istream_t stream = pb_istream_from_buffer(buffer, 10);

			bool status = pb_decode(&stream, jms_electronics_UpdateField2Node_fields, &tmpInputMessage);

			if (!status) {
				printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
			}

			receive_packet = tmpInputMessage;

			if (status) {
				system("clear");
				std::cout << "Scoring Abort button: " << receive_packet.data.scoringTable.abort << std::endl;
			}
		}

		// printf("From client: %s, bytes: %d\n",buffer,valread);
	}
	return 0;
}