#include "Point.h"

PointParser::PointParser(string input) : FunctionParser(input) {}

void PointParser::start() {

	if (input.size() < 3) {
		return;
	}

	if (input[0] != '(' || input[input.size() - 1] != ')') {
		throw runtime_error("Incorrect Syntax for Point");
	}

	string data = input.substr(1, input.size() - 2);

	LinkedList<string> coordinates;

	splitString(coordinates, data, ',');

	for (string s : coordinates) {
		computables.add(operationMatch(s));
	}

}

LinkedList<Number> PointParser::eval() {

	LinkedList<Number> numbers;

	for (Computable* c : computables) {
		if (c == nullptr) continue;
		numbers.add(*c);
	}

	return numbers;

}

Point::Point(string input) : parser(input) {
	parser.start();
}

Point::Point(Number x, Number y) : parser(""), x(x), y(y) {}

LinkedList<Number> Point::eval(){
	if (parser.input.size() == 0) {
		LinkedList<Number> list;
		list.add(x);
		list.add(y);
		return list;
	}
	return parser.eval();
}
