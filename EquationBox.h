#ifndef EQUATION_BOX_H
#define EQUATION_BOX_H

#include<SDL.h>
#include<SDL_ttf.h>
#include<iostream>
#include"Function.h"
#include"U8String.h"

class EquationBox {
	int x = 0;
	int y = 0;
	int w = 100;
	int h = 20;
	U8String input;
	U8String pendingInput;
	string error;
	SDL_Texture* errorText = nullptr;
	bool dirtyError = false;
	SDL_Window* window = nullptr;
	TTF_Font* font = nullptr;
	int index = 0;
	int pendingIndex = 0;
	bool hasFocus = false;
	long time = 0;
	int getStringPixelLength(const U8String& string);
	bool setErrorTexture(SDL_Renderer* renderer);
public:
	EquationBox(SDL_Window* window, TTF_Font* font);

	void setPos(int xIn, int yIn);
	void onEvent(SDL_Event& event);
	void render(SDL_Renderer* renderer);
	string getString();
	void setError(string& string);
};

#endif