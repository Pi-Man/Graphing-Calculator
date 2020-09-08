
#include<stdio.h>
#include<iostream>
#include<string>
#include<sstream>
#include<exception>
#include<stdexcept>

//#include"LinkedList.cpp"
//#include"function.cpp"
#include"math.h"
#include"Slider.h"
#include<SDL.h>
#include<SDL_ttf.h>
#include"Function.h"
#include"DefaultFunctions.h"
#include"EquationBox.h"
#include"Variable.h"
#include"point.h"

#define HANDLE_FERROR(a) if (a < 0) {close(); return a;}

#define if_for(a, b, c) a; if (b) for(a; b; c)

LinkedList<Function*> functions;
LinkedList<Variable*> globalVariables;

typedef Uint8 Byte;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

double xScale = 100;
double yScale = 100;

double xAxisScale = 1;
double yAxisScale = 1;

double xOffset = 0;
double yOffset = 0;

Byte running = 1;

Number k;

//The main window
SDL_Window* graphWindow = NULL;
SDL_Window* settingsWindow = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

SDL_Renderer* graphRenderer = NULL;
SDL_Renderer* settingsRenderer = NULL;

TTF_Font* axisFont;
TTF_Font* equationFont;

bool grabbed = false;

int init();

void close();

void drawFunction(Function& f, unsigned int color);
void drawFunction2(Function& f, unsigned int color);
void renderPoint(Point& point, unsigned int color);

void drawAxis(unsigned int color);

string dtoa(double j);

unsigned int getDefaultColor(int i);

bool isRecursive(const string& input);
bool isFunction(const string& input);
bool isVariable(const string& input);
string getVariableName(const string& input);
string getFunctionName(const string& input);

