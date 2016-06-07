#include "Sprite2D.h"
#include <SDL2\SDL_image.h>
#include "PathLoader.h"
#pragma comment(lib, "SDL2_image.lib")

namespace SerraPlo {

	void Sprite2D::LoadImage(const std::string &path, SDL_Window *window, SDL_Renderer *renderer) {
		SDL_Surface *loadSurface = IMG_Load(GetPathToAsset(path).c_str());
		//SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
		//SDL_Surface *textureSurface = SDL_ConvertSurface(loadSurface, screenSurface->format, NULL);
		texture = SDL_CreateTextureFromSurface(renderer, loadSurface);
		SDL_FreeSurface(loadSurface);
		//SDL_FreeSurface(textureSurface);
	}

	void Sprite2D::Draw(SDL_Renderer *renderer) {
		SDL_Rect destRect = { position.x, position.y, width, height };
		SDL_RenderCopy(renderer, texture, nullptr, &destRect);
	}

	void Button2D::LoadImages(const std::string &pathDefault, const std::string &pathHover, SDL_Window *window, SDL_Renderer *renderer) {
		SDL_Surface *loadSurface = IMG_Load(GetPathToAsset(pathDefault).c_str());
		SDL_Surface *textureSurface = SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_ARGB8888, 0);
		textureDefault = SDL_CreateTextureFromSurface(renderer, textureSurface);
		loadSurface = IMG_Load(GetPathToAsset(pathHover).c_str());
		textureSurface = SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_ARGB8888, 0);
		textureHover = SDL_CreateTextureFromSurface(renderer, textureSurface);
		SDL_FreeSurface(loadSurface);
		SDL_FreeSurface(textureSurface);
	}

	void Button2D::Update(InputManager &inputManager) {
		hover = false;
		if (inputManager.mouseCoords.x > position.x && inputManager.mouseCoords.x < position.x + width &&
			inputManager.mouseCoords.y > position.y && inputManager.mouseCoords.y < position.y + height) {
			hover = true;
			if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) pressed = true;
		}
	}

	void Button2D::Draw(SDL_Renderer *renderer) {
		SDL_Rect destRect = { position.x, position.y, width, height };
		if (hover && textureHover != nullptr) SDL_RenderCopy(renderer, textureHover, nullptr, &destRect);
		else SDL_RenderCopy(renderer, textureDefault, nullptr, &destRect);
	}

}