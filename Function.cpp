#ifndef Function_CPP
#define Function_CPP

#include<sstream>
#include<iostream>
#include "Function.h"

extern LinkedList<Function*> functions;
extern LinkedList<Variable*> globalVariables;


FunctionParser::FunctionParser(const string& input) : input(input) {}


Number FunctionParser::eval(Number arg) {
	LinkedList<Number> list;
	list.add(arg);
	return eval(list);
}


template<class... Args>
Number FunctionParser::eval(Number arg, Args... args) {
	LinkedList<Number> list;
	list.add(arg);
	return eval(list, args...);
}


template<class... Args>
Number FunctionParser::eval(LinkedList<Number>& list, Number arg, Args... args) {
	list.add(arg);
	return eval(list, args...);
}


Number FunctionParser::eval(LinkedList<Number>& list, Number arg) {
	list.add(arg);
	return eval(list);
}


Number FunctionParser::eval() {
	LinkedList<Number> list;
	return eval(list);
}


Number FunctionParser::eval(LinkedList<Number>& list) {
	if (output == nullptr) {
		throw runtime_error("Incomplete Function");
	}
	if (list.size() != argCount) {
		throw runtime_error("Incorrect number of Arguments");
	}
	for (int i = 0; i < list.size(); i++) {
		localVariables[i].setValue(list[i]);
	}
	return output->operator Number();
}


string FunctionParser::start() {

	if (input.size() == 0) {
		return "";
	}

	if (input.find(';') != -1) {
		input = input.substr(0, input.find(';'));
	}

	localVariables = LinkedList<Variable>();
	argCount = 0;

	input = removeSpace(input);

	long split = input.find('=');

	string left = input.substr(0, split);

	string right = input.substr(split + 1, input.size());

	string name;
	string variables;

	matchFunctionDef(left, name, variables);

	this->name = name;

	LinkedList<string> varList;
	splitString(varList, variables, ',');

	if (varList.size()) {
		for (int i = 0; i < varList.size(); i++) {
			localVariables.add(Variable(varList[i]));
			argCount++;
		}
	}

	if (split == -1) {
		return name;
	}

	Computable* num = operationMatch(right);

	output = num;
	return name;
}


string FunctionParser::removeSpace(const string& input) {
	string copy;
	for (char c : input) {
		if (c != ' ') {
			copy += c;
		}
	}
	return copy;
}


bool FunctionParser::matchFunctionDef(const string& input, string& name, string& variables) {
	int i = 0;
	if (!isLetter(input[i])) {
		throw runtime_error("SyntaxError: Variables must start with a letter");
	}
	else {
		name += input[i];
	}
	bool flag = false;
	bool flag2 = false;
	for (i = 1; i < input.size(); i++) {
		if (input[i] == '(') {
			flag = true;
		}
		else if (input[i] == ')') {
			flag2 = true;
		}
		else if (flag) {
			variables += input[i];
		}
		else {
			name += input[i];
		}
	}
	return flag2;
}

Computable* FunctionParser::operationMatch(string expresion) {
	return operationMatch(expresion, 0);
}

