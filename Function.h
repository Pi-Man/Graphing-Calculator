#ifndef FUNCTION_H
#define FUNCTION_H

#include<string>
#include"math.h"
#include"LinkedList.h"
#include"Computable.h"
#include"Variable.h"
#include"Operation.h"

class Function;

class FunctionParser {
	LinkedList<Variable> localVariables;
	Computable* output = nullptr;
	int argCount = 0;
protected:
	string name;
	string input;
public:
	FunctionParser(const string& input);
	template<class... Args>
	Number eval(Number arg, Args... args);
	Number eval(Number arg);
	template<class... Args>
	Number eval(LinkedList<Number>& list, Number arg, Args... args);
	Number eval(LinkedList<Number>& list, Number arg);
	Number eval();
	Number eval(LinkedList<Number>& list);
	string start();
	static string removeSpace(const string& input);
	static bool matchFunctionDef(const string& input, string & name, string & variables);
	Computable* operationMatch(string expresion);
	Computable* operationMatch(string expresion, int depth);
	Computable* matchRightFunction(const string& right, int depth);
	static bool isNumber(char c);
	bool isOperator(char c);
	static bool isLetter(char c);
	int count(const string & input, const char test);
	static void readNumber(const string& input, string& output, int& i);
	static void readVariable(const string& input, string& output, string& arguments, int& i);
	static void splitString(LinkedList<string>& list, const string& input, char spliter);
	Variable* findVariable(const string& name);
	Function* findFunction(const string& name);
	int getArgCount();

	friend Function;
};

class Function: public Computable {
protected:
	string name;
	FunctionParser parser;
	LinkedList<Computable*> computables;
public:
	unsigned int color;
	bool draw;
	Function(Function& other);
	Function(Function&& other) noexcept;
	Function(const string& function, unsigned int color = 0, bool draw = false);
	virtual Computable* Int(Computable* computable, const string& variable);
	void addComputable(Computable* computable);
	string& getName();
	int getArgCount();
	virtual Function* copy();
	Number eval(Number arg);
	template<class ...Args>
	Number eval(Number arg, Args ...args);
	template<class ...Args>
	Number eval(LinkedList<Number>& list, Number arg, Args ...args);
	Number eval(LinkedList<Number>& list, Number arg);
	Number eval();
	virtual Number eval(LinkedList<Number>& list);
	operator Number();
	bool dependentOn(const string& variable);
	Computable* replaceVariable(const string& variable, Computable* computable);
};

#endif
