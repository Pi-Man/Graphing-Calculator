#ifndef SLIDER_H
#define SLIDER_H

#include<SDL.h>
#include"Texture.h"

class Slider {

	int value;
	SDL_Rect rect;
	bool grabbed = false;
	SDL_Window* window;

	public:
		Slider(SDL_Window* windowIn, int x, int y, int w, int h);
		Slider(SDL_Window* windowIn, int x, int y, int w, int h, int v);
		~Slider();
		operator int();
		Slider& operator+=(int amount);
		void render(SDL_Renderer* renderer);
		void handleEvent(SDL_Event event);
};

#endif