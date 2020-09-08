#ifndef COMPUTABLE_H
#define COMPUTABLE_H

#include"math.h"

class Variable;
class Operation;

class Computable {
public:
	virtual operator Number() = 0;
	virtual Computable* Int(Computable* computable, const string& variable) = 0;
	Computable* defInt(Computable* start, Computable* end, const string& variable);
	virtual bool dependentOn(const string& variable) = 0;
	virtual Computable* replaceVariable(const string& variable, Computable* computable) = 0;

};

class Operation : public Computable {
	Computable* left;
	Computable* right;
	string operation;
public:
	Operation(Computable* left, Computable* right, const string& operation);
	operator Number();
	Computable* Int(Computable* computable, const string& variable);
	bool dependentOn(const string& variable);
	Computable* replaceVariable(const string& variable, Computable* computable);
};

class Variable : public Computable {
	bool set = false;
public:
	string name;
	Number value;

	Variable(string name);
	Variable(string name, Number value);
	Variable(string name, double real, double imaginary = 0);
	Variable(Number value);
	Variable(double real, double imaginary = 0);

	Variable& operator+=(const Number& other);
	Variable& operator-=(const Number& other);
	Variable& operator*=(const Number& other);
	Variable& operator/=(const Number& other);
	Variable& operator^=(const Number& other);

	void setValue(Number number);
	void setValue(double real, double imaginary = 0);

	operator Number();
	Computable* Int(Computable* computable, const string& variable);
	bool dependentOn(const string& variable);
	Computable* replaceVariable(const string& variable, Computable* computable);
};

#endif