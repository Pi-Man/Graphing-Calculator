#ifndef POINT_H
#define POINT_H

#include <string>
#include "Function.h"

class Point;

class PointParser : public FunctionParser {
	LinkedList<Computable*> computables;
public:
	PointParser(string input);
	void start();
	LinkedList<Number> eval();
	friend Point;
};

class Point {
	Number x;
	Number y;
public:
	PointParser parser;
	Point(string input);
	Point(Number x, Number y);
	LinkedList<Number> eval();
};

#endif