/* Source written by Hugo Woesthuis
 * Licensed under GPLv3
 * 
 */



#include "host_handler.h"
#include "motorcontrol.h"
#include "tcp_handler.h"
#include <sys/stat.h>
#include <unistd.h>
#include "rapidxml.hpp"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#define POSITION_FILE "/positions.xml"
#define SERVER_FILE "/srvset"
#define FILE_COUNT 2
#define ROOTNODE_NAME "Positions"
#define POSNODE_NAME "Position"
#define I2C_ADDRESS 4
#define STR_NULL ""
using namespace rapidxml;
void tSleep(long _milliseconds){
	std::this_thread::sleep_for(std::chrono::milliseconds(_milliseconds));
}
inline bool check_file_exists(const std::string& name){
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

host_handler::host_handler(std::string settings_dir){
	pos_path = settings_dir + POSITION_FILE;
	srv_path = settings_dir + SERVER_FILE;
}

std::vector<std::string> SplitByComma(std::string _string){
	std::vector<std::string> sps;
	std::string it;
	std::stringstream ss(_string);
	while(std::getline(ss, it, ',')){
		sps.push_back(it);
	}
	return sps;
}
std::vector<position> ConvertXMLtoPositions(std::string pathtofile){
	xml_document<> doc;
	xml_node<> * rootNode;
	
	// Open the file in a ifstream and put it in a vector
	std::ifstream xFile (pathtofile);
	std::vector<char> buf((std::istreambuf_iterator<char>(xFile)), std::istreambuf_iterator<char>());
	buf.push_back('\0');
	
	// Parse buf
	doc.parse<0>(&buf[0]);
	
	//Find the root node
	rootNode = doc.first_node(ROOTNODE_NAME);
	
	// Loop through the entire node, and convert
	std::vector<position> p;
	position posi;
	
	std::vector<step> tdfb;
	std::vector<step> tdtb;
	std::string ws;
	std::string::size_type sz;
	for(xml_node<> * posi_node = rootNode->first_node("Position"); posi_node; posi_node = posi_node->next_sibling()){
		posi.name = posi_node->first_attribute("name")->value();
			// loop through the steps
			
			// Tell the program where each node is
			xml_node<> * tdfb_node = posi_node->first_node("tdfb"); 
			xml_node<> * tdtb_node = posi_node->first_node("tdtb"); 
			
			// Loop though all step nodes
			for(xml_node<> * stepf_node = tdfb_node->first_node("step"); stepf_node; stepf_node = stepf_node->next_sibling()){
				// Get the value of the node
				ws = stepf_node->value();
				step st;
					
				// If cmd is 'STOP' break the loop
				if(ws == "STOP"){
					st.dir = STOP;
					st.time = 5;
					tdfb.push_back(st);
					break;
				}
				// Split string into two parts
				std::vector<std::string> values = SplitByComma(ws);
		
				st.dir = MotorController::ConvertStringToCommand(values[0]);
				st.time = (long)std::stoi(values[1], &sz);
				tdfb.push_back(st);
			}
			for(xml_node<> * stepb_node = tdtb_node->first_node("bstep"); stepb_node; stepb_node = stepb_node->next_sibling()){
				// Get the value of the node
				ws = stepb_node->value();
				step st;
					
				// If cmd is 'STOP' break the loop
				if(ws == "STOP"){
					st.dir = STOP;
					st.time = 5;
					tdtb.push_back(st);
					break;
				}
				// Split string into two parts
				std::vector<std::string> values = SplitByComma(ws);
		
				st.dir = MotorController::ConvertStringToCommand(values[0]);
				st.time = (long)std::stoi(values[1], &sz);
				tdtb.push_back(st);
			}
		posi.tdtb = tdtb;
		posi.tdfb = tdfb;
		p.push_back(posi);
	}
	return p;
}
void executemovements(position p, bool tdfb = true, int addr = I2C_ADDRESS){
	if(tdfb){
		std::cout<<"Going from base to " << p.name << std::endl;
		for(uint16_t i = 0; i < p.tdfb.size(); i++){
			step cS = p.tdfb.at(i);
			MotorController::SendCommand(cS.dir, addr);
			std::cout<<"Sending " << cS.dir << std::endl;
			tSleep(cS.time);
			
		}
	}
	if(!tdfb){
		std::cout<<"Going from base to " << p.name << std::endl;
		for(uint16_t i = 0; i < p.tdtb.size(); i++){
			step cS = p.tdtb.at(i);
			std::cout<<"Sending " << cS.dir << std::endl;
			MotorController::SendCommand(cS.dir, addr);
			
			tSleep(cS.time);
			
		}
	}
}

std::vector<position> FindAllPositionsByName(std::vector<std::string> querys, std::vector<position> AllPositions){
	std::vector<position> p;
	for(uint16_t c = 0; c < querys.size(); c++){
		for(uint16_t i = 0; i < AllPositions.size(); i++){
			position po = AllPositions.at(i);
			if(po.name != querys.at(c))
				continue;
			
			p.push_back(po);
		} 
	}
	return p;
	
}

void TestAll(std::vector<position> alps){
	for(uint16_t c = 0; c < alps.size(); c++){
		position p = alps.at(c);
		std::cout<<"Testing pos " << p.name << std::endl;
		std::cout<< "Going from base...";
		executemovements(p, true, 4);
		std::cout << "done\n";
		tSleep(1000);
		std::cout << "Going to base...";
		tSleep(1000);
		executemovements(p, false, 4);
		std::cout << "done\n";
	}
}

// Listen to commands
void CommandParser(tcp_client tcpc, std::vector<position> posiss){
	bool busy = false;
	std::string d = STR_NULL;
	do{ d = tcpc.receive_data(512);}while(d == STR_NULL);
	if(d == "PAUSE")
		MotorController::SendCommand(STOP, I2C_ADDRESS);
	else if(d == "NOTG"){
		if(busy){
			tcpc.send_data("ERRBSY");
			return;
		}
		busy = true;
		// Give the OK command;
		tcpc.send_data("OK");
		// recieve the next packet
		std::string pd = tcpc.receive_data(1024);
		
		// Split the recieved string
		std::vector<std::string> posis = SplitByComma(pd);
		
		// Search and find all positions
		std::vector<position> exec = FindAllPositionsByName(posis, posiss);
		
		// Loop though all positions and execute them
		for(uint16_t d = 0; d < exec.size(); d++){
			position ps = exec.at(d);
			executemovements(ps, true, I2C_ADDRESS);
			tSleep(5000);
			executemovements(ps, false, I2C_ADDRESS);
		}
		
		// Client is not performing a task anymore.
		busy = false;
		// Send the OK response
		tcpc.send_data("OK");
	}
	else if (d == "TESTALLPS"){
		if(busy){
			tcpc.send_data("ERRBSY");
			return;
		}
		busy = true;
		TestAll(posiss);
		busy = false;
	}
}







void host_handler::start(std::string IP, int port){
	// check if files are existent
	if(!check_file_exists(srv_path)){
		//serverSetup()
	}
	if(!check_file_exists(pos_path)){
		//positionSetup()
		return;
	}
	
	// Parse the XML file and convert into std::vector<postition>
	std::cout << "Parsing POSITION file...";
	std::cout << pos_path << "...";
	tSleep(1000);
	std::vector<position> positions = ConvertXMLtoPositions(pos_path);
	std::cout <<"done\n";
	MotorController::SendCommand(ENABLE, 4);
	
	// Connect to server
	tcp_client cl(IP, port);
	if(!cl.init()){
		return;
	}
	
	// Handshake flow
	cl.send_data("INITHDSK");
	std::string response = cl.receive_data(512);
	if(response == "HSKERROR"){std::cout<<"ERROR: Server returned handshake-error\n"; return;}
	std::cout << "Received " << response << " from server\n";
	
	// Start to listen
	CommandParser(cl, positions);
}

void host_handler::stop(){
	std::cout<<"stopping service...";
}
