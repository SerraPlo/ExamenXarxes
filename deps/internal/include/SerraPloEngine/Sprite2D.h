#pragma once
#include <string>
#include <SDL2\SDL.h>
#include "InputManager.h"

namespace SerraPlo {

	class Sprite2D {
	public:
		SDL_Texture *texture{ nullptr };
		glm::ivec2 position;
		int width, height;
		Sprite2D() = default;
		virtual ~Sprite2D() { SDL_DestroyTexture(texture); }
		void LoadImage(const std::string &path, SDL_Window *window, SDL_Renderer *renderer);
		virtual void Draw(SDL_Renderer *renderer);
	};

	class Button2D {
	public:
		SDL_Texture *textureDefault{ nullptr };
		SDL_Texture *textureHover{ nullptr };
		glm::ivec2 position;
		int width, height;
		bool hover{ false };
		bool pressed{ false };
		Button2D() = default;
		~Button2D() { SDL_DestroyTexture(textureDefault), SDL_DestroyTexture(textureHover); }
		void Reset() { hover = false; pressed = false; }
		void LoadImages(const std::string &pathDefault, const std::string &pathHover, SDL_Window *window, SDL_Renderer *renderer);
		void Update(InputManager &inputManager);
		void Draw(SDL_Renderer *renderer);
	};
}