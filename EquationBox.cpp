#include "EquationBox.h"
#include<ctime>
#include<algorithm>


int EquationBox::getStringPixelLength(const U8String& string) {
	int x;
	TTF_SizeUTF8(font, string, &x, NULL);
	return x;
}

bool EquationBox::setErrorTexture(SDL_Renderer * renderer) {
	if (!dirtyError) {
		return errorText != nullptr;
	}
	dirtyError = false;
	if (errorText != nullptr) {
		SDL_DestroyTexture(errorText);
	}
	SDL_Surface* surf = TTF_RenderText_Blended(font, error.c_str(), SDL_Color{ 0xff, 0, 0 });
	if (surf == nullptr) {
		errorText = nullptr;
		return false;
	}
	errorText = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	return true;
}

EquationBox::EquationBox(SDL_Window* window, TTF_Font* font) : window(window), font(font) {}

void EquationBox::setPos(int xIn, int yIn){
	x = xIn;
	y = yIn;
}

void EquationBox::onEvent(SDL_Event & event) {
	if (event.window.windowID == SDL_GetWindowID(window)) {
		if (hasFocus && event.type == SDL_KEYDOWN && pendingInput.size() == 0) {
			if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				index = Math::clamp(index - 1, 0, input.size());
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				index = Math::clamp(index + 1, 0, input.size());
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
				if (index > 0) {
					input.remove(index-- - 1);
				}
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_DELETE) {
				if (index < input.size()) {
					input.remove(index);
					index = Math::clamp(index, 0, input.size());
				}
			}
			else {
				//const char c = event.key.keysym.sym;
				//if (input.length() > 0) {
				//	input = input.insert(index++, 1, c);
				//}
				//else {
				//	input = c;
				//	index = 1;
				//}
				//std::cout << input << endl;
			}
			time = clock();
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			int mx;
			int my;
			SDL_GetMouseState(&mx, &my);
			if (mx > x && mx < x + w && my > y && my < y + h) {
				index = Math::clamp(round((mx - x) * input.size() / (double)w), 0, input.size());
				hasFocus = true;
				SDL_StartTextInput();
			}
			else {
				hasFocus = false;
				//SDL_StopTextInput();
			}
		}
		else if (hasFocus && event.type == SDL_TEXTINPUT) {
			char* c = event.text.text;
			pendingInput.clear();
			pendingIndex = 0;
			if (input.size() > 0) {
				index += input.sinsert(index, c);
			}
			else {
				input = c;
				index = input.size();
			}
			
			//std::cout << input << endl;
		}
		else if(hasFocus && event.type == SDL_TEXTEDITING) {
			pendingInput = event.edit.text;
			pendingIndex = event.edit.start;
		}
	}
}

void EquationBox::render(SDL_Renderer * renderer) {
	U8String output = input;
	output.sinsert(index, pendingInput);
	if (output.size() > 0) {
		long ms = clock() - time;
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, output, SDL_Color{ 0, 0, 0 });
		SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);
		w = std::max(surf->w + 4, 100);
		h = std::max(surf->h + 4, TTF_FontHeight(font) * 2 + TTF_FontLineSkip(font));
		bool flag = setErrorTexture(renderer);
		SDL_Rect rectError;
		if (flag) {
			int x1, y1;
			TTF_SizeText(font, error.c_str(), &x1, &y1);
			w = max(w, x1 + 4);
			rectError = SDL_Rect{ x + 2, y + TTF_FontHeight(font) + TTF_FontLineSkip(font), x1, y1 };
		}
		SDL_Rect rectOut{ x, y, w, h };
		SDL_Rect rectIn{ x + 2, y + 2, w - 4, h - 4 };
		SDL_Rect rectCursor{ x + getStringPixelLength(input.subString(0, index)) + getStringPixelLength(pendingInput.subString(0, pendingIndex)) + 1, y + 2, 2, surf->h };
		SDL_Rect rectText{ x + 2, y + 2, surf->w, surf->h };
		SDL_Rect rectPending{ x + getStringPixelLength(input.subString(0, index)) + 1, y + surf->h, getStringPixelLength(pendingInput), 2 };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderFillRect(renderer, &rectOut);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderFillRect(renderer, &rectIn);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		if (hasFocus && ms / 500 % 2 == 0) {
			SDL_RenderFillRect(renderer, &rectCursor);
		}
		SDL_RenderFillRect(renderer, &rectPending);
		if (flag) {
			SDL_RenderCopy(renderer, errorText, NULL, &rectError);
		}
		SDL_RenderCopy(renderer, text, NULL, &rectText);
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(text);
	}
	else {
		w = 100;
		h = TTF_FontHeight(font) * 2 + TTF_FontLineSkip(font);
		SDL_Rect rectOut{ x, y, w, h };
		SDL_Rect rectIn{ x + 2, y + 2, w - 4, h - 4 };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderFillRect(renderer, &rectOut);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderFillRect(renderer, &rectIn);
	}
}

string EquationBox::getString() {
	return string(input);
}

void EquationBox::setError(string& string) {
	if (string != error) {
		error = std::move(string);
		dirtyError = true;
	}
}
