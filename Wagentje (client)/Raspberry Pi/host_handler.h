/* Written by Hugo Woesthuis and Mike Krop
 * Licensed under GPLv3
 */

#ifndef HOSTHANDLER_H
#define HOSTHANDLER_H
#include <iostream>

class host_handler{
	public:
	host_handler();
	host_handler(std::string setting_dir);
	void start();
	void stop();
	private:
	std::string settings_dir;
	
};

struct position{
	// Name of the position
	std::string pos_name;
	
	// Time to Drive From the Beginning of the Lane;
	int tdf_bLane;
	
	// Time to Drive From the End of the Lane
	int tdf_eLane;
	
	// Time to drive after turning.
	int tda_turn;
};

struct Lane{
	bool isEndStation = false;
	bool isBaseStation = false;
	
	position positions[];
};


#endif
/* DLHCP (DistroLink Host-Client Protocol)
 * === MAIN COMMANDS ===
 * PAUSE = Instructs to stop the client with his movements and wait for a RSME instruction.
 * NOTG = 'New Order To Grab'. This commands instructs the client to get ready to recieve a new assignment.
 * RTB = Cancels any assignment and Return To Base
 * RSME = Resume operations after PAUSE command
 * GTEP = 'Go To End Point'
 * FLREBOOT = Resets both the microcontroller and the Raspberry Pi.
 * 
 * === HANDSHAKE FLOW === 
 * This flow describes the first connection setup.
 * 1. Server connects to client and sends the INITHDSK command.
 * 2. The client responds with a RDYFORHS 'Ready for handshake' or a HDSERROR 'Handshake error'
 * 3. In case RDYFORHS was recieved, the server sends his IP and handshake-id to the client
 * 4. If no errors occur the client sends a HDSKCMPT 'Handshake complete' command and stores the info on his drive in a file.
 * 
 * === COMMAND ARGUMENTS ===
 * NOTG = See NOTG DATA TFR
 * GTEP = Number of end point.
 * 
 * === NOTG DATA TFR ===
 * After NOTG is sent, the client will listen till next packet is recieved. The following packet will need a list of positions.
 * Postitions are seperated though a comma. 
 * Example: 
 * A5, A6, A9, B1
 * 
 * You can specify a different starting position, if it's specified in the Positions File
 * Example:
 * A5, A6, A9, B1, FROM base_1
 * 
 * A full example would be, base-to-base:
 * FROM base_1, A5, A6, A9, B1, base_1
 * 
 * The client will take care of the movements.
 */

/* Postition file orginisation:
 * Since the prototype relies on dead-reckoning, the postitions contain steps for the car to follow.
 * <Postitions>
 * 		<Position name="A1">
 * 			<tdfb>
 * 				<step>FWD, 25000</step>
 * 				<step>LFT, 3000</step>
 * 				<step>FWD, 10000</step>
 * 				<step>SLFT, 1265</step>
 * 				<step>FWD, 5000</step>
 * 				<step>STOP</step>
 * 			</tdfb>
 * 			<tdtb>
 * 				<step>BCK, 7000</step>
 * 				<step>SLFT, 1265</step>
 * 				<step>FWD, 12000</step>
 * 				<step>RGHT, 3000</step>
 * 				<step>STOP</step>
 * 			</tdtb>
 * 		</Postition>
 * </Positions> 
 * </Positions>
 * 
 * Server File:
 * IP 10.0.0.200
 * SID a3c2cfd243ac