int SDL_main(int argc, char* args[]) {
	HANDLE_FERROR(init());

	initPredefinedFunctions(functions);

	globalVariables.add(new Variable("tick", 0));
	globalVariables.add(new Variable("pi", Math::pi));
	globalVariables.add(new Variable("e", Math::e));
	globalVariables.add(new Variable("i", 0, 1));
	FunctionParser phi = FunctionParser("phi=0.5 + 5^0.5/2");
	phi.start();
	try {
		globalVariables.add(new Variable("phi", phi.eval()));
	}
	catch (runtime_error& e) {
		cout << "wtf?: " << e.what() << endl;
	}

	int preFnctSize = functions.size();

	bool running = true;

	LinkedList<EquationBox> equationBoxes;

	EquationBox pwr_k_box(settingsWindow, equationFont);
	pwr_k_box.setPos(500, 10);

	equationBoxes.add(EquationBox(settingsWindow, equationFont));
	equationBoxes[0].setPos(10, 70);

	Slider xSlider(settingsWindow, 10, 10, 200, 20, xScale);
	Slider ySlider(settingsWindow, 10, 40, 200, 20, yScale);
	Slider alphaSlider(settingsWindow, 220, 10, 260, 20, 255);

	int xfactor = 2;
	int yfactor = 2;

	while (running) {

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
				running = false;
			}
			else {
				xSlider.handleEvent(event);
				ySlider.handleEvent(event);
				alphaSlider.handleEvent(event);
				pwr_k_box.onEvent(event);
				for (EquationBox& box : equationBoxes) {
					box.onEvent(event);
				}
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.windowID == SDL_GetWindowID(graphWindow)) {
					grabbed = true;
				}
				else if (event.type == SDL_MOUSEBUTTONUP) {
					grabbed = false;
				}
				else if (event.type == SDL_MOUSEWHEEL && event.wheel.windowID == SDL_GetWindowID(graphWindow)) {
					xSlider += event.wheel.y;
					ySlider += event.wheel.y;
				}
			}
		}

		LinkedList<Point> points;

		*(globalVariables.get(0)) += 1;

		FunctionParser pwr_k_parser("pwr_k=" + pwr_k_box.getString());
		try {
			pwr_k_parser.start();
			k = pwr_k_parser.eval();
			string error;
			pwr_k_box.setError(error);
		}
		catch (runtime_error& e) {
			string error = e.what();
			pwr_k_box.setError(error);
		}
		for (int i = 0; i < equationBoxes.size(); i++) {
			EquationBox& box = equationBoxes[i];
			string data = box.getString();
			if (data.size() == 0) {

			}
			else if (isRecursive(data)) {
				try {
					string name = getFunctionName(data);
					int index = (int)data.find(';');
					int index2 = data.find('@');
					string initial, at;
					if (index == -1) {
						initial = "0";
					}
					else {
						initial = data.substr(index + 1, index2 == -1 ? UINT64_MAX : (index2 - index - 1));
					}
					double d1 = atof(initial.c_str());
					int max = SCREEN_WIDTH / 2 / xScale - xOffset;

					Function f(data);

					Number last(d1);
					if (index2 == -1) {
						int j;
						for (j = 0; j <= max; j++) {
							points.add(new Point(Number(j), last));
							last = f.eval(last);
						}
						points.add(new Point(Number(j), last));
					}
					else {
						at = data.substr(index2 + 1);
						double d2 = atof(at.c_str());
						for (int j = 0; j < d2; j++) {
							last = f.eval(last);
						}
						points.add(new Point(Number(0), last));
					}

				}
				catch (runtime_error& e) {

				}
			}
			else if (isFunction(data)) {
				try {
					string name = getFunctionName(data);
					int j = -1;
					for (int k = 0; k < functions.size(); k++) {
						if (functions[k]->getName() == name) {
							j = k;
						}
					}
					if (j == -1) {
						j = functions.size();
						functions.add(new Function(data, getDefaultColor(i), data.size() != 0));
					}
					
					Function* f = functions[j];
					//delete f;
					functions[j] = new Function(data, getDefaultColor(i), data.size() != 0);

					if (!functions[j]->draw || functions[j]->getArgCount() != 1) {
						string error;
						box.setError(error);
					}
				}
				catch (runtime_error& e) {
					if (!functions[preFnctSize + i]->draw || functions[preFnctSize + i]->getArgCount() != 1) {
						//cout << e.what() << endl;
						string error = e.what();
						box.setError(error);
					}
				}
			}
			else if (isVariable(data)) {
				try {
   					string name = getVariableName(data);
					Variable* variable = nullptr;
					for (Variable* var : globalVariables) {
						if (var->name == name) {
							variable = var;
						}
					}
					if (variable == nullptr) {
						variable = new Variable(name);
						globalVariables.add(variable);
					}
					FunctionParser p(data);
					p.start();
					variable->setValue(p.eval());
					
					string error;
					box.setError(error);
				}
				catch (runtime_error& e) {
 					string error = e.what();
					box.setError(error);
				}
			}
			else {
				try {
					points.add(Point(data));
				}
				catch (runtime_error& e) {

				}
			}
		}

		if (equationBoxes[equationBoxes.size() - 1].getString().size() != 0) {
			equationBoxes.add(EquationBox(settingsWindow, equationFont));
			equationBoxes[equationBoxes.size() - 1].setPos(10, 70 + (TTF_FontHeight(equationFont) * 2 + TTF_FontLineSkip(equationFont)) * (equationBoxes.size() - 1));
		}

		xScale = pow<double, int>(1.2, xSlider - 75);
		yScale = pow<double, int>(1.2, ySlider - 75);

		int x;
		int y;

		SDL_GetRelativeMouseState(&x, &y);

		if (grabbed) {
			xOffset += x / xScale;
			yOffset -= y / yScale;
		}

		while (xScale * xAxisScale < 50) {
			xAxisScale *= xfactor == 0 ? 2.5 : 2;
			xfactor = Math::mod(xfactor + 1, 3);
		}
		while (xScale * xAxisScale > 150) {
			xAxisScale /= xfactor == 1 ? 2.5 : 2;
			xfactor = Math::mod(xfactor - 1, 3);
		}
		while (yScale * yAxisScale < 50) {
			yAxisScale *= yfactor == 0 ? 2.5 : 2;
			yfactor = Math::mod(yfactor + 1, 3);
		}
		while (yScale * yAxisScale > 150) {
			yAxisScale /= yfactor == 1 ? 2.5 : 2;
			yfactor = Math::mod(yfactor - 1, 3);
		}

		if (SDL_SetRenderDrawBlendMode(graphRenderer, SDL_BLENDMODE_BLEND)) {
			cout << "error enabling blend" << endl;
		}

		SDL_SetRenderDrawColor(graphRenderer, 0xff, 0xff, 0xff, alphaSlider);
		SDL_RenderFillRect(graphRenderer, NULL);
		drawAxis(0xc0c0c0);
		for (int i = preFnctSize; i < functions.size(); i++) {
			Function* function = functions[i];
			EquationBox& box = equationBoxes[i - preFnctSize];
			if (function->getArgCount() == 1 && function->draw) {
				try {
					drawFunction2(*function, function->color);
					string error;
					box.setError(error);
				}
				catch (runtime_error& e) {
					//cout << e.what() << endl;
					string error = e.what();
					box.setError(error);
				}
			}
		}

		int i = 0;
		for (Point& point : points) {
			try {
				renderPoint(point, getDefaultColor(i));
			}
			catch (runtime_error& e) {
				string error = e.what();
				cout << error << "\r";
			}
			i++;
		}

		SDL_RenderPresent(graphRenderer);

		SDL_SetRenderDrawColor(settingsRenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(settingsRenderer);

		xSlider.render(settingsRenderer);
		ySlider.render(settingsRenderer);
		alphaSlider.render(settingsRenderer);

		for (EquationBox& box : equationBoxes) {
			pwr_k_box.render(settingsRenderer);
			box.render(settingsRenderer);
		}

		SDL_RenderPresent(settingsRenderer);

	}

	close();

	return 0;
}

