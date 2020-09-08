#ifndef OPERATION_H
#define OPERATION_H

#include"Computable.h"
#include"Variable.h"

//class Variable;
//
//class Operation : public Computable {
//	Computable* left;
//	Computable* right;
//	string operation;
//public:
//	Operation(Computable* left, Computable* right, const string& operation);
//	operator Number();
//	Computable* Int(Computable* computable, const string& variable);
//	bool dependentOn(const string& variable);
//	void replaceVariable(const string& variable, Variable* computable);
//};
//
//class Variable : public Computable {
//	bool set = false;
//public:
//	string name;
//	Number value;
//
//	Variable(string name);
//	Variable(string name, double value);
//
//	Variable& operator+=(const Number& other);
//	Variable& operator-=(const Number& other);
//	Variable& operator*=(const Number& other);
//	Variable& operator/=(const Number& other);
//	Variable& operator^=(const Number& other);
//
//	void setValue(double value);
//
//	operator Number();
//	Computable* Int(Computable* computable, const string& variable);
//	bool dependentOn(const string& variable);
//	void replaceVariable(const string& variable, Variable* computable);
//};

#endif