#include "host_handler.h"
#include "tcp_listener.h"
#include <sys/stat.h>
#include <unistd.h>
#define POSITION_FILE "/positions"
#define SERVER_FILE "/srvset"
#define FILE_COUNT 2

inline bool check_file_exists(const std::string& name){
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

host_handler::host_handler(std::string settings_dir){
	struct stat buffer;
	std::string pos_path = settings_dir + POSITION_FILE;
	std::string srv_path = settings_dir + SERVER_FILE;
	if(!check_file_exists(srv_path){
		//serverSetup()
	}
	if(!check_file_exists(pos_path){
		//positionSetup()
	}
	
	
}