void drawFunction(Function& f, unsigned int color) {
	SDL_Texture* graph = SDL_CreateTexture(graphRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (SDL_SetTextureBlendMode(graph, SDL_BLENDMODE_BLEND)) {
		cout << "error enabling blend" << endl;
	}

	void* pixels;
	int pitch;

	SDL_LockTexture(graph, NULL, &pixels, &pitch);

	unsigned int transparency = 0;

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		((unsigned int*)pixels)[i] = transparency;
	}

	int curve[SCREEN_WIDTH];
	int size = 2;
	int sizemin = -size / 2;
	int sizemax = size / 2 + (size % 2);

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		try {
			double num = (f.eval(Number((i - SCREEN_WIDTH / 2) / xScale - xOffset)).real * yScale + yOffset * yScale + SCREEN_HEIGHT / 2);
			if (!isfinite(num)) {
				curve[i] = yOffset * yScale + SCREEN_HEIGHT / 2;
				continue;
			}
			curve[i] = (int)((double)Math::clamp(num, -10, SCREEN_HEIGHT + 10) + 0.5);
			if (curve[i] < SCREEN_HEIGHT && curve[i] >= 0 || i > 0 && curve[i - 1] < SCREEN_HEIGHT && curve[i - 1] >= 0) {
				for (int j = 0; i == 0 ? j == 0 : j < abs(curve[i] - curve[i - 1]) || j == 0 && curve[i] == curve[i - 1]; j++) {

					int n = abs(curve[i] - curve[i - 1]);

					int s = i == 0 ? 0 : curve[i] - curve[i - 1] < 0 ? 1 : -1;

					int k = curve[i] + j * s;

					for (int x = sizemin; x < sizemax; x++) {
						for (int y = sizemin; y < sizemax; y++) {

							int xIndex = i + x;
							int yIndex = SCREEN_HEIGHT - k - 1 + y;

							if (xIndex >= 0 && xIndex < SCREEN_WIDTH && yIndex >= 0 && yIndex < SCREEN_HEIGHT) {

								int index = yIndex * SCREEN_WIDTH + xIndex;

								//cout << index % SCREEN_WIDTH << ": " << index / SCREEN_HEIGHT << "     " << i << ": " << curve[i] << endl;
								((unsigned int*)pixels)[index] = (color << 8) + 0xff;
							}

						}
					}
				}
			}
		}
		catch (runtime_error& e) {
			SDL_DestroyTexture(graph);
			throw e;
		}
	}

	SDL_UnlockTexture(graph);

	SDL_Rect rect;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	rect.x = 0;
	rect.y = 0;

	SDL_RenderCopy(graphRenderer, graph, NULL, &rect);

	SDL_DestroyTexture(graph);
}

