#include "Variable.h"

Variable::Variable(string name) : name(name) {}

Variable::Variable(string name, Number value) : name(name), value(value), set(true) {}

Variable::Variable(string name, double real, double imaginary) : name(name), value(real, imaginary), set(true) {}

Variable::Variable(Number value) : name(""), value(value), set(true) {}

Variable::Variable(double real, double imaginary) : name(""), value(real, imaginary), set(true) {}


Variable& Variable::operator+=(const Number& other) {
	value += other;
	return *this;
}


Variable& Variable::operator-=(const Number& other) {
	value -= other;
	return *this;
}


Variable& Variable::operator*=(const Number& other) {
	if (set) {
		value *= other;
	}
	return *this;
}


Variable& Variable::operator/=(const Number& other) {
	value /= other;
	return *this;
}


Variable& Variable::operator^=(const Number& other) {
	value ^= other;
	return *this;
}

void Variable::setValue(Number number) {
	this->value = number;
	set = true;
}


void Variable::setValue(double real, double imaginary) {
	this->value = { real, imaginary };
	set = true;
}


Variable::operator Number() {
	if (!set) {
		throw runtime_error("Tried Casting an Unset Variable");
	}
	return value;
}

Computable * Variable::Int(Computable* computable, const string & variable) {
	if (this->name == variable) {
		return new Operation(new Variable("", 0.5), new Operation(computable, new Variable("", 2), "^"), "*");
	}
	else {
		return nullptr;
	}
}

bool Variable::dependentOn(const string & variable) {
	return name == variable;
}

Computable* Variable::replaceVariable(const string & variable, Computable * computable) {
	if (name == variable) {
		return computable;
	}
	else {
		return this;
	}
}
