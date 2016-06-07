#include <SerraPloEngine/PathLoader.h>
#include <SerraPloEngine/Utils.h>
#include <SerraPloEngine/Timing.h>
#include <SerraPloEngine/IScreen.h>
#include "AppClient.h"
#include <ctime>

void AppClient::Init(void) {
	InitSDL(); // Initialize everything related to SDL for the window
	window = SDL_CreateWindow("Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	font = TTF_OpenFont(GetPathToAsset("fonts/ARIAL.TTF").c_str(), FONT_SIZE);
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

	// Add the game screens into the list
	m_screenList->AddScreen(new LoginScreen, SCREEN_LOGIN);
	m_screenList->AddScreen(new GameScreen, SCREEN_GAME);
	currentScreen = m_screenList->SetScreen(SCREEN_LOGIN); // Set to be the first screen
}

void AppClient::ChangeScreen(int index) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	currentScreen = m_screenList->SetScreen(index);
}

void AppClient::OnSDLEvent(SDL_Event & evnt) {
	switch (evnt.type) { // Check for SDL event type
		case SDL_QUIT:
		currentScreen->currentState = ScreenState::EXIT; // Set screen state to exit application
		break; case SDL_MOUSEMOTION:
		inputManager.mouseCoords = { static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y) }; // Store the mouse coordinates each time mouse moves through the screen
		break; case SDL_KEYDOWN:
		inputManager.pressKey(evnt.key.keysym.sym); // Store which key has been pressed
		break; case SDL_KEYUP:
		inputManager.releaseKey(evnt.key.keysym.sym); // Store which key has been released
		break; case SDL_MOUSEBUTTONDOWN:
		inputManager.pressKey(evnt.button.button); // Store when mouse button is pressed
		break; case SDL_MOUSEBUTTONUP:
		inputManager.releaseKey(evnt.button.button); // Store when mouse button is released
		break; case SDL_MOUSEWHEEL:
		inputManager.zoom = evnt.wheel.y;
	}
}

void AppClient::Update(void) {
	if (currentScreen) { // If current screen exists
		switch (currentScreen->currentState) { // Check for the state of the screen
			case ScreenState::RUNNING:
			inputManager.update();	// Update the input manager instance
			currentScreen->Update(); // Update the current screen if running
			break;
			case ScreenState::EXIT: Exit(); break;
			case ScreenState::SLEEP: default: break;
		}
	} else Exit(); // Call exit function if screen doesn't exist
}

void AppClient::Draw(void) const {
	if (currentScreen && currentScreen->currentState == ScreenState::RUNNING) { // If screen object exists and its state is running
		currentScreen->Draw(); // Then call the draw method of the screen
	}
}

void AppClient::Run(void) {
	Init(); // Call the init everything function
	FPSLimiter fpsLimiter; // Spawn the main instance of the timing limiter
	fpsLimiter.setTargetFPS(TARGET_FPS); // Set the frames per second we whish to have, ideally 60-120

	while (m_isRunning) { // While game is running
		fpsLimiter.begin();					// Init FPS counter
		Update();							// Main update function
		if (!m_isRunning) break;			// Break main game loop if running attribute set to false
		Draw();								// Main draw function
		fpsLimiter.end();					// Calculate and restore FPS
		fps = fpsLimiter.fps;				// Get the current fps of the class instance
		deltaTime = fpsLimiter.deltaTime;	// Get the current fps of the class instance
	}
}

void AppClient::Exit(void) {
	mainSocket << UDPStream::packet << MSG_EXIT << serverAddress;
	currentScreen->OnExit(); // Call the leaving method of the current screen
	if (m_screenList) m_screenList->Destroy(), m_screenList.reset();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	DestroySDL();
	m_isRunning = false; // Execution ends
}