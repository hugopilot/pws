/* Written by Hugo Woesthuis and Mike Krop
 * Licensed under the GPLv3 license
 */

#include "tcp_handler.h"
#define DEFAULT_PORT 5555
#define DEFAULT_SOCKET -1
#define DEFAULT_RECIEVE_SIZE 512
tcp_client::tcp_client(std::string _address){
	socke = DEFAULT_SOCKET;
	port = DEFAULT_PORT;
	address = _address;
	connected = false;
}
tcp_client::tcp_client(std::string _address, int _port){
	socke = DEFAULT_SOCKET;
	port = _port;
	address = _address;
	connected = false;
}
tcp_client::tcp_client(std::string _address, int _port, int _socket){
	socke = _socket;
	port = _port;
	address = _address;
	connected = false;
}
bool tcp_client::init(){
	std::cout << "Connecting...";
	if(connected){
		std::cout<<"already done\n";
		return true;
	}
	
	// Create a socket if -1
	if(socke == DEFAULT_SOCKET){
		socke = socket(AF_INET, SOCK_STREAM, 0);
		if(socke == -1){
			std::cerr << "failed\n Socket could not be created!\n";
			return false;
		}
	}
	
	// set address to proper types
	if(inet_addr(address.c_str()) == (unsigned)-1){
		struct in_addr **address_list;
		struct hostent *he;
		
		// try to see if it's an hostname
		if((he = gethostbyname(address.c_str())) == NULL)
		{
			// It's not a host name either, throw an error
			std::cerr << "failed\n Failed to parse IP or reach hostname\n";
			return false;
		}
		
		// Cast h_addr_;ist into in_addr
		address_list = (struct in_addr **) he->h_addr_list;
		
		for(int c = 0; address_list[c] != NULL; c++){
			server.sin_addr = *address_list[c];
			
			std::cout<<"\n" << address << " resolved to " << inet_ntoa(*address_list[c])<<std::endl;
			break;
		}
	}
	// Seems it's an normal ip address...yey
	else{
		server.sin_addr.s_addr = inet_addr(address.c_str());
	}
	
	server.sin_port = htons(port);
	server.sin_family = AF_INET;
	
	// Let's connect to the server!
	if(connect(socke, (struct sockaddr*)&server, sizeof(server)) < 0){
		std::cerr << "failed\n Couldn't connect to server\n";
		return false;
	}
	
	connected = true;
	std::cout << "success\n";
	return true;
}

bool tcp_client::send_data(std::string data){
	// Send the damn data
	std::cout<<"Sending data...";
	if(send(socke, data.c_str(), strlen(data.c_str()), 0) < 0 ){
		std::cerr << "failed\n";
		return false;
	}
	std::cout<<"sended!\n";
	return true;
}

std::string tcp_client::receive_data(int size=DEFAULT_RECIEVE_SIZE){
	// This is a buffer
	char b[size];
	// Fill it in here
	std::string data_received;
	
	// Recieve a reply from the server
	if(recv(socke, b, sizeof(b), 0) < 0){
		std::cerr << "recv failed to reveive data!\n";
		return NULL;
	}
	data_received = b;
	return data_received;
}
