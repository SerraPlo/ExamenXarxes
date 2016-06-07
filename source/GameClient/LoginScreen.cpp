#include "LoginScreen.h"
#include "AppClient.h"
#include <ctime>

void LoginScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	m_textLogin.Load("Please enter a nickname:", { 255,255,255 }, m_app->window, m_app->renderer, m_app->font);
	m_textLogin.width = int(m_app->screenWidth*0.9f);
	m_textLogin.height = int(m_app->screenHeight*0.2f);
	m_textLogin.position = { m_app->screenWidth/2- m_textLogin.width/2, m_app->screenHeight*0.3f };

	m_textLoading.Load("Waiting for server response...", { 20,150,0 }, m_app->window, m_app->renderer, m_app->font);
	m_textLoading.width = int(m_app->screenWidth*0.9f);
	m_textLoading.height = int(m_app->screenHeight*0.2f);
	m_textLoading.position = { m_app->screenWidth / 2 - m_textLoading.width / 2, m_app->screenHeight / 2 - m_textLoading.height / 2 };
}

void LoginScreen::Destroy(void) {

}

void LoginScreen::OnEntry(void) {
	SDL_StartTextInput();
	m_counterSend = 0;
	curState = LoginState::ASK_NICK;
}

void LoginScreen::OnExit(void) {
	SDL_StopTextInput();
}

void LoginScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->Exit();

	try {
	switch (curState) {
		case LoginState::ASK_NICK: {
			if (SDL_PollEvent(&evnt)) if (evnt.type == SDL_TEXTINPUT) if (m_inputText.size() < NICK_MAX_SIZE) m_inputText += evnt.text.text;
			if (m_app->inputManager.isKeyPressed(SDLK_RETURN) && !m_inputText.empty()) m_app->nick = m_inputText, curState = LoginState::WAIT_SERVER;
			if (m_app->inputManager.isKeyPressed(SDLK_BACKSPACE)) m_inputText = m_inputText.substr(0, m_inputText.size() - 1);

			m_textNick.Load(m_inputText, { 0,50,140 }, m_app->window, m_app->renderer, m_app->font);
			m_textNick.width = int(m_app->screenWidth*0.001f*m_inputText.size()*FONT_SIZE);
			m_textNick.height = int(m_app->screenHeight*0.2f);
			m_textNick.position = { m_app->screenWidth / 2 - (m_textNick.width / 2), m_app->screenHeight* 0.6f };
		} break;
		case LoginState::WAIT_SERVER: {
			if (clock() > m_counterSend + MS_RESEND_NICK_DELAY) {
				m_counterSend = float(clock());
				m_app->mainSocket << UDPStream::packet << MSG_LOGIN << m_app->nick << m_app->serverAddress;
				std::cout << "Nick sent. Waiting server response..." << std::endl;
			}
			int header;
			m_app->mainSocket >> UDPStream::packet >> header;
			switch (header) {
				case MSG_ACCEPT: { // Set screen to multiplayer when server indicates so
					std::cout << "Server accepted entry. Waiting for the other players..." << std::endl;
					m_textLoading.Load("Waiting for players...", { 20,150,0 }, m_app->window, m_app->renderer, m_app->font);
					curState = LoginState::WAIT_CLIENTS;
				} break;
				case MSG_REFUSE: { // Leave to main menu if server is disconnected
					std::cout << "Server refused entry. Full players capacity reached. Returning to main menu..." << std::endl;
					m_app->nick.clear();
					curState = LoginState::ASK_NICK;
				} break;
				default: break;
			}
		} break;
		case LoginState::WAIT_CLIENTS: {
			int header;
			m_app->mainSocket >> UDPStream::packet >> header;
			switch (header) {
				case MSG_BEGIN: {
					std::cout << "Game begins!" << std::endl;
					m_app->ChangeScreen(SCREEN_GAME);
				} break;
			}
		} break;
	}
	} catch (UDPStream::wrong) { //if the amount of packet data not corresponding to the amount of data that we are trying to read
		std::cout << "--> ALERT: Wrongly serialized data received!" << std::endl;
	} catch (UDPStream::empty) {} //if the package is empty or have not received anything
}

void LoginScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
	(m_app->nick.empty()) ? m_textLogin.Draw(m_app->renderer), m_textNick.Draw(m_app->renderer) : m_textLoading.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}
