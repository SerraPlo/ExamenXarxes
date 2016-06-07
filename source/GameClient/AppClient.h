#pragma once
#include <SerraPloEngine/IApp.h>
#include <SerraPloEngine/InputManager.h>
#include <SerraPloEngine/ScreenList.h>
#include <UDPStream/UDPStream.hh>
#include <memory>
#include "LoginScreen.h"
#include "GameScreen.h"

#define INIT_SCREEN_WIDTH 600
#define INIT_SCREEN_HEIGHT 450
#define IP_PORT "127.0.0.1:5000"
#define FONT_SIZE 70

enum SreenType { SCREEN_LOGIN, SCREEN_GAME };

class AppClient : public IApp {
	explicit AppClient() : serverAddress(IP_PORT) {};
	AppClient(AppClient const&) = delete;
	void operator=(AppClient const&) = delete;
	~AppClient() = default;

	// Initialize everything related to game internals
	void Init(void) override;
	// Main update function of the game
	void Update(void);
	// Main draw function of the game
	void Draw(void) const;
public:
	std::unique_ptr<ScreenList> m_screenList{ std::make_unique<ScreenList>(dynamic_cast<IApp*>(this)) }; // Unique pointer instance to the list of screens of the game
	IScreen *currentScreen{ nullptr }; // Reference pointer to the screen running at the moment
	UDPStream mainSocket;
	sockaddr serverAddress;
	InputManager inputManager;	// Main instance of the input manager class
	std::string nick{ "" };

	int screenWidth{ INIT_SCREEN_WIDTH }, screenHeight{ INIT_SCREEN_HEIGHT };
	SDL_Window *window{ nullptr };
	SDL_Renderer *renderer{ nullptr };
	TTF_Font *font{ nullptr };

	static AppClient& Instance() {
		static AppClient instance;
		return instance;
	}
	void ChangeScreen(int index);
	// Manage main SDL event types
	void OnSDLEvent(SDL_Event &evnt);
	// Where magic occurs, to be used to play the whole game
	void Run(void) override;
	// Destroy screen list and set game running to false
	void Exit(void);
};