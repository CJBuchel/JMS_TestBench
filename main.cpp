/**
 * Server Test using the proto files form JMS
 */

#include <iostream>
#include <pb_encode.h>
#include <pb_decode.h>

// System socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

// Compiled JMS Messages
#include "JMS/electronics/src/libs/Network/Messages/JMS_Messages/messages.h"


// Server settings
#define PORT 5333

int main() {
	std::cout << "-- Test Bench Start --" << std::endl;

	int listenfd, connfd;
	struct sockaddr_in servaddr;
	int reuse = 1;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servaddr.sin_port = htons(PORT);

	std::cout << "Binding..." << std::endl;
	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		std::cout << "Binding error" << std::endl;
	}
	std::cout << "Binded" << std::endl;


	std::cout << "Listening..." << std::endl;
	if (listen(listenfd, 5) != 0) {
		std::cout << "Listen Error" << std::endl;
	}

	std::cout << "Listen complete" << std::endl;

	std::cout << "Accepting..." << std::endl;
	while (true) {
		connfd = accept(listenfd, NULL, NULL);

		if (connfd < 0) {
			std::cout << "Accept" << std::endl;
		}

		std::cout << "Connection made" << std::endl;
		
	}

	return 0;
}