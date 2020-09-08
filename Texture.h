#ifndef TEXTURE_H
#define TEXTURE_H

#include<SDL.h>
#include"math.h"

class Texture {
	SDL_Texture* texture;
	Vec2 size;
	void* pixels;
	int pitch;
	public:
		Texture(SDL_Renderer* renderer, Vec2 size);
		Texture(SDL_Renderer* renderer, Vec2 size, unsigned int color);
		~Texture();

		void unlock();
		void lock();
		int* getPixels();
		int getPitch();
		void fill(unsigned int color);
};

#endif