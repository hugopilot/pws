/* Written by Hugo Woesthuis and Mike Krop
 * Licensed under the GPLv3 license
 */
#ifndef TCPHANLDER_H
#define TCPHANDLER_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

class tcp_client{
	public:
	tcp_client(std::string _address);
	tcp_client(std::string _address, int _port);
	tcp_client(std::string _address, int _port, int _socket);
	bool init();
	bool send_data(std::string data);
	std::string receive_data(int);
	
	private:
	int socke;
	std::string address;
	int port;
    struct sockaddr_in server;
	bool connected;
	
};
#endif
