#include "Slider.h"
#include <iostream>



Slider::Slider(SDL_Window* windowIn, int x, int y, int w, int h) : Slider(windowIn, x, y, w, h, 0) {}

Slider::Slider(SDL_Window* windowIn, int x, int y, int w, int h, int v) : value(v){
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	window = windowIn;
}

Slider::~Slider() {}

Slider::operator int() {
	return value;
}

Slider& Slider::operator+=(int amount) {
	value = Math::clamp(value + amount, 0, rect.w - 4 - 5);
	return *this;
}

void Slider::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderFillRect(renderer, &rect);

	SDL_Rect otherRect;

	otherRect.x = rect.x + 2;
	otherRect.y = rect.y + 2;
	otherRect.w = rect.w - 4;
	otherRect.h = rect.h - 4;

	SDL_SetRenderDrawColor(renderer, 0xf0, 0xf0, 0xf0, 0xff);
	SDL_RenderFillRect(renderer, &otherRect);

	SDL_Rect bar;
	bar.x = rect.x + value + 2;
	bar.y = rect.y;
	bar.w = 5;
	bar.h = rect.h;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderFillRect(renderer, &bar);

}

void Slider::handleEvent(SDL_Event event) {

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.x > rect.x && event.button.x < rect.x + rect.w && event.button.y > rect.y && event.button.y < rect.y + rect.h && event.button.windowID == SDL_GetWindowID(window)) {
			grabbed = true;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		grabbed = false;
		break;
	case SDL_MOUSEWHEEL:
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h && event.wheel.windowID == SDL_GetWindowID(window)) {
			value = Math::clamp(value + event.wheel.y, 0, rect.w - 4 - 5);
		}
	}
	if (grabbed) {
		int x;
		SDL_GetMouseState(&x, NULL);
		value = Math::clamp(x - rect.x - 2 - 3, 0, rect.w - 4 - 5);
	}

}