void drawFunction2(Function& f, unsigned int color) {
	//SDL_Texture* graph = SDL_CreateTexture(graphRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	//if (SDL_SetTextureBlendMode(graph, SDL_BLENDMODE_BLEND)) {
		//cout << "error enabling blend" << endl;
	//}

	//void* pixels;
	//int pitch;

	//SDL_LockTexture(graph, NULL, &pixels, &pitch);

	//for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		//((unsigned int*)pixels)[i] = transparency;
	//}

	SDL_SetRenderDrawColor(graphRenderer, color >> 16, (color >> 8) & 0xff, color & 0xff, 0xff);

	int curve[SCREEN_WIDTH];
	int size = 2;
	int sizemin = -size / 2;
	int sizemax = size / 2 + (size % 2);

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		try {
			Number number = f.eval(Number((i - SCREEN_WIDTH / 2) / xScale - xOffset));
			if (!number.isReal()) {
				curve[i] = yOffset * yScale + SCREEN_HEIGHT / 2;
				continue;
			}
			double num = number.real * yScale + yOffset * yScale + SCREEN_HEIGHT / 2;
			if (!isfinite(num)) {
				curve[i] = yOffset * yScale + SCREEN_HEIGHT / 2;
				continue;
			}
			curve[i] = (int)((double)Math::clamp(num, -10, SCREEN_HEIGHT + 10) + 0.5);
			if (curve[i] < SCREEN_HEIGHT && curve[i] >= 0 || i > 0 && curve[i - 1] < SCREEN_HEIGHT && curve[i - 1] >= 0) {
				for (int j = 0; i == 0 ? j == 0 : j < abs(curve[i] - curve[i - 1]) || j == 0 && curve[i] == curve[i - 1]; j++) {

					int n = abs(curve[i] - curve[i - 1]);

					int s = i == 0 ? 0 : curve[i] - curve[i - 1] < 0 ? 1 : -1;

					int k = curve[i] + j * s;

					for (int x = sizemin; x < sizemax; x++) {
						for (int y = sizemin; y < sizemax; y++) {

							int xIndex = i + x;
							int yIndex = SCREEN_HEIGHT - k - 1 + y;

							if (xIndex >= 0 && xIndex < SCREEN_WIDTH && yIndex >= 0 && yIndex < SCREEN_HEIGHT) {

								int index = yIndex * SCREEN_WIDTH + xIndex;

								SDL_RenderDrawPoint(graphRenderer, xIndex, yIndex);

								//cout << index % SCREEN_WIDTH << ": " << index / SCREEN_HEIGHT << "     " << i << ": " << curve[i] << endl;
								//((unsigned int*)pixels)[index] = (color << 8) + 0xff;
							}

						}
					}
				}
			}
		}
		catch (runtime_error& e) {
			//SDL_DestroyTexture(graph);
			//throw e;
		}
	}

	//SDL_UnlockTexture(graph);

	SDL_Rect rect;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	rect.x = 0;
	rect.y = 0;

	//SDL_RenderCopy(graphRenderer, graph, NULL, &rect);

	//SDL_DestroyTexture(graph);
}

