//
// Created by gras on 11/25/23.
//

#include "Texture.h"

#include <SDL_image.h>

Texture::Texture() {
    texture = NULL;
    width = 0;
    height = 0;
}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string& path) {
	SDL_Texture* t = IMG_LoadTexture(renderer, path.c_str());
	int w = 0;
	int h = 0;
	SDL_QueryTexture(t, NULL, NULL, &w, &h);
	if(t == NULL) {
		printf("Unable to load image: %s with error msg: %s", path, IMG_GetError());
		return false;
	}
	width = w;
	height = h;
	texture = t;
	return true;
}

void Texture::free() {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y) {
	SDL_Rect render_quad = { x, y, width, height};
	SDL_RenderCopy(renderer,texture, NULL, &render_quad);
}

int Texture::get_height() {
	return height;
}

int Texture::get_width() {
	return width;
}