Computable* FunctionParser::operationMatch(string expresion, int depth) {

  	int p = 0;
	string left2;
	string right2;
	string operation;

	long index = -1;
	for (int i = expresion.size() - 1; i >= 0; i--) {
		char c = expresion[i];
		if (c == '(') p++;
		if (c == ')') p--;
		if (p == 0 && (c == '+' || c == '-')) {
			operation = c;
			index = i;
			break;
		}
	}

	if (index == -1) {
		for (int i = expresion.size() - 1; i >= 0; i--) {
			char c = expresion[i];
			if (c == '(') p++;
			if (c == ')') p--;
			if (p == 0 && (c == '*' || c == '/' || c == '%')) {
				operation = c;
				index = i;
				break;
			}
		}
	}

	if (index == -1) {
		for (int i = expresion.size() - 1; i >= 0; i--) {
			char c = expresion[i];
			if (c == '(') p++;
			if (c == ')') p--;
			if (p == 0 && c == '^') {
				operation = c;
				index = i;
				break;
			}
		}
	}

	if (index != -1) {
		if (index == 0) {
			if (operation == "-") {
				Operation* opp = new Operation(new Variable("", -1), operationMatch(expresion.substr(1), depth + 1), "*");
				return opp;
			}
		}
		left2 = expresion.substr(0, index);
		right2 = expresion.substr(index + 1);

		if (left2[0] == '(' && left2[left2.size() - 1] == ')') {
			left2 = left2.substr(1, left2.size() - 2);
		}

		if (right2[0] == '(' && right2[right2.size() - 1] == ')') {
			right2 = right2.substr(1, right2.size() - 2);
		}

		Operation* opp = new Operation(operationMatch(left2, depth + 1), operationMatch(right2, depth + 1), operation);

		return opp;
	}
	else {
		Computable* num = matchRightFunction(expresion, depth);
		return num;
	}
}


Computable* FunctionParser::matchRightFunction(const string& right, int depth) {
	Computable* v = nullptr;
	string number;
	string variable;
	string arguments;
	Number sign = 1;
	for (int i = 0; i < right.size(); i++) {
		char c = right[i];
		if (c == '-') {
			sign *= -1;
		}
		else if (isNumber(c)) {
			readNumber(right, number, i);
			char* cp = &number[0];
			double k = atof(cp);
			//cout << k << endl;
			v = new Variable("", k);
		}
		else if (isLetter(c)) {
			readVariable(right, variable, arguments, i);

			//cout << quoted(variable) << endl;
			//cout << quoted(arguments) << endl;
			LinkedList<string> argList;
			splitString(argList, arguments, ',');

			if (variable == "int") {
				if (argList.size() == 4) {
					Computable* start = operationMatch(argList[0], depth + 1);
					Computable* end = operationMatch(argList[1], depth + 1);
					localVariables.add(Variable(argList[3]));
					if (start == nullptr || end == nullptr) {
						throw runtime_error("Incomplete Function");
					}
					Computable* f = operationMatch(argList[2], depth + 1);
					if (f == nullptr) {
						throw runtime_error("Incomplete Function");
					}
					string v = argList[3];
					return f->defInt(start, end, v);
					cout << "definite integral\r";
				}
			}
			else if (variable == "der") {
				cout << "derivative       \r";
			}
			else if (variable == "sum") {
				cout << "summation        \r";
			}
			else if (variable == name) {
				cout << "recursion        \r";
				return &localVariables[0];
			}
			else if (argList.size()) {
				Function* f = findFunction(variable);
				int j = 0;
				for (string arg : argList) {
					f->addComputable(operationMatch(arg, depth + 1));
					j++;
				}
				return f;
			}
			else {
				v = findVariable(variable);
			}
		}
		else if (isOperator(c)) {
			//cout << c << endl;
		}
	}
	//v = new Operation(new Variable("", sign), v, "*");
	return v;
}


bool FunctionParser::isNumber(char c) {
	return c >= '0' && c <= '9';
}


bool FunctionParser::isOperator(char c) {
	return c == '*' || c == '/' || c == '+' || c == '-';
}