void renderPoint(Point & point, unsigned int color) {

	LinkedList<Number> numbers = point.eval();

	if (numbers.size() != 2) {
		return;
	}

	LinkedList<double> doubles;
	for (Number number : numbers) {
		if (!number.isReal()) {
			return;
		}
		double num = number.real;
		if (!isfinite(num)) {
			return;
		}
		doubles.add(num);
	}

	int size = 9;
	int sizemin = -size / 2;
	int sizemax = size / 2 + (size % 2);

	SDL_SetRenderDrawColor(graphRenderer, color >> 16, (color >> 8) & 0xff, color & 0xff, 0xff);

	for (int x = sizemin; x < sizemax; x++) {
		for (int y = sizemin; y < sizemax; y++) {

			int xIndex = x + (int)(doubles[0] * xScale + xOffset * xScale + SCREEN_WIDTH / 2);
			int yIndex = y + (int)(-doubles[1] * yScale - yOffset * yScale + SCREEN_HEIGHT / 2);

			if (xIndex >= 0 && xIndex < SCREEN_WIDTH && yIndex >= 0 && yIndex < SCREEN_HEIGHT) {

				SDL_RenderDrawPoint(graphRenderer, xIndex, yIndex);

			}

		}
	}

}

void drawAxis(unsigned int color) {
	//SDL_Texture* background = SDL_CreateTexture(graphRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	//if (SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND)) {
	//	cout << "error enabling blend" << endl;
	//}

	//void* pixels;
	//int pitch;

	//SDL_SetRenderDrawColor(graphRenderer, 0xff, 0xff, 0xff, 0xff);
	//SDL_RenderClear(graphRenderer);

	//SDL_LockTexture(background, NULL, &pixels, &pitch);

	//for (int i = 0; i < pitch / 4 * SCREEN_HEIGHT; i++) {
	//	SDL_Rect rect;
	//	double x = (i % (pitch / 4));
	//	double y = SCREEN_HEIGHT - i / (pitch / 4) - 1;
	//	rect.x = x + 3;
	//	rect.y = SCREEN_HEIGHT - y + 3;
	//	x -= xOffset * xScale + SCREEN_WIDTH / 2;
	//	y -= yOffset * yScale + SCREEN_HEIGHT / 2;

	//	bool flag = (int)fmod(x, xScale * xAxisScale) == 0 && (int)x != 0 || (int)fmod(y, yScale * yAxisScale) == 0 && (int)y != 0;
	//	bool flag2 = (int)x == 0 || (int)y == 0;

	//	((unsigned int*)pixels)[i] = ((int)x == 0 && (int)y == 0) ? 0xff0000ff : flag2 ? 0x000000ff : flag ? (color << 8) | 0xff : 0;

	//	if (flag && flag2) {

	//		double x2 = x / xScale;
	//		double y2 = y / yScale;

	//		double j = x2 == 0 ? y2 : x2;

	//		cout << j << endl;

	//		SDL_Surface* text = TTF_RenderText_Solid(font, to_string(j).c_str(), SDL_Color{ 0, 0, 0 });

	//		SDL_Texture* ttext = SDL_CreateTextureFromSurface(graphRenderer, text);

	//		rect.w = text->w;
	//		rect.h = text->h;

	//		SDL_RenderCopy(graphRenderer, ttext, NULL, &rect);

	//		SDL_FreeSurface(text);

	//		SDL_DestroyTexture(ttext);
	//	}
	//}

	//SDL_UnlockTexture(background);

	//SDL_RenderCopy(graphRenderer, background, NULL, NULL);

	//SDL_DestroyTexture(background);

	double x = xOffset * xScale + SCREEN_WIDTH / 2;
	double y = -yOffset * yScale + SCREEN_HEIGHT / 2;

	int count = SCREEN_WIDTH / (xScale * xAxisScale);
	double offset = fmod(x, xScale * xAxisScale);
	SDL_SetRenderDrawColor(graphRenderer, 0xc0, 0xc0, 0xc0, 0xff);
	for (int i = -1; i <= count + 1; i++) {
		SDL_RenderDrawLine(graphRenderer, offset + i*xScale*xAxisScale, 0, offset + i*xScale*xAxisScale, SCREEN_HEIGHT);
		double j = (offset + i * xScale*xAxisScale - x)/xScale;
		string s = dtoa(j);
		SDL_Surface* surf = TTF_RenderText_Blended(axisFont, s.c_str(), SDL_Color{ 0, 0, 0 });
		SDL_Texture* text = SDL_CreateTextureFromSurface(graphRenderer, surf);
		SDL_Rect rect{ offset + i * xScale*xAxisScale + 3, y + 3, surf->w, surf->h };
		SDL_RenderCopy(graphRenderer, text, NULL, &rect);
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(text);
	}

	count = SCREEN_HEIGHT / (yScale * yAxisScale);
	offset = fmod(y, yScale * yAxisScale);
	SDL_SetRenderDrawColor(graphRenderer, 0xc0, 0xc0, 0xc0, 0xff);
	for (int i = -1; i <= count + 1; i++) {
		SDL_RenderDrawLine(graphRenderer, 0, offset + i * yScale*yAxisScale, SCREEN_WIDTH, offset + i * yScale*yAxisScale);
		double j = -(offset + i * yScale*yAxisScale - y) / yScale;
		string s = dtoa(j);
		SDL_Surface* surf = TTF_RenderText_Blended(axisFont, s.c_str(), SDL_Color{ 0, 0, 0 });
		SDL_Texture* text = SDL_CreateTextureFromSurface(graphRenderer, surf);
		SDL_Rect rect{ x + 3, offset + i * yScale*yAxisScale + 3, surf->w, surf->h };
		SDL_RenderCopy(graphRenderer, text, NULL, &rect);
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(text);
	}

	SDL_SetRenderDrawColor(graphRenderer, 0, 0, 0, 0xff);
	SDL_RenderDrawLine(graphRenderer, x, 0, x, SCREEN_HEIGHT);
	SDL_RenderDrawLine(graphRenderer, 0, y, SCREEN_WIDTH, y);

}

