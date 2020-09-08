#include"DefaultFunctions.h"


Sin::Sin() : Function("sin(x)") {}


Function* Sin::copy() {
	return new Sin(*this);
}

Computable * Sin::Int(Computable * computable, const string & variable) {
	Cos* cos = new Cos();
	cos->addComputable(computables[0]->replaceVariable(variable, computable));
	return new Operation(new Variable("", -1), cos, "*");
}


Sin::operator Number() {
	if (Function::computables.size() == 1) {
		return sin((Number)*(Function::computables[0]));
	}
	else throw runtime_error("Incorrect Number of Arguments");
}



Cos::Cos() : Function("cos(x)") {}


Function* Cos::copy() {
	return new Cos(*this);
}

Computable * Cos::Int(Computable * computable, const string & variable) {
	Sin* sin = new Sin();
	sin->addComputable(computables[0]->replaceVariable(variable, computable));
	return sin;
}


Cos::operator Number() {
	if (Function::computables.size() == 1) {
		return cos((Number)*(Function::computables[0]));
	}
	else throw runtime_error("Incorrect Number of Arguments");
}



Tan::Tan() : Function("tan(x)") {}


Function* Tan::copy() {
	return new Tan(*this);
}

Computable * Tan::Int(Computable * computable, const string & variable) {
	return nullptr;
}


Tan::operator Number() {
	if (Function::computables.size() == 1) {
		return tan((Number)*(Function::computables[0]));
	}
	else throw runtime_error("Incorrect Number of Arguments");
}



Asin::Asin() : Function("asin(x)") {}


Function* Asin::copy() {
	return new Asin(*this);
}

Computable * Asin::Int(Computable * computable, const string & variable) {
	Function* fun = new Function("int::asin(x) = x*asin(x)+(1-x^2)^0.5");
	fun->addComputable(computables[0]->replaceVariable(variable, computable));
	return fun;
}


Asin::operator Number() {
	if (Function::computables.size() == 1) {
		return asin((Number)*(Function::computables[0]));
	}
	else throw runtime_error("Incorrect Number of Arguments");
}



Acos::Acos() : Function("acos(x)") {}


Function* Acos::copy() {
	return new Acos(*this);
}

Computable * Acos::Int(Computable * computable, const string & variable) {
	Function* fun = new Function("int::acos(x) = x*acos(x) - (1-x^2)^0.5");
	fun->addComputable(computables[0]->replaceVariable(variable, computable));
	return fun;
}


Acos::operator Number() {
	if (Function::computables.size() == 1) {
		return acos((Number)*(Function::computables[0]));
	}
	else throw runtime_error("Incorrect Number of Arguments");
}



Atan::Atan() : Function("atan(x)") {}


Function* Atan::copy() {
	return new Atan(*this);
}

Computable * Atan::Int(Computable * computable, const string & variable) {
	return nullptr;
}


Atan::operator Number() {
	if (Function::computables.size() == 1) {
		return atan((Number)*(Function::computables[0]));
	}
	else throw runtime_error("Incorrect Number of Arguments");
}



UnitStep::UnitStep() : Function("u(x)") {}


Function* UnitStep::copy() {
	return new UnitStep(*this);
}

Computable * UnitStep::Int(Computable * computable, const string & variable) {
	Function* fun = new Function("int::u(x) = x*u(x)");
	fun->addComputable(computables[0]->replaceVariable(variable, computable));
	return fun;
}


UnitStep::operator Number() {
	if (Function::computables.size() == 1) {
		return (double)(Number)*Function::computables[0] > 0 ? 1 : 0;
	}
	else throw runtime_error("Incorrect Number of Arguments");
}


Function* DeltaFunction::copy() {
	return new DeltaFunction(*this);
}

DeltaFunction::DeltaFunction() : Function("d(x)") {}

Computable * DeltaFunction::Int(Computable * computable, const string & variable) {
	UnitStep* step = new UnitStep();
	step->addComputable(computables[0]->replaceVariable(variable, computable));
	return step;
}

DeltaFunction::operator Number() {
	if (this->computables.size() == 1) {
		Number num = *computables[0];
		return num == 0 ? INFINITY : 0;
	}
	else throw runtime_error("Incorrect Number of Arguments");
}


Real::Real() : Function("real(x)") {}

Function* Real::copy() {
	return new Real(*this);
}

Computable* Real::Int(Computable* computable, const string& variable) {
	return copy();
}

Real::operator Number() {
	return Number(((Number)*computables[0]).real);
}

Imaginary::Imaginary() : Function("imaginary(x)") {}

Function* Imaginary::copy() {
	return new Imaginary(*this);
}

Computable* Imaginary::Int(Computable* computable, const string& variable) {
	return copy();
}

Imaginary::operator Number() {
	return Number(((Number)*computables[0]).imaginary);
}


void initPredefinedFunctions(LinkedList<Function*>& functions) {
	functions.add(new Sin());
	functions.add(new Cos());
	functions.add(new Tan());

	functions.add(new Asin());
	functions.add(new Acos());
	functions.add(new Atan());

	functions.add(new UnitStep());
	functions.add(new DeltaFunction());

	functions.add(new Real());
	functions.add(new Imaginary());
}
