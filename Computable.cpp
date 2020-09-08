 
#include"Computable.h"
#include"Operation.h"

Computable* Computable::defInt (Computable* start, Computable* end, const string& variable) {
	if (this->dependentOn(variable)) {
		return new Operation(this->Int(end, variable), this->Int(start, variable), "-");
	}
	else {
		return new Operation(this, new Operation(end, start, "-"), "*");
	}
}