string dtoa(double j) {
	if (j == 0) {
		return "0";
	}
	j *= 1 + 1e-12;
	string out = "";
	bool negative = j < 0;
	if (negative) {
		out += '-';
		j *= -1;
	}
	if (j - (int)j >= 1 - 0.5*pow(10, -5)) {
		j += 1e-5;
	}
	int exp = 0;
	while (j >= 10) {
		j /= 10;
		exp++;
	}
	while (j < 1) {
		j *= 10;
		exp--;
	}
	if (abs(exp) < 6) {
		if_for (int i = 0, i <= exp, i++) {
			int k = j;
			out += '0' + k;
			j -= k;
			j *= 10;
		}
		else {
			out += '0';
			j *= pow(10, exp+1);
		}
		if (j >= 1e-4) {
			out += '.';
			for (int i = 0; i < 5; i++) {
				int k = j;
				out += '0' + k;
				j -= k;
				if (j < pow(10, i - 4)) {
					break;
				}
				j *= 10;
			}
		}
	}
	else {
		int k = j;
		out += '0' + k;
		j -= k;
		j *= 10;
		if (j >= 1e-4) {
			out += '.';
			for (int i = 0; i < 5; i++) {
				int k = j;
				out += '0' + k;
				j -= k;
				if (j < pow(10, i - 4)) {
					break;
				}
				j *= 10;
			}
		}
		out += "E";
		out += to_string(exp);
	}

	return out;
}

unsigned int getDefaultColor(int i) {
	i %= 8;
	if (i == 0) {
		return 0x0000ff;
	}
	else if (i == 1) {
		return 0xff0000;
	}
	else if (i == 2) {
		return 0x000000;
	}
	else if (i == 3) {
		return 0x00ff00;
	}
	else if (i == 4) {
		return 0xff00ff;
	}
	else if (i == 5) {
		return 0xffff80;
	}
	else if (i == 6) {
		return 0x80ffff;
	}
	else if (i == 7) {
		return 0xff80ff;
	}
	return 0;
}

