#include "Texture.h"



Texture::Texture(SDL_Renderer* renderer, Vec2 sizeIn) : size(sizeIn) {
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);
}

Texture::Texture(SDL_Renderer* renderer, Vec2 size, unsigned int color) {
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);
	fill(color);
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}


void Texture::unlock() {
	SDL_UnlockTexture(texture);
	pixels = nullptr;
	pitch = 0;
}

void Texture::lock() {
	SDL_LockTexture(texture, NULL, &pixels, &pitch);
}

int* Texture::getPixels() {
	return (int*)pixels;
}

int Texture::getPitch() {
	return pitch / 4;
}

void Texture::fill(unsigned int color) {
	lock();
	int* pixels = getPixels();
	for (int i = 0; i < getPitch() * size.y; i++) {
		pixels[i] = color;
	}
	unlock();
}
