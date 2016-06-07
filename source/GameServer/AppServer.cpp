#include <SerraPloEngine/Timing.h>
#include "AppServer.h"
#include <ctime>

void AppServer::Init(void) {
	m_aliveCounter = float(clock());
	m_counterUpdate = float(clock());
}

void AppServer::Send(void) {
	// Alive counter to tell player that the server is working
	if (clock() > m_aliveCounter + MS_ALIVE_DELAY && clientList.size() == MAX_PLAYERS) {
		//std::cout << "Sending alive..." << std::endl;
		dispatcher << UDPStream::packet << MSG_ALIVE;
		for (auto &client : clientList) dispatcher << client.second->address;
		m_aliveCounter = float(clock());
	}
	// Update each player connected with the nick and the position of the other players
	if (clock() > m_counterUpdate + MS_UPDATE_DELAY && clientList.size() == MAX_PLAYERS) {
		dispatcher << UDPStream::packet << MSG_UPDATE;
		for (auto &client : clientList) dispatcher << client.second->nick;
		for (auto &player : clientList) dispatcher << player.second->address;
		m_counterUpdate = float(clock());
	}
}

void AppServer::Receive(void) {
	try {
		int header;
		sockaddr sender;
		dispatcher >> UDPStream::packet >> sender >> header;
		switch (header) {
			case MSG_LOGIN: {
				std::string nick;
				dispatcher >> nick; // Receive client nick and store it
				if (clientList.size() != MAX_PLAYERS) {
					if (clientList.find(sender.hash) == clientList.end()) { // Check if player exists in client list
						clientList[sender.hash] = new ClientProxy(sender, nick, clientList.size()); ///TODO: new placement constructor
						std::cout << nick << " has logged in. Added to client database." << std::endl;
						dispatcher << UDPStream::packet << MSG_ACCEPT << sender; // Let the new player connected enter the game
						if (clientList.size() == MAX_PLAYERS) { // Check if race begins
							dispatcher << UDPStream::packet << MSG_BEGIN << int(clientList.size()); // Send player enemies size
							for (auto &client : clientList) dispatcher << client.second->nick;
							for (auto &player : clientList) dispatcher << player.second->address;
							m_counterUpdate = float(clock());
							m_aliveCounter = float(clock());
						}
					}
				} else dispatcher << UDPStream::packet << MSG_REFUSE << sender;
			} break;
			case MSG_EXIT: { // Remove from client list the player disconnected
				std::cout << clientList[sender.hash]->nick << " has been disconnected." << std::endl;
				clientList.erase(sender.hash);
			} break;
			case MSG_UPDATE: {
				int listID;
				dispatcher >> listID;
				input10 input;
				float x[10]; float y[10];
				dispatcher >> input.w >> input.a >> input.s >> input.d >> x >> y >> input.dt;
				bool temp[5];
				for (int i = 0; i < 10; i++) {
					temp[0] = input.w[i]; temp[1] = input.a[i];
					temp[2] = input.s[i]; temp[3] = input.d[i];
					temp[4] = false;
				}
			}
		}
		//for (auto &clientList : clientLists) if (clientList.empty()) std::cout << "All players disconnected." << std::endl;
	} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything
}

void AppServer::Run(void) {
	Init(); // Call the init everything function
	FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
	fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120
	
	std::cout << "///////////////////" << std::endl;
	std::cout << "//  Game Server  //" << std::endl;
	std::cout << "///////////////////" << std::endl;
	std::cout << std::endl << "Listening to new player..." << std::endl;
	while (m_isRunning) {
		fpsLimiter.begin(); // Init FPS counter
		Send();
		Receive();
		fps = fpsLimiter.fps;
		deltaTime = fpsLimiter.deltaTime;
		fpsLimiter.end(); // Calculate and restore FPS
	}
}

void AppServer::Destroy(void) {
	dispatcher << UDPStream::packet << MSG_EXIT;
	for (auto &client : clientList) dispatcher << client.second->address, delete client.second;
	//m_isRunning = false; // Execution ends
}