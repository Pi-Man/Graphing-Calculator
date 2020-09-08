#ifndef D_FUNCTIONS_H
#define D_FUNCTIONS_H

#include"Function.h"

class Sin : public Function {
public:
	Sin();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Cos : public Function {
public:
	Cos();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Tan : public Function {
public:
	Tan();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Asin : public Function {
public:
	Asin();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Acos : public Function {
public:
	Acos();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Atan : public Function {
public:
	Atan();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class UnitStep : public Function {
public:
	UnitStep();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class DeltaFunction : public Function{
public:
	DeltaFunction();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Real : public Function {
public:
	Real();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

class Imaginary : public Function {
public:
	Imaginary();
	Function* copy();
	Computable* Int(Computable* computable, const string& variable);
	operator Number();
};

void initPredefinedFunctions(LinkedList<Function*>& functions);

#endif