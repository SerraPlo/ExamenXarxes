#include "GameScreen.h"
#include "AppClient.h"
#include <ctime>

void GameScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	textures = { "images/solete.png", "images/roco.png" , "images/fogo.png" };

	m_rectangle.position = { 250, 250 };
	m_rectangle.width = 300;
	m_rectangle.height = 100;
	m_rectangle.color = { 100, 0, 200 };

	curState = GameState::INIT;
}

void GameScreen::Destroy(void) {

}

void GameScreen::OnEntry(void) {
	m_aliveCounter = float(clock());
	m_counterUpdate = float(clock());
}

void GameScreen::OnExit(void) {

}

void GameScreen::UpdateInit() {
	int header;
	m_app->mainSocket >> UDPStream::packet >> header;
	switch (header) {
	case MSG_INIT: {
		std::cout << "Receiving data for inizialization" << std::endl;
		int sizeAgents;
		m_app->mainSocket >> sizeAgents; // Receive number of agents in game
		int textureID = 0;
		for (int i = 0; i < sizeAgents; ++i) {
			uint64_t agentID;
			m_app->mainSocket >> agentID; // Receive client ID
			auto &agent = m_agents[agentID]; // Create agent by ID received
			std::string nickReceived;
			m_app->mainSocket >> nickReceived;  // Set agent nick
			if (m_app->nick == nickReceived) m_player = &agent;// Assign player according to nick compare
			agent.nick.Load(nickReceived, { 255, 255, 255 }, m_app->window, m_app->renderer, m_app->font);
			agent.nick.position = { 0,0 };
			agent.nick.width = int(m_app->screenWidth*0.1f);
			agent.nick.height = int(m_app->screenWidth*0.05f);
			m_app->mainSocket >> agent.sprite.position; // Init position
			agent.sprite.LoadImage(textures[textureID++ % MAX_TEXTURES], m_app->window, m_app->renderer);
			agent.sprite.width = 75;
			agent.sprite.height = 75;
		}
		m_counterUpdate = float(clock());
		m_aliveCounter = float(clock());
		curState = GameState::PLAY;
	} break;
	}
}

void GameScreen::UpdatePlay() {
	// PLAYER UPDATE
	static int input; //0001 w - 0010 a - 0100 s - 1000 d + combos
	static int currI = 0; //9 inputs sencers = 9999
	if (m_app->inputManager.isKeyDown(SDLK_w)) --m_player->sprite.position.y, input += 1;
	if (m_app->inputManager.isKeyDown(SDLK_a)) --m_player->sprite.position.x, input += 10;
	if (m_app->inputManager.isKeyDown(SDLK_s)) ++m_player->sprite.position.y, input += 100;
	if (m_app->inputManager.isKeyDown(SDLK_d)) ++m_player->sprite.position.x, input += 1000;
	currI++;

	// AGENTS UPDATE
	for (auto &agent : m_agents) { // ALERT: player is an agent
		agent.second.nick.position = glm::ivec2{ agent.second.sprite.position.x, agent.second.sprite.position.y - 30 }; // Update nick position
		if (m_player != &agent.second) {
			agent.second.sprite.position = agent.second.sprite.position + (agent.second.targetPosition- agent.second.sprite.position)/3;
		}
	}

	// SEND
	if (currI == 8) { // Send update info
		if(input!=0) m_app->mainSocket << UDPStream::packet << MSG_UPDATE << input << m_app->serverAddress;
		input = 0; currI = 0;
	}

	// RECEIVE
	int header;
	m_app->mainSocket >> UDPStream::packet >> header;
	switch (header) {
	case MSG_UPDATE: { // Check server update
		for (size_t i = 0; i < m_agents.size(); ++i) {
			uint64_t idReceived;
			m_app->mainSocket >> idReceived;
			m_app->mainSocket >> m_agents[idReceived].targetPosition;
			std::cout << m_agents[idReceived].targetPosition.x << std::endl;
		}
	} break;
	case MSG_ALIVE: m_aliveCounter = float(clock()); break; // Check alive
	} 
	if (clock() > m_aliveCounter + MS_ALIVE_DELAY + 1000) std::cout << "Server closed. Disconecting..." << std::endl, m_app->nick.clear(), m_app->ChangeScreen(SCREEN_LOGIN);
}

void GameScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->Exit();

	try {
		switch (curState) {
		case GameState::INIT: UpdateInit(); break;
		case GameState::PLAY: UpdatePlay(); break;
		}
	}
	catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything
}

void GameScreen::DrawInit() {}

void GameScreen::DrawPlay() {
	for (auto &agent : m_agents) {
		agent.second.sprite.Draw(m_app->renderer);
		agent.second.nick.Draw(m_app->renderer);
	}
	m_rectangle.Draw(m_app->renderer);
}

void GameScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
	switch (curState) {
	case GameState::INIT: DrawInit(); break;
	case GameState::PLAY: DrawPlay(); break;
	}
	SDL_RenderPresent(m_app->renderer);
}
