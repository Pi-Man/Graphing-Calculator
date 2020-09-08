#include"Operation.h"
#include"Variable.h"

Operation::Operation(Computable* left, Computable* right, const string& operation) : left(left), right(right), operation(operation) {}

Operation::operator Number() {
	if (left == nullptr || right == nullptr) {
		throw runtime_error("Incomplete Function");
	}
	if (operation == "^") {
		return left->operator Number() ^ right->operator Number();
	}
	else if (operation == "*") {
		return left->operator Number() * right->operator Number();
	}
	else if (operation == "/") {
		return left->operator Number() / right->operator Number();
	}
	else if (operation == "+") {
		return left->operator Number() + right->operator Number();
	}
	else if (operation == "-") {
		return left->operator Number() - right->operator Number();
	}
	else if (operation == "%") {
		return (Number)*left % (Number)*right;
	}
	else {
		throw runtime_error("Unsupported Operation: " + operation);
	}
}

Computable * Operation::Int(Computable * computable, const string & variable) {
	if (left == nullptr || right == nullptr) {
		throw runtime_error("Incomplete Function");
	}
	if (operation == "^") {
		if (Variable* v = dynamic_cast<Variable*>(left)) {
			if (v->name == variable) {
				if (!right->dependentOn(variable)) {
					return new Operation(new Operation(computable, new Operation(right, new Variable(1), "+"), "^"), new Operation(right, new Variable(1), "+"), "/");
				}
			}
			else {
				if (!right->dependentOn(variable)) {
					return new Operation(this, computable, "*");
				}
			}
		}
		return 0;
	}
	else if (operation == "*") {
		if (Variable* v = dynamic_cast<Variable*>(left)) {
			if (v->name != variable) {
				return new Operation(v, right->Int(computable, variable), "*");
			}
		}
		else if (Variable* v = dynamic_cast<Variable*>(right)) {
			if (v->name != variable) {
				return new Operation(left->Int(computable, variable), v, "*");
			}
		}
		return 0;
	}
	else if (operation == "/") {
		if (Variable* v = dynamic_cast<Variable*>(right)) {
			if (v->name.size() == 0) {
				return new Operation(left->Int(computable, variable), v, "/");
			}
		}
		return 0;
	}
	else if (operation == "+") {
		return new Operation(left->Int(computable, variable), right->Int(computable, variable), "+");
	}
	else if (operation == "-") {
		return new Operation(left->Int(computable, variable), right->Int(computable, variable), "-");
	}
	else {
		throw runtime_error("Unsupported Operation: " + operation);
	}
}

bool Operation::dependentOn(const string & variable) {
	if (right == nullptr || left == nullptr) {
		throw runtime_error("incomplete function");
	}
	return left->dependentOn(variable) || right->dependentOn(variable);
}

Computable* Operation::replaceVariable(const string & variable, Computable * computable) {
	Operation* opp = new Operation(*this);
	if (left == nullptr || right == nullptr) {
		throw runtime_error("incomplete function");
	}
	opp->left = left->replaceVariable(variable, computable);
	opp->right = right->replaceVariable(variable, computable);
	return opp;
}
