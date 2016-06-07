#include "GameScreen.h"
#include "AppClient.h"

void GameScreen::Build(void) {
	m_app = dynamic_cast<AppClient*>(gameApp);

	m_image.LoadImage("images/solete.png", m_app->window, m_app->renderer);
	m_image.position = { 0,0 };
	m_image.width = 100;
	m_image.height = 100;

	m_rectangle.position = { 250, 250 };
	m_rectangle.width = 300;
	m_rectangle.height = 100;
	m_rectangle.color = { 100, 0, 200 };
}

void GameScreen::Destroy(void) {
	
}

void GameScreen::OnEntry(void) {

}

void GameScreen::OnExit(void) {

}

void GameScreen::Update(void) {
	SDL_Event evnt;
	if (SDL_PollEvent(&evnt)) m_app->OnSDLEvent(evnt);
	if (m_app->inputManager.isKeyPressed(SDLK_ESCAPE)) m_app->Exit();
}

void GameScreen::Draw(void) {
	SDL_RenderClear(m_app->renderer);
	m_image.Draw(m_app->renderer);
	m_rectangle.Draw(m_app->renderer);
	SDL_RenderPresent(m_app->renderer);
}
