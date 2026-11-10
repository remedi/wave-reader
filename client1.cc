#include <iostream>

#include <Socket.h>

#include <sys/socket.h>

int main() {
	Socket sock1(AF_INET, SOCK_STREAM, 0);

	std::cout << "client1" << std::endl;
	return 0;
}