bool isRecursive(const string& input) {
	int k;

	if ((k = input.find('=')) == -1) {
		return false;
	}

	string data = input.substr(0, k - 1);

	if (data.size() == 0) {
		return false;
	}

	string name;
	string variables;

	FunctionParser::matchFunctionDef(data, name, variables);

	string right = FunctionParser::removeSpace(input.substr(k + 1));

	string number;
	string variable;
	string arguments;
	for (int i = 0; i < right.size(); i++) {
		char c = right[i];
		if (FunctionParser::isLetter(c)) {
			try {
				FunctionParser::readVariable(right, variable, arguments, i);
				if (variable == name) {
					return true;
				}
				while (arguments.size() != 0) {
					int j = 0;
					string args = arguments;
					FunctionParser::readVariable(args, variable, arguments, j);
					if (variable == name) {
						return true;
					}
				}
			}
			catch (runtime_error& e) {
				return false;
			}
		}
	}

	return false;

}

bool isFunction(const string & input) {

	int i;

	if ((i = input.find('=')) == -1) {
		return false;
	}

	string data = input.substr(0, i);

	if (data.size() == 0) {
		return false;
	}

	string name;
	string variables;
	try {
		return FunctionParser::matchFunctionDef(data, name, variables) && name.size();
	}
	catch (runtime_error& e) {
		return false;
	}

}

bool isVariable(const string & input) {

	int i;

	if ((i = input.find('=')) == -1) {
		return false;
	}

	string data = input.substr(0, i);

	if (data.size() == 0) {
		return false;
	}

	string name;
	string variables;

	bool fun = FunctionParser::matchFunctionDef(data, name, variables) && name.size();

	return !variables.size() && !fun;

}

string getVariableName(const string & input) {
	
	if (input.find('=') == -1) {
		return "";
	}

	string nospace = FunctionParser::removeSpace(input);

	long split = nospace.find('=');

	string name = nospace.substr(0, split);

	return name;

}

string getFunctionName(const string & input) {

	if (input.find('=') == -1) {
		return "";
	}

	string nospace = FunctionParser::removeSpace(input);

	long split = nospace.find('=');

	string left = nospace.substr(0, split);

	string name;
	string variables;

	FunctionParser::matchFunctionDef(input, name, variables);

	return name;

}

int init() {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	//Initialize SDL_ttf
	if (TTF_Init() < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	axisFont = TTF_OpenFont("fonts/CamingoCode-Regular.ttf", 10);
	equationFont = TTF_OpenFont("fonts/mplus-1m-regular.ttf", 14);

	//Create window
	graphWindow = SDL_CreateWindow("Graph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (graphWindow == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	graphRenderer = SDL_CreateRenderer(graphWindow, -1, SDL_RENDERER_ACCELERATED);
	if (graphRenderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! Error: %s\n", SDL_GetError());
		return -1;
	}

	//Create window
	settingsWindow = SDL_CreateWindow("Settings", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (settingsWindow == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	settingsRenderer = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_ACCELERATED);
	if (settingsRenderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! Error: %s\n", SDL_GetError());
		return -1;
	}

	//Get window surface
	screenSurface = SDL_GetWindowSurface(graphWindow);

	return 1;
}

void close() {

	if (axisFont != NULL) {
		TTF_CloseFont(axisFont);
	}

	if (equationFont != NULL) {
		TTF_CloseFont(equationFont);
	}

	if (graphRenderer != NULL) {
		SDL_DestroyRenderer(graphRenderer);
	}

	if (graphWindow != NULL) {
		SDL_DestroyWindow(graphWindow);
	}

	if (settingsRenderer != NULL) {
		SDL_DestroyRenderer(settingsRenderer);
	}

	if (settingsWindow != NULL) {
		SDL_DestroyWindow(settingsWindow);
	}

	SDL_Quit();
}