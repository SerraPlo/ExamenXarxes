#pragma once
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#pragma comment(lib, "SDL2main.lib")

namespace SerraPlo {

	struct Point {
		glm::ivec2 position;
		SDL_Color color;
		void Draw(SDL_Renderer *renderer) const {
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawPoint(renderer, position.x, position.y);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		}
	};

	struct Line {
		glm::ivec2 origin;
		glm::ivec2 end;
		SDL_Color color;
		void Draw(SDL_Renderer *renderer) const {
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawLine(renderer, origin.x, origin.y, end.x, end.y);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		}
	};

	struct Rectangle : Point {
		glm::ivec2 position;
		int width, height;
		SDL_Color color;
		void Draw(SDL_Renderer *renderer) const {
			SDL_Rect rectangle;
			rectangle.x = position.x;
			rectangle.y = position.y;
			rectangle.w = width;
			rectangle.h = height;
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &rectangle);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		}
	};
	
}