bool FunctionParser::isLetter(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

int FunctionParser::count(const string& input, const char test) {
	int count = 0;
	for (char c : input) {
		if (c == test) {
			count++;
		}
	}
	return count;
}

void FunctionParser::readNumber(const string& input, string& output, int& i) {
	output = "";
	while ((isNumber(input[i]) || input[i] == '.') && i < input.size()) {
		output += input[i];
		i++;
	}
	i--;
}


void FunctionParser::readVariable(const string& input, string& output, string& arguments, int& i) {
	output = "";
	arguments = "";
	while (isLetter(input[i]) || isNumber(input[i]) || input[i] == '(') {
		output += input[i];
		i++;
		if (input[i] == '(') {
			int p = 0;
			do {
				if (i >= input.size()) {
					throw runtime_error("Unbalanced Parenthases");
				}
				//output += input[i];
				if (input[i] == '(') {
					p++;
				}
				else if (input[i] == ')') {
					p--;
				}
				arguments += input[i];
				i++;
			} while (p);
			arguments = arguments.substr(1, arguments.size() - 2);
		}
	}
	i--;
}


void FunctionParser::splitString(LinkedList<string>& list, const string& input, char spliter) {

	if (input.size() == 0) {
		return;
	}

	int p = 0;
	string s;
	for (char c : input) {
		if (p == 0 && c == spliter) {
			list.add(string(s));
			s.clear();
		}
		else {
			s += c;
			if (c == '(') {
				p++;
			}
			if (c == ')') {
				p--;
			}
		}
	}
	list.add(s);

}


Variable* FunctionParser::findVariable(const string& name) {
	for (Variable& variable : localVariables) {
		if (name == variable.name) {
			Variable* ptr = &variable;
			return &variable;
		}
	}
	for (Variable* variable : globalVariables) {
		if (name == variable->name) {
			return variable;
		}
	}
	throw runtime_error("Variable \"" + name + "\" not found");
}


Function* FunctionParser::findFunction(const string& name) {
	for (Function* function : functions) {
		if (name == function->getName()) {
			return function->copy();
		}
	}
	throw runtime_error("Function \"" + name + "\" not found");
}

int FunctionParser::getArgCount() {
	return argCount;
}


Function::Function(Function& other) : name(other.name), parser(other.parser), computables(other.computables), color(other.color), draw(other.draw) {
	parser.start();
}


Function::Function(Function&& other) noexcept : name(std::move(other.name)), parser(std::move(other.parser)), computables(std::move(other.computables)), color(std::move(other.color)), draw(std::move(other.draw)) {
	parser.start();
}


Function::Function(const string& function, unsigned int color, bool draw) : parser(function), color(std::move(color)), draw(std::move(draw)) {
	name = parser.start();
}

Computable * Function::Int(Computable * computable, const string & variable) {
	int i = 0;
	for (Computable* c : computables) {
		if (c->dependentOn(variable)) {
			break;
		}
		i++;
	}
	string old = parser.localVariables[i].name;
	return parser.output->replaceVariable(old, new Variable(variable))->Int(computable, variable);
}

void Function::addComputable(Computable* computable) {
	computables.add(computable);
}


string& Function::getName() {
	return name;
}

int Function::getArgCount() {
	return parser.getArgCount();
}


Function* Function::copy() {
	return new Function(*this);
}


Number Function::eval(Number arg) {
	LinkedList<Number> list;
	list.add(arg);
	return eval(list);
}


template<class... Args>
Number Function::eval(Number arg, Args... args) {
	LinkedList<Number> list;
	list.add(arg);
	return eval(list, args...);
}


template<class... Args>
Number Function::eval(LinkedList<Number>& list, Number arg, Args... args) {
	list.add(arg);
	return eval(list, args...);
}


Number Function::eval(LinkedList<Number>& list, Number arg) {
	list.add(arg);
	return eval(list);
}


Number Function::eval() {
	LinkedList<Number> list;
	return eval(list);
}


Number Function::eval(LinkedList<Number>& list) {
	return parser.eval(list);
}


Function::operator Number() {
	LinkedList<Number> list;
	for (Computable* computable : computables) {
		if (computable == nullptr) {
			throw runtime_error("Incomplete Function");
		}
		list.add(*computable);
	}
	return parser.eval(list);
}

bool Function::dependentOn(const string & variable) {
	for (Computable* c : computables) {
		if (c->dependentOn(variable)) {
			return true;
		}
	}
	return false;
}

Computable* Function::replaceVariable(const string & variable, Computable * computable) {
	Function* f = this->copy();
	for (int i = 0; i < f->computables.size(); i++) {
		f->computables[i] = f->computables[i]->replaceVariable(variable, computable);
	}
	return f;
}

#endif