#ifndef MATH_H
#define MATH_H

#include<string>
#include<sstream>
#include<cmath>

using namespace std;

class Number {
public:
	double real;
	double imaginary;
	Number();
	Number(double x, double y = 0);

	bool isReal();
	bool isImaginary();

	Number operator+ (const Number& other);

	Number operator- (const Number& other);

	Number operator* (const Number& other);

	Number operator/ (const Number& other);

	Number operator^ (const Number& other);

	Number operator% (const Number& other);

	Number& operator+= (const Number& other);

	Number& operator-= (const Number& other);

	Number& operator*= (const Number& other);

	Number& operator/= (const Number& other);

	Number& operator^= (const Number& other);

	operator double() const;

};

class Vec2 {
public:

	Number x, y;

	Vec2() = default;

	Vec2(Number xIn, Number yIn);

	string toString();

};

namespace Math {

	const Number e = 2.7182818284590452353602874713527;

	const Number pi = 3.141592653589793238462643383279502;

	Number clamp(Number value, Number min, Number max);

	int mod(int base, int dividend);
}

#endif
