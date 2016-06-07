#pragma once
#include <GL\glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#pragma comment(lib, "SDL2_ttf.lib")
#pragma comment(lib, "SDL2_image.lib")
#include "PathLoader.h"

namespace SerraPlo {
	
	// Initialize SDL systems
	inline void InitSDL(void) {
		SDL_Init(SDL_INIT_EVERYTHING);	// Initialize everything in SDL (VIDEO, AUDIO, EVENTS,...)
		TTF_Init();						// Initialize SDL_ttf for text purposes
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		// Tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	}

	inline void DestroySDL(void) {
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	// Create a message box in which to ask the user whether to play on fullscreen or windowed mode
	int AskUserForWindow(void) {
		const SDL_MessageBoxButtonData buttons[] = { { 0, 0, "NO" }, { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "YES" } };
		const SDL_MessageBoxColorScheme colorScheme = { { { 255,   0,   0 }, { 0, 255,   0 }, { 255, 255,   0 }, { 0,   0, 255 }, { 255,   0, 255 } } };
		const SDL_MessageBoxData messageBoxData = { SDL_MESSAGEBOX_INFORMATION, nullptr, "Select an option", "Do you want to play on full screen mode?", SDL_arraysize(buttons), buttons, &colorScheme };
		int buttonID;
		SDL_ShowMessageBox(&messageBoxData, &buttonID); // Whether to play on fullscreen mode or default normal mode
		return buttonID;
	}

